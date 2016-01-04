#ifndef _CAMERA_H
#define _CAMERA_H
#include "math.h"
#include "vect.h"
class camera 
{
    vect camPos, camDir, camRight, camDown;
	//camRight and camDown define a 2D plane orthogonal to the ray emitted from camPos
	public :
	camera();
	camera (vect, vect, vect, vect);
	
	//member functions
	vect getCameraPosition ()  {return camPos;}
	vect getCameraDirection () {return camDir;}
	vect getCameraRight()      {return camRight;}
	vect getCameraDown()       {return camDown;}
	
};

camera::camera () 
{
	camPos   = vect(0, 0, 0);
	camDir   = vect(0, 0, 1);
	camRight = vect(0, 0, 0);
	camDown  = vect(0, 0, 0);
	
	
}
camera::camera (vect otherPos, vect otherDirection, vect otherRight, vect otherDown) 
{
	camPos    = otherPos;
	camDir    = otherDirection;
	camRight  = otherRight;
	camDown   = otherDown;
}



#endif
