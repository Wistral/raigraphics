//
// Created by jhwangbo on 01.12.16.
//

#ifndef RAI_SPHERE_HPP
#define RAI_SPHERE_HPP
#include "Object.hpp"
#include <vector>

namespace rai_graphics {
namespace object {

class Sphere : public Object {

 public:
  Sphere(float radius, int rings=20);
  float radius_;

};

} // object
} // rai_graphics

#endif //RAI_SPHERE_HPP
