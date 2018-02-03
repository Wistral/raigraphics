#include "raiGraphics/obj/Mesh.hpp"
#include <map>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include "raiGraphics/imp/vector3d.h"
#include "SDL2/SDL_image.h"

inline bool fileexists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

namespace rai_graphics {
namespace object {

Mesh::Mesh(const std::string& fileName, float scale, std::string texture) {
  selectable_ = true;

  RAIFATAL_IF(!fileexists(fileName),"could not find the mesh file"<<std::endl);
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(fileName.c_str(),
                                           aiProcess_GenSmoothNormals | aiProcess_Triangulate
                                               | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
  RAIFATAL_IF(scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode, "ThIS file wasn't successfully opened: " << fileName);
  scale_ = scale;
  recursiveProcess(scene->mRootNode, scene);
  hasColorCoord = colorsCoords.size() == normals.size();
  std::cout<<"colorsCoords.size() "<<colorsCoords.size()<<"\n";
  std::cout<<"normals.size() "<<normals.size()<<"\n";

  com = glm::vec3(0,0,0);
  for(auto& pos: positions)
    com += pos;
  com /= positions.size();
}

Mesh::Mesh(Vertex *vertices, unsigned int numVertices, unsigned int *indicesL, unsigned int numIndices, std::string texture) {
  selectable_ = true;

  for (unsigned int i = 0; i < numVertices; i++) {
    positions.push_back(*vertices[i].GetPos());
    texCoords.push_back(*vertices[i].GetTexCoord());
    normals.push_back(*vertices[i].GetNormal());
  }

  for (unsigned int i = 0; i < numIndices; i++)
    indices.push_back(indicesL[i]);

  com = glm::vec3(0,0,0);
  for(auto& pos: positions)
    com += pos;
  com /= positions.size();
}

Mesh::Mesh(const float *vertices, unsigned int numVertices, const unsigned int *indicesL, unsigned int numIndices, std::string texture) {
  selectable_ = true;

  for (unsigned int i = 0; i < numVertices; i++) {
    glm::vec3 vertexVector(vertices[i * 4], vertices[i * 4 + 1], vertices[i * 4 + 2]);
    positions.push_back(vertexVector);
  }

  for (unsigned int i = 0; i < numIndices; i++)
    indices.push_back(indicesL[i]);

  for (unsigned int i = 0; i < numIndices/3; i++) {
    normals.push_back(glm::cross(positions[indices[i*3]] - positions[indices[i*3 + 1]], positions[indices[i*3 + 1]] - positions[indices[i*3 + 2]]));
    normals.back() = glm::normalize(normals.back());
  }

  com = glm::vec3(0,0,0);
  for(auto& pos: positions)
    com += pos;
  com /= positions.size();
}

Mesh::~Mesh() {}

void Mesh::recursiveProcess(aiNode *node, const aiScene *scene) {
  //process
  for (int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, scene);
  }

  //recursion
  for (int i = 0; i < node->mNumChildren; i++) {
    recursiveProcess(node->mChildren[i], scene);
  }

}
void Mesh::processMesh(aiMesh *mesh, const aiScene *scene) {
  aiColor4D col;
  aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
  aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col);
  vector3d defaultColor(col.r, col.g, col.b);

  for (int i = 0; i < mesh->mNumVertices; i++) {
    positions.emplace_back(mesh->mVertices[i].x*scale_, mesh->mVertices[i].y*scale_, mesh->mVertices[i].z*scale_);
    normals.emplace_back(float(mesh->mNormals[i].x), float(mesh->mNormals[i].y), float(mesh->mNormals[i].z));

    if (mesh->mColors[0])
      colorsCoords.emplace_back(float(mesh->mColors[0][i].r), float(mesh->mColors[0][i].g), float(mesh->mColors[0][i].b));
    else
      colorsCoords.emplace_back(0.5f,0.5f,0.5f);
  }

  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++) //0..2
      indices.push_back(face.mIndices[j]);
  }

}

} // object
} // rai_graphics