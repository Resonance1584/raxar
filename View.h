/*
 * Contains the View class, which is responsible
 * for storing camera information and generating rays.
 */

#pragma once

#include "GeomX.h"
#include "pi.h"

#include <math.h>

class View{

	Point3 eyePoint;
	Point3 lookPoint;
	Vector3 viewUp;
	double fov;
	int width;
	int height;

	//Camera coordinate system
	Vector3 n;
	Vector3 u;
	Vector3 v;

public:
	//Empty constructor allows us to pass a View object as an argument
	View(){};

	//Creates a view
	View(Point3 eyePosition, Point3 lookAtPoint, Vector3 upVector, double fieldOfView, int imageWidth, int imageHeight);

	//Generates a ray within the viewplane
	Ray3 createRay(float column, float row);
};