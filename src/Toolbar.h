#pragma once
#include "AUIScreen.h"

class Toolbar : public AUIScreen
{
public:
    Toolbar();
    ~Toolbar() = default;

    void DrawUI() override;

private:
    void FileMenu();
    void GameObjects();
    void Lighting();
    void Windows();
    void DrawRightSideInfo();
    void DisplayFPS();
    // Function to draw the playback controls
    void DrawPlaybackControls();
};