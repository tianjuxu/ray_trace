#ifndef _OBJECT_H
#define _OBJECT_H
#include "math.h"
#include "vect.h"
#include "color.h"
#include "ray.h"
class object 
{
	public :
	object();
	
	//memeber functions
	virtual color getColor () { return color(0.0, 0.0, 0.0, 0); }
	virtual vect getNormalAtPoint(vect point) {return vect(0.0, 0.0, 0.0); }
	virtual double findIntersection(ray ray)
	{
		return 0;
	}
	
};

object::object () 
{
}



#endif
