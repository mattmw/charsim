/*
 * ResourceManager.cpp
 *
 *      Author: MMW
 */

#include "ResourceManager.h"
#include <fstream>
#include <cstring>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
#if DEBUG==true
    printf("\nDestructing ResourceManager");
#endif
}

std::vector<Bone> ResourceManager::loadArmatureClip(String meshId, String path)
{
    /* Get bone hierarchy from mesh. */
    std::shared_ptr<SkinnedMesh> mesh =
            std::dynamic_pointer_cast<SkinnedMesh>(
                    getResourceMesh(meshId));

    uint boneCount = 0;
    /* Number of animated pose keyframes incl. bind pose. */
    uint timestampCount = 0;
    std::vector<float> timestamps;

    /* Import text data. */
    char* cXMLData = loadTextFile(path);
    std::string XMLData = cXMLData;
    delete[] cXMLData;

    char* buffer = new char[XMLData.size() + 1];
    strcpy(buffer, XMLData.c_str());

    rapidxml::xml_document<> doc;
    doc.parse<0>(buffer);

    rapidxml::xml_node<> *node = doc.first_node();
    std::vector<rapidxml::xml_node<>*> nodes;
    fillXMLNodeList(nodes, node);

    /* Get animation nodes. */
    std::vector<rapidxml::xml_node<>*> animationNodes;
    /* For bone count. */
    std::vector<rapidxml::xml_node<>*> libraryControllerNodes;
    std::for_each(nodes.begin(), nodes.end(),
            [this, &animationNodes, &libraryControllerNodes]
            (rapidxml::xml_node<>* node)
    {
        String name = node->name();

        if (name == "library_animations")
        {
            fillXMLNodeList(animationNodes, node);
        }

        if (name == "library_controllers")
        {
            fillXMLNodeList(libraryControllerNodes, node);
        }
    });

    /* Get number of bones. */
    std::for_each(libraryControllerNodes.begin(), libraryControllerNodes.end(),
            [&boneCount]
            (rapidxml::xml_node<>* node)
    {
        String name = node->name();

        if (name == "Name_array")
        {
            boneCount = std::stoi(node->first_attribute("count")->value());
        }
    });

    /* Parse animation nodes. */
    uint count = 0;
    uint maxCount = boneCount*2;
    std::vector<String> floatArrays;
    std::for_each(animationNodes.begin(), animationNodes.end(),
            [&count, &maxCount, &floatArrays] (rapidxml::xml_node<>* node)
    {
        String name = node->name();

        if (name == "float_array" && count < maxCount)
        {
            floatArrays.push_back(node->value());

            count++;
        }
    });

    /* Get timestamps. */
    String element;
    std::stringstream stream(floatArrays[0]);
    while (getline(stream, element, ' '))
    {
        timestampCount++;
        float timestamp = std::stod(element);
        timestamps.push_back(timestamp);
    }

    /* Get matrices. */
    Mat4Array matList = Mat4Array();
    for (uint i = 0; i < floatArrays.size(); i++)
    {
        if (i % 2)
        {
            std::vector<float> elements;
            String element;
            std::stringstream stream(floatArrays[i]);
            while (getline(stream, element, ' '))
            {
                elements.push_back(std::stod(element));
            }

            uint stride = 16;
            for (uint j = 0; j < timestampCount*stride; j += stride)
            {
                Mat4 mat = Mat4(
                        elements[j+0], elements[j+1], elements[j+2], elements[j+3],
                        elements[j+4], elements[j+5], elements[j+6], elements[j+7],
                        elements[j+8], elements[j+9], elements[j+10], elements[j+11],
                        elements[j+12], elements[j+13], elements[j+14], elements[j+15]
                );

                matList.push_back(glm::transpose(mat));
            }
        }
    }

    std::vector<Bone> boneChain = mesh->getBoneChain();

    const uint& stride = timestampCount;
    count = 0;
    for (uint i = 0; i < boneCount*timestampCount; i += stride)
    {
        std::vector<BonePose> poses;

        for (uint j = 0; j < stride; j++)
        {
            BonePose pose;
            Quat rot;
            Vec3 pos;

            rot = glm::toQuat(matList[i+j]);
            pos.x = matList[i+j][3].x;
            pos.y = matList[i+j][3].y;
            pos.z = matList[i+j][3].z;

            /* Rotate armature so it sits upright. */
            Vec3 shiftEuler(glm::radians(90.0f), 0.0f, 0.0f);
            Quat shiftQuat = Quat(shiftEuler);
            pos = pos * shiftQuat;

            pose._timestamp = timestamps[j];
            pose._localPoseRotation = rot;
            pose._localPoseTranslation = pos;

            poses.push_back(pose);
        }
        boneChain[count]._poses = poses;
        count++;
    }

    delete[] buffer;

    return boneChain;
}

void ResourceManager::fillXMLNodeList(std::vector<rapidxml::xml_node<>*>& nodes,
        rapidxml::xml_node<> *node)
{
    char zero = '\0';

    for (rapidxml::xml_node<>* nextNode = node->first_node();
            nextNode;
            nextNode = nextNode->next_sibling())
    {
        if (*nextNode->name() != zero)
        {
            nodes.push_back(nextNode);
            fillXMLNodeList(nodes, nextNode);
        }
    }
}

void ResourceManager::loadMeshFromFile(String meshId, String path)
{
    cout<<"\nLoading mesh: "<<path<<"..."<<endl;;

    if (_meshes.find(meshId) == _meshes.end())
    {
        /* Load file */
        char* JSONData = loadTextFile(path);

        Mesh* tempMesh = createMesh(JSONData);

        delete[] JSONData;

        /* Declare new mesh and pass vertex list. */
        std::shared_ptr<Mesh> newMesh;
        newMesh = std::make_shared<Mesh>(meshId);
        newMesh->setVertices(tempMesh->getVertices());
        newMesh->setNormals(tempMesh->getNormals());
        newMesh->setUVs(tempMesh->getUVs());
        newMesh->setIndices(tempMesh->getIndices());
        newMesh->loadVertexBuffers();
        newMesh->generateNormalLines();
        _meshes[meshId] = newMesh;

        delete tempMesh;
    }
}

std::shared_ptr<Mesh> ResourceManager::getResourceMesh(String meshId)
{
    if (_meshes.find(meshId) == _meshes.end())
    {
        String msg = "Mesh of ID: \"";
        msg += meshId;
        msg+= "\" not found.";
        throw InitException(msg);
    }

    return _meshes[meshId];
}

std::shared_ptr<SkinnedMesh> ResourceManager::createSkinnedMesh(
        String meshId,
        String path)
{
    /* Load file */
    char* JSONData = loadTextFile(path);

    Mesh* tempMesh = createMesh(JSONData);

    /* Imported bone data. */
    std::vector<int> tempBoneParents;
    std::vector<String> tempBoneNames;
    Vec3Array tempBonePositions;
    QuatArray tempBoneRotations;

    /* Imported skinning data. */
    IndexArray tempSkinIndices;
    std::vector<float> tempSkinWeights;

    /* Get skeleton, pose and animation info. */
    json_object* jobj = json_tokener_parse(JSONData);
    enum json_type type;
    json_object_object_foreach(jobj, key, val)
    {
        String keyString = key;
        type = json_object_get_type(val);
        switch (type)
        {
            case json_type_array:
                if (keyString == "bones")
                {
                    /*
                     * Iterate through array and grab object inside.
                     */
                    json_object* boneArrayObj = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(boneArrayObj);

                    for (int i = 0; i < arrayLength; i++)
                    {
                        json_object* boneObj = json_object_array_get_idx(boneArrayObj, i);

                        json_object_object_foreach(boneObj, key, val)
                        {
                            String keyString = key;
                            type = json_object_get_type(val);
                            switch (type) {
                                case json_type_array:

                                    if (keyString == "pos")
                                    {
                                        json_object* posObjArray = json_object_object_get(boneObj, key);
                                        for (int j = 0; j < 3; j+=3)
                                        {
                                            Vec3 bonePos;
                                            json_object* posObj = json_object_array_get_idx(posObjArray, j);
                                            bonePos.x = json_object_get_double(posObj);
                                            posObj = json_object_array_get_idx(posObjArray, j+1);
                                            bonePos.y = json_object_get_double(posObj);
                                            posObj = json_object_array_get_idx(posObjArray, j+2);
                                            bonePos.z = json_object_get_double(posObj);
                                            tempBonePositions.push_back(bonePos);
                                        }
                                    }
                                    else if (keyString == "rotq")
                                    {
                                        json_object* rotqObjArray = json_object_object_get(boneObj, key);
                                        for (int j = 0; j < 4; j+=4)
                                        {

                                            Quat boneRot;
                                            json_object* rotqObj = json_object_array_get_idx(rotqObjArray, j);
                                            boneRot.x = json_object_get_double(rotqObj);
                                            rotqObj = json_object_array_get_idx(rotqObjArray, j+1);
                                            boneRot.y = json_object_get_double(rotqObj);
                                            rotqObj = json_object_array_get_idx(rotqObjArray, j+2);
                                            boneRot.z = json_object_get_double(rotqObj);
                                            rotqObj = json_object_array_get_idx(rotqObjArray, j+3);
                                            boneRot.w = json_object_get_double(rotqObj);

                                            tempBoneRotations.push_back(boneRot);
                                        }
                                    }
                                    break;
                                case json_type_string:
                                    if (keyString == "name")
                                    {
                                        json_object* nameObj = json_object_object_get(boneObj, key);
                                        String boneName = json_object_get_string(nameObj);
                                        tempBoneNames.push_back(boneName);
                                    }
                                    break;
                                case json_type_int:
                                    if (keyString == "parent")
                                    {
                                        json_object* parentObj = json_object_object_get(boneObj, key);
                                        int parentId = json_object_get_int(parentObj);
                                        tempBoneParents.push_back(parentId);
                                    }
                                    break;
                            }
                        }
                    }
                }
                else if (keyString == "skinIndices")
                {
                    json_object* skinIndicesArrayObj = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(skinIndicesArrayObj);

                    for (int i = 0; i < arrayLength; i++)
                    {
                        json_object* skinIndicesObj = json_object_array_get_idx(skinIndicesArrayObj, i);
                        uint skinIndex = json_object_get_int(skinIndicesObj);
                        tempSkinIndices.push_back(skinIndex);
                    }
                }
                else if (keyString == "skinWeights")
                {
                    json_object* skinWeightsArrayObj = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(skinWeightsArrayObj);

                    for (int i = 0; i < arrayLength; i++)
                    {
                        json_object* skinWeightsObj = json_object_array_get_idx(skinWeightsArrayObj, i);
                        float skinWeight = (float)json_object_get_double(skinWeightsObj);
                        tempSkinWeights.push_back(skinWeight);
                    }
                }
                break;
            case json_type_string:
                break;
        }
    }
    json_object_put(jobj);
    delete[] JSONData;

    /*
     * Fill SkinnedMesh structs and pass them to new_skinned_mesh.
     * Check if vertex matches that in vertex map, add vertex weight list.
     */
    std::vector<SkinWeights> skinWeights;
    std::vector<SkinIndices> skinIndices;
    uint vertexCount = tempMesh->getVertexCount();
    Vec3Array JSONVertices = tempMesh->getJSONVertices();
    Vec3Array tempVertices = tempMesh->getVertices();
    skinWeights.resize(vertexCount);
    skinIndices.resize(vertexCount);

    /* Index 'i' refers to Three.JS vertex weight list. */
    /* Index 'j' refers to Three.JS vertex list. */
    uint i, j;
    for (i = 0, j = 0; i < tempSkinWeights.size(); i+=INFLUENCES, j++)
    {
        /* Create vertex weight table */
        SkinWeights skinWeightList;
        skinWeightList._boneWeight1 = tempSkinWeights[i];
        skinWeightList._boneWeight2 = tempSkinWeights[i+1];
        skinWeightList._boneWeight3 = tempSkinWeights[i+2];
        skinWeightList._boneWeight4 = tempSkinWeights[i+3];
        /* Create vertex weight index table. */
        SkinIndices skinIndexList;
        skinIndexList._boneIndex1 = tempSkinIndices[i];
        skinIndexList._boneIndex2 = tempSkinIndices[i+1];
        skinIndexList._boneIndex3 = tempSkinIndices[i+2];
        skinIndexList._boneIndex4 = tempSkinIndices[i+3];

        /*
         * Check where JSON vertex matches in vertex list and add skin weight
         * and index tables to relevant positions.
         */
        for (uint k = 0; k < vertexCount; k++)
        {
            if (isNear(JSONVertices[j], tempVertices[k]))
            {
                skinWeights[k] = skinWeightList;
                skinIndices[k] = skinIndexList;
            }
        }
    }

    std::vector<Bone> bones;
    for (uint i = 0; i < tempBoneParents.size(); i++)
    {
        Bone bone;
        bone._parentIndex = tempBoneParents[i];
        bone._name = tempBoneNames[i];

        /* Load bind pose. */
        BonePose bindPose;
        bindPose._localPoseTranslation = tempBonePositions[i];
        bindPose._localPoseRotation = tempBoneRotations[i];
        bone._poses.push_back(bindPose);

        bones.push_back(bone);
    }

    std::shared_ptr<SkinnedMesh> tempSkinnedMesh;
    tempSkinnedMesh = std::make_shared<SkinnedMesh>(meshId);

    tempSkinnedMesh->setIndexRecord(tempMesh->getIndexRecord());
    tempSkinnedMesh->setPosRecords(tempMesh->getVertexPosRecord(),
            tempMesh->getNormalPosRecord());
    tempSkinnedMesh->setIndices(tempMesh->getIndices());
    tempSkinnedMesh->setVertices(tempMesh->getVertices());
    tempSkinnedMesh->setNormals(tempMesh->getNormals());
    tempSkinnedMesh->setUVs(tempMesh->getUVs());

    tempSkinnedMesh->setBoneChain(bones);
    tempSkinnedMesh->setSkinIndices(skinIndices);
    tempSkinnedMesh->setSkinWeights(skinWeights);

    delete tempMesh;

    return tempSkinnedMesh;
}

std::shared_ptr<SkinnedMorphMesh> ResourceManager::createSkinnedMorphMesh(
        String meshId,
        String path,
        std::vector<std::string> morphPaths)
{
    std::shared_ptr<SkinnedMesh> tempSkinnedMesh;
    tempSkinnedMesh = createSkinnedMesh("", path);

    std::shared_ptr<SkinnedMorphMesh> tempSkinnedMorphMesh;
    tempSkinnedMorphMesh = std::make_shared<SkinnedMorphMesh>(meshId);

    /* Set skinning and mesh properties. */
    tempSkinnedMorphMesh->setIndices(tempSkinnedMesh->getIndices());
    tempSkinnedMorphMesh->setVertices(tempSkinnedMesh->getVertices());
    tempSkinnedMorphMesh->setNormals(tempSkinnedMesh->getNormals());
    tempSkinnedMorphMesh->setUVs(tempSkinnedMesh->getUVs());

    tempSkinnedMorphMesh->setBoneChain(tempSkinnedMesh->getBoneChain());
    tempSkinnedMorphMesh->setSkinIndices(tempSkinnedMesh->getSkinIndices());
    tempSkinnedMorphMesh->setSkinWeights(tempSkinnedMesh->getSkinWeights());

    Vec3Array tempMorphVertices;
    Vec3Array tempMorphNormals;
    int vertexCountPerMesh = 0;
    for (std::vector<std::string>::iterator itMorphPaths = morphPaths.begin();
            itMorphPaths != morphPaths.end();
            itMorphPaths++)
    {
        cout<<"Loading morph target mesh: "<<*itMorphPaths<<"..."<<endl;

        char* JSONData = loadTextFile(*itMorphPaths);
        Mesh* tempMesh = createMorphTargetMesh(JSONData,
                tempSkinnedMesh->getIndexRecord(),
                tempSkinnedMesh->getVertexPosRecord(),
                tempSkinnedMesh->getNormalPosRecord());
        delete[] JSONData;

        auto tempVertices = tempMesh->getVertices();
        auto tempNormals = tempMesh->getNormals();
        auto tempBaseVertices = tempSkinnedMesh->getVertices();
        auto tempBaseNormals = tempSkinnedMesh->getNormals();

        /*
         * Make sure the vertices of each file match.
         * If so, set number of vertices per morph.
         */
        if (itMorphPaths == morphPaths.begin())
        {
            vertexCountPerMesh = tempVertices.size();
        }
        else
        {
            int newVertexCount = tempVertices.size();
            if (newVertexCount != vertexCountPerMesh)
            {
                throw InitException("Error: vertex count of morph targets don't match.");
            }
        }

        delete tempMesh;

        /* Offset vertices and normals. */
        for (uint i = 0; i < tempVertices.size(); i++)
        {
            tempVertices[i] = tempVertices[i] - tempBaseVertices[i];
            tempNormals[i] = tempNormals[i] - tempBaseNormals[i];
        }

        tempMorphVertices.insert(tempMorphVertices.end(),
                tempVertices.begin(),
                tempVertices.end());

        tempMorphNormals.insert(tempMorphNormals.end(),
                tempNormals.begin(),
                tempNormals.end());
    }
    tempSkinnedMorphMesh->setMorphVertices(tempMorphVertices);
    tempSkinnedMorphMesh->setMorphNormals(tempMorphNormals);
    /*
     * Set vertex offset for renderer.
     * (vertexCountPerMesh * size of float * number of floats in vertex.)
     */
    tempSkinnedMorphMesh->setVertexOffset(vertexCountPerMesh*sizeof(float)*3);

    return tempSkinnedMorphMesh;
}

std::shared_ptr<MorphMesh> ResourceManager::createMorphMesh(
        String meshId,
        String path,
        std::vector<std::string> morphPaths)
{
    char* JSONData = loadTextFile(path);

    Mesh* tempMesh = createMesh(JSONData);

    delete[] JSONData;

    std::shared_ptr<MorphMesh> tempMorphMesh;
    tempMorphMesh = std::make_shared<MorphMesh>(meshId);

    /* Set skinning and mesh properties. */
    tempMorphMesh->setIndices(tempMesh->getIndices());
    tempMorphMesh->setVertices(tempMesh->getVertices());
    tempMorphMesh->setNormals(tempMesh->getNormals());
    tempMorphMesh->setUVs(tempMesh->getUVs());

    Vec3Array tempMorphVertices;
    Vec3Array tempMorphNormals;
    int vertexCountPerMesh = 0;
    for (std::vector<std::string>::iterator itMorphPaths = morphPaths.begin();
            itMorphPaths != morphPaths.end();
            itMorphPaths++)
    {
        cout<<"Loading morph target mesh: "<<*itMorphPaths<<"..."<<endl;

        char* JSONData = loadTextFile(*itMorphPaths);
        Mesh* tempMorphTargetMesh = createMorphTargetMesh(JSONData,
                tempMesh->getIndexRecord(),
                tempMesh->getVertexPosRecord(),
                tempMesh->getNormalPosRecord());
        delete[] JSONData;

        auto tempVertices = tempMorphTargetMesh->getVertices();
        auto tempNormals = tempMorphTargetMesh->getNormals();
        auto tempBaseVertices = tempMesh->getVertices();
        auto tempBaseNormals = tempMesh->getNormals();

        /*
         * Make sure the vertices of each file match.
         * If so, set number of vertices per morph.
         */
        if (itMorphPaths == morphPaths.begin())
        {
            vertexCountPerMesh = tempVertices.size();
        }
        else
        {
            int newVertexCount = tempVertices.size();
            if (newVertexCount != vertexCountPerMesh)
            {
                throw InitException("Error: vertex count of morph targets don't match.");
            }
        }

        delete tempMesh;
        delete tempMorphTargetMesh;

        /* Offset vertices and normals. */
        for (uint i = 0; i < tempVertices.size(); i++)
        {
            tempVertices[i] = tempVertices[i] - tempBaseVertices[i];
            tempNormals[i] = tempNormals[i] - tempBaseNormals[i];
        }

        tempMorphVertices.insert(tempMorphVertices.end(),
                tempVertices.begin(),
                tempVertices.end());

        tempMorphNormals.insert(tempMorphNormals.end(),
                tempNormals.begin(),
                tempNormals.end());
    }
    tempMorphMesh->setMorphVertices(tempMorphVertices);
    tempMorphMesh->setMorphNormals(tempMorphNormals);
    /*
     * Set vertex offset for renderer.
     * (vertexCountPerMesh * size of float * number of floats in vertex.)
     */
    tempMorphMesh->setVertexOffset(vertexCountPerMesh*sizeof(float)*3);

    return tempMorphMesh;
}

void ResourceManager::loadSkinnedMeshFromFile(String meshId, String path)
{
    cout<<"\nLoading skinned mesh: "<<path<<"..."<<endl;

    std::shared_ptr<SkinnedMesh> newSkinnedMesh;
    newSkinnedMesh = createSkinnedMesh(meshId, path);

    newSkinnedMesh->loadVertexBuffers();
    newSkinnedMesh->loadSkinnedVertexBuffers();

    _meshes[meshId] = newSkinnedMesh;
}

void ResourceManager::loadSkinnedMorphMeshFromFile(String meshId,
        String path,
        std::vector<String> morphPaths)
{
    cout<<"\nLoading skinned morph mesh: "<<path<<"..."<<endl;

    std::shared_ptr<SkinnedMorphMesh> newSkinnedMorphMesh;
    newSkinnedMorphMesh = createSkinnedMorphMesh(meshId, path, morphPaths);

    newSkinnedMorphMesh->loadVertexBuffers();
    newSkinnedMorphMesh->loadSkinnedVertexBuffers();
    newSkinnedMorphMesh->loadMorphVertexBuffers();

    _meshes[meshId] = newSkinnedMorphMesh;
}

void ResourceManager::loadMorphMeshFromFile(String meshId,
        String path,
        std::vector<String> morphPaths)
{
    cout<<"\nLoading morph mesh: "<<path<<"..."<<endl;

    std::shared_ptr<MorphMesh> newMorphMesh;
    newMorphMesh = createMorphMesh(meshId, path, morphPaths);

    newMorphMesh->loadVertexBuffers();
    newMorphMesh->loadMorphVertexBuffers();

    _meshes[meshId] = newMorphMesh;
}

char* ResourceManager::loadTextFile(String path)
{
    std::ifstream f;
    f.open(path, std::ios::in);

    if (!f.is_open())
        throw FileException("Could not open file.", path);

    std::streampos fileSize;
    char* textData;

    /* Set position to end. */
    f.seekg(0, std::ios::end);
    /* Get current position. */
    fileSize = f.tellg();
    /* Create charstring with extra byte for null terminator. */
    textData = new char[(long)fileSize+1];
    /* set pos to beginning. */
    f.seekg(0, std::ios::beg);
    f.read(textData, fileSize);

    textData[fileSize] = '\0';

    f.close();

    return textData;
}

bool ResourceManager::isNear(float v1, float v2)
{
    float epsilon = 0.0001f;
    return fabs( v1-v2 ) < epsilon;
}

bool ResourceManager::isNear(Vec3 v1, Vec3 v2)
{
    float epsilon = 0.0001f;
    if (fabs(v1.x-v2.x) < epsilon
            && fabs(v1.y-v2.y) < epsilon
            && fabs(v1.z-v2.z) < epsilon)
        return true;
    else
        return false;
}

bool ResourceManager::isNear(Vec2 v1, Vec2 v2)
{
    float epsilon = 0.0001f;
    if (fabs(v1.x-v2.x) < epsilon
            && fabs(v1.y-v2.y) < epsilon)
        return true;
    else
        return false;
}

Mesh* ResourceManager::createMesh(const char* JSONData)
{
    /* Declare temporary coord lists */
    std::vector<float> vertexCoords;
    std::vector<float> normalCoords;
    std::vector<float> UVCoords;
    /* Faces list */
    IndexArray faces;
    /* Declare temporary vertex and vertex normals list. */
    Vec3Array tempVertices;
    Vec3Array tempNormals;
    Vec2Array tempUVs;
    Vec3Array indexedVertices;
    Vec3Array indexedNormals;
    Vec2Array indexedUVs;
    IndexArray tempIndices;

    /* Parse JSON file and add vertices to vertex list. */
    json_object* jobj = json_tokener_parse(JSONData);

    enum json_type type;
    json_object_object_foreach(jobj, key, val)
    {
        String keyString = key;
        type = json_object_get_type(val);
        switch (type)
        {
            case json_type_array:
                if (keyString == "vertices")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(jobj2);

                    json_object* jval;

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jobj2, i);
                        vertexCoords.push_back((float)json_object_get_double(jval));
                    }
                }
                if (keyString == "normals")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(jobj2);

                    json_object* jval;

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jobj2, i);
                        normalCoords.push_back((float)json_object_get_double(jval));
                    }
                }
                if (keyString == "uvs")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    json_object* jsonArray;
                    json_object* jval;
                    jsonArray = json_object_array_get_idx(jobj2, 0);

                    int arrayLength = json_object_array_length(jsonArray);

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jsonArray, i);
                        UVCoords.push_back((float)json_object_get_double(jval));
                    }
                }
                if (keyString == "faces")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(jobj2);

                    json_object* jval;

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jobj2, i);
                        faces.push_back(json_object_get_int(jval));
                    }
                }
                break;
            case json_type_string:
                break;
        }
    }
    json_object_put(jobj);

    /* Parse 'faces' and put vertices into 'temp_vertices'. */
    uint coordsPerVertex = 3;
    uint coordsPerUV = 2;
    int pos;

    /*
     * Hashtable for Three.JS indices so that Three.JS vertex weights can be
     * matched.
     */
    MapIndexVec3 mapVertices;
    MapIndexVec3 mapNormals;

    Vec3Array baseVertices;
    for (uint i = 0; i < vertexCoords.size(); i+=3)
    {
        Vec3 tempVertex;
        tempVertex.x = vertexCoords[i];
        tempVertex.y = vertexCoords[i+1];
        tempVertex.z = vertexCoords[i+2];
        baseVertices.push_back(tempVertex);
    }

    IndexArray vertexPosRecord;
    IndexArray normalPosRecord;

    /* Generate vertices, normals and UVs from faces. */
    for (uint i = 1; i < faces.size(); i+=10)
    {
        /* Generate three vertices for face. */
        for (int j = 0; j < 3; j++)
        {
            Vec3 tempVertex;
            Vec3 tempNormal;
            Vec2 tempUV;

            pos = faces[i+j] * coordsPerVertex;
            vertexPosRecord.push_back(pos);

            tempVertex.x = vertexCoords[pos];
            tempVertex.y = vertexCoords[pos+1];
            tempVertex.z = vertexCoords[pos+2];

            pos = faces[(i+6)+j] * coordsPerVertex;
            normalPosRecord.push_back(pos);

            tempNormal.x = normalCoords[pos];
            tempNormal.y = normalCoords[pos+1];
            tempNormal.z = normalCoords[pos+2];

            pos = faces[(i+3)+j] * coordsPerUV;

            tempUV.x = UVCoords[pos];
            tempUV.y = UVCoords[pos+1];

            /* Generate unindexed lists. Indexing comes later. */
            tempVertices.push_back(tempVertex);
            tempNormals.push_back(tempNormal);
            tempUVs.push_back(tempUV);
        }
    }

    /*
     * Index test record so that morph target meshes are indexed identically
     * to the base mesh.
     */
    std::vector<bool> indexRecord;

    /* Actually generate indices. */
    bool found = false;
    Index index;
    for (Index i = 0; i < tempVertices.size(); i++)
    {
        found = false;
        /* Compare with indexed list. */
        for (Index j = 0; j < indexedVertices.size(); j++)
        {
            if (
                    isNear(tempVertices[i], indexedVertices[j]) &&
                    isNear(tempNormals[i], indexedNormals[j]) &&
                    isNear(tempUVs[i], indexedUVs[j])
            )
            {
                found = true;
                index = j;
            }
        }

        if (found)
        {
            tempIndices.push_back(index);
            indexRecord.push_back(found);
        }
        else
        {
            indexedVertices.push_back(tempVertices[i]);
            indexedNormals.push_back(tempNormals[i]);
            indexedUVs.push_back(tempUVs[i]);
            tempIndices.push_back(indexedVertices.size()-1);
            indexRecord.push_back(found);
        }
    }

    /* Normalise vertex normals. */
    std::for_each(indexedNormals.begin(), indexedNormals.end(), [] (Vec3& normal)
    {
        normal = glm::normalize(normal);
    });

    Mesh* tempMesh = new Mesh("");
    tempMesh->setIndexRecord(indexRecord);
    tempMesh->setPosRecords(vertexPosRecord, normalPosRecord);
    tempMesh->setJSONVertices(baseVertices);
    tempMesh->setVertices(indexedVertices);
    tempMesh->setNormals(indexedNormals);
    tempMesh->setUVs(indexedUVs);

    tempMesh->setIndices(tempIndices);

    return tempMesh;
}

Mesh* ResourceManager::createMorphTargetMesh(const char* JSONData,
        std::vector<bool> indexRecord,
        IndexArray vertexPosRecord,
        IndexArray normalPosRecord)
{
    /* Declare temporary coord lists */
    std::vector<float> vertexCoords;
    std::vector<float> normalCoords;
    std::vector<float> UVCoords;
    /* Faces list */
    IndexArray faces;
    /* Declare temporary vertex and vertex normals list. */
    Vec3Array tempVertices;
    Vec3Array tempNormals;
    Vec2Array tempUVs;
    Vec3Array indexedVertices;
    Vec3Array indexedNormals;
    Vec2Array indexedUVs;
    IndexArray tempIndices;

    /* Parse JSON file and add vertices to vertex list. */
    json_object* jobj = json_tokener_parse(JSONData);

    enum json_type type;
    json_object_object_foreach(jobj, key, val)
    {
        String keyString = key;
        type = json_object_get_type(val);
        switch (type)
        {
            case json_type_array:
                if (keyString == "vertices")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(jobj2);

                    json_object* jval;

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jobj2, i);
                        vertexCoords.push_back((float)json_object_get_double(jval));
                    }
                }
                if (keyString == "normals")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(jobj2);

                    json_object* jval;

                    /* Fill array of floats.*/
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jobj2, i);
                        normalCoords.push_back((float)json_object_get_double(jval));
                    }
                }
                if (keyString == "uvs")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    json_object* jsonArray;
                    json_object* jval;
                    jsonArray = json_object_array_get_idx(jobj2, 0);

                    int arrayLength = json_object_array_length(jsonArray);

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jsonArray, i);
                        UVCoords.push_back((float)json_object_get_double(jval));
                    }
                }
                if (keyString == "faces")
                {
                    json_object* jobj2 = json_object_object_get(jobj, key);
                    int arrayLength = json_object_array_length(jobj2);

                    json_object* jval;

                    /* Fill array of floats. */
                    for (int i = 0; i < arrayLength; i++)
                    {
                        jval = json_object_array_get_idx(jobj2, i);
                        faces.push_back(json_object_get_int(jval));
                    }
                }
                break;
            case json_type_string:
                break;
        }
    }
    json_object_put(jobj);

    /* Parse 'faces' and put vertices into 'temp_vertices'. */
    uint coordsPerVertex = 3;
    uint coordsPerUV = 2;
    int pos;

    /*
     * Hashtable for Three.JS indices so that Three.JS vertex weights can be
     * matched.
     */
    MapIndexVec3 mapVertices;
    MapIndexVec3 mapNormals;

    Vec3Array baseVertices;
    for (uint i = 0; i < vertexCoords.size(); i+=3)
    {
        Vec3 tempVertex;
        tempVertex.x = vertexCoords[i];
        tempVertex.y = vertexCoords[i+1];
        tempVertex.z = vertexCoords[i+2];
        baseVertices.push_back(tempVertex);
    }

    /* Generate vertices, normals and UVs from faces. */
    Index count = 0;
    for (Index i = 1; i < faces.size(); i+=10)
    {
        /* Generate three vertices for face. */
        for (Index j = 0; j < 3; j++)
        {
            Vec3 tempVertex;
            Vec3 tempNormal;
            Vec2 tempUV;

            pos = vertexPosRecord[count];

            tempVertex.x = vertexCoords[pos];
            tempVertex.y = vertexCoords[pos+1];
            tempVertex.z = vertexCoords[pos+2];

            pos = faces[(i+6)+j] * coordsPerVertex;

            count++;

            tempNormal.x = normalCoords[pos];
            tempNormal.y = normalCoords[pos+1];
            tempNormal.z = normalCoords[pos+2];

            pos = faces[(i+3)+j] * coordsPerUV;

            tempUV.x = UVCoords[pos];
            tempUV.y = UVCoords[pos+1];

            /* Generate unindexed lists. Indexing comes later. */
            tempVertices.push_back(tempVertex);
            tempNormals.push_back(tempNormal);
            tempUVs.push_back(tempUV);
        }
    }

    /* Actually generate indices. */
    uint index;
    for (uint i = 0; i < tempVertices.size(); i++)
    {
        /* Compare with indexed list. */
        for (uint j = 0; j < indexedVertices.size(); j++)
        {
            if (indexRecord[i])
            {
                index = j;
            }
        }

        if (indexRecord[i])
        {
            tempIndices.push_back(index);
        }
        else
        {
            indexedVertices.push_back(tempVertices[i]);
            indexedNormals.push_back(tempNormals[i]);
            indexedUVs.push_back(tempUVs[i]);
            tempIndices.push_back(indexedVertices.size()-1);
        }
    }

    /* Normalise vertex normals. */
    std::for_each(indexedNormals.begin(), indexedNormals.end(), [] (Vec3 &normal)
    {
        normal = glm::normalize(normal);
    });

    Mesh* tempMesh = new Mesh("");

    tempMesh->setJSONVertices(baseVertices);
    tempMesh->setVertices(indexedVertices);
    tempMesh->setNormals(indexedNormals);
    tempMesh->setUVs(indexedUVs);

    tempMesh->setIndices(tempIndices);

    return tempMesh;
}

void ResourceManager::createMaterial(String materialId, std::shared_ptr<Material> material)
{
    if (_materials.find(materialId) == _materials.end())
    {
        _materials[materialId] = material;
    }
}

std::shared_ptr<Material> ResourceManager::getResourceMaterial(String materialId)
{
    if (_materials.find(materialId) == _materials.end())
    {
        String msg = "Material of ID: \"";
        msg += materialId;
        msg+= "\" not found.";
        throw InitException(msg);
    }

    return _materials[materialId];
}

String ResourceManager::loadShaderFromFile(String path)
{
    /* Load file */
    std::ifstream f;
    f.open(path, std::ios::in);


    if (!f.is_open())
        throw FileException("Could not open file.", path);

    String shaderFileContents;
    f.seekg(0, std::ios::end);
    shaderFileContents.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    shaderFileContents.insert(shaderFileContents.end(),
            std::istreambuf_iterator<char>(f),
            std::istreambuf_iterator<char>());

    f.close();

    return shaderFileContents;
}
