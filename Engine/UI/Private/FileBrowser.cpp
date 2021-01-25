#include "UI/FileBrowser.h"
#include "imgui.h"
#include <filesystem>
#include <string>

namespace ImGui
{
namespace fs = std::filesystem;

// string literals
static const std::string kDriveLetters    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const std::string kDriveTerminator = ":\\";
static const std::string kCancel          = "Cancel";
static const std::string kSelect          = "Select";
static const std::string kRoot            = "Root";
static const std::string kUp              = "Up";

// Numeric literals
static const int kHPadding = 10;
static const int kVPadding = 5;

struct FileBrowserContext
{
    enum class Result
    {
        ENone,
        ECancel,
        ESelect
    };

    struct Entry {
        bool isFolder;
        fs::path path;
        bool isSelected;
    };

    bool                             isOpen{ false };
    fs::path                         currentPath;
    std::vector<Entry>               currentEntries;
    fs::path                         selectedPath;
    int                              currentSelectionIndex{ -1 };
    ImGuiFileBrowserFlags            flags{ ImGuiFileBrowserFlags_None };

    void init()
    {
        currentSelectionIndex = -1;
        currentEntries.clear();
        currentPath.clear();
        // get available drives
        for (char c : kDriveLetters)
        {
            fs::directory_entry drive(std::string(1, c) + kDriveTerminator);
            std::error_code     errorCode;
            if (drive.exists(errorCode))
            {
                Entry entry{true, std::string(1, c)  + kDriveTerminator, false};
                currentEntries.emplace_back(entry);
            }
        }
    }

    void openNextPath()
    {
        currentSelectionIndex = -1;
        currentEntries.clear();
        fs::directory_entry de(currentPath);
        std::error_code ec;
        if (!de.exists(ec))
        {
            init();
            return;
        }
        for (const auto& e : fs::directory_iterator(currentPath))
        {
            std::error_code ec;
            bool isDirectory = e.is_directory(ec);
            if (flags & ImGuiFileBrowserFlags_SelectDirectory && !isDirectory)
            {
                continue;
            }
            Entry entry{ isDirectory, e.path(), false };
            currentEntries.emplace_back(entry);
        }
    }

    Result handleUI()
    {
        // ================= toolbar ====================
        // handle go to root
        if (Button(kRoot.c_str()))
        {
            init();
        }
        SameLine();
        // handle go up one folder
        if (Button(kUp.c_str()))
        {
            if (currentPath.has_parent_path() && currentPath.has_filename())
            {
                currentPath = currentPath.parent_path();
                openNextPath();
            }
            else
            {
                init();
            }
        }
        if (!currentPath.empty())
        {
            SameLine();
            Bullet();
            Text("%s", currentPath.string().c_str());
        }
        Separator();

        // ========= filelist =================

        Result result = Result::ENone;
        bool shouldOpenNext = false;

        float lisBoxHeight = GetWindowHeight() - 2 * GetCursorPosY();
        if (ListBoxHeader("##", ImVec2(GetWindowWidth(), lisBoxHeight)))
        {
            int count = 0;
            for (const auto& entry : currentEntries)
            {
                std::string displayName = entry.isFolder ? "[D] " : "[F] ";
                if (entry.path.has_filename())
                {
                    displayName += entry.path.filename().string();
                }
                else
                {
                    displayName += entry.path.root_name().string();
                }
                if (Selectable(displayName.c_str(), currentSelectionIndex == count ? true : false, ImGuiSelectableFlags_AllowDoubleClick))
                {
                    currentSelectionIndex = count;
                    if (IsMouseDoubleClicked(ImGuiPopupFlags_MouseButtonLeft))
                    {
                        if (currentEntries[currentSelectionIndex].isFolder)
                        {
                            currentPath    = currentEntries[currentSelectionIndex].path;
                            shouldOpenNext = true;
                        }
                        else
                        {
                            selectedPath = currentEntries[currentSelectionIndex].path;
                            result = Result::ESelect;
                        }
                    }
                }
                count++;
            }
            ListBoxFooter();
        }
        Separator();
        if (shouldOpenNext)
        {
            openNextPath();
            return result;
        }

        // ========= buttons ===========

        NewLine();
        ImVec2 cancelSize = CalcTextSize(kCancel.c_str());
        ImVec2 selectSize = CalcTextSize(kSelect.c_str());
        float buttonWidth = std::max(cancelSize.x, selectSize.x);
        float itemWidth = buttonWidth + 2 * kHPadding;
        float itemHeight = std::max(cancelSize.y, selectSize.y) + 2 * kVPadding;
        SameLine(GetWindowWidth() - 2 * (itemWidth + kHPadding));
        if (Button(kSelect.c_str(), ImVec2(itemWidth, itemHeight)))
        {
            if (currentSelectionIndex > -1)
            {
                selectedPath = currentEntries[currentSelectionIndex].path.string();
            }
            else
            {
                selectedPath = currentPath.string();
            }
            result = Result::ESelect;
        }
        SameLine(0.f, kHPadding);
        if (Button(kCancel.c_str(), ImVec2(itemWidth, itemHeight)))
        {
            result = Result::ECancel;
            selectedPath.clear();
        }
        return result;
    }
};

static FileBrowserContext sContext;

bool FileBrowser(const std::string& name, std::string& outPath, bool& open, ImGuiFileBrowserFlags flags)
{
    if (!open)
    {
        return false;
    }

    if (!sContext.isOpen)
    {
        OpenPopup(name.c_str());
        sContext.isOpen = true;
        sContext.flags = flags;
        sContext.init();
    }

    bool result = false;

    if (BeginPopupModal(name.c_str(), &open, ImGuiWindowFlags_NoCollapse))
    {
        FileBrowserContext::Result selectionResult = sContext.handleUI();
        switch (selectionResult)
        {
        case FileBrowserContext::Result::ESelect:
            outPath = sContext.selectedPath.string();
            result = true;
            [[fallthrough]];
        case FileBrowserContext::Result::ECancel:
            open = false;
            break;
        default:
            break;
        }
        EndPopup();
    }

    int escIdx = GetIO().KeyMap[ImGuiKey_Escape];
    if (open && escIdx >= 0 && IsKeyPressed(escIdx))
    {
        open = false;
    }

    if (!open)
    {
        CloseCurrentPopup();
        sContext.isOpen = false;
    }

    return result;
}

}  // ImGui namespace
