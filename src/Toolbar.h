#pragma once
#include "AUIScreen.h"

class Toolbar : public AUIScreen
{
public:
    Toolbar();
    ~Toolbar() = default;

    // Inherited via AUIScreen
    void DrawUI() override;

    void DisplayFPS();

    void GameObjects();

    void FileMenu();

private:

};

