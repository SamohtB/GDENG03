#include "AUIScreen.h"

AUIScreen::AUIScreen(String name) : name(name)
{
}

std::string AUIScreen::GetName()
{
    return this->name;
}
