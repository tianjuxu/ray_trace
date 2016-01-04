#ifndef _COLOR_H
#define _COLOR_H
#include "math.h"
class color 
{
    double red, green, blue, special;
	
	public :
	
	color();
	color (double, double, double, double);
	
	//member functions
	double getColorRed() { return red;}
	double getColorGreen() { return green;}
	double getColorBlue() { return blue;}
	double getColorSpecial() { return special;}
	
	
	double setColorRed(double redValue) {red = redValue;}
	double setColorGreen(double greenValue) {green = greenValue;}
	double setColorBlue(double blueValue) {blue = blueValue;}
	double setColorSpecial(double specialValue) {special = specialValue;}

	double brightness ()
	{
		return (red + green + blue)/3;
	}
	color colorScalar(double scalar)
	{
		return color (red * scalar, green * scalar, blue * scalar, special);
	}
	color colorAdd (color otherColor)
	{
		return color (red + otherColor.getColorRed(), green + otherColor.getColorGreen(), blue + otherColor.getColorBlue(), special );
	}
	color colorMult(color otherColor)
	{
		return color (red * otherColor.getColorRed(), green * otherColor.getColorGreen(), blue * otherColor.getColorBlue(), special );
	}
	
	color colorAverage(color otherColor)
	{
		return color ((red + otherColor.getColorRed())/2, (green + otherColor.getColorGreen())/2, (blue + otherColor.getColorBlue())/2, special );
	}
	
	color clip()
	{
		double allLight = red + blue + green;
		double excessLight = allLight - 3;
		if (excessLight > 0)
		{
			red = red + excessLight*(red/allLight);
			green = green + excessLight*(green/allLight);
			blue = blue + excessLight*(blue/allLight);
			
		}
		if(red > 1) {red = 1;}
		if(green > 1) {green = 1;}
		if(blue > 1) {blue = 1;}
		
		if(red < 0) {red = 0;}
		if(green < 0) {green = 0;}
		if(blue < 0) {blue = 0;}
    return color (red, green, blue, special);
	}
	//default destructor
};

color::color () 
{
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}
color::color (double newRed, double newGreen, double newBlue, double newSpecial) 
{
	red = newRed;
	green = newGreen;
	blue = newBlue;
	special = newSpecial;
}



#endif
