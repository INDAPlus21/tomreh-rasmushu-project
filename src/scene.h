#pragma once
#include <string>
#include <vector>
#include <functional>

namespace Scene
{
    struct Entity
    {
        uint32_t uid;
        Signature signature;
    };

    struct RenderData
    {
        uint32_t va_handle;
        uint32_t vb_handle;
        uint32_t ib_handle;
    };

    void addFunction(std::function<void(Entity)> func);
    void createEntity(std::string type_string);
    void update();
}
