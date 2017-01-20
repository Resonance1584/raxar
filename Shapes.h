#pragma once

#include "GeomX.h"
#include "Illumination.h"
#include "pi.h"
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

  virtual Point3 getPoint() = 0;

  virtual Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                           Vector3 inverseRay, bool isShadowed) = 0;
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
  Point3 getPoint() { return centre; }
  Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                   Vector3 inverseRay, bool isShadowed);
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
  Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                   Vector3 inverseRay, bool isShadowed);
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
  Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                   Vector3 inverseRay, bool isShadowed);
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
  Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                   Vector3 inverseRay, bool isShadowed);
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
  Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                   Vector3 inverseRay, bool isShadowed);
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
  Colour getColour(Point3 position, Vector3 normal, Lighting *light,
                   Vector3 inverseRay, bool isShadowed);
};
