#pragma once
class SceneWriter
{
public:
    SceneWriter(String directory);
    ~SceneWriter() = default;

    void WriteToFile(String fileName);

private:
    String m_directory;
};

