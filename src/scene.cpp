#include "scene.h"
#include <bitset>
#include <vector>
#include <queue>
#include <array>
#include <functional>
#include <unordered_map>
#include <memory>
#include <string>
#include <cinttypes>

using namespace Scene;

typedef std::bitset<8> Signature;
std::vector<Entity> entities;
Signature rendersignature(std::string("10000000"));

std::vector<std::function<void(Entity)>> functions;

void Scene::addFunction(std::function<void(Entity)> func)
{
    functions.push_back(func);
}

void Scene::createEntity(std::string type_string)
{
    Entity ent = 
    {
        0,
        Signature(type_string)
    };
    entities.push_back(ent);
}

void Scene::update()
{
    for (Entity ent : entities)
    {
        if ((ent.signature & rendersignature) == rendersignature)
        {
            functions.at(0)(ent);
        }
    }
}



