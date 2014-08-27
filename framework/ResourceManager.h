/*
 * ResourceManager.h
 *
 * ResourceManager (ResMan) loads assets from the HDD and distributes
 * references for that data to nodes that request it. Such assets include
 * meshes and textures.
 * All asset data types are reference counted so that the memory is not
 * duplicated and is properly disposed of when the game shuts down.
 *
 *      Author: MMW
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <json-c/json.h>
#include <rapidxml.hpp>

#include "framework/core.h"
#include "framework/Mesh.h"
#include "framework/SkinnedMesh.h"
#include "framework/SkinnedMorphMesh.h"
#include "framework/MorphMesh.h"
#include "materials/Material.h"

class ResourceManager
{
public:
    ResourceManager();

    ~ResourceManager();

    void loadMeshFromFile(String meshId, String path);
    std::shared_ptr<Mesh> getResourceMesh(String meshId);

    void loadSkinnedMeshFromFile(String meshId, String path);
    void loadSkinnedMorphMeshFromFile(String meshId,
            String path,
            std::vector<String> morphPaths);
    void loadMorphMeshFromFile(String meshId,
            String path,
            std::vector<String> morphPaths);

    std::vector<Bone> loadArmatureClip(String meshId, String path);

    // void loadTextureFromFile(String textureId, String path);
    // GLuint getTexture(String textureId);

    void createMaterial(String materialId, std::shared_ptr<Material>);
    std::shared_ptr<Material> getResourceMaterial(String materialId);

    static String loadShaderFromFile(String path);

private:
    char* loadTextFile(String path);
    Mesh* createMesh(const char* JSONData);
    Mesh* createMorphTargetMesh(const char* JSONData,
            std::vector<bool> indexRecord,
            IndexArray vertexPosRecord,
            IndexArray normalPosRecord);
    std::shared_ptr<SkinnedMesh> createSkinnedMesh(String meshId,
            String path);
    std::shared_ptr<SkinnedMorphMesh> createSkinnedMorphMesh(
            String meshId,
            String path,
            std::vector<String> morphPaths);
    std::shared_ptr<MorphMesh> createMorphMesh(
            String meshId,
            String path,
            std::vector<String> morphPaths);

    void fillXMLNodeList(std::vector<rapidxml::xml_node<>*>& nodes,
            rapidxml::xml_node<> *node);
    std::vector<rapidxml::xml_node<>*> _nodes;

    bool isNear(float v1, float v2);
    bool isNear(Vec3 v1, Vec3 v2);
    bool isNear(Vec2 v1, Vec2 v2);

    /* Map of mesh objects by Id. */
    std::map<String, std::shared_ptr<Mesh>> _meshes;
    std::map<String, std::shared_ptr<Material>> _materials;

    /*
     * Map of OpenGL texture buffer object by Id.
     * (This should be changed to a texture object because ResMan isn't meant
     * to handle device memory.)
     */
    std::map<String, GLuint> _textures;
};

#endif /* RESOURCEMANAGER_H_ */
