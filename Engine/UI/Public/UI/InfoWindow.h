#pragma once

#include "UIWindow.h"

namespace atlas
{
class InfoWindow : public UIWindow
{
public:
    InfoWindow(const std::string& name, ImGuiWindowFlags flags)
        : UIWindow(name, flags)
    {
    }

    // UIWindow interface
    void onGUI(float deltaTime);
};
} // atlas namespace
