#pragma once

#include "Hq/StringHash.h"
#include <unordered_map>
#include <memory>

namespace atlas
{

class UIWindow;

class UIManager
{
public:
    UIManager();
    ~UIManager();
    // takes ownership of the window
    bool addWindow(UIWindow* window);
    void removeWindow(hq::StringHash windowId);
    void update(float deltaTime);

private:
    std::unordered_map<hq::StringHash, std::unique_ptr<UIWindow>> _windows;
    std::vector<hq::StringHash> _closedWindows;
};

}  // atlas
