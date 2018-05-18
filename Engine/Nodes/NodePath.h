#pragma once

#include <string>

namespace atlas
{
class NodePath
{
public:
    NodePath(const char* path);
    NodePath(const std::string& path);
    bool isAbsolute() const;

private:
    std::string _path;
};
}
