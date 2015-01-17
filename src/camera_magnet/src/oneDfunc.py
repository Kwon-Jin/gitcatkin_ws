import math
import numpy
import csv 

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
