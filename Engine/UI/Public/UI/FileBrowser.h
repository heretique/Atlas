#pragma once

#include <string>

typedef int ImGuiFileBrowserFlags;

namespace ImGui {

    bool FileBrowser(const std::string& name, std::string& outPath,  bool& open, ImGuiFileBrowserFlags flags = 0);

} // ImGui namespace

enum ImGuiFileBrowserFlags_
{
    ImGuiFileBrowserFlags_None = 0,
    ImGuiFileBrowserFlags_SelectDirectory = 1 << 0, // Select folders
    ImGuiFileBrowserFlags_MultiSelection = 1 << 1, // Allow multi-selection
    ImGuiFileBrowserFlags_CloseOnEsc = 1 << 2, // Close dialog if "Esc" key pressed
};
