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
# for subscriber
def callback(data):
    # trouble shoot - print data from subscriber
    #rospy.loginfo(rospy.get_caller_id() + "leftx: %s, rightx: %s", data.leftx, data.rightx)    
    
    #print 'The value of PI is approximately {0:.3f}.'.format(math.pi)    
	# unpack values
    global m1val
    global m2val
    xl = data.leftx;
    xr = data.rightx;
    xlcm = xl * 100;
    xrcm = xr * 100;
    print 'leftx: {0:.3f} cm, rightx: {1:.3f} cm.'.format(xlcm, xrcm)
    
    #x1des = 


    # read currents
    m1cur, m2cur = readcurrents();
    #print "Current M1: ",m1cur/10.0," M2: ",m2cur/10.0

    #initialize m1val, m2val
    #m1val = numpy.array([-500,-400,-300,-200,-100,0,100,200,300,400,500]);
    #m1val = numpy.array([0,500])
    #m1val = 1;
    #m2val = 1;
    sleeptime = 0;
    #sleeptime = 0.1;
    #for idx in range(0,m1val.size):
    
    #print("==" + str(idx) + "==")
    #input("Press Enter to continue...")
    #SetM1DutyAccel(1500,m1val[idx])
    #SetM1DutyAccel(1500,m1val)
    #SetM2DutyAccel(1500,m2val)
    #print "xl: ",xl," xr:", xr
    print "m1: ",m1val," m2:", m2val
    m1cur, m2cur = readcurrents();
    #print "Current M1: ",m1cur/10.0," M2: ",m2cur/10.0
    # publish current
    time.sleep(sleeptime)
    
    # publish
    #msg = roboclawCmd();
    #msg.m1 = m1val;
    #msg.m2 = m2val;
    #rospy.loginfo(msg);
    #pub.publish(msg);
    
def timer_callback(event):
	global m1val 

	global m2val 

	m1val = 512
	m2val = -512
	print 'Timer called at ' + str(event.current_real)
	SetM1DutyAccel(1500,m1val)
	SetM2DutyAccel(1500,m2val)

def talker():
    global m1val
    global m2val
	# publishing to roboclawcommand topic
    #pub = rospy.Publisher('roboclawcommand',roboclawCmd, queue_size = 10)
    pub = rospy.Publisher('/magnet_track/roboclawCmd', roboclawCmd, queue_size=10)
    #rospy.init_node('talker',anonymous=True)
    rate = rospy.Rate(100) #100Hz
    msg = roboclawCmd();

    while not rospy.is_shutdown():
        #hello_str = "hello world %s" % rospy.get_time()
        msg.m1 = m1val;
        msg.m2 = m2val;
        rospy.loginfo(msg);
        pub.publish(msg);
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

    rospy.Timer(rospy.Duration(2), timer_callback)
    
    try: 
    	talker()
    except rospy.ROSInterruptException:
    	pass

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
	SetM1DutyAccel(1500,-512)
	SetM2DutyAccel(1500,512)
	listener()

    
# # initialize m1val, m2val
# m1val = 0;
# m2val = 0;

# # set m1 and m2 val
# if xr > 100:
# 	print (" =============Update m2val===============")
    
# SetM1DutyAccel(1500,m1val)
# SetM2DutyAccel(1500,m2val)
# print ("Black Coil: ", m2val)
# print ("White Coil: ", m1val)