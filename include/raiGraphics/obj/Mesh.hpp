#ifndef MESH_INCLUDED_H
#define MESH_INCLUDED_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "raiGraphics/imp/obj_loader.h"
#include "Eigen/Core"
#include "raiGraphics/imp/transform.h"
#include "raiGraphics/imp/Light.hpp"
#include "Object.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RAI {
namespace rai_graphics {
namespace objects {

struct Vertex {
 public:
  Vertex(const glm::vec3 &pos, const glm::vec2 &texCoord, const glm::vec3 &normal) {
    this->pos = pos;
    this->texCoord = texCoord;
    this->normal = normal;
  }

  glm::vec3 *GetPos() { return &pos; }
  glm::vec2 *GetTexCoord() { return &texCoord; }
  glm::vec3 *GetNormal() { return &normal; }

 private:
  glm::vec3 pos;
  glm::vec2 texCoord;
  glm::vec3 normal;
};


class Mesh : public RAI::rai_graphics::objects::Object {
 public:
  Mesh(const std::string& fileName, float scale=1.0f);
  Mesh(Vertex *vertices, unsigned int numVertices, unsigned int *indices, unsigned int numIndices);

  void draw(Light& light);

  virtual ~Mesh();

 protected:
 private:
  void operator=(const Mesh &mesh) {}
  Mesh(const Mesh &mesh) {}

  void InitMesh();

  void recursiveProcess(aiNode* node,const aiScene* scene);
  void processMesh(aiMesh* mesh,const aiScene* scene);

 public:

  float scale_;
};

}
}
}
#endif
