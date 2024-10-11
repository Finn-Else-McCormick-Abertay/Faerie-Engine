#pragma once

#include <string>

namespace Components
{
    struct Model
    {
        std::string vertexShader, fragmentShader;
    
        Model();
    };

} // Components