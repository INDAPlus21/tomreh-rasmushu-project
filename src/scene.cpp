#include "scene.h"
#include <array>
#include <bitset>
#include <unordered_map>
#include <vector>
#include <queue>
#include <memory>
#include <set>
#include <functional>
#define MAX_ENTITES 500
#define MAX_COMPONENTS 32
#define MAX_SYS_FUNC 8
#define MAX_SYSTEMS 100

typedef uint32_t Entity;
typedef uint16_t System;
typedef uint8_t Component;
typedef std::bitset<MAX_COMPONENTS> Signature;

template<typename T>
struct ComponentArray
{
    std::vector<T> comparr(MAX_ENTITES);
    std::unordered_map<Entity, u_int32_t> indexmap;

    void add(T component, Entity entityid)
    {
        u_int32_t new_index = comparr.size();
        comparr.push_back(component);
        indexmap[entityid] = new_index;
    }

    T &get(Entity entityid)
    {
        return comparr[indexmap[entityid]];
    }

    void remove(Entity entityid)
    {
        comparr.erase(indexmap[entityid]);

        for (auto kv : indexmap)
        {
            if (kv.second > indexmap[entityid])
            {
                indexmap[kv.first] = kv.second - 1;
            }
        }
    }

    void compEntityDestroyed(Entity entity)
    {
        if (indexmap.find(entity) != indexmap.end())
        {
            remove(entity);
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

    void setSignature(Entity ent_id, Signature signature)
    {
        sign_list[ent_id] = signature;
    }

    Signature getSignature(Entity ent_id)
    {
        return sign_list[ent_id];
    }
};

struct ComponentManager
{
    std::queue<Component> available_comp_id;
    std::unordered_map<const char*, std::shared_ptr<void> > typemap;

    ComponentManager()
    {
        for (Component i = 0; i < MAX_COMPONENTS; i ++)
        {
            available_comp_id.push(i);
        }
    }

    template<typename T>
    std::shared_ptr<ComponentArray<T>> getCompArr()
    {
        const char* type_name = typeid(T).name();
        std::shared_ptr<void> void_p = typemap[type_name];
        return std::static_pointer_cast<ComponentArray<T> >(void_p);
    }

    template<typename T>
    Component registerComponent()
    {
        Component comp_id;
        const char* type_name = typeid(T).name();
        std::shared_ptr<ComponentArray<T> > comp = std::make_shared<ComponentArray<T> >();
        typemap[type_name] = std::static_pointer_cast<void>(comp);
        return comp_id;
    }

    template<typename T>
    void addComponent(Entity ent_id, T comp)
    {
        getCompArr<T>()->add(ent_id, comp);        
    }

    template<typename T>
    void deleteComponent(Entity ent_id)
    {
        getCompArr<T>()->remove(ent_id);
    }

    template<typename T>
    T &getComponent(Entity ent_id)
    {
        return getCompArr<T>()->get(ent_id);
    }

    void entityDestroyed(Entity entity)
    {
        for (auto const &pair : typemap)
        {
            auto const &type_id = pair.first;
            auto const &component = pair.second;
            // TODO: Solve the problem, fuck classes bror
        }
    }
};

struct SystemType
{
    std::set<Entity> entities;
    std::array<std::shared_ptr<void()>, MAX_SYS_FUNC> funcs;
    std::queue<uint8_t> available_func_id;

    SystemType()
    {
        for (uint8_t i = 0; i < MAX_SYS_FUNC; i ++)
        {
            available_func_id.push(i);
        }
    }

    void addFunc(std::function<void()> func)
    {
        uint8_t func_id = available_func_id.front();
        available_func_id.pop();

        funcs[func_id] = std::make_shared<void()>(func);
    }
};

struct SystemManager
{
    std::array<std::shared_ptr<SystemType>, MAX_SYSTEMS> sys_list;
    std::array<Signature, MAX_SYSTEMS> sign_list;
    std::queue<System> available_sys_id;

    SystemManager()
    {
        for (System i = 0; i < MAX_SYSTEMS; i ++)
        {
            available_sys_id.push(i);
        }
    }

    System addSystem(SystemType system)
    {
        System sys_id = available_sys_id.front();
        available_sys_id.pop();
        sys_list[sys_id] = std::make_shared<SystemType>(system);
        return sys_id;
    }

    void setSignature(System system, Signature signature)
    {
        sign_list[system] = signature;
    }

    void entityDestroyed(Entity entity)
    {
        for (System i = 0; i < MAX_SYSTEMS; i ++)
        {
            if (sys_list[i])
            {
                sys_list[i]->entities.erase(entity);
            }
        }   
    }

    void entitySignatureChanged(Entity entity, Signature signature)
    {
        for (System i = 0; i < MAX_SYSTEMS; i ++)
        {
            if (sys_list[i])
            {
                if ((signature & sign_list[i]) == sign_list[i])
                {
                    sys_list[i]->entities.insert(entity);
                } 
                else
                {
                    sys_list[i]->entities.erase(entity);
                }
            }
        }
    }
};

struct Scene
{
    std::array<std::bitset<MAX_COMPONENTS>, MAX_ENTITES> entity_list;
    std::array<void*, MAX_COMPONENTS> component_list;
};