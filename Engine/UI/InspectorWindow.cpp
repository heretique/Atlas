#include "InspectorWindow.h"
#include "Core/Engine.h"
#include "Managers/ECSManager.h"
#include "Components/Common.h"
#include "Hq/Math/MathTypes.h"

namespace atlas
{
namespace ui = ImGui;

void InspectorWindow::onGUI(float deltaTime)
{
    auto& registry = Engine::ecs().registry();
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
                Component* comp = registry.getByType(entity, typeInfo);
                if (comp)
                {
                    rttr::type compType     = rttr::type::get(comp);
                    rttr::type compTrueType = rttr::type::get_derived_type((void*)comp, compType);
                    if (ui::CollapsingHeader(compTrueType.get_name().data()))
                    {
                        auto properties = compTrueType.get_properties();
                        for (auto prop : properties)
                        {
                            ui::Text(prop.get_name().data());
                            rttr::type propType = prop.get_type();
                            ui::SameLine();
                            ui::Text("Type: %s", propType.get_name().data());
                            rttr::variant propVal = prop.get_value(comp);
                            if (propType == rttr::type::get<hq::math::Mat4x4>())
                            {
                                hq::math::Mat4x4 mat = propVal.get_value<hq::math::Mat4x4>();
                                ui::Indent();
                                //    ImGui::PushItemWidth(80);
                                ui::InputFloat4("##hidelabel", mat.data);
                                ui::InputFloat4("##hidelabel", mat.data + 4);
                                ui::InputFloat4("##hidelabel", mat.data + 8);
                                ui::InputFloat4("##hidelabel", mat.data + 12);
                                //    ImGui::PopItemWidth();
                                ui::Unindent();
                            }
                        }
                    }
                }
            });
        }
    }
}

}
