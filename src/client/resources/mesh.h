#pragma once

#include <maths_types.h>
#include <vector>

#include <resources/resource_info.h>

#ifdef OPENGL3
#include <systems/platform/renderer_opengl3.h>
#endif // OPENGL3

namespace faerie {

    struct Vertex { vec3 position; vec3 normal; vec2 texCoords; };

    // Not loaded directly, created when models are loaded
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>&, const std::vector<unsigned int>&);
        Mesh(Mesh&&);
        Mesh(const Mesh&) = delete;
        ~Mesh();

    #ifdef OPENGL3
        GLuint VertexArray() const;
    #endif // OPENGL3

        unsigned int NumIndices() const;

    private:
        unsigned int m_numIndices;
    #ifdef OPENGL3
        GLuint m_vertexArray;
        GLuint m_vertexBuffer, m_indexBuffer;
    #endif // OPENGL3
    };

} // faerie

template<>
class ResourceInfo<faerie::Mesh>
{
public:
    ResourceInfo(const std::string& modelPath, unsigned int meshIndex);

    ResourceIdentifier Identifier() const;

    const std::string& Path() const;
    unsigned int MeshIndex() const;
private:
    std::string m_path; unsigned int m_index;

    friend std::ostream& operator<<(std::ostream& os, const ResourceInfo<faerie::Mesh>& info) {
        os << info.Path() << "::mesh_" << std::to_string(info.MeshIndex());
        return os;
    }
};