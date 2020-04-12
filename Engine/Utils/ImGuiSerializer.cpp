#include "ImGuiSerializer.h"
#include <imgui/imgui.h>
#include <Hq/Math/Math.h>
#include <Hq/Math/Vec3.h>
#include <Hq/Math/Mat4x4.h>

namespace atlas
{
void ImGuiSerializer::operator()(std::string& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::LabelText("%s:", value.c_str());
}

void ImGuiSerializer::operator()(const std::string& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::LabelText("%s:", value.c_str());
}

void ImGuiSerializer::operator()(int& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputInt("", &value);
}

void ImGuiSerializer::operator()(const int& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    int val = value;
    ImGui::InputInt("", &val, ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(u8& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputScalar("", ImGuiDataType_U8, &value);
}

void ImGuiSerializer::operator()(const u8& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    u8 val = value;
    ImGui::InputScalar("", ImGuiDataType_U8, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(u32& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputScalar("", ImGuiDataType_U32, &value);
}

void ImGuiSerializer::operator()(const u32& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    u32 val = value;
    ImGui::InputScalar("", ImGuiDataType_U32, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(u64& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputScalar("", ImGuiDataType_U64, &value);
}

void ImGuiSerializer::operator()(const u64& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    u64 val = value;
    ImGui::InputScalar("", ImGuiDataType_U64, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(float& value, const std::string& s)
{
    ImGui::Text("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputFloat("##hidelabel", &value);
}

void ImGuiSerializer::operator()(const float& value, const std::string& s)
{
    float val = value;
    ImGui::InputFloat(s.c_str(), &val, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(double& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputDouble("", &value);
}

void ImGuiSerializer::operator()(const double& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    double val = value;
    ImGui::InputDouble("", &val, 0.0, 0.0, "%.6f", ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(entt::entity& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    ImGui::InputScalar("", ImGuiDataType_U32, &value);
}

void ImGuiSerializer::operator()(const entt::entity& value, const std::string& s)
{
    ImGui::LabelText("%s:", s.c_str());
    ImGui::SameLine();
    u32 val = static_cast<std::underlying_type_t<entt::entity> >(value);
    ImGui::InputScalar("", ImGuiDataType_U32, &val, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
}

void ImGuiSerializer::operator()(hq::math::Vec3& value, const std::string& s)
{
    ImGui::Text("%s", s.c_str());
    ImGui::Indent();
    ImGui::PushItemWidth(60);
    ImGui::Text("X");
    ImGui::SameLine();
    ImGui::InputFloat("##hidelabel", &value.x);
    ImGui::SameLine();
    ImGui::Text("Y");
    ImGui::SameLine();
    ImGui::InputFloat("##hidelabel", &value.y);
    ImGui::SameLine();
    ImGui::Text("Z");
    ImGui::SameLine();
    ImGui::InputFloat("##hidelabel", &value.z);
    ImGui::PopItemWidth();
    ImGui::Unindent();
}

void ImGuiSerializer::operator()(const hq::math::Vec3& value, const std::string& s)
{
    ImGui::Text("%s", s.c_str());
    ImGui::Indent();
    ImGui::PushItemWidth(60);
    ImGui::Text("X");
    ImGui::SameLine();
    float val = value.x;
    ImGui::InputFloat("##hidelabel", &val, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    ImGui::Text("Y");
    ImGui::SameLine();
    val = value.y;
    ImGui::InputFloat("##hidelabel", &val, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    ImGui::Text("Z");
    ImGui::SameLine();
    val = value.z;
    ImGui::InputFloat("##hidelabel", &val, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();
    ImGui::Unindent();
}

void ImGuiSerializer::operator()(hq::math::Mat4x4& value, const std::string& s)
{
    ImGui::Text("%s", s.c_str());
    ImGui::Indent();
//    ImGui::PushItemWidth(80);
    ImGui::InputFloat4("##hidelabel", value.data);
    ImGui::InputFloat4("##hidelabel", value.data + 4);
    ImGui::InputFloat4("##hidelabel", value.data + 8);
    ImGui::InputFloat4("##hidelabel", value.data + 12);
//    ImGui::PopItemWidth();
    ImGui::Unindent();
}

void ImGuiSerializer::operator()(const hq::math::Mat4x4& value, const std::string& s)
{
    ImGui::Text("%s", s.c_str());
    ImGui::Indent();
//    ImGui::PushItemWidth(80);
    float val[4];
    memcpy(val, value.data, sizeof (val));
    ImGui::InputFloat4("##hidelabel", val, "%.3f", ImGuiInputTextFlags_ReadOnly);
    memcpy(val, value.data + 4, sizeof (val));
    ImGui::InputFloat4("##hidelabel", val, "%.3f", ImGuiInputTextFlags_ReadOnly);
    memcpy(val, value.data + 8, sizeof (val));
    ImGui::InputFloat4("##hidelabel", val, "%.3f", ImGuiInputTextFlags_ReadOnly);
    memcpy(val, value.data + 12, sizeof (val));
    ImGui::InputFloat4("##hidelabel", val, "%.3f", ImGuiInputTextFlags_ReadOnly);
//    ImGui::PopItemWidth();
    ImGui::Unindent();
}

void ImGuiSerializer::operator()(std::string& value) { }

void ImGuiSerializer::operator()(const std::string& value) { }

void ImGuiSerializer::operator()(int& value) { }

void ImGuiSerializer::operator()(const int& value) { }

void ImGuiSerializer::operator()(u8& value) { }

void ImGuiSerializer::operator()(const u8& value) { }

void ImGuiSerializer::operator()(u32& value) { }

void ImGuiSerializer::operator()(const u32& value) { }

void ImGuiSerializer::operator()(u64& value) { }

void ImGuiSerializer::operator()(const u64& value) { }

void ImGuiSerializer::operator()(float& value) { }

void ImGuiSerializer::operator()(const float& value) { }

void ImGuiSerializer::operator()(double& value) { }

void ImGuiSerializer::operator()(const double& value) { }

void ImGuiSerializer::operator()(entt::entity& value) { }

void ImGuiSerializer::operator()(const entt::entity& value) { }

//////////////// ImGuiDeserializer

void ImGuiDeserializer::operator()(std::string& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsString())
    //        return;

    //    value = val.GetString();
}

void ImGuiDeserializer::operator()(int& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetInt();
}

void ImGuiDeserializer::operator()(u8& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetUint();
}

void ImGuiDeserializer::operator()(u32& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetUint();
}

void ImGuiDeserializer::operator()(u64& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetUint64();
}

void ImGuiDeserializer::operator()(float& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetFloat();
}

void ImGuiDeserializer::operator()(double& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetDouble();
}

void ImGuiDeserializer::operator()(entt::entity& value, const std::string& s)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());
    //    if (!_stack.back()->HasMember(s))
    //        return;

    //    const Value& val = (*_stack.back())[s];
    //    if (!val.IsNumber())
    //        return;

    //    value = static_cast<entt::entity>(val.GetUint());
}

void ImGuiDeserializer::operator()(std::string& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsString())
    //        return;

    //    value = val.GetString();
}

void ImGuiDeserializer::operator()(int& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetInt();
}

void ImGuiDeserializer::operator()(u8& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetUint();
}

void ImGuiDeserializer::operator()(u32& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetUint();
}

void ImGuiDeserializer::operator()(u64& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetUint64();
}

void ImGuiDeserializer::operator()(float& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetFloat();
}

void ImGuiDeserializer::operator()(double& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;

    //    value = val.GetDouble();
}

void ImGuiDeserializer::operator()(entt::entity& value)
{
    //    using namespace rapidjson;
    //    assert(!_stack.empty());

    //    const Value& val = *_stack.back();
    //    if (!val.IsNumber())
    //        return;
    //    value = static_cast<entt::entity>(val.GetUint());
}

}  // atlas namespace
