#include "Illumination.h"

Vector3 Lighting::direction() { return lightDirection; }

double Lighting::ambient() { return ambientIntensity; }

DirectionLight::DirectionLight(double intensity, Vector3 direction,
                               double ambient) {
  this->lightIntensity = intensity;
  this->lightDirection = direction;
  this->ambientIntensity = ambient;
}

double DirectionLight::intensity(Point3 p) { return lightIntensity; }

SpotLight::SpotLight(double intensity, Vector3 direction, double ambient,
                     Point3 origin, double attenuation) {
  this->lightIntensity = intensity;
  this->lightDirection = direction;
  this->ambientIntensity = ambient;
  this->origin = origin;
  this->attn = attenuation;
}

/*
 * Attenuates light by cos^attenuation(theta)
 * Reduces intensity as distance from direct hit increases
 */
double SpotLight::intensity(Point3 p) {
  Vector3 vToHit = p - this->origin;

  double intens = max(dot(-unit(this->lightDirection), unit(vToHit)), 0.0);
  intens = pow(intens, attn);

  return intens;
}

Material::Material(Colour diffuse, Colour specular, double shininess,
                   double reflectiveness, double alpha,
                   double refractiveIndex) {
  this->diffuseColour = diffuse;
  this->specularColour = specular;
  this->shininessAmount = shininess;
  this->reflectCoef = reflectiveness;
  this->alpha = alpha;
  this->refract = refractiveIndex;
  isTexture = false;
}

Material::Material(STGA texture, Colour specular, double shininess,
                   double reflectiveness, double alpha,
                   double refractiveIndex) {
  this->texture = texture;
  this->specularColour = specular;
  this->shininessAmount = shininess;
  this->reflectCoef = reflectiveness;
  this->alpha = alpha;
  this->refract = refractiveIndex;
  isTexture = true;
}

Colour Material::lit_colour(Point3 pos, double u, double v, Vector3 normal,
                            Lighting *light, Vector3 view, bool isShadowed) {
  // I = Ka Ia + Kd Is max(0, L . N) + Ks Is (max(0, H.N)) ^ n
  Colour i;
  if (isTexture) {
    diffuseColour = posColour(u, v, texture);
  }

  // Ka * Ia ambient
  i = diffuseColour * light->ambient();

  if (isShadowed) {
    return i;
  }

  // Kd * Is diffuse
  i += (diffuseColour * light->intensity(pos)) *
       max(0.0, (light->direction().dot(normal)));

  if (shininessAmount == 0) {
    return i;
  }

  // Ks * Is specular
  Vector3 h = norm(light->direction() + view);
  i += (specularColour * light->intensity(pos)) *
       pow(max(0.0, h.dot(normal)), shininessAmount);

  return i;
}

Colour Material::diffuse() { return diffuseColour; }

Colour Material::specular() { return specularColour; }

double Material::shininess() { return shininessAmount; }

double Material::reflCoef() { return reflectCoef; }

/*
 * Returns the colour at a position u, v on a texture
 */
Colour Material::posColour(double u, double v, STGA texture) {
  Colour c;
  int texWidth = texture.width;
  int texHeight = texture.height;

  // This allows us to loop a texture if we aren't using uv 0 - 1
  int xOffset = (int)abs(floor(u * texWidth)) % texWidth;
  int yOffset = (int)abs(floor(v * texHeight)) % texHeight;

  // Get the index in 1d array for our position, multiply by 3 to account for
  // each channel
  int index = 3 * (yOffset * texWidth + xOffset);

  // TGA stores in BGR order
  float b = ((float)texture.data[index]) / 255.0f;
  float g = ((float)texture.data[index + 1]) / 255.0f;
  float r = ((float)texture.data[index + 2]) / 255.0f;

  c = Colour(r, g, b);

  return c;
}

bool Material::isTex() { return isTexture; }
