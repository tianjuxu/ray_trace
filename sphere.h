#ifndef _SPHERE_H
#define _SPHERE_H
#include "math.h"
#include "vect.h"
#include "color.h"
#include "object.h"
class sphere : public object
{
    vect center;
	double radius;
	color sphereColor;
	public :
	sphere();
	sphere (vect, double, color);
	
	//memeber functions
	vect getSphereCenter () {return center;}
	color getColor () {return sphereColor;}
	double getsphereRadius () {return radius;}
	
	vect getNormalAtPoint(vect point)
	{
		// normal always points away from the center of a sphere
		vect normalVect = point.vectAdd(center.negative()).normalize();
		return normalVect;
	}
	double findIntersection(ray ray)
	{
		vect rayOrigin = ray.getRayOrigin();
		double rayOriginX = rayOrigin.getVectX();
		double rayOriginY = rayOrigin.getVectY();
		double rayOriginZ = rayOrigin.getVectZ();
		
		vect rayDirection = ray.getRayDirection();
		double rayDirectionX = rayDirection.getVectX();
		double rayDirectionY = rayDirection.getVectY();
		double rayDirectionZ = rayDirection.getVectZ();
		
		vect sphereCenter = center;
		double sphereCenterX = sphereCenter.getVectX();
		double sphereCenterY = sphereCenter.getVectY();
		double sphereCenterZ = sphereCenter.getVectZ();
		//basic ray tracing formulas
		double rayDotNormal = 1; //normalized
		double rayDotOrigin = (2*(rayOriginX - sphereCenterX)*rayDirectionX) 
		                    + (2*(rayOriginY - sphereCenterY)*rayDirectionY) 
							+ (2*(rayOriginZ - sphereCenterZ)*rayDirectionZ);
		double c            = pow(rayOriginX - sphereCenterX, 2) + pow(rayOriginY - sphereCenterY, 2) + pow(rayOriginZ - sphereCenterZ, 2) - (radius * radius);
		double discriminant = rayDotOrigin*rayDotOrigin - 4*c;
		
		if (discriminant > 0)
		{//the ray intersects the sphere
	        double firstRoot = ((-1*rayDotOrigin - sqrt(discriminant))/2) - 0.000000000001;
            if(firstRoot > 0)
			{
				//the first root is the smallest positive root which is what we want, the nearest intersection
				return firstRoot;
			}				
			else
			{
				//second root is the smallest positive root
				double secondRoot = ((sqrt(discriminant) - rayDotOrigin)/2) - 0.000000000001;
				return secondRoot;
			}			
		}
		else 
		{//no intersection
			return -1;
		}
	}
};

sphere::sphere () 
{
	center       = vect(0, 0, 0);
	radius       = 1.0;
	sphereColor  = color(0.5, 0.5, 0.5, 0);
}
sphere::sphere (vect newCenter, double newRadius, color newColor) 
{
	center            = newCenter;
	radius            = newRadius;
	sphereColor       = newColor;
}



#endif
