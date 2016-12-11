//
//  GeomX.h
//  RaXar
//  Port of Geom3.py

#pragma once

#include <cmath>
#include <assert.h>
#include <iostream>

class Vector3;
class Point3;
class Ray3;
class Line3;


/*
 * Point3
 */

class Point3{

    double x;
    double y;
    double z;

    public:

    double getX();
    double getY();
    double getZ();

	Point3(){}

    Point3(double value);

    Point3(double xP, double yP, double zP);

	Point3(Vector3 vector);

    Point3& operator=(Point3 rhs);
    Point3 operator+(Vector3 rhs);
    Vector3 operator-(Point3 rhs);
    bool operator==(Point3 rhs);
    bool operator!=(Point3 rhs);

    //Point3 operator-(Vector3 rhs);
};



/*
 * Vector3
 */

class Vector3{

    double dirX;
    double dirY;
    double dirZ;


    public:

	Vector3(){}

    Vector3(double xDir, double yDir, double zDir);
    Vector3(double value);
	Vector3(Point3 point);


    double getXDir();
    double getYDir();
    double getZDir();

    bool operator==(Vector3 rhs);
    bool operator!=(Vector3 rhs);

    Vector3& operator=(Vector3 rhs);

    Vector3 operator+(Vector3 rhs);
    Vector3 operator-(Vector3 rhs);
    Vector3 operator-(); //negation

    Vector3 operator*(double scalar);
    Vector3 operator/(double scalar);

    Vector3 norm();
    Vector3 unit();

    double dot(Vector3 rhs);
    Vector3 cross(Vector3 rhs);
    double length();
};




/*
* Ray3
* Ray3 is defined by a starting point and a direction
*/
class Ray3{

    //Private data
    Point3 start;
    Vector3 direction; //Unit vector

public:
    /*
     * Ray3 Constructor
     */
	Ray3(){}
    Ray3(Point3 startP, Vector3 directionV);

    /*
     * Returns a Point3 on the ray at alpha*direction
     */
    Point3 pos(double alpha);

	Point3 startP(){
		return start;
	}

	Vector3 directionV(){
		return direction;
	}

    /*
     * Overidden operators
     */
    Ray3& operator=(Ray3 rhs);

    bool operator==(Ray3 rhs);

    bool operator!=(Ray3 rhs);
};


/*
 * A line is defined by two points
 */
class Line3{

    //Private data
    Point3 p1;
    Point3 p2;

public:
    /*
     * Line3 constructor
     */
	Line3(){}
    Line3(Point3 p1, Point3 p2);

    /*
     * Point at position alpha on the line
     */
    Point3 pos(double alpha);

    /*
     * Overidden operators
     */
    Line3& operator=(Line3 rhs);

    bool operator==(Line3 rhs);

    bool operator!=(Line3 rhs);
};

/*
 * Global functions
 */

double dot(Vector3 v1, Vector3 v2);

Vector3 cross(Vector3 v1, Vector3 v2);

double length(Vector3 v);

Vector3 unit(Vector3 v);

Vector3 norm(Vector3 v);

int testGeom();

Vector3 left(double distance);
Vector3 right(double distance);
Vector3 up(double distance);
Vector3 down(double distance);
Vector3 far(double distance);
Vector3 near(double distance);
