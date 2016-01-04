#ifndef _VECT_H
#define _VECT_H
#include "math.h"
class vect 
{
    double x, y, z;
	public :
	vect();
	vect (double, double, double);
	
	//member functions
	double getVectX() { return x;}
	double getVectY() { return y;}
	double getVectZ() { return z;}
	
	double magnitude ()
	{
		return sqrt((x*x)+(y*y)+(z*z));
	}
	vect normalize () 
	{
		double magnitudeOfVect = sqrt((x*x)+(y*y)+(z*z));
		return vect (x/magnitudeOfVect, y/magnitudeOfVect, z/magnitudeOfVect);
	}
	
	vect negative ()
	{
		return vect (-x, -y, -z);
	}
	double dotProduct (vect v)
	{
		return (x * v.getVectX() + y * v.getVectY() + z * v.getVectZ());
	}
	
	vect crossProduct (vect v)
	{
		return vect (y * v.getVectZ() - z * v.getVectY(), z * v.getVectX() - x * v.getVectZ(), x * v.getVectY() - y * v.getVectX());
	}
	vect vectAdd (vect v)
	{
		return vect (x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}
	
	vect vectMult (double scalar)
	{
		return vect (x * scalar, y * scalar, z * scalar);
	}
	//default destructor
};

vect::vect () 
{
	x = 0;
	y = 0;
	z = 0;
}
vect::vect (double objX, double objY, double objZ) 
{
	x = objX;
	y = objY;
	z = objZ;
}



#endif
