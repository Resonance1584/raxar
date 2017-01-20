#include "Shapes.h"

Sphere::Sphere(Point3 centre, double radius, Material mat) {
  this->centre = centre;
  this->radius = radius;
  this->material = mat;
}
Vector3 Sphere::normal(Point3 p) { return unit(p - centre); }
double Sphere::intersect(Ray3 r) {

  double t = -1;
  Vector3 q = centre - r.startP();
  double vDotQ = dot(r.directionV(), q);
  double squareDiffs = dot(q, q) - (radius * radius);
  double discrim = (vDotQ * vDotQ) - squareDiffs;

  if (discrim >= 0) {
    double root = sqrt(discrim);
    double t0 = (vDotQ - root);
    double t1 = (vDotQ + root);
    if (t0 > 0)
      t = t0;
    else if (t1 > 0)
      t = t1;
  }

  return t;
}
Colour Sphere::getColour(Point3 position, Vector3 normal, Lighting *light,
                         Vector3 inverseRay, bool isShadowed) {

  double u = 0.0;
  double v = 0.0;

  if (this->material.isTex()) {
    // UV Calculation
    // http://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html

    // This calculates the uv coordinates for a sphere
    // First we take three vectors
    // unit vector pointing up (vertical axis)
    Vector3 vn = Vector3(0, 1, 0);
    // unit vector pointing left (horizontal axis)
    Vector3 ve = Vector3(1, 0, 0);
    // unit vector from center to hit point (hit axis)
    Vector3 vp = unit(position - this->getPoint());

    // Inverse cosine of dot(vn,vp) gives us the angle
    // between the vertical axis and the hit axis
    // this is our latitude
    double phi = acos(-dot(vn, vp));

    // Vary v between zero and one (divide by half a circle)
    v = phi / PI;

    // Find the longitude, using the latitude
    double theta = (acos(dot(vp, ve) / sin(phi)) / (2 * PI));
    if (dot(cross(vn, ve), vp) > 0) {
      u = theta;
    } else {
      u = 1 - theta;
    }
  }

  return this->material.lit_colour(position, u, v, normal, light, inverseRay,
                                   isShadowed);
}

Plane::Plane(Point3 point, Vector3 normal, Material mat) {
  this->point = point;
  this->norm = normal;
  this->material = mat;
}
Vector3 Plane::normal(Point3 p) { return norm; }
double Plane::intersect(Ray3 r) {
  double t = -1;
  double top = dot((point - r.startP()), norm);
  double bot = dot(r.directionV(), norm);

  t = top / bot;

  return t;
}
Point3 Plane::getPoint() { return point; }
Colour Plane::getColour(Point3 position, Vector3 normal, Lighting *light,
                        Vector3 inverseRay, bool isShadowed) {
  // TODO: Allow for planes along arbitrary axis
  return this->material.lit_colour(position, position.getX(), position.getZ(),
                                   normal, light, inverseRay, isShadowed);
}

Triangle::Triangle(Point3 p1, Point3 p2, Point3 p3, Material mat) {
  this->point1 = p1;
  this->point2 = p2;
  this->point3 = p3;

  this->material = mat;

  // Construct the norm for our plane
  Vector3 norm = cross((p3 - p2), (p1 - p2));

  // Note that the material field of plane is not used
  this->internalPlane = Plane(p1, norm, mat);
}
Vector3 Triangle::normal(Point3 p) { return this->internalPlane.normal(p); }
/*
 * This intersect method uses Barycentric coordinates to
 * test if a point is inside our triangle.
 */
double Triangle::intersect(Ray3 r) {
  double t = internalPlane.intersect(r);

  // Barycentric point inside triangle
  if (t > 0) {
    Point3 hit = r.startP() + (r.directionV() * t);

    // Vectors describing the point
    Vector3 v0 = point2 - point1;
    Vector3 v1 = point3 - point1;
    Vector3 v2 = hit - point1;

    // Dot products
    double dot00 = dot(v0, v0);
    double dot01 = dot(v0, v1);
    double dot02 = dot(v0, v2);
    double dot11 = dot(v1, v1);
    double dot12 = dot(v1, v2);

    // Baycentric coordinates (u and v)
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // If the point can be described by moving u units of
    // one vector, and v of the other, but not further, it
    // is inside the triangle.
    if ((u > 0) && (v > 0) && ((u + v) < 1)) {
      return t;
    } else {
      return -1;
    }
  }

  return -1;
}
Point3 Triangle::getPoint() { return this->point1; }
Colour Triangle::getColour(Point3 position, Vector3 normal, Lighting *light,
                           Vector3 inverseRay, bool isShadowed) {
  // TODO: Allow for textured triangles
  return this->material.lit_colour(position, 0.0, 0.0, normal, light,
                                   inverseRay, isShadowed);
}

/*
 * Square
 */
Square::Square(Point3 point1, Point3 point2, Vector3 direction, Material mat) {

  this->material = mat;
  this->minX = min(point1.getX(), point2.getX());
  this->maxX = max(point1.getX(), point2.getX());
  this->minY = min(point1.getY(), point2.getY());
  this->maxY = max(point1.getY(), point2.getY());
  this->minZ = min(point1.getZ(), point2.getZ());
  this->maxZ = max(point1.getZ(), point2.getZ());

  this->internalPlane = Plane(point1, unit(direction), mat);
}
Vector3 Square::normal(Point3 p) { return this->internalPlane.normal(p); }
/*
 * This intersection uses simple bounds checking to test
 * if a point is inside our square
 */
double Square::intersect(Ray3 r) {
  double t = internalPlane.intersect(r);

  // Check that the intersect is inside the bounds
  if (t > 0) {
    Point3 hit = r.startP() + (r.directionV() * t);

    // Adding and subtracting an epsilon to account for rounding error
    if (hit.getX() >= (this->minX - 10e-9) &&
        hit.getX() <= (this->maxX + 10e-9) &&
        hit.getY() >= (this->minY - 10e-9) &&
        hit.getY() <= (this->maxY + 10e-9) &&
        hit.getZ() >= (this->minZ - 10e-9) &&
        hit.getZ() <= (this->maxZ + 10e-9)) {
      return t;
    } else {
      return -1;
    }
  }

  return -1;
}
Point3 Square::getPoint() { return internalPlane.getPoint(); }
Colour Square::getColour(Point3 position, Vector3 normal, Lighting *light,
                         Vector3 inverseRay, bool isShadowed) {
  // TODO: Allow for textured triangles
  return this->material.lit_colour(position, 0.0, 0.0, normal, light,
                                   inverseRay, isShadowed);
}

Cube::Cube(Point3 p, double size, Material mat) {
  this->origin = p;
  this->width = size;
  this->material = mat;

  double d = this->width;

  Point3 frontLeft =
      Point3(p.getX() - width / 2, p.getY() - width / 2, p.getZ() + width / 2);
  Point3 backRight = frontLeft + (right(d) + up(d) + far(d));

  // Front
  squares.push_back(
      new Square(frontLeft, frontLeft + right(d) + up(d), near(d), mat));
  // Left
  squares.push_back(
      new Square(frontLeft, frontLeft + far(d) + up(d), left(d), mat));
  // Back
  squares.push_back(
      new Square(backRight, backRight + down(d) + left(d), far(d), mat));
  // Right
  squares.push_back(
      new Square(backRight, backRight + near(d) + down(d), right(d), mat));
  // Top
  squares.push_back(
      new Square(backRight, backRight + near(d) + left(d), up(d), mat));
  // Bot
  squares.push_back(
      new Square(frontLeft, frontLeft + far(d) + right(d), down(d), mat));

  // Improves chances of getting both intersections in a row (by about a second,
  // dont forget worst case still occurs)
  random_shuffle(squares.begin(), squares.end());

  lastHit = squares.front();
}
Vector3 Cube::normal(Point3 p) { return lastHit->normal(p); }
/*
 * Checks each square for an intersection
 * Returns best hit or -1
 */
double Cube::intersect(Ray3 r) {
  double best = -1;

  // We can stop checking if we have intersected two polygons
  int hits = 0;

  for (vector<Square *>::iterator it = squares.begin(); it != squares.end();
       it++) {
    if (hits >= 2)
      break; // makes about 1 seconds difference
    Square *itShape = *it;
    double t = itShape->intersect(r);
    if (t > 0) {
      if (best < 0 || (best > 0 && t < best)) {
        best = t;
        lastHit = itShape;
      }
      hits++;
    }
  }
  return best;
}
/*
 * Removes (culls) any faces of the cube that are not visible
 * from the current position. Needs to be called during scene
 * definition, as it should not be used if a reflection could
 * reach the back of the cube.
 */
void Cube::removeBackFaces(Point3 eyePoint) {
  vector<Square *> culledPolys;
  for (vector<Square *>::iterator it = squares.begin(); it != squares.end();
       it++) {
    Square *itShape = *it;
    Vector3 v = unit(eyePoint - itShape->getPoint());
    if (dot(itShape->normal(itShape->getPoint()), v) >=
        0) { // Our check that it is visible
      culledPolys.push_back(itShape);
    }
  }

  this->squares = culledPolys;
}
Colour Cube::getColour(Point3 position, Vector3 normal, Lighting *light,
                       Vector3 inverseRay, bool isShadowed) {
  // TODO: Allow for textured triangles
  return this->material.lit_colour(position, 0.0, 0.0, normal, light,
                                   inverseRay, isShadowed);
}

Polyhedron::Polyhedron(vector<Triangle *> polygons, Material mat) {

  this->polys = polygons;
  this->material = mat;

  this->lastHit = polys.front();
}
Vector3 Polyhedron::normal(Point3 p) { return lastHit->normal(p); }
/*
 * Checks each triangle for intersection
 * returns best hit or -1
 */
double Polyhedron::intersect(Ray3 r) {
  double best = -1;

  for (vector<Triangle *>::iterator it = polys.begin(); it != polys.end();
       it++) {
    Triangle *tri = *it;
    double t = tri->intersect(r);
    if (t > 0) {
      if (best < 0 || (best > 0 && t < best)) {
        best = t;
        lastHit = tri;
      }
    }
  }
  return best;
}
/*
 * Same as cube removeBackFaces
 */
void Polyhedron::removeBackFaces(Point3 eyePoint) {
  vector<Triangle *> culledPolys;
  for (vector<Triangle *>::iterator it = polys.begin(); it != polys.end();
       it++) {
    Triangle *itShape = *it;
    Vector3 v = unit(eyePoint - itShape->getPoint());
    if (dot(itShape->normal(itShape->getPoint()), v) >=
        0) { // Our check that it is visible
      culledPolys.push_back(itShape);
    }
  }

  this->polys = culledPolys;
}
Colour Polyhedron::getColour(Point3 position, Vector3 normal, Lighting *light,
                             Vector3 inverseRay, bool isShadowed) {
  // TODO: Allow for textured triangles
  return this->material.lit_colour(position, 0.0, 0.0, normal, light,
                                   inverseRay, isShadowed);
}
