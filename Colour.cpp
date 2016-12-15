#include "Colour.h"

Colour::Colour(double r, double g, double b) {
  this->r = r;
  this->g = g;
  this->b = b;
  constrain();
}

Colour Colour::modulate(Colour other) {

  double nR = r * other.red();
  double nG = g * other.green();
  double nB = b * other.blue();

  return Colour(nR, nG, nB);
}

Colour Colour::operator*(double factor) {

  return Colour(r * factor, g * factor, b * factor);
}

Colour Colour::operator/(double factor) {

  return Colour(r / factor, g / factor, b / factor);
}

Colour Colour::operator+(Colour other) {

  double nR = r + other.red();
  double nG = g + other.green();
  double nB = b + other.blue();

  return Colour(nR, nG, nB);
}

Colour &Colour::operator+=(Colour other) {

  this->r += other.red();
  this->g += other.green();
  this->b += other.blue();
  constrain();
  return *this;
}

double Colour::red() { return r; }

double Colour::green() { return g; }

double Colour::blue() { return b; }
