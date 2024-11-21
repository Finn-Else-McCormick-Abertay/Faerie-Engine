#pragma once

#include <maths_types.h>
#include <vector>

#ifdef OPENGL3
#include <systems/platform/render_system_opengl3.h>
#endif // OPENGL3

namespace faerie {

    struct Vertex { vec3 position; vec3 normal; vec2 texCoords; };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>&, const std::vector<unsigned int>&);
        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&);
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