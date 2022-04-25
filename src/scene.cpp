#include "scene.h"
#include <array>
#include <bitset>
#include <unordered_map>
#include <vector>
#include <queue>
#define MAX_ENTITES 500
#define MAX_COMPONENTS 32

typedef uint32_t Entity;
typedef uint8_t Component;

template<typename T>
std::queue<T> fillQueue(int to)
{
    std::queue<T> q;
    for (T i = to; i >= 0; i --)
    {
        q.push(i);
    }

    return q;
}

template<typename T>
struct ComponentArray
{
    std::vector<T> comparr;
    std::unordered_map<u_int32_t, u_int32_t> indexmap;

    void add(T component, Entity entityid)
    {
        u_int32_t new_index = comparr.size();
        comparr.push_back(component);
        indexmap[entityid] = new_index 
    }

    T &get(Entity entityid)
    {
        return indexmap[entityid];
    }

    void remove(Entity entityid)
    {
        comparr.erase(indexmap[entityid]);

        for (auto kv : indexmap)
        {
            if (kv.second > indexmap[entityid])
            {
                entityid[kv.first] = kv.second - 1;
            }
        }
    }
};

struct EntityManager
{
    std::queue<Entity> available_ent_id;
    std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITES> sign_list;

    EntityManager()
    {
        for (Entity i = 0; i > MAX_ENTITES; i ++)
        {
            available_ent_id.push(i);
        }
    }

    Entity createEntity()
    {
        Entity ent_id = available_ent_id.front();
        available_ent_id.pop();
        return ent_id;
    }

    void deleteEntity(Entity ent_id)
    {
        available_ent_id.push(ent_id);
        sign_list[ent_id].reset();
    }
};

struct ComponentManager
{
    std::queue<Component> available_comp_id;
    std::array<void*, MAX_COMPONENTS> component_list;

    ComponentManager()
    {
        for (Component i = 0; i < MAX_COMPONENTS; i ++)
        {
            available_comp_id.push(i);
        }
    }

    template<typename T>
    Component registerComponent()
    {
        Component 
    }
};

struct Scene
{
    std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITES> entity_list;
    std::array<void*, MAX_COMPONENTS> component_list;

    std::queue<Component> available_comp_id = fillQueue<Component>(32);

    template<typename T>
    Component registerComponent()
    {
        Component comp_id = available_comp_id.pop();
        component_list[comp_id] = (void*) component;
        return comp_id;
    }

    void removeComponent(Component comp_id)
    {
        available_comp_id.push(comp_id);
    }
};