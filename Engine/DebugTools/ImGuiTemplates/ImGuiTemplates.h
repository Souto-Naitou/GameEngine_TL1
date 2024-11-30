#pragma once

#include <string>
#include <type_traits>

#include <functional>
#include <Vector2.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


namespace ImGuiTemplate
{
    void VariableTable(const std::string& _stringID, std::function<void()> _function);

    template <typename T>
    void VariableTableRow(const std::string& _varName, const T _var)
    {
#ifdef _DEBUG
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::Text(_varName.c_str());
        ImGui::TableNextColumn();

        if constexpr (std::is_same<T, uint32_t>::value)
        {
            ImGui::Text("%u", _var);
        }
        else if constexpr (std::is_integral<T>::value)
        {
            ImGui::Text("%d", _var);
        }
        else if constexpr (std::is_floating_point<T>::value)
        {
            ImGui::Text("%f", _var);
        }
        else if constexpr (std::is_same<T, std::string>::value)
        {
            ImGui::Text(_var.c_str());
        }
        else if constexpr (std::is_same<T, Vector2>::value)
        {
            ImGui::Text("{ %f, %f }", _var.x, _var.y);
        }
        else
        {
            ImGui::Text("ERROR TYPE!");
        }
#endif // _DEBUG
    }
}