#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace ImGui
{

    void SpinnerCircle(const char* label, const float indicator_radius, const ImVec4& main_color,
                                   const ImVec4& backdrop_color, const int circle_count, const float speed)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
    {
        return;
    }

    ImGuiContext& g  = *GImGui;
    const ImGuiID id = window->GetID(label);

    const ImVec2 pos           = window->DC.CursorPos;
    const float  circle_radius = indicator_radius / 10.0f;
    const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f, pos.y + indicator_radius * 2.0f));
    ItemSize(bb, ImGui::GetStyle().FramePadding.y);
    if (!ItemAdd(bb, id))
    {
        return;
    }
    const float t             = g.Time;
    const auto  degree_offset = 2.0f * IM_PI / circle_count;
    for (int i = 0; i < circle_count; ++i)
    {
        const auto x      = indicator_radius * std::sin(degree_offset * i);
        const auto y      = indicator_radius * std::cos(degree_offset * i);
        const auto growth = std::max(0.0f, std::sin(t * speed - i * degree_offset));
        ImVec4     color;
        color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
        color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
        color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
        color.w = 1.0f;
        window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x, pos.y + indicator_radius - y),
                                          circle_radius + growth * circle_radius,
                                          GetColorU32(color));
    }
}

bool Spinner(const char* label, float radius, float thickness, const ImU32& color)
{
    ImGuiContext& g      = *GetCurrentContext();
    ImGuiWindow*  window = g.CurrentWindow;
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID     id    = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size((radius)*2, (radius + style.FramePadding.y) * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ItemSize(bb, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    // Render
    window->DrawList->PathClear();

    int num_segments = 30;
    int start        = (int)abs(ImSin((float)g.Time * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

    for (int i = 0; i < num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(
            ImVec2(centre.x + ImCos(a + (float)g.Time * 8) * radius, centre.y + ImSin(a + (float)g.Time * 8) * radius));
    }

    window->DrawList->PathStroke(color, false, thickness);
    return true;
}

bool Spinner(const char* label)
{
    const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
    return Spinner(label, GetCurrentContext()->Font->FontSize * 0.5f, 2.0f, col);
}

} // namespace ImGui
