#pragma once
#include "Math.h"

// Forward-declare ifstream
#include <fstream>

class SceneReader
{
public:
    SceneReader(String directory);
    ~SceneReader() = default;

    void ReadFromFile(const String& fileName);

private:
    void ParseFile(std::ifstream& file);

    String m_directory;

    struct ObjectData
    {
        String name;
        String parentName;
        Vector3 position;
        Vector3 scale;
        rp3d::Quaternion rotation;
        String meshType;
        String rigidbodyType; // [MODIFIED] Changed from bool to String
    };
};