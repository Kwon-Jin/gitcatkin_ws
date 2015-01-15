#  Controller that takes in ROS msg xyReal.
#  Toggles left coil between +/-512 depending on position of left magnet.

#!/usr/bin/env python
import rospy
from camera_magnet.msg import xyReal

# for roboclaw : 
import serial
import struct
import time
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

# for subscriber
def callback(data):
    # trouble shoot - print data from subscriber
    rospy.loginfo(rospy.get_caller_id() + "leftx: %s, rightx: %s", data.leftx, data.rightx)    
    # unpack values
    xl = data.leftx;
    xr = data.rightx;
    

    # read currents
    m1cur, m2cur = readcurrents();
    #print "Current M1: ",m1cur/10.0," M2: ",m2cur/10.0

    #initialize m1val, m2val
    m1val = 0;
    m2val = -200;

    xlimlo = 0.005;
    xlimhi = 0.035;
    if xl < xlimlo and m1val != 512:
    	print (" ----------- ++ m2 val ------------")
    	m1val = 512
    	SetM1DutyAccel(1500,m1val)
    	SetM2DutyAccel(1500,m2val)
    	print "xl: ",xl," xr:", xr
    	print "m1: ",m1val," m2:", m2val

    if xl > xlimhi and m1val != -512:
    	print (" ----------- -- m2 val ------------")
    	m1val = -512
    	SetM1DutyAccel(1500,m1val)
    	SetM2DutyAccel(1500,m2val)
    	print "xl: ",xl," xr:", xr
    	print "m1: ",m1val," m2:", m2val
	

def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("/magnet_track/xyReal", xyReal, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
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