#ifndef _RAY_H
#define _RAY_H
#include "math.h"
#include "vect.h"
class ray 
{
    vect origin, direction;
	public :
	ray();
	ray (vect, vect);
	
	//member function
	vect getRayOrigin () {return origin;}
	vect getRayDirection () {return direction;}
	
};

ray::ray () 
{
	origin    = vect(0, 0, 0);
	direction = vect(1, 0, 0);
}
ray::ray (vect otherOrigin, vect otherDirection) 
{
	origin    = otherOrigin;
	direction = otherDirection;
}



#endif
