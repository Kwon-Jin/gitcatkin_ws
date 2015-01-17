import math
import numpy
import csv 

# Define global values
PI = 3.14159265358979
L = 0.1
m = 2.1841
mu = 0.004
u0 = math.pow(10,-7)*4*PI
R = 0.0286
mass = 0.000282


def computecurrent(x1,x2,gainx): #
   print x1 
   print x2
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

def main():
	# load csv of x1,x2 values
   with open('traj.dat','rb') as f:
   
   		reader = csv.reader(f, delimiter = ',')
   		row_count = sum(1 for low in reader)
   		print(row_count)
   # initialize an array with the number of rows
   x1all = numpy.zeros((1,row_count))
   x2all = numpy.zeros((1,row_count))
   with open('traj.dat','rb') as f:		
   		reader = csv.reader(f, delimiter = ',')
   		idx = 0;
   		for row in reader:
   			print(row)
   			
   			x1all[0,idx] = float(row[0])
   			x2all[0,idx] = float(row[1])
   			#print("x1: " + x1all[0,idx] ) #" x2:" + x2)
	
			idx = idx + 1
   print(x1all[0,1])

   
   for i in range(len(x1all)):
		# compute I
	    x1 = x1all[0,i]
	    x2 = x2all[0,i]
	    gainx = numpy.array([[0],[0]])
	    I = computecurrent(x1,x2,gainx)
	    print(I)


	#x1 = 0.01
	#print(3*mu/2 * (0.01*u0*math.pow(R,2))/math.pow(math.pow(x1,2) + math.pow(R,2),(5/2)))
	#print((0.01*u0*math.pow(R,2)))
	#print(math.pow((math.pow(x1,2) + math.pow(R,2)),(2.5)))
main()

