#pragma once

#include <string>
#include "Hq/StringHash.h"
#include "imgui.h"

namespace atlas
{
class UIManager;

class UIWindow
{
    friend class UIManager;

public:
    UIWindow(const std::string& name, ImGuiWindowFlags flags);
    virtual ~UIWindow();

    virtual void onGUI(float deltaTime) = 0;
    hq::StringHash windowId() const;

private:
    // if it returns false means the windows is closed and it is removed from UIManager
    bool update(float deltaTime);

private:
    std::string      _name;
    ImGuiWindowFlags _flags;
    bool             _isOpen {true};
    hq::StringHash   _hash;
};

}  // atlas namespace
