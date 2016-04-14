/*
RfMeshGL - Implementation

TODO: Optimize draw().
*/
#include "rf_mesh_gl.h"
#include "zpd.h"

#include <string>
#include <fstream>
#include <sstream>

#include "rf_shader_gl.h"
#include "rf_compositor_gl.h"

using namespace zootopia;

RfMeshGL::RfMeshGL(std::vector<VertexGL> vertices, std::vector<GLuint> indices, std::vector<TextureGL> textures, const MaterialGL& materials) :
    _VAO(GL_NONE)
,   _VBO(GL_NONE)
,   _EBO(GL_NONE)
{
    _vertices = vertices;
    _indices = indices;
    _textures = textures;
    _materials = materials;

    setupMesh();
}

RfMeshGL::~RfMeshGL()
{
    //ZLOG_I("~RfMeshGL() CALL DESTRUCTOR!!!");
}

void RfMeshGL::draw()
{
    ZASSERT(RfCompositorGL::_deferredShader->getShaderProgObj());

    // Bind appropriate textures
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    GLuint normalNr = 1;
    GLuint heightNr = 1;

    for (GLuint i = 0; i < _textures.size(); i++) {

        // TODO: Optimize.

        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
                                          // Retrieve texture number (the N in diffuse_textureN)
        std::stringstream ss;
        std::string number;
        std::string name = _textures[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if (name == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        else if (name == "texture_normal")
            ss << normalNr++; // Transfer GLuint to stream
        else if (name == "texture_height")
            ss << heightNr++; // Transfer GLuint to stream
        number = ss.str();

        // Now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(RfCompositorGL::_deferredShader->getShaderProgObj(), (name + number).c_str()), i);
        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, _textures[i].id);
    }
    //*
    glUniform3f(glGetUniformLocation(RfCompositorGL::_deferredShader->getShaderProgObj(), 
        "material.diffuse"), 
        _materials.diffuse.r,
        _materials.diffuse.g,
        _materials.diffuse.b);

    glUniform3f(glGetUniformLocation(RfCompositorGL::_deferredShader->getShaderProgObj(),
        "material.specular"),
        _materials.specular.r,
        _materials.specular.g,
        _materials.specular.b);
    //*/

    // Draw mesh
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, ZBUFFER_OFFSET(0));
    glBindVertexArray(0);

    // Always good practice to set everything back to defaults once configured.
    for (GLuint i = 0; i < _textures.size(); i++) {

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void RfMeshGL::setupMesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(VertexGL), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGL), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGL), (GLvoid*)offsetof(VertexGL, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexGL), (GLvoid*)offsetof(VertexGL, texcoords));
    // Vertex Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGL), (GLvoid*)offsetof(VertexGL, tangent));
    // Vertex Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGL), (GLvoid*)offsetof(VertexGL, bitangent));

    glBindVertexArray(0);

}