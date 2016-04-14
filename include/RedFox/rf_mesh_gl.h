#pragma once

/*
RfMeshGL - Class
*/

#ifndef __RF_MESH_GL_H__
#define __RF_MESH_GL_H__

// STD LIBRARY
#include <vector>

// THIRD PARTY
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// INTERFACE
#include "rf_mesh.h"

// OTHER INTERFACE OR CLASS
#include "rf_mesh_gl.h"
#include "rf_shader_gl.h"

namespace zootopia {

    struct VertexGL {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    struct TextureGL {
        GLuint      id;
        std::string type;
        aiString    path;
    };

    struct MaterialGL {
        aiColor4D	ambient;
        aiColor4D	diffuse;
        aiColor4D	specular;
        aiColor4D	emission;
        GLfloat		shininess;
        GLfloat		strength;
    };

    class RfMeshGL : public RfMesh {

    public:

        explicit RfMeshGL(std::vector<VertexGL> vertices, std::vector<GLuint> indices, std::vector<TextureGL> textures, const MaterialGL& materials);
        ~RfMeshGL();

    public:

        void draw() override;

    private:

        void setupMesh();

    private:

        std::vector<VertexGL>   _vertices;
        std::vector<GLuint>     _indices;
        std::vector<TextureGL>  _textures;
        MaterialGL              _materials;

        GLuint  _VAO;
        GLuint  _VBO;
        GLuint  _EBO;

    };

}

#endif
