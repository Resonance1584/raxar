#pragma once

#include "Colour.h"

#include <fstream>
#include <time.h>

using namespace std;

class TGAWriter {

  int width;
  int height;
  float *data;
  int currentPixel;

public:
  TGAWriter(int nWidth, int nHeight);

  void putNextPixel(float red, float green, float blue);

  void putNextPixel(Colour c);

  bool writeImage();

  ~TGAWriter();
};
