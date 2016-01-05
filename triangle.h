#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include "math.h"
#include "vect.h"
#include "color.h"
#include "object.h"
class triangle : public object
{
    vect p1, p2, p3; //three points to define a triangle
	double distanceToSceneCenter;
	color triangleColor;
	vect normal;
	public :
	triangle (vect, vect, vect, color);
	
	//memeber functions
	vect getTriangleNormal () 
	{
		return normal;
	}
	
	color getColor () {return triangleColor;}
	
	double getTriangleDistance () 
	{		
		return distanceToSceneCenter;
	}
	
	vect getNormalAtPoint (vect point)
	{
		return normal;
	}
	
	double findIntersection (ray ray) //returns a distance between the ray origin and point of intersection
	{
		vect rayDirection = ray.getRayDirection();
		vect rayOrigin    = ray.getRayOrigin();
		
		
		double rayDotNormal = rayDirection.dotProduct(normal);
		
		if(rayDotNormal == 0)
		{//ray is parallel to the triangle
			return -1;
		}
		else
		{
			double rayDotOrigin = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distanceToSceneCenter).negative()));
			double distanceToTriangle = -1*rayDotOrigin/rayDotNormal;
			
			double p4X = rayDirection.vectMult(distanceToTriangle).getVectX() + rayOrigin.getVectX();
			double p4Y = rayDirection.vectMult(distanceToTriangle).getVectY() + rayOrigin.getVectY();
			double p4Z = rayDirection.vectMult(distanceToTriangle).getVectZ() + rayOrigin.getVectZ();
			
			vect p4 (p4X, p4Y, p4Z);
			
			//condition to check the ray lies in triangle
			//assuming point p4 is the intersection
			//p4 lies in the triangle if (edge) [p3p1 x p4p1]*normal >=0 
	        vect edgeP3P1  ( p3.getVectX()-p1.getVectX(), p3.getVectY()-p1.getVectY() , p3.getVectZ()-p1.getVectZ() );
            vect edgeP4P1  ( p4.getVectX()-p1.getVectX(), p4.getVectY()-p1.getVectY() , p4.getVectZ()-p1.getVectZ() );
            double test1   = (edgeP3P1.crossProduct(edgeP4P1)).dotProduct(normal);
			//p4 lies in the triangle if (edge) [p2p3 x p4p3]*normal >=0 
	        vect edgeP2P3  ( p2.getVectX()-p3.getVectX(), p2.getVectY()-p3.getVectY() , p2.getVectZ()-p3.getVectZ() );
            vect edgeP4P3  ( p4.getVectX()-p3.getVectX(), p4.getVectY()-p3.getVectY() , p4.getVectZ()-p3.getVectZ() );
            double test2   = (edgeP2P3.crossProduct(edgeP4P3)).dotProduct(normal);
			//p4 lies in the triangle if (edge) [p1p2 x p4p2]*normal >=0 
	        vect edgeP1P2  ( p1.getVectX()-p2.getVectX(), p1.getVectY()-p2.getVectY() , p1.getVectZ()-p2.getVectZ() );
            vect edgeP4P2  ( p4.getVectX()-p2.getVectX(), p4.getVectY()-p2.getVectY() , p4.getVectZ()-p2.getVectZ() );
            double test3   = (edgeP1P2.crossProduct(edgeP4P2)).dotProduct(normal);

			if ( (test1 >=0) && (test2 >= 0) && (test3 >= 0) ) 
			{
				return distanceToTriangle;
			}
			else
			{
				return -1; //outside the triangle
			}
		}
	}
};

triangle::triangle (vect newP1, vect newP2, vect newP3, color newColor) 
{
	p1 = newP1;
	p2 = newP2;
	p3 = newP3;
	
	vect edgeP3P1  ( p3.getVectX()-p1.getVectX(), p3.getVectY()-p1.getVectY() , p3.getVectZ()-p1.getVectZ() );
    vect edgeP2P1  ( p2.getVectX()-p1.getVectX(), p2.getVectY()-p1.getVectY() , p2.getVectZ()-p1.getVectZ() );
    normal         = edgeP3P1.crossProduct(edgeP2P1).normalize();
	distanceToSceneCenter = normal.dotProduct(p1);
	triangleColor  = newColor;
}



#endif
