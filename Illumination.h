/*
 * Illumination.h
 * Contains definitions of the Lighting and Material Classes
 */

#pragma once

#include "Colour.h"
#include "GeomX.h"
#include "TGAReader.h"
#include "pi.h"

class Shape; // legit? nope

/*
 * Abstract class Lighting
 * Contains an intensity, direction and ambient
 */
class Lighting {
protected:
  double lightIntensity;
  Vector3 lightDirection;
  double ambientIntensity;

public:
  /*
   * abstract method intensity(Point3 p)
   * @return double the intensity at the given point
   */
  virtual double intensity(Point3 p) = 0;
  /*
   * direction()
   * @return Vector3 a vector pointing towards the light
   */
  Vector3 direction();

  /*
   * ambient()
   * @return double the level of ambient light
   */
  double ambient();
};

/*
 * DirectionLight
 * concrete subclass of lighting
 * Shines light globally
 */
class DirectionLight : public Lighting {
public:
  DirectionLight(double intensity, Vector3 direction, double ambient);
  double intensity(Point3 p);
};

/*
 * SpotLight
 * concrete subclass of lighting
 * attenuates light by cos^attn(theta)
 */
class SpotLight : public Lighting {
  Point3 origin;
  double attn;

public:
  SpotLight(double intensity, Vector3 direction, double ambient, Point3 origin,
            double attenuation);
  double intensity(Point3 p);
};

/*
 * Material
 * Contains colour, shininess, reflection and texture information
 * Is responsible for calculating the colour at a point
 */
class Material {
  STGA texture;
  Colour diffuseColour;
  Colour specularColour;
  double shininessAmount;
  double reflectCoef;
  double alpha;
  double refract;
  bool isTexture;
  Colour posColour(double u, double v, STGA texture);

public:
  // Empty constructor allows us to pass Material instances into methods
  Material(){};
  // Constructs a Material with a base colour
  Material(Colour diffuse, Colour specular, double shininess,
           double reflectiveness, double alpha, double refractiveIndex);
  // Constructs a Material with a texture file
  Material(STGA texture, Colour specular, double shininess,
           double reflectiveness, double transparency, double refractiveIndex);
  // Calculates the colour at a given point
  Colour lit_colour(Point3 pos, double u, double v, Vector3 normal,
                    Lighting *light, Vector3 view, bool isShadowed);
  Colour diffuse();
  Colour specular();
  double shininess();
  double reflCoef();
  bool isTex();
};