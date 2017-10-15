//
// Created by jhwangbo on 01.05.17.
//
#include "raiGraphics/obj/MultiBodyObject.hpp"

namespace rai_graphics {
namespace object {

void MultiBodyObject::draw(Camera *camera,  Light *light){
  drawSnapshot(camera, light, 1.0);
//  turnOnGhost(true);
//  for(auto& ghost : ghosts) {
//    setTrans(ghost);
//    drawSnapshot(camera, light, 1.0);
//  }
//  turnOnGhost(false);
}

void MultiBodyObject::showGhosts(int maxGhosts, float transparency){
  ghostTransparency = transparency;
  maxGhostN = maxGhosts;
  ghosts.reserve(maxGhosts);
}

void MultiBodyObject::addGhostsNow() {
  if (ghosts.size() < maxGhostN) {
    std::vector<Transform> ghost(objs.size());
    ghosts.push_back(ghost);
  }
  getTrans(ghosts[oldestGhost]);
  oldestGhost = (oldestGhost + 1) % maxGhostN;
}

void MultiBodyObject::setTrans(std::vector<Transform>& trans){
  for(int i = 0; i < objs.size(); i++)
    objs[i]->setTempTransform(trans[i]);
}

void MultiBodyObject::getTrans(std::vector<Transform>& trans){
  for(int i = 0; i < objs.size(); i++)
    objs[i]->getTransform(trans[i]);
}

void MultiBodyObject::turnOnGhost(bool ghostOn) {
  for(auto& ob: objs)
    ob->usingTempTransform(ghostOn);
}

void MultiBodyObject::drawSnapshot(Camera *camera, Light *light, float transparency){
  for(int i = 0; i < objs.size(); i++){
    objs[i]->setTransparency(transparency);
    shader->Bind();
    shader->Update(camera, light, objs[i]);
    objs[i]->draw();
    shader->UnBind();
  }
}

} // object
} // rai_graphics