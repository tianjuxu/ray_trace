#ifndef _LIGHT_H
#define _LIGHT_H
#include "math.h"
#include "vect.h"
#include "color.h"
#include "source.h"
class light : public source
{
    vect position;
	color lightColor;
	public :
	light();
	light (vect, color);
	
	//memeber functions
	vect getLightPosition () {return position;}
	color getLightColor () {return lightColor;}
	
};

light::light () 
{
	position    = vect(0, 0, 0);
	lightColor  = color(1, 1, 1, 0);
}
light::light (vect newPosition, color newColor) 
{
	position    = newPosition;
	lightColor       = newColor;
}



#endif
