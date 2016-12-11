#pragma once

#include "GeomX.h"
#include "Illumination.h"
#include <list>
#include <vector>

/*
 * Abstract class shape
 * Defines that all shapes have a material, return a normal at a point,
 * and have an intersect method
 */
class Shape {
protected:
  Material material;

public:
  Material getMaterial() { return material; }
  virtual Vector3 normal(Point3 p) = 0;
  /*
   * MUST check that result is postive.
   */
  virtual double intersect(Ray3 r) = 0;

  virtual bool isSphere() { return false; }

  virtual Point3 getPoint() = 0;
};

/*
 * Sphere
 */
class Sphere : public Shape {
  Point3 centre;
  double radius;

public:
  Sphere(Point3 centre, double radius, Material mat);
  Vector3 normal(Point3 p);
  double intersect(Ray3 r);
  bool isSphere() { return true; }
  Point3 getPoint() { return centre; }
};

/*
 * Plane
 */
class Plane : public Shape {

  Vector3 norm;
  Point3 point;

public:
  Plane(){};
  Plane(Point3 point, Vector3 normal, Material mat);
  Vector3 normal(Point3 p);
  double intersect(Ray3 r);
  Point3 getPoint();
};

/*
 * Triangles contain a plane, but check against their own
 * geometry for intersections.
 */
class Triangle : public Shape {
  Point3 point1;
  Point3 point2;
  Point3 point3;

  Plane internalPlane;

public:
  Triangle(Point3 p1, Point3 p2, Point3 p3, Material mat);
  Vector3 normal(Point3 p);
  double intersect(Ray3 r);
  Point3 getPoint();
};

/*
 * Square
 * Defined by two coordinates and a normal
 * These are much faster to calculate a hit than two triangles
 */
class Square : public Shape {
  double minX, maxX, minY, maxY, minZ, maxZ;

  Plane internalPlane;

public:
  Square(Point3 point1, Point3 point2, Vector3 direction, Material mat);
  Vector3 normal(Point3 p);
  double intersect(Ray3 r);
  Point3 getPoint();
};

/*
 * Cubes are made of 6 squares
 * Intersections require each square to be tested
 * Complexity can be reduced with removeBackFaces
 */
class Cube : public Shape {
  Point3 origin;
  double width;
  vector<Square *> squares;
  Square *lastHit;

public:
  Cube(Point3 p, double size, Material mat);
  Vector3 normal(Point3 p);
  double intersect(Ray3 r);
  void removeBackFaces(Point3 eyePoint);
  Point3 getPoint() { return origin; }
};

/*
 * A Polyhedron is a collection of triangles
 * Complexity can be reduced with removeBackFaces
 * This is very similar to Cube's implementation
*/
class Polyhedron : public Shape {
  vector<Triangle *> polys;
  Triangle *lastHit;

public:
  Polyhedron(vector<Triangle *> polygons, Material mat);
  Vector3 normal(Point3 p);
  double intersect(Ray3 r);
  void removeBackFaces(Point3 eyePoint);
  Point3 getPoint() { return Point3(0, 0, 0); }
};

/*
        This code can create a cube from Triangles.
        This could be achieved in the Polyhdron class.
        //Front face
        polygons.push_back(new Triangle(frontLeft,frontLeft + right(d),frontLeft
   + up(d),mat));
        polygons.push_back(new
   Triangle(frontLeft+right(d),frontLeft+right(d)+up(d),frontLeft+up(d),mat));
        //Left face
        polygons.push_back(new Triangle(frontLeft, frontLeft + far(d) +
   up(d),frontLeft + far(d),mat));
        polygons.push_back(new Triangle(frontLeft,frontLeft + up(d),frontLeft +
   far(d) + up(d),mat));
        //Back face
        polygons.push_back(new Triangle(backRight, backRight + left(d) +
   down(d),backRight + left(d),mat));
        polygons.push_back(new Triangle(backRight,backRight + down(d), backRight
   + down(d) + left(d),mat));
        //Right face
        polygons.push_back(new Triangle(backRight, backRight + down(d) +
   near(d),backRight + down(d),mat));
        polygons.push_back(new Triangle(backRight,backRight + near(d), backRight
   + down(d) + near(d),mat));
        //Top face
        polygons.push_back(new Triangle(backRight,backRight + left(d), backRight
   + left(d) + near(d),mat));
        polygons.push_back(new Triangle(backRight, backRight + near(d) +
   left(d),backRight + near(d),mat));
        //Bottom face
        polygons.push_back(new Triangle(frontLeft,frontLeft + far(d),frontLeft +
   far(d) + right(d),mat));
        polygons.push_back(new Triangle(frontLeft, frontLeft + right(d) +
   far(d),frontLeft + right(d),mat));
        */