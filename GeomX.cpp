//
//  GeomX.cpp
//  RaXar
//  Max Brosnahan and Lewis Christie

// using namespace std;

#include "GeomX.h"

double epsilon = 1.e-10;

/*
 * Point3
 */

Point3::Point3(double value) {
  this->x = value;
  this->y = value;
  this->z = value;
}

Point3::Point3(double xP, double yP, double zP) : x(xP), y(yP), z(zP) {}

Point3::Point3(Vector3 vector) {
  this->x = vector.getXDir();
  this->y = vector.getYDir();
  this->z = vector.getZDir();
}

// getters

double Point3::getX() { return this->x; }

double Point3::getY() { return this->y; }
double Point3::getZ() { return this->z; }

// operators

Point3 &Point3::operator=(Point3 rhs) {
  this->x = rhs.getX();
  this->y = rhs.getY();
  this->z = rhs.getZ();
  return *this;
}

Point3 Point3::operator+(Vector3 rhs) {
  return Point3(this->getX() + rhs.getXDir(), this->getY() + rhs.getYDir(),
                this->getZ() + rhs.getZDir());
}

Vector3 Point3::operator-(Point3 rhs) {

  // So confused by this one
  // return Vector3(rhs.getX() - this->getX(), rhs.getY()  - this->getY(),
  // rhs.getZ() - this->getZ());
  return Vector3(this->getX() - rhs.getX(), this->getY() - rhs.getY(),
                 this->getZ() - rhs.getZ());
}

bool Point3::operator==(Point3 rhs) {
  if (fabs(this->x - rhs.x) < epsilon && fabs(this->y - rhs.y) < epsilon &&
      fabs(this->z - rhs.z) < epsilon) {
    return true;
  }
  return false;
}

bool Point3::operator!=(Point3 rhs) { return !this->operator==(rhs); }

/*
 * Vector3
 */

Vector3::Vector3(double value) {
  this->dirX = value;
  this->dirY = value;
  this->dirZ = value;
}

Vector3::Vector3(double xDir, double yDir, double zDir) {
  this->dirX = xDir;
  this->dirY = yDir;
  this->dirZ = zDir;
}

Vector3::Vector3(Point3 point) {
  this->dirX = point.getX();
  this->dirY = point.getY();
  this->dirZ = point.getZ();
}

// getters

double Vector3::getXDir() { return this->dirX; }
double Vector3::getYDir() { return this->dirY; }
double Vector3::getZDir() { return this->dirZ; }

// operators

bool Vector3::operator==(Vector3 rhs) {
  if (fabs(this->dirX - rhs.getXDir()) < epsilon &&
      fabs(this->dirY - rhs.getYDir()) < epsilon &&
      fabs(this->dirZ - rhs.getZDir()) < epsilon) {
    return true;
  }
  return false;
}

bool Vector3::operator!=(Vector3 rhs) { return !this->operator==(rhs); }

Vector3 &Vector3::operator=(Vector3 rhs) {
  this->dirX = rhs.getXDir();
  this->dirY = rhs.getYDir();
  this->dirZ = rhs.getZDir();
  return *this;
}

Vector3 Vector3::operator*(double scalar) {
  return Vector3(this->dirX * scalar, this->dirY * scalar, this->dirZ * scalar);
}

Vector3 Vector3::operator+(Vector3 rhs) {
  return Vector3(this->dirX + rhs.getXDir(), this->dirY + rhs.getYDir(),
                 this->dirZ + rhs.getZDir());
}
Vector3 Vector3::operator-(Vector3 rhs) {
  return Vector3(this->dirX - rhs.getXDir(), this->dirY - rhs.getYDir(),
                 this->dirZ - rhs.getZDir());
}

Vector3 Vector3::operator-() { // negation
  return Vector3(-this->dirX, -this->dirY, -this->dirZ);
}

Vector3 Vector3::operator/(double scalar) {
  return Vector3(this->dirX / scalar, this->dirY / scalar, this->dirZ / scalar);
}

Vector3 Vector3::norm() { return *this / this->length(); }
Vector3 Vector3::unit() { return this->norm(); }

double Vector3::dot(Vector3 rhs) {
  return this->dirX * rhs.getXDir() + this->dirY * rhs.getYDir() +
         this->dirZ * rhs.getZDir();
}

Vector3 Vector3::cross(Vector3 rhs) {
  return Vector3(this->dirY * rhs.getZDir() - this->dirZ * rhs.getYDir(),
                 this->dirZ * rhs.getXDir() - this->dirX * rhs.getZDir(),
                 this->dirX * rhs.getYDir() - this->dirY * rhs.getXDir());
}

double Vector3::length() {
  return sqrt(this->dirX * this->dirX + this->dirY * this->dirY +
              this->dirZ * this->dirZ);
}

/*
 * Ray3
 */

Ray3::Ray3(Point3 startP, Vector3 directionV) {
  start = Point3(startP.getX(), startP.getY(), startP.getZ());
  direction =
      Vector3(directionV.getXDir(), directionV.getYDir(), directionV.getZDir());
}

Point3 Ray3::pos(double alpha) {
  Point3 rtrn = this->start;
  return rtrn + this->direction * alpha;
}

Ray3 &Ray3::operator=(Ray3 rhs) {
  this->start = rhs.start;
  this->direction = rhs.direction;
  return *this;
}

bool Ray3::operator==(Ray3 rhs) {
  return (this->start == rhs.start) && (this->direction == rhs.direction);
}

bool Ray3::operator!=(Ray3 rhs) { return !(*this == rhs); }

/*
 * Line3
 */

Line3 &Line3::operator=(Line3 rhs) {
  this->p1 = rhs.p1;
  this->p2 = rhs.p2;
  return *this;
}

bool Line3::operator==(Line3 rhs) {
  return (this->p1 == rhs.p1 && this->p2 == rhs.p2);
}

bool Line3::operator!=(Line3 rhs) { return !(*this == rhs); }

/*
 * Global functions
 */

double dot(Vector3 v1, Vector3 v2) { return v1.dot(v2); }

Vector3 cross(Vector3 v1, Vector3 v2) { return v1.cross(v2); }

double length(Vector3 v) { return sqrt(v.dot(v)); }

Vector3 unit(Vector3 v) { return v / length(v); }

Vector3 norm(Vector3 v) { return unit(v); }

int testGeom() {
  Vector3 v1 = Vector3(1, 2, 3);
  Vector3 v2 = Vector3(3, 2, 1);

  assert(true);
  assert(1 == 1);

  assert(Vector3(1, 2, 3) == v1);
  assert(Vector3(Point3(1, 2, 3)) == v1);
  assert(v1 + v2 == Vector3(4, 4, 4));
  assert(v1 - v2 == Vector3(-2, 0, 2));
  assert(v1 * 3 == Vector3(3, 6, 9));
  assert(v1 * 3.0 == Vector3(3, 6, 9));
  assert(v1 / 2.0 == Vector3(0.5, 1, 1.5));
  assert(-v1 == Vector3(-1, -2, -3));
  assert(v1.getXDir() == 1 && v1.getYDir() == 2 && v1.getZDir() == 3);

  assert(v1.dot(v2) == 10);
  assert(v1.dot(v2) == dot(v1, v2));
  assert(v1.cross(v2) == Vector3(-4, 8, -4));
  assert(length(unit(Vector3(2, 3, 4))) == 1.0);
  assert(length(Vector3(2, 3, 4).norm()) == 1.0);

  Point3 p1 = Point3(2, 4, 6);
  Point3 p2 = Point3(4, 7, 3);
  assert(Point3(2, 4, 6) == p1);
  assert(Point3(Vector3(2, 4, 6)) == p1);
  assert(p1 - p2 == Vector3(-2, -3, 3));
  assert(p1 + v1 == Point3(3, 6, 9));

  return 0;
}

Vector3 left(double distance) { return Vector3(-1, 0, 0); }
Vector3 right(double distance) { return Vector3(1, 0, 0); }
Vector3 up(double distance) { return Vector3(0, 1, 0); }
Vector3 down(double distance) { return Vector3(0, -1, 0); }
Vector3 far(double distance) { return Vector3(0, 0, -1); }
Vector3 near(double distance) { return Vector3(0, 0, 1); }
