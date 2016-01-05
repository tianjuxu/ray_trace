#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
/***************local includes********************************************/
#include "object.h"
#include "vect.h"
#include "ray.h"
#include "camera.h"
#include "color.h"
#include "light.h"
#include "sphere.h"
#include "plane.h"
#include "source.h"
#include "triangle.h"
using namespace std;
/***************data struct to store basic color components***************/
struct RGBType 
{
	double r;
	double g;
	double b;
	
};
/**
  *function that writes pixel info to a image file.
  @param fileName the file name that it writes to
  @param w        width of how many elements to write
  @param h        height of how many elements to write
  @param dpi      dots per inch image sensity
  @param data     points to the location where all the color details are stored
**/
void saveBmp(const char *fileName, int w, int h, int dpi, RGBType *data)
{
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int fileSize = 54 + s;
	
	double factor = 39.375;
	int m = static_cast<int>(factor);
	
	int ppm = dpi*m;
	
	unsigned char bmpFileHeader[14] = {'B','M', 0, 0, 0, 0,   0, 0, 0, 0,  54, 0, 0, 0};
	unsigned char bmpInfoHeader[40] = {40, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   1, 0, 24, 0};
	
	bmpFileHeader[2] = (unsigned char)(fileSize);
	bmpFileHeader[3] = (unsigned char)(fileSize>>8);
	bmpFileHeader[4] = (unsigned char)(fileSize>>16);
	bmpFileHeader[5] = (unsigned char)(fileSize>>24);
	
	bmpInfoHeader[4] = (unsigned char) (w);
	bmpInfoHeader[5] = (unsigned char) (w>>8);
	bmpInfoHeader[6] = (unsigned char) (w>>16);
	bmpInfoHeader[7] = (unsigned char) (w>>24);
	
	bmpInfoHeader[8] = (unsigned char) (h);
	bmpInfoHeader[9] = (unsigned char) (h>>8);
	bmpInfoHeader[10] = (unsigned char) (h>>16);
	bmpInfoHeader[11] = (unsigned char) (h>>24);
	
	bmpInfoHeader[21] = (unsigned char) (s);
	bmpInfoHeader[22] = (unsigned char) (s>>8);
	bmpInfoHeader[23] = (unsigned char) (s>>16);
	bmpInfoHeader[24] = (unsigned char) (s>>24);
	
	bmpInfoHeader[25] = (unsigned char) (ppm);
	bmpInfoHeader[26] = (unsigned char) (ppm>>8);
	bmpInfoHeader[27] = (unsigned char) (ppm>>16);
	bmpInfoHeader[28] = (unsigned char) (ppm>>24);
	
	bmpInfoHeader[29] = (unsigned char) (ppm);
	bmpInfoHeader[30] = (unsigned char) (ppm>>8);
	bmpInfoHeader[31] = (unsigned char) (ppm>>16);
	bmpInfoHeader[32] = (unsigned char) (ppm>>24);
	
	f = fopen(fileName,"wb");
	
	fwrite(bmpFileHeader,1,14,f);
	fwrite(bmpInfoHeader,1,40,f);
	
	for(int i=0; i<k;i++)
	{
		RGBType rgb = data[i];
		double red = (data[i].r) * 255;
		double green = (data[i].g) * 255;
		double blue = (data[i].b) * 255;
		
		unsigned char color[3] = {(int) floor (blue), (int) floor (green), (int) floor (red)};
		fwrite(color,1,3,f);
		
	}
	fclose(f);
}
/**
  *returns the index of the winning intersection, namely which object is closer to the camera
  @param objectIntersections the array of intersections
**/
int winningObjectIndex (vector<double> objectIntersections)
{
	int indexOfMinimumValue;
	//prevent unnessary calculations by eliminating rays that don't intersect or with hidden intersections
	if (objectIntersections.size() == 0)
	{//no intersections
		return -1;
	}
	else if (objectIntersections.size() == 1)
	{
		if (objectIntersections.at(0) > 0) 
		{//if intersection is larger than 0, then it's our minimum intersection 
			return 0;
		}
	    else
		{//otherwise the only intersection value is negative
			return -1;
		}
	}
	else
	{//more than one intersections, find the maximum value
		double max = 0;
		for (int i = 0; i < objectIntersections.size(); i ++)
		{
			if (max < objectIntersections.at(i))
			{
				max = objectIntersections.at(i);
			}
		}
		//then star from the maximum value find the minimum value
		if( max > 0 )
		{//positive intersections
			for (int index = 0; index < objectIntersections.size(); index ++)
			{
				if ( objectIntersections.at(index) > 0 && objectIntersections.at(index) <= max )
				{
					max = objectIntersections.at(index);
					indexOfMinimumValue = index;
				}
			}
			return indexOfMinimumValue;
		}
		else
		{//all intersections negative
			return -1;
		}
	}
}
/**
  *A function that recursively emits rays to compute each pixel color based on nearby reflecting objects and shadowing conditions
  @param intersectingPos the position of intersection which is the start point of secondary ray tracing towards light sources and nearby specular objects
  @param intersectingRayDir the direction of intersecting ray
  @param sceneObjects all objects in scene
  @param indexOfWinningObject index of the winning object that is the closest to camera
  @param lightSources all light sources
  @param accuracy magic number to adjust calculation errors
  @param ambientLight ambient property of light
**/
color getColorAt(vect intersectingPos, 
                 vect intersectingRayDir, 
				 vector<object*> sceneObjects, 
				 int indexOfWinningObject, 
				 vector<source*> lightSources, 
				 double accuracy, 
				 double ambientLight)
{
	//get color and normal at winning object
	color winningObjectColor  = sceneObjects.at(indexOfWinningObject)->getColor();
	vect  winningObjectNormal = sceneObjects.at(indexOfWinningObject)->getNormalAtPoint(intersectingPos);
	//draw black and white tiles if special is 2
	if (winningObjectColor.getColorSpecial() == 2) {
		// checkered/tile floor pattern
		
		int square = (int)floor(intersectingPos.getVectX()) + (int)floor(intersectingPos.getVectZ());
		
		if ((square % 2) == 0) {
			// black tile
			winningObjectColor.setColorRed(0);
			winningObjectColor.setColorGreen(0);
			winningObjectColor.setColorBlue(0);
		}
		else {
			// white tile
			winningObjectColor.setColorRed(1);
			winningObjectColor.setColorGreen(1);
			winningObjectColor.setColorRed(1);
		}
	}

    color finalColor = winningObjectColor.colorScalar(ambientLight);
	//deal with reflection from nearby objects
	if (winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1) {
		// reflection from other objects with specular intensity
		double dot1 = winningObjectNormal.dotProduct(intersectingRayDir.negative());
		vect scalar1 = winningObjectNormal.vectMult(dot1);
		vect add1 = scalar1.vectAdd(intersectingRayDir);
		vect scalar2 = add1.vectMult(2);
		vect add2 = intersectingRayDir.negative().vectAdd(scalar2);
		vect reflectingDir = add2.normalize();
		
		ray reflectingRay (intersectingPos, reflectingDir);
		
		// determine what the ray intersects with first
		vector<double> reflectingIntersections;
		
		for (int reflectingIndex = 0; reflectingIndex < sceneObjects.size(); reflectingIndex++) {
			reflectingIntersections.push_back(sceneObjects.at(reflectingIndex)->findIntersection(reflectingRay));
		}
		
		int indexOfWinningObjectWithReflection = winningObjectIndex(reflectingIntersections);
		
		if (indexOfWinningObjectWithReflection != -1) {
			// reflection ray missed everthing else
			if (reflectingIntersections.at(indexOfWinningObjectWithReflection) > accuracy) {
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects the color if it reflected off something
				
				vect refInteractingPos = intersectingPos.vectAdd(reflectingDir.vectMult(reflectingIntersections.at(indexOfWinningObjectWithReflection)));
				vect refInteractingRayDir = reflectingDir;
				
				color refInteractingColor = getColorAt(refInteractingPos, refInteractingRayDir, sceneObjects, indexOfWinningObjectWithReflection, lightSources, accuracy, ambientLight);
				
				finalColor = finalColor.colorAdd(refInteractingColor.colorScalar(winningObjectColor.getColorSpecial()));
			}
		}
	}

	for (int lightIndex = 0; lightIndex < lightSources.size(); lightIndex++)
	{
		vect lightDirection = lightSources.at(lightIndex)->getLightPosition().vectAdd(intersectingPos.negative()).normalize();
		
		double cosineAngle  = winningObjectNormal.dotProduct(lightDirection);
		
		if (cosineAngle > 0.0)
		{//test for shadows
			bool shadowed = false;
			
			vect distanceToLight = lightSources.at(lightIndex)->getLightPosition().vectAdd(intersectingPos.negative()).normalize();			
			double distanceToLightMag = distanceToLight.magnitude();
			
			//emitting a ray from intersection to light source
			ray shadowRay (intersectingPos, lightSources.at(lightIndex)->getLightPosition().vectAdd(intersectingPos.negative()).normalize());
			
			vector<double> secondaryIntersections;
			
			for(int objectIndex = 0; objectIndex < sceneObjects.size() && shadowed == false; objectIndex++)
			{
				secondaryIntersections.push_back(sceneObjects.at(objectIndex)->findIntersection(shadowRay));
			}
			for(int i = 0; i < secondaryIntersections.size(); i++)
			{//loop thru secondary intersections. if value is less than distance to light source, then that intersection is in shadow
				if(secondaryIntersections.at(i) > accuracy)
				{
					if(secondaryIntersections.at(i)<= distanceToLightMag)
					{
						shadowed = true;

					}
					break;

				}

			}
			
			if (shadowed == false)
			{
				finalColor = finalColor.colorAdd(winningObjectColor.colorMult(lightSources.at(lightIndex)->getLightColor()).colorScalar(cosineAngle));
				
				if(winningObjectColor.getColorSpecial() > 0 && winningObjectColor.getColorSpecial() <= 1)
				{//if shininess between 0 and 1, create mirror vectors as the reflection direction
					double dot1  = winningObjectNormal.dotProduct(intersectingRayDir.negative());
					vect scalar1 = winningObjectNormal.vectMult(dot1);
					vect add1    = scalar1.vectAdd(intersectingRayDir);
					vect scalar2 = add1.vectMult(2);
					vect add2    = intersectingRayDir.negative().vectAdd(scalar2);
					vect reflectionDir = add2.normalize();
					
					double specular = reflectionDir.dotProduct(lightDirection);
					if (specular > 0)
					{
						specular = pow(specular, 10);
						finalColor = finalColor.colorAdd(lightSources.at(lightIndex)->getLightColor().colorScalar(specular*winningObjectColor.getColorSpecial()));
					}
				}
			}
		}
		
	}
	return finalColor.clip();
}
/**
  *main entry to do rendering
  @param argc    default input argument count
  @param *argv[] default input arguments  
**/
int main(int argc, char *argv[])
{
	cout << "rendering ..." << endl;
	int current;

	int dpi         = 72;
	int width       = 640;
	int height      = 480;
	int n           = height*width;
	RGBType *pixels = new RGBType [n];
	
	int    aaDepth      = 4;//how small you want the pixel to be "divided". 1 means normal, greater than 1 means anti-aliasing
	int    aaDivide     = aaDepth * aaDepth; //how many sub-pixels our aa pixel is divided into
	double aaThreshold  = 0.1;
	double ambientLight = 0.2;
	double accuracy = 0.00000001; //a magic constant to adjust caculations slightly 
    double aspectRatio = (double)width/(double)height;
	
	double xOffset, yOffset;//offsets to set the camera in the center of a pixel
    int aaIndex; //anti-aliasing index
	double aaRedPixel, aaGreenPixel, aaBluePixel;
    //scene basic set-ups
	vect dirX (1, 0, 0);
	vect dirY (0, 1, 0);
	vect dirZ (0, 0, 1);
	vect sphereCenter (0, 0, 0);
	vect sphereCenter2 (0, 1.5, 0);

	vect cameraPos (3, 1.5, -4);
	
	vect lookAt  (0, 0, 0);
	vect diffBetweenCamAndLookPoint (cameraPos.getVectX() - lookAt.getVectX(),
	                                 cameraPos.getVectY() - lookAt.getVectY(),
						             cameraPos.getVectZ() - lookAt.getVectZ());
									 
	vect cameraDirection = diffBetweenCamAndLookPoint.negative().normalize();
	vect cameraRightVect = dirY.crossProduct(cameraDirection).normalize();
	vect cameraDownVect  = cameraRightVect.crossProduct(cameraDirection);
	camera sceneCamera (cameraPos, cameraDirection, cameraRightVect, cameraDownVect);
	
	color whiteLight (1.0, 1.0, 1.0, 0);
	color green (0.5, 1.0, 0.5, 0.3);
	color shinyGray  (0.5, 0.5, 0.5, 0.2);
	color black (0, 0, 0, 0);
	color maroon (0.5, 0.25, 0.25, 0.2);
	color checkerFloor (1, 1, 1, 2);
	color blue (0.25, 0.5, 0.8, 0.2);

	vect lightPosition (-7, 10, -10);
	light sceneLight (lightPosition, whiteLight);
    vector<source*> lightSources;
	lightSources.push_back(dynamic_cast<source*>(&sceneLight));

	//scene objects
	sphere sceneSphere (sphereCenter, 1, green);
	sphere sceneSphere2 (sphereCenter2, 0.5,blue);
	plane scenePlane (dirY, -1, checkerFloor);
	
	vector<object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<object*>(&sceneSphere));
	sceneObjects.push_back(dynamic_cast<object*>(&sceneSphere2));
	sceneObjects.push_back(dynamic_cast<object*>(&scenePlane));
	
	double aaRed[aaDivide];
	double aaGreen[aaDivide];
	double aaBlue[aaDivide];
	
	
	for (int x=0; x<width;x++)
	{
		for(int y=0; y<height; y++)
		{
			current = y*width + x;
			//temporary pixel for anti-aliasing
			//anti-alasing loop starts
			for (int i = 0; i < aaDepth; i++)
			{
				for(int j = 0; j < aaDepth; j++)
				{
					aaIndex = j*aaDepth + i;
					//create the ray from the camera to this pixel
					
					if(aaDepth==1)
					{
						//start with no anti-aliasing
						if (width > height) 
						{//the image's width is larger than its height
							xOffset = ((x+0.5)/(double)width)*aspectRatio - (((width - height)/(double) height)/2);
							yOffset = ((height - y) + 0.5) / height;
						}
						else if (height > width)
						{//the height is larger than width
							xOffset = (x + 0.5) / width;
							yOffset = (((height-y)+0.5)/height) / aspectRatio - (((height - width) / (double) width)/2);
						}
						else 
						{
							xOffset = (x + 0.5)/width;
							yOffset = ((height - y) + 0.5)/height;
						}
					}
					else if (aaDepth >1)
					{
						//anti-aliasing
						if (width > height) 
						{//the image's width is larger than its height
							xOffset = ((x+(double)i/((double)aaDepth-1))/(double)width)*aspectRatio - (((width - height)/(double) height)/2);
							yOffset = ((height - y) + (double)i/((double)aaDepth-1)) / height;
						}
						else if (height > width)
						{//the height is larger than width
							xOffset = (x + (double)i/((double)aaDepth-1)) / width;
							yOffset = (((height-y)+(double)i/((double)aaDepth-1))/height) / aspectRatio - (((height - width) / (double) width)/2);
						}
						else 
						{
							xOffset = (x + (double)i/((double)aaDepth-1))/width;
							yOffset = ((height - y) + (double)i/((double)aaDepth-1))/height;
						}

						
					}
					vect camRayOrigin    = sceneCamera.getCameraPosition();
					vect camRayDirection = cameraDirection.vectAdd(cameraRightVect.vectMult(xOffset - 0.5).vectAdd(cameraDownVect.vectMult(yOffset - 0.5))).normalize();
					
					ray camRay (camRayOrigin, camRayDirection);
					
					vector<double> intersections;
					
					for (int index = 0; index < sceneObjects.size(); index++)
					{
						intersections.push_back(sceneObjects.at(index)->findIntersection(camRay));
					}
					
					int indexOfWinningObject = winningObjectIndex (intersections);
					
					if(indexOfWinningObject == -1) 
					{//set background black
						aaRed[aaIndex] = 0;
						aaGreen[aaIndex] = 0;
						aaBlue[aaIndex] = 0;
						
					}
					else
					{
						//index each of our objects in the scene
						if(intersections.at(indexOfWinningObject) > accuracy)
						{//caculation position and directions vectors at intersections
							
							vect intersectionPosition = camRayOrigin.vectAdd(camRayDirection.vectMult(intersections.at(indexOfWinningObject)));
							vect intersectingRayDirection = camRayDirection;
							
							color intersectionColor = getColorAt(intersectionPosition, 
																 intersectingRayDirection, 
																 sceneObjects, 
																 indexOfWinningObject, 
																 lightSources,
																 accuracy, 
																 ambientLight);
							
							aaRed[aaIndex] = intersectionColor.getColorRed();
							aaGreen[aaIndex] = intersectionColor.getColorGreen();
							aaBlue[aaIndex] = intersectionColor.getColorBlue();
						}
					}
				}
			}
		    //now we calculate the average of pixel color
		    double redSum = 0;
			double greenSum = 0;
			double blueSum = 0;
			
			for (int indexColor = 0; indexColor < aaDivide; indexColor++)
			{
				redSum = redSum + aaRed[indexColor];
				greenSum = greenSum + aaGreen[indexColor];
				blueSum = blueSum + aaBlue[indexColor];
				
			}
			double finalRed   = redSum / (aaDivide);
			double finalGreen = greenSum / (aaDivide);
			double finalBlue  = blueSum / (aaDivide);
			
			pixels[current].r = finalRed;
			pixels[current].g = finalGreen;
			pixels[current].b = finalBlue;
			
			
		}
	}
	
	saveBmp("scene_aa.bmp", width, height, dpi, pixels);
	delete pixels;
	return 0;
}
