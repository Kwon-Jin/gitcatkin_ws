#  Controller that takes in ROS msg xyReal.
#  Given desired positions along the straw, keep magnet at that position

#  =========== TO ADD =================
# load csv of x, xdot, xdotdot values for a trajectory


#!/usr/bin/env python
import rospy
from camera_magnet.msg import xyReal
from camera_magnet.msg import roboclawCmd
from std_msgs.msg import String

# for roboclaw : 
import serial
import struct
import time
import math
import numpy
import csv

try:
    import numpy
except ImportError:
    print "numpy is not installed"

checksum = 0

def sendcommand(address,command):
	global checksum
	checksum = address
	port.write(chr(address));
	checksum += command
	port.write(chr(command));
	return;

def readbyte():
	global checksum
	val = struct.unpack('>B',port.read(1));
	checksum += val[0]
	return val[0];	
def readword():
	global checksum
	val = struct.unpack('>H',port.read(2));
	checksum += (val[0]&0xFF)
	checksum += (val[0]>>8)&0xFF
	return val[0];	
def writebyte(val):
	global checksum
	checksum += val
	return port.write(struct.pack('>B',val));
def writeword(val):
	global checksum
	checksum += val
	checksum += (val>>8)&0xFF
	return port.write(struct.pack('>H',val));
def writesword(val):
	global checksum
	checksum += val
	checksum += (val>>8)&0xFF
	return port.write(struct.pack('>h',val));

def readversion():
	sendcommand(128,21)
	return port.read(32);

def readcurrents():
	sendcommand(128,49);
	motor1 = readword();
	motor2 = readword();
	crc = checksum&0x7F
	if crc==readbyte():
		return (motor1,motor2);
	return (-1,-1);

def SetM1DutyAccel(accel,duty):
	sendcommand(128,52)
	writesword(duty)
	writeword(accel)
	writebyte(checksum&0x7F);
	return;

def SetM2DutyAccel(accel,duty):
	sendcommand(128,53)
	writesword(duty)
	writeword(accel)
	writebyte(checksum&0x7F);
	return;

def readtemperature():
	sendcommand(128,82);
	val = readword()
	crc = checksum&0x7F
	if crc==readbyte():
		return val
	return -1

def readerrorstate():
	sendcommand(128,90);
	val = readbyte()
	#val = readword()
	crc = checksum&0x7F
	if crc==readbyte():
#	if crc==readword():
		return val
	return -1
# end for roboclaw

port = serial.Serial("/dev/ttyUSB0", baudrate=38400, timeout=0.1)
m1val = 0
m2val = 0

# Define global values
PI = 3.14159265358979
L = 0.1
m = 2.1841
mu = 0.004
u0 = math.pow(10,-7)*4*PI
R = 0.0286
mass = 0.000282

# for calculating current for 1D system
def computecurrent(x1,x2,gainx): #
#   print x1 
#   print x2
   A = numpy.matrix([[.1,.2],[.3,.4]]);
   A[0,0] = 3*mu/2 * (x1*u0*math.pow(R,2))/math.pow((math.pow(x1,2) + math.pow(R,2)),(2.5));
   A[0,1] = 3*mu/2 * (x1-L)*u0*math.pow(R,2)/math.pow(math.pow(x1-L,2) + math.pow(R,2),(2.5));
   A[1,0] = -3*mu/2 * (x2*u0*math.pow(R,2))/math.pow(math.pow(x2,2) + math.pow(R,2),(2.5));
   A[1,1] = -3*mu/2 * (x2-L)*u0*math.pow(R,2)/math.pow(math.pow(x2-L,2) + math.pow(R,2),(2.5));

   b = - u0*math.pow(m,2)/(4*PI)*numpy.array([[-math.pow((x2-x1),-2)],[math.pow(x2-x1,-2)]]) + gainx;
   I = numpy.linalg.solve(A,b);
   # set limits to +/- 512 and round to integer value
   I[numpy.nonzero(numpy.absolute(I)>512),:] = numpy.copysign(512,I[numpy.nonzero(numpy.absolute(I)>512),:])
   I = numpy.around(I,decimals=0)
   return I

# for subscriber
def callback(data):
    # trouble shoot - print data from subscriber
    #rospy.loginfo(rospy.get_caller_id() + "leftx: %s, rightx: %s", data.leftx, data.rightx)    
    
	# unpack values
    global m1val
    global m2val
    global xl 
    global xr
    xl = float(data.leftx);
    xr = float(data.rightx);
    xlcm = xl * 100;
    xrcm = xr * 100;
    #print 'leftx: {0:.3f} cm, rightx: {1:.3f} cm.'.format(xlcm, xrcm)

    # read currents
    m1cur, m2cur = readcurrents();
    #print "Current M1: ",m1cur/10.0," M2: ",m2cur/10.0


def talker():
	# publishing to roboclawcommand topic
    #pub = rospy.Publisher('roboclawcommand',roboclawCmd, queue_size = 10)
    pub = rospy.Publisher('/magnet_track/roboclawCmd', roboclawCmd, queue_size=10)
    #rospy.init_node('talker',anonymous=True)
    rate = rospy.Rate(5) #Hz
    msg = roboclawCmd();
    i = 0;
    while not rospy.is_shutdown():
        #hello_str = "hello world %s" % rospy.get_time()

        x1 = x1all[0,i]
        x2 = x2all[0,i]
        
        #gainx = 
        xdiff1 = x1-xl
        xdiff2 = x2-xr 
        print 'xdiff1: {0:.2f}, xdiff2: {1:.2f}.'.format(xdiff1*100,xdiff2*100)

        gainx = numpy.array([[0],[0]])
        gainx = numpy.array([[.01],[.01]]) * numpy.array([[xdiff1],[xdiff2]])
        #print(gainx)
        I = computecurrent(x1,x2,gainx)
        print(I)
        i = i + 1
        if i>row_count-1: 
        	i = 0
        	print("==end==")
        	time.sleep(2)
        m1val = int(I[0])
        m2val = int(I[1])
        msg.m1 = m1val;
        msg.m2 = m2val;
        SetM1DutyAccel(1500,m1val)
        SetM2DutyAccel(1500,m2val)
        rospy.loginfo(msg);
        pub.publish(msg);

        rate.sleep()


def motorupdate():
    global m1val
    global m2val
    # compute gainx


    rate = rospy.Rate(75)
    
    i = 0;
    #for i in range(len(x1all)):
    while not rospy.is_shutdown():
    	# compute I



    	# assign velocity

	    rate.sleep()


def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    
    
    rospy.init_node('controller', anonymous=True)


    rospy.Subscriber("/magnet_track/xyReal", xyReal, callback)

    # define timer that updates every x:
    
    try: 
    	talker()
    	#motorupdate()
    except rospy.ROSInterruptException:
    	pass

	
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
   global x1all
   global x2all
   global row_count
   SetM1DutyAccel(1500,-512)
   SetM2DutyAccel(1500,512)
	# load csv of x1,x2 values
   filename = 'trajoscillate5.dat'
   with open(filename,'rb') as f:
   
   		reader = csv.reader(f, delimiter = ',')
   		row_count = sum(1 for low in reader)
   		print(row_count)
   # initialize an array with the number of rows
   x1all = numpy.zeros((1,row_count))
   x2all = numpy.zeros((1,row_count))
   with open(filename,'rb') as f:		
   		reader = csv.reader(f, delimiter = ',')
   		idx = 0;
   		for row in reader:
   			print(row)
   			
   			x1all[0,idx] = float(row[0])
   			x2all[0,idx] = float(row[1])
   			#print("x1: " + x1all[0,idx] ) #" x2:" + x2)
	
			idx = idx + 1
   SetM1DutyAccel(1500,512)
   SetM2DutyAccel(1500,-512)
   xl = x1all[0,0]
   xr = x2all[0,0]
   time.sleep(0)
   listener()