//
// Created by jhwangbo on 01.05.17.
//
#include "raiGraphics/obj/MultiBodyObject.hpp"
#include "raiGraphics/obj/CheckerBoard.hpp"

namespace rai_graphics {
namespace object {

void MultiBodyObject::draw(Camera *camera,  Light *light, float transparency, CheckerBoard* chk){
  drawSnapshot(camera, light, transparency, chk);
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

void MultiBodyObject::drawSnapshot(Camera *camera, Light *light, float transparency, CheckerBoard* chk){
  for(int i = 0; i < objs.size(); i++) {
    if( !objs[i]->reflectable && chk ) continue;
    objs[i]->setTransparency(transparency);
    shader[i]->Bind();
    if(chk)
      shader[i]->UpdateForReflection(camera, light, objs[i], chk);
    else
      shader[i]->Update(camera, light, objs[i]);
    objs[i]->draw();
    shader[i]->UnBind();
  }
}

} // object
} // rai_graphics