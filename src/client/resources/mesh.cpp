#include "mesh.h"

#include <systems/logger.h>

#ifdef OPENGL3

faerie::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &m_vertexArray);
    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_indexBuffer);
  
    glBindVertexArray(m_vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);  

    m_numIndices = indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // UVs
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    // Unbind
    glBindVertexArray(0);
}

faerie::Mesh::Mesh(Mesh&& rhs) : m_numIndices(rhs.m_numIndices), m_vertexArray(rhs.m_vertexArray), m_vertexBuffer(rhs.m_vertexBuffer), m_indexBuffer(rhs.m_indexBuffer) {
    rhs.m_numIndices = 0; rhs.m_vertexArray = 0; rhs.m_vertexBuffer = 0; rhs.m_indexBuffer = 0;
}

faerie::Mesh::~Mesh() {
    if (m_vertexArray) { glDeleteVertexArrays(1, &m_vertexArray); }
    if (m_vertexBuffer) { glDeleteBuffers(1, &m_vertexBuffer); }
    if (m_indexBuffer) { glDeleteBuffers(1, &m_indexBuffer); }
}

GLuint faerie::Mesh::VertexArray() const { return m_vertexArray; }

#endif // OPENGL3

unsigned int faerie::Mesh::NumIndices() const { return m_numIndices; }