#pragma once
#include "Math.h"

class SceneReader
{
public:
    SceneReader(String directory);
    ~SceneReader() = default;

    void ReadFromFile(const String& fileName);
    void ParseGameObject(std::ifstream& file);

private:
    String m_directory;

    struct ObjectData 
    {
        String name;
        String parentName;
        Vector3 position;
        Vector3 scale;
        rp3d::Quaternion rotation;
        String meshType;
        bool hasRigidbody;
    };
};

