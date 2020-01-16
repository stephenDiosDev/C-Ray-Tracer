#include <iostream>
#include <vector>
#include <cmath>
#include <cassert> //assert
#include <random>
#include <fstream>
#include <string>

#include "mat3f.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "image.hpp"
#include "vec2i.hpp"
#include "grid2.hpp"
#include "timer.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "ray_intersect.hpp"


using namespace math;
using namespace geometry;
using namespace std;

//output file width and height
int width;
int height;



namespace raytracing {

Vec3f normalAt(Vec3f const &p, Triangle const &t) {

}

Vec3f normalAt(Vec3f const &p, Sphere const &s) {
  Vec3f n = (p - s.origin) / s.radius;

  n = normalized(n);

  return n;
}

Vec3f normalAt(Vec3f const &point, Plane const &plane) { return plane.normal; }

struct Surface {
  virtual ~Surface() = default;
  virtual Hit intersectSelf(Ray const &ray) const = 0;
  virtual Vec3f normalAtSelf(Vec3f const &p) const = 0;
  virtual Vec3f colour() const = 0;
};

// helper class/function to make, e.g., class Sphere : public Surface
// Wrapping the geometry (e.g., sphere) in a class for intersection
// does not 'pollute' the geometry with inheritence
template <class T> struct Intersect_ : public Surface {
  template <typename... Args>
  Intersect_(Args... args)
      : m_self(std::forward<Args>(args)...) {}

  Hit intersectSelf(Ray const &ray) const { return intersect(ray, m_self); }
  Vec3f normalAtSelf(Vec3f const &p) const { return normalAt(p, m_self); }
  Vec3f colour() const {return m_self.colour;}

  T m_self;
};

template <typename T> std::unique_ptr<Intersect_<T>> makeIntersectable(T t) {
  return std::unique_ptr<Intersect_<T>>(new Intersect_<T>(t));
}

struct ImagePlane {
  using Screen = geometry::Grid2<raster::RGB>;

  Screen screen;
  math::Vec3f origin;
  math::Vec3f u;
  math::Vec3f v;
  float left;   // right symmetric
  float bottom; // top symmetric

  //ignore the given parameters, use global width and height instead
  ImagePlane &resolution(uint32_t local_width, uint32_t local_height) {
    screen = Screen(width, height);
    return *this;
  }
  ImagePlane &center(Vec3f center) {
    origin = center;
    return *this;
  }
  ImagePlane &uvAxes(Vec3f up, Vec3f right) {
    u = up;
    v = right;
    return *this;
  }
  ImagePlane &dimensions(float width, float height) {
    left = -(0.5f * width);
    bottom = -(0.5f * height);
    return *this;
  }

  math::Vec3f pixelTo3D(math::Vec2f pixel) const {
    using std::abs;

    // shift to center
    pixel += {0.5f, 0.5f};

    float u_x = left + (2.f * abs(left)) * (pixel.x) / screen.width();
    float v_y = bottom + (2.f * abs(bottom)) * (pixel.y) / screen.height();

    return origin + u_x * u + v_y * v;
  }
};

ImagePlane makeImagePlane(Vec3f const &eye, Vec3f const &lookatPosition,
                          Vec3f const &canonicalUp, int xResolution,
                          int yResolution, float width, float height,
                          float nearPlaneDistanace) {
  // make orthonormal basis around eye
  auto gaze = normalized(lookatPosition - eye);

  auto u = gaze ^ canonicalUp;
  u.normalize();

  auto v = u ^ gaze;
  v.normalize();

  // image plane is orthogoanl to camera gaze so use u,v for image plane
  ImagePlane imagePlane;
  // using method chaining to have named parameter/configuation)
  imagePlane.resolution(xResolution, yResolution)
      .center(eye + gaze * nearPlaneDistanace)
      .dimensions(width, height)
      .uvAxes(u, v);

  return imagePlane;
}

using s_ptr = std::unique_ptr<Surface>;

Vec3f castRay(Ray ray,
              math::Vec3f eye,   //
              math::Vec3f light, //
              std::vector<s_ptr> const &surfaces,
              int reflectionDepth) {

  constexpr float ambientIntensity = 0.1f;

  // background color
  Vec3f colorOut(0.1f, 0.1f, 0.1f);

  // find closed object, if any
  Hit closest;
  // pointer to closest object
  Surface const *surface = nullptr;
  for (auto const &s : surfaces) {
    auto hit = s->intersectSelf(ray);
    if (hit && (hit.rayDepth < closest.rayDepth) && hit.rayDepth > 0.f) {
      closest = hit;
      surface = s.get();
    }
  }

  // if hit get point
  if (surface != nullptr) {
      Vec3f lightColour = surface->colour();
    float t = closest.rayDepth;



    //spot on sphere where the intersection occurs
    Vec3f rayP = ray.origin + (t * ray.direction);

    Vec3f normal = surface->normalAtSelf(rayP);
    normal = normalized(normal);

    //we can now do the phong lighting equation using that point

    //phong
    float ambientStrength = ambientIntensity;
    float diffuseStrength = 0.3;
    float specularStrength = 0.5f;

    //ambient
    Vec3f ambient = ambientStrength * lightColour;

    //diffuse
    Vec3f lightDir = light - rayP;
    lightDir = normalized(lightDir);
    float diff = max((normal * lightDir), 0.f);
    Vec3f diffuse = (diff * diffuseStrength) * lightColour;

    //specular
    Vec3f viewVector = rayP - eye;
    viewVector = normalized(viewVector);
    Vec3f reflectionVector = (lightDir) + (2.f * ((-lightDir * normal) * (normal)));
    float spec = std::pow(max((viewVector*reflectionVector), 0.f), 32);
    Vec3f specular = specularStrength * spec * lightColour;

    //lighting before shadow and reflections
    Vec3f result = (ambient + diffuse + specular);

    colorOut = result;

    Ray shadow;
    shadow.direction = normalized(light - rayP);
    //p = e + td
    shadow.origin = rayP + (shadow.direction * 0.00001f);
    Surface const *surface = nullptr;
    for(auto const &s : surfaces) {
        auto hit = s->intersectSelf(shadow);
        //if shadow ray hits anything within bounds, set that to ambient light
        if(hit && (hit.rayDepth < 1e+5) && (hit.rayDepth > 0)) {
            colorOut = ambient;
            break;
        }
    }

    //reflection
    if(reflectionDepth >= 0) {
        //find reflection ray and shoot it
        //adjust colourOut

        Vec3f reflectionDirection = eye - (2 * normal * (eye * normal));

        reflectionDirection = -normalized(reflectionDirection);

        auto bias = 1e-4f;
        Ray reflectionRay(rayP + (reflectionDirection * bias), reflectionDirection);


        float reflectionMagnitude = 0.7f;

        colorOut += reflectionMagnitude * castRay(reflectionRay, eye, light, surfaces, reflectionDepth - 1);
    }


  }
  return colorOut;
}

void render(ImagePlane &imagePlane, //
            math::Vec3f eye,        // all below could be in 'scene' object
            math::Vec3f light,      //
            std::vector<s_ptr> const &surfaces) {

  // Standard mersenne_twister_engine seeded
  thread_local std::mt19937 gen(0);
  auto sampleRange = [](float a, float b) {
    using distrubution = std::uniform_real_distribution<>;
    return distrubution(a, b)(gen);
  };


  for (int32_t y = 0; y < imagePlane.screen.height(); ++y) {
    for (int32_t x = 0; x < imagePlane.screen.width(); ++x) {

      math::Vec2f pixel(x, y);
      auto pixel3D = imagePlane.pixelTo3D(pixel);
      auto direction = normalized(pixel3D - eye);
      auto bias = 1e-4f;
      auto p = pointOnLne(eye, direction, bias);
      Ray r(p, direction);

      auto colorOut = castRay(r, eye, light, surfaces, 1);

      // correct to quantiezed error
      // (i.e., removes banded aliasing when converting to 8bit RGB)
      constexpr float halfStep = 1.f / 512;
      colorOut = raster::quantizedErrorCorrection(
          colorOut, sampleRange(-halfStep, halfStep));

      imagePlane.screen({x, y}) = raster::convertToRGB(colorOut);
    }
  }
}
} // namespace

int main() {
    Vec3f light;
    Vec3f eye;
    Vec3f lookat;
    Vec3f up;
    int resolutionX;
    int resolutionY;
    float planeWidth;
    float planeHeight;
    float focalDist;


    //read in the width and height from a file named "parameters.txt"
    //this file must be in the project file and not in src!
    fstream fileInput ("../parameters.txt");
    string line;

    //open file and read in values
    //file format must be the line label followed by a space followed by the value
    if(fileInput.is_open()) {
        while(getline(fileInput,line)) {
            if(line.find("width") == 0) {
                width = stoi(line.substr(5));
                cout<<"width: "<<width<<endl;
            }
            else if(line.find("height") == 0) {
                height = stoi(line.substr(6));
                cout<<"height: "<<height<<endl;
            }
            else {
                cout<<"Invalid file I/O\n";
                return -1;
            }
        }

        fileInput.close();
    }




  //viewing parameters
  light = Vec3f{20, 15, 10};
  eye = Vec3f{0.f, 7.5f, 15.f};
  lookat = Vec3f {0.f, 0.f, 0.f};
  up = Vec3f{0.f, 1.f, 0.f};
  resolutionX = 1000;
  resolutionY = 1000;
  planeWidth = 50.f;
  planeHeight = 50.f;
  focalDist = 50.f;

  int scene = 3; //***************************************SET SCENE HERE

  using namespace raytracing;

  auto imagePlane = makeImagePlane(eye, lookat, up,          //
                                   resolutionX, resolutionY, //
                                   planeWidth, planeHeight,  //
                                   focalDist);

  Plane p;  //this plane is default and present in all 3 scenes
  p.origin = Vec3f(0.f, 0.f, 0.f);
  p.normal = Vec3f(0.f, 1.f, 0.f);
  p.colour = Vec3f(0.4f, 0.4f, 0.4f);


  // setup scene, defaults to scene 1
  std::vector<s_ptr> surfaces;
  //add the default plane to every scene
  surfaces.push_back(makeIntersectable(p));

  //********************************************SCENE 1********************************************

  Sphere s_s1;
  s_s1.origin = Vec3f(0,1,0);
  s_s1.colour = Vec3f(1.f, 1.f, 0.f);

  Sphere s2_s1;//default red colour
  s2_s1.origin = Vec3f(1.5,3.2f,0);

  Sphere s3_s1;
  s3_s1.origin = Vec3f(-1.5, 3.2f, 0);
  s3_s1.colour = Vec3f(0.f, 1.f, 0.f);

  Triangle t1_s1;
  t1_s1.a() = Vec3f(0.f,2.2f,0.f);
  t1_s1.b() = Vec3f(3.0f,2.2f,-1.5f);
  t1_s1.c() = Vec3f(3.0f,2.2f,1.5f);
  t1_s1.colour = Vec3f(0.f,0.f,1.f);

  Triangle t2_s1;
  t2_s1.a() = Vec3f(0.f,2.2f,0.f);
  t2_s1.b() = Vec3f(-3.0f,2.2f,-1.5f);
  t2_s1.c() = Vec3f(-3.0f,2.2f,1.5f);
  t2_s1.colour = Vec3f(0.f,0.f,1.f);



  //********************************************SCENE 1********************************************



  //********************************************SCENE 2********************************************
    Sphere s_s2;
    s_s2.origin = Vec3f(0,1,0);
    s_s2.colour = Vec3f(1.f,0.f,0.f);

    Sphere s1_s2;
    s1_s2.origin = Vec3f(0,3,0);
    s1_s2.colour = Vec3f(0.f,0.5f,0.f);

    Sphere s2_s2;
    s2_s2.origin = Vec3f(0,5,0);
    s2_s2.colour = Vec3f(0.f,0.f,1.f);

    Sphere s3_s2;
    s3_s2.origin = Vec3f(-2,3,0);
    s3_s2.colour = Vec3f(0.5f,0.5f,0.f);

    Sphere s4_s2;
    s4_s2.origin = Vec3f(2,3,0);
    s4_s2.colour = Vec3f(0.f,0.5f,0.5f);

  //********************************************SCENE 2********************************************

  //********************************************SCENE 3********************************************

    //tis the season!

    Triangle t_s3;
    t_s3.a() = Vec3f(0,0,0);
    t_s3.b() = Vec3f(3,0,-2);
    t_s3.c() = Vec3f(0,6,-1);

    Triangle t1_s3;
    t1_s3.a() = Vec3f(0,0,0);
    t1_s3.b() = Vec3f(-3,0,-2);
    t1_s3.c() = Vec3f(0,6,-1);

    Sphere s_s3;
    s_s3.origin = Vec3f(0.7,4,-1);
    s_s3.radius = 0.3;
    s_s3.colour = Vec3f(1.f,0.f,0.f);

    Sphere s1_s3;
    s1_s3.origin = Vec3f(-0.7,3,-0.6f);
    s1_s3.radius = 0.3;
    s1_s3.colour = Vec3f(1.f,0.f,1.f);

    Sphere s2_s3;
    s2_s3.origin = Vec3f(-1.8,1,-1.f);
    s2_s3.radius = 0.3;
    s2_s3.colour = Vec3f(1.f,1.f,0.f);

    Sphere s3_s3;
    s3_s3.origin = Vec3f(0.6,1.6,-0.6f);
    s3_s3.radius = 0.3;
    s3_s3.colour = Vec3f(0.f,0.4f,1.f);

    Sphere s4_s3;
    s4_s3.origin = Vec3f(-0.2,4.8,-0.5);
    s4_s3.radius = 0.3;
    s4_s3.colour = Vec3f(1.f,0.55f,0.63f);

  //********************************************SCENE 3********************************************

  if(scene == 2) {
    surfaces.push_back(makeIntersectable(s_s2));
    surfaces.push_back(makeIntersectable(s1_s2));
    surfaces.push_back(makeIntersectable(s2_s2));
    surfaces.push_back(makeIntersectable(s3_s2));
    surfaces.push_back(makeIntersectable(s4_s2));
  }
  else if(scene == 3) {
    surfaces.push_back(makeIntersectable(t_s3));
    surfaces.push_back(makeIntersectable(t1_s3));
    surfaces.push_back(makeIntersectable(s_s3));
    surfaces.push_back(makeIntersectable(s1_s3));
    surfaces.push_back(makeIntersectable(s2_s3));
    surfaces.push_back(makeIntersectable(s3_s3));
    surfaces.push_back(makeIntersectable(s4_s3));
  }
  else {    //default scene is 1
      surfaces.push_back(makeIntersectable(s_s1));
      surfaces.push_back(makeIntersectable(s2_s1));
      surfaces.push_back(makeIntersectable(s3_s1));
      surfaces.push_back(makeIntersectable(t1_s1));
      surfaces.push_back(makeIntersectable(t2_s1));
  }

  // render that thing...
  temporal::Timer timer(true);

  render(imagePlane, eye, light, surfaces);

  std::cout << "Time elapsed: " << std::fixed << timer.minutes() << " min.\n";

  raster::write_screen_to_file("./test.png", imagePlane.screen);

  return EXIT_SUCCESS;
}
