#pragma once

class AGameObject;

class SceneWriter
{
public:
    SceneWriter(String directory);
    ~SceneWriter() = default;

    void WriteToFile(String fileName);

    void SaveObjectData(std::shared_ptr<AGameObject> object, std::ofstream& sceneFile);

private:
    String m_directory;
};

