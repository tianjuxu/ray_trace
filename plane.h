#ifndef _PLANE_H
#define _PLANE_H
#include "math.h"
#include "vect.h"
#include "color.h"
#include "object.h"
class plane : public object
{
    vect normal;
	double distanceToSceneCenter;
	color planeColor;
	public :
	plane();
	plane (vect, double, color);
	
	//memeber functions
	vect getplaneNormal () {return normal;}
	color getColor () {return planeColor;}
	double getplaneDistance () {return distanceToSceneCenter;}
	
	vect getNormalAtPoint (vect point)
	{
		return normal;
	}
	double findIntersection (ray ray) //returns a distance between the ray origin and point of intersection
	{
		vect rayDirection = ray.getRayDirection();
		
		double rayDotNormal = rayDirection.dotProduct(normal);
		
		if(rayDotNormal == 0)
		{//ray is parallel to the plane
			return -1;
		}
		else
		{
			double rayDotOrigin = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distanceToSceneCenter).negative()));
			return -1*rayDotOrigin/rayDotNormal;
		}
	}
};

plane::plane () 
{
	normal                      = vect(1, 0, 0);
	distanceToSceneCenter       = 0.0;
	planeColor                  = color(0.5, 0.5, 0.5, 0);
}
plane::plane (vect newNormal, double newDistance, color newColor) 
{
	normal                           = newNormal;
	distanceToSceneCenter            = newDistance;
	planeColor                       = newColor;
}



#endif
