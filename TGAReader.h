// Taken from
// http://steinsoft.net/index.php?site=Programming/Code%20Snippets/Cpp/no8
// Reads TGA files

#pragma once

#include <stdio.h>

struct STGA {
  STGA() {
    data = (unsigned char *)0;
    width = 0;
    height = 0;
    byteCount = 0;
  }

  // Had to disable delete as we were losing reference
  // after reading one texel
  ~STGA() { // delete[] data; data = 0;
  }

  void destroy() { // delete[] data; data = 0;
  }

  int width;
  int height;
  unsigned char byteCount;
  unsigned char *data;
};

bool loadTGA(const char *filename, STGA &tgaFile);