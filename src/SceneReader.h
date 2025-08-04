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
        String objectName;
        String meshType;
        Vector3 position;
        rp3d::Quaternion rotation;
        Vector3 scale;
        bool hasRigidbody;
    };
};

