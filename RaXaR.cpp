/* RaXaR - A C++ Ray Tracer
 * Lewis Christie
 *
 * This file contains the main entry point,
 * which includes the tracing iteration.
 */

#include "GeomX.h"
#include "Illumination.h"
#include "Shapes.h"
#include "TGAReader.h"
#include "TGAWriter.h"
#include "View.h"
#include "pi.h"

#include <list>
#include <vector>

// Image size definition
#define WIDTH 1920
#define HEIGHT 1080

// Lighting values
#define LIGHT_DIR unit(Vector3(1, 5, 2))
#define LIGHT_DIR2 unit(Vector3(-1, 5, 0))
#define LIGHT_INTENS 0.4
#define AMBIENT 0.05

// Camera coordinates
#define EYEPOINT Point3(3, 2, 4)
#define LOOKAT Point3(0, 1, 0)
#define FOV 60
#define VIEW_UP Vector3(0, 1.0, 0)

// Material Colours
#define SHINY_RED                                                              \
  Material(Colour(0.7, 0.3, 0.2), Colour(0.4, 0.4, 0.4), 100, 0.25, 1.0, 1.0)
#define SHINY_BLUE                                                             \
  Material(Colour(0.2, 0.3, 0.7), Colour(0.8, 0.8, 0.8), 200, 0.5, 1.0, 1.0)
#define MATT_GREEN                                                             \
  Material(Colour(0.1, 0.7, 0.1), Colour(0, 0, 0), 0, 0, 1.0, 1.0)
#define MATT_BLUE                                                              \
  Material(Colour(0.1, 0.1, 0.7), Colour(0, 0, 0), 0, 0, 1.0, 1.0)
#define MATT_CYAN                                                              \
  Material(Colour(0.1, 0.7, 0.7), Colour(0, 0, 0), 0, 0, 1.0, 1.0)

// Material textures
#define MATT_GRASS Material(grassTGA, Colour(0, 0, 0), 0, 0, 1.0, 1.0)
#define MATT_EARTH Material(earthTGA, Colour(0, 0, 0), 0, 0, 1.0, 1.0)
#define MATT_CHECK Material(checkerTGA, Colour(0, 0, 0), 0, 0, 1.0, 1.0)

// Mirror material
#define MIRROR                                                                 \
  Material(Colour(0.5, 0.5, 0.5), Colour(0, 0, 0), 0, 0.9, 1.0, 1.0)

// Recursion depth level
#define REC_DEPTH 10

// Antialiasing ifdef flags
// #define SUPER_SAMPLE
// #define JITTER_AA

// Random generator for jitter AA (bytes.com)
float jitRand(float min, float max) {
  return min + (max - min) * rand() / ((float)RAND_MAX);
}

int main() {
  // assert(testGeom() == 0);

  // Start point to time the render
  double time_taken = clock();

  // imageWriter to store pixel values
  TGAWriter *imageWriter = new TGAWriter(WIDTH, HEIGHT);

  // Loading our texture files into STGA
  STGA grassTGA;
  if (!loadTGA("grass.tga", grassTGA)) {
    return 1;
  }

  STGA earthTGA;
  if (!loadTGA("earth.tga", earthTGA)) {
    return 1;
  }

  STGA checkerTGA;
  if (!loadTGA("checkerboard.tga", checkerTGA)) {
    return 1;
  }

  // The lights that are active on our scene
  list<Lighting *> lights;

  lights.push_back(new DirectionLight(LIGHT_INTENS, LIGHT_DIR2, AMBIENT));

  // Spotlight
  lights.push_back(
      new SpotLight(LIGHT_INTENS, LIGHT_DIR, AMBIENT, Point3(2, 4, 2), 8));

  // Scene definition
  list<Shape *> scene;

  scene.push_back(new Sphere(Point3(0.35, 1, -2), 0.5, SHINY_RED));
  scene.push_back(new Sphere(Point3(0.75, 0.4, -1.6), 0.3, MATT_BLUE));
  scene.push_back(new Plane(Point3(0, 0, 0), Vector3(0, 1, 0), MATT_GRASS));

  vector<Triangle *> pyramid;
  pyramid.push_back(new Triangle(Point3(-1, 0, 0), Point3(0, 0, 1),
                                 Point3(0, 1, 0), SHINY_BLUE));
  pyramid.push_back(new Triangle(Point3(0, 0, 1), Point3(1, 0, 0),
                                 Point3(0, 1, 0), SHINY_BLUE));
  pyramid.push_back(new Triangle(Point3(1, 0, 0), Point3(0, 0, -1),
                                 Point3(0, 1, 0), SHINY_BLUE));
  pyramid.push_back(new Triangle(Point3(0, 0, -1), Point3(-1, 0, 0),
                                 Point3(0, 1, 0), SHINY_BLUE));

  Polyhedron *p = new Polyhedron(pyramid, MATT_CYAN);
  p->removeBackFaces(EYEPOINT);
  scene.push_back(p);

  scene.push_back(new Sphere(Point3(-1, 1, 1), 0.7, MATT_EARTH));

  // Camera definition
  View view = View(EYEPOINT, LOOKAT, VIEW_UP, FOV, WIDTH, HEIGHT);

  // The main loop
  // Iterate Y axis first
  for (float y = 0.0f; y < HEIGHT; y++) {
    for (float x = 0.0f; x < WIDTH; x++) {

      Colour colour = Colour(0.0, 0.0, 0.0);

#ifdef SUPER_SAMPLE
      // Allows us to increase the samples per pixel to four
      for (float fragmentx = x; fragmentx < x + 1.0f; fragmentx += 0.5f) {
        for (float fragmenty = y; fragmenty < y + 1.0f; fragmenty += 0.5f) {
          double coef = 0.25;
#else
      float fragmentx = x;
      float fragmenty = y;
      double coef = 1.0;
#endif

#ifdef JITTER_AA
          // Offset our ray so it doesnt always travel through the
          // center of the pixel / pixelfragment
          float rx = fragmentx + jitRand(-0.125, 0.125);
          float ry = fragmenty + jitRand(-0.125, 0.125);
          Ray3 ray = view.createRay(rx, ry);
#else
          Ray3 ray = view.createRay(fragmentx, fragmenty);
#endif

          int level = 0; // Recursion level

          // Main loop
          do {

            // Test each shape in the scene list for an intersection
            // store the closest shape
            double bestHit = -1;

            Shape *shape = NULL;
            for (list<Shape *>::iterator it = scene.begin(); it != scene.end();
                 it++) {
              Shape *itShape = *it;
              double intersect = itShape->intersect(ray);
              if (intersect > 0) {
                if (bestHit < 0 || (bestHit > 0 && intersect < bestHit)) {
                  bestHit = intersect;
                  shape = itShape;
                }
              }
            }

            // Stop this iteration if there was no intersections
            if (shape == NULL) {
              // Sky Blue Background?
              // Looks a bit weird as reflections still have a black sky
              if (level == 0) {
                colour = colour + (Colour(0.529, 0.808, 0.922) * coef);
              }
              break;
            }

            // The location of our hit
            Point3 hit = ray.startP() + (ray.directionV() * bestHit);

            // The normal at this position
            Vector3 normal = shape->normal(hit);

            // Iterate through each light, accumulating values
            for (list<Lighting *>::iterator it = lights.begin();
                 it != lights.end(); it++) {
              Lighting *light = *it;
              // Calculate shadowed
              bool isShadowed = false;

              // Generate a shadow ray
              Ray3 shadowRay = Ray3(hit + (unit(light->direction()) / 100),
                                    unit(light->direction()));

              // Check the shadow ray against our scene
              for (list<Shape *>::iterator it = scene.begin();
                   it != scene.end(); it++) {
                Shape *itShape = *it;
                double intersect = itShape->intersect(shadowRay);
                if (intersect > 0) {
                  isShadowed = true; // Can exit after first hit
                  break;
                }
              }

              // Colour at this intersection
              Colour hitColour = shape->getColour(
                  hit, normal, light, -ray.directionV(), isShadowed);

              // Accumulate colour values multiplying by the
              // supersampling coeffecient
              colour = colour + (hitColour * coef);
            }

            // Get the next reflection coefficeint
            coef *= shape->getMaterial().reflCoef();

            // Reflect and generate a new ray
            if (coef > 0.0) {
              Vector3 reflection =
                  (normal * (ray.directionV().dot(normal)) * -2) +
                  ray.directionV();
              ray = Ray3(hit + (unit(reflection) / 100), unit(reflection));
            }

            // Increase our recursion level counter
            level++;
          } while (coef > 0.0 && level < REC_DEPTH);
// Stop iterating if no reflections or at recurision limit
#ifdef SUPER_SAMPLE
        }
      }
#endif
      // Push our final pixel into the imageWriter
      imageWriter->putNextPixel(colour);
    }
  }

  // Time measurement
  time_taken = (clock() - time_taken) / (double)CLOCKS_PER_SEC;

  // Write our image to a file
  if (imageWriter->writeImage()) {
    std::cout << time_taken * 1000 << endl;
    return 0;
  } else {
    std::cout << "Error writing image" << endl;
    return -1;
  }
}
