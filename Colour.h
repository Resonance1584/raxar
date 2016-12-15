#pragma once

//
//	Colour.h
//	RaXaR
//	Port of Colour.py by Richard Lobb
#include <algorithm>

using namespace std;

class Colour {

  double r;
  double g;
  double b;

  void constrain() {
    r = max(0.0, min(r, 1.0));
    g = max(0.0, min(g, 1.0));
    b = max(0.0, min(b, 1.0));
  }

public:
  Colour(){};

  Colour(double r, double g, double b);

  Colour modulate(Colour other);

  Colour operator*(double factor);

  Colour operator/(double divisor);

  Colour operator+(Colour other);

  Colour &operator+=(Colour other);

  double red();

  double green();

  double blue();
};
