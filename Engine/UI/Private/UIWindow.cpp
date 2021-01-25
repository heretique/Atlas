#include "UI/UIWindow.h"

namespace atlas
{
UIWindow::UIWindow(const std::string& name, ImGuiWindowFlags flags)
    : _name(name)
    , _flags(flags)
    , _hash(hq::StringHash(_name))
{
}

UIWindow::~UIWindow() { }

hq::StringHash UIWindow::windowId() const
{
    return _hash;
}

bool UIWindow::update(float deltaTime)
{
    namespace ui = ImGui;
    if(ui::Begin(_name.c_str(), &_isOpen, _flags))
    {
        onGUI(deltaTime);
    }
    ui::End();
    return _isOpen;
}

}  // atlas namespace
