/*
RfModelGL - Implementation.

TODO: FIX COPY ISSUE. (RfMeshGL)
*/
#include "rf_model_gl.h"
#include "zpd.h"

#include <iostream>
#include <IL/il.h>

#include "rf_mesh_gl.h"

using namespace zootopia;

RfModelGL::RfModelGL(const std::string& path, bool gamma) :
    _directory("models/")
,   _gammaCorrection(gamma)
{
    loadModel(path);
}

RfModelGL::~RfModelGL()
{
    //ZLOG_I("~RfModelGL DESTRUCTOR CALL~!");
}

void RfModelGL::draw()
{
    // We must use 'pass by reference'.
    for (auto &mesh : _meshes)
        mesh.draw();
}

void RfModelGL::loadModel(const std::string& path)
{
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    // Check for errors
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero

        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    
    // Retrieve the directory path of the filepath
    _directory = path.substr(0, path.find_last_of('/'));

    // Process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void RfModelGL::processNode(aiNode* node, const aiScene* scene)
{
    // Process each mesh located at the current node
    for (GLuint i = 0; i < node->mNumMeshes; i++) {

        // The node object only contains indices to index the actual objects in the scene. 
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        // TODO: FIX COPY ISSUE.
        _meshes.push_back(processMesh(mesh, scene)); 
    }
    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (GLuint i = 0; i < node->mNumChildren; i++) {

        processNode(node->mChildren[i], scene);
    }
}

RfMeshGL RfModelGL::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    std::vector<VertexGL>       vertices;
    std::vector<GLuint>         indices;
    std::vector<TextureGL>      textures;
    MaterialGL                  mat;

    // Walk through each of the mesh's vertices
    for (GLuint i = 0; i < mesh->mNumVertices; i++) {

        VertexGL vertex;
        glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        
        // Positions
        if (mesh->HasPositions()) {

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
        }
        
        // Normals
        if (mesh->HasNormals()) {

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // Texture Coordinates
        if (mesh->mTextureCoords[0]) {// Does the mesh contain texture coordinates?

            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texcoords = vec;
        }
        else {

            vertex.texcoords = glm::vec2(0.0f, 0.0f);
        }
            
        // Tangent And Bitangent
        if (mesh->HasTangentsAndBitangents()) {

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.bitangent = vector;
        }
        
        vertices.push_back(vertex);
    }
    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {

        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process materials
    if (mesh->mMaterialIndex >= 0) {

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // Diffuse: texture_diffuseN
        // Specular: texture_specularN
        // Normal: texture_normalN

        // 1. Diffuse maps
        std::vector<TextureGL> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. Specular maps
        std::vector<TextureGL> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. Normal maps
        std::vector<TextureGL> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. Height maps
        std::vector<TextureGL> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        material->Get(AI_MATKEY_COLOR_DIFFUSE, mat.diffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR, mat.specular);
        material->Get(AI_MATKEY_COLOR_AMBIENT, mat.ambient);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, mat.emission);
        mat.shininess = 0.4f;
        mat.strength = 128.0f;

        /*
        std::cout << "mesh start" << std::endl;
        std::cout << "ambient: " << mat.ambient.r << " " << mat.ambient.g << " " << mat.ambient.b << std::endl;
        std::cout << "diffuse: " << mat.diffuse.r << " " << mat.diffuse.g << " " << mat.diffuse.b << std::endl;
        std::cout << "specular: " << mat.specular.r << " " << mat.specular.g << " " << mat.specular.b << std::endl;
        std::cout << "emission: " << mat.emission.r << " " << mat.emission.g << " " << mat.emission.b << std::endl;
        std::cout << "mesh end" << std::endl;
        */
    }

    // Return a mesh object created from the extracted mesh data
    return RfMeshGL(vertices, indices, textures, mat);
}

std::vector<TextureGL> RfModelGL::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<TextureGL> textures;

    for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;

        for (GLuint j = 0; j < _texturesLoaded.size(); j++) {

            if (_texturesLoaded[j].path == str) {

                textures.push_back(_texturesLoaded[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

        if (!skip) { // If texture hasn't been loaded already, load it

            TextureGL texture;
            texture.id = textureFromFile(str.C_Str(), _directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            _texturesLoaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }

    return textures;
}

GLint RfModelGL::textureFromFile(const char* path, std::string directory)
{
    //Generate texture ID and load texture data 
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    // Generate DevIL image.
    ILuint	img;
    ilGenImages(1, &img);
    ilBindImage(img);
    ilLoadImage(filename.c_str());

    /*
    printf("image loaded. (width=%d, height=%d, format=%d, type=%d)\n",
        ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT),
        ilGetInteger(IL_IMAGE_FORMAT),
        ilGetInteger(IL_IMAGE_TYPE));
    //*/

    // TO LOAD CORRECTLY. (IGNORE ALPHA VALUE. JUST LOAD IMAGE TO RGB FORMAT.)
    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

    // Assign texture to ID
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Always call this? NO.
    glTexImage2D(GL_TEXTURE_2D, 0, _gammaCorrection ? GL_SRGB : GL_RGB,
        ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // free DevIL image.
    ilDeleteImages(1, &img);

    return textureID;
}
