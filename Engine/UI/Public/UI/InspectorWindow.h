#pragma once

#include "UIWindow.h"
#include "Core/Component.h"

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

    template<typename T>
    void inspect(T& t);

    template<>
    void inspect(Component& component);

private:

};
}
