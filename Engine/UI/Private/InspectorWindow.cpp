#include "UI/InspectorWindow.h"
#include "Ecs/ECSManager.h"
#include "Core/Component.h"
#include "Hq/Math/MathTypes.h"
#include "Hq/Math/Mat4x4.h"

namespace atlas
{
namespace ui = ImGui;

void InspectorWindow::onGUI(float /*deltaTime*/)
{
    auto& registry = ECSManager::instance().registry();
    auto  view     = registry.view<Selected>();
    if (view.empty())
    {
        ui::Text("Inspector empty");
    }
    else
    {
        for (auto entity : view)
        {
            registry.visit(entity, [&](const auto typeInfo) {
                Component*     comp = registry.getByType(entity, typeInfo);
                rttr::instance inst(comp);
                if (comp && inst)
                {
                    rttr::type compTrueType = inst.get_derived_type();
                    if (ui::CollapsingHeader(compTrueType.get_name().data()))
                    {
                        auto properties = compTrueType.get_properties();
                        for (auto prop : properties)
                        {
                            // TODO: use custom transient allocators with std::string for generating the
                            // widget ids
                            std::string propName(prop.get_name().data());
                            ui::Text(propName.c_str());
                            rttr::type    propType = prop.get_type();
                            rttr::variant propVal  = prop.get_value(comp);
                            if (propType == rttr::type::get<hq::math::Mat4x4>())
                            {
                                hq::math::Mat4x4 mat = propVal.get_value<hq::math::Mat4x4>();
                                ui::Indent();
                                //    ImGui::PushItemWidth(80);
                                ui::InputFloat4(("##" + propName + "row1").c_str(), mat.data);
                                ui::InputFloat4(("##" + propName + "row2").c_str(), mat.data + 4);
                                ui::InputFloat4(("##" + propName + "row3").c_str(), mat.data + 8);
                                ui::InputFloat4(("##" + propName + "row4").c_str(), mat.data + 12);
                                //    ImGui::PopItemWidth();
                                ui::Unindent();
                                if (mat != propVal.get_value<hq::math::Mat4x4>())
                                {
                                    prop.set_value(inst, mat);
                                }
                            }
                        }
                    }
                }
            });
        }
    }
}

template <>
void InspectorWindow::inspect<>(Component& component)
{
}

}
