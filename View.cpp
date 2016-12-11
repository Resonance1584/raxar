#include "View.h"

View::View(Point3 eyePosition, Point3 lookAtPoint, Vector3 upVector,
           double fieldOfView, int imageWidth, int imageHeight) {

  eyePoint = eyePosition;
  lookPoint = lookAtPoint;
  viewUp = unit(upVector);
  fov = fieldOfView;
  width = imageWidth;
  height = imageHeight;

  n = unit(eyePoint - lookPoint);
  u = unit(cross(viewUp, n));
  v = cross(n, u);
}

Ray3 View::createRay(float column, float row) {

  double x = column - (width / 2);
  double y = row - (height / 2);

  Point3 VPC = eyePoint - n;

  double pxWidth = 2 * tan(((fov / 2.0) * PI) / 180) / width;

  Point3 pixelPosition = VPC + (u * x * pxWidth) + (v * y * pxWidth);

  Ray3 ray = Ray3(eyePoint, unit(pixelPosition - eyePoint));

  return ray;
}