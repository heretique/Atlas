#pragma once

#include "UIWindow.h"

namespace atlas
{
class InspectorWindow : public UIWindow
{
public:
    InspectorWindow(const std::string& name, ImGuiWindowFlags flags)
        : UIWindow(name, flags)
    {
    }

    // UIWindow interface
public:
    void onGUI(float deltaTime);
};
}
