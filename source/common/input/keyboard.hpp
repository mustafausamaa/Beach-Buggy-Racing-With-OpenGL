#pragma once

#include <GLFW/glfw3.h>
#include <cstring>

namespace our
{

    template <typename T>
    static void OptionMapCombo(const char *label, T &selected, const std::map<T, const char *> &options)
    {
        if (ImGui::BeginCombo(label, options.at(selected)))
        {
            for (auto const &[key, value] : options)
            {
                bool is_selected = selected == key;
                if (ImGui::Selectable(value, is_selected))
                    selected = key;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    template <typename It>
    static void IteratorCombo(const char *label, std::string &selected, It begin, It end)
    {
        if (ImGui::BeginCombo(label, selected.c_str()))
        {
            for (It it = begin; it != end; ++it)
            {
                auto &key = *it;
                bool is_selected = selected == key;
                if (ImGui::Selectable(key.c_str(), is_selected))
                    selected = key;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    template <typename It>
    static void PairIteratorCombo(const char *label, std::string &selected, It begin, It end)
    {
        if (ImGui::BeginCombo(label, selected.c_str()))
        {
            for (It it = begin; it != end; ++it)
            {
                auto &[key, value] = *it;
                bool is_selected = selected == key;
                if (ImGui::Selectable(key.c_str(), is_selected))
                    selected = key;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    static bool ColorEdit4U8(const char *label, uint8_t *color, ImGuiColorEditFlags flags = 0)
    {
        float color_f32[4] = {color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f};
        bool result = ImGui::ColorEdit4(label, color_f32, flags);
        color[0] = 255 * color_f32[0];
        color[1] = 255 * color_f32[1];
        color[2] = 255 * color_f32[2];
        color[3] = 255 * color_f32[3];
        return result;
    }

    template <typename It>
    static void ReorderableList(It begin, It end,
                                std::function<void(size_t, typename std::iterator_traits<It>::value_type &)> item_gui,
                                std::function<void(size_t)> item_add,
                                std::function<void(size_t)> item_delete,
                                size_t starting_index = 0)
    {
        size_t index = starting_index;
        size_t distance = std::distance(begin, end);
        size_t last = starting_index + distance - 1;
        std::optional<size_t> item_to_delete, item_to_add;
        std::optional<std::tuple<It, It>> items_to_swap;
        float item_spacing = ImGui::GetStyle().ItemSpacing.x, window_width = ImGui::GetWindowWidth();
        float full_button_width = window_width - 2 * item_spacing;
        float third_button_width = (window_width - 4 * item_spacing) / 3;
        for (It it = begin, prev; it != end; prev = it, it++, ++index)
        {
            ImGui::PushID(index);

            ImGui::PushStyleColor(ImGuiCol_Button, {0.25, 0.65, 0.15, 1});
            if (ImGui::Button("+", ImVec2(full_button_width, 0)))
                item_to_add = index;
            ImGui::PopStyleColor();
            ImGui::Separator();

            item_gui(index, *it);

            ImGui::PushStyleColor(ImGuiCol_Button, {0.65, 0.15, 0.25, 1});
            if (ImGui::Button("DELETE", ImVec2(third_button_width, 0)))
                item_to_delete = index;
            ImGui::PopStyleColor();
            ImGui::SameLine();
            bool up_enabled = index > starting_index;
            if (!up_enabled)
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            if (ImGui::Button("UP", ImVec2(third_button_width, 0)) && up_enabled)
                items_to_swap = {prev, it};
            if (!up_enabled)
                ImGui::PopStyleVar();
            ImGui::SameLine();
            bool down_enabled = index < last;
            if (!down_enabled)
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            if (ImGui::Button("DOWN", ImVec2(third_button_width, 0)) && down_enabled)
                items_to_swap = {it, it + 1};
            if (!down_enabled)
                ImGui::PopStyleVar();
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::PushStyleColor(ImGuiCol_Button, {0.25, 0.65, 0.15, 1});
        if (ImGui::Button("+", ImVec2(full_button_width, 0)))
            item_to_add = index;
        ImGui::PopStyleColor();

        if (item_to_add.has_value())
            item_add(item_to_add.value());
        else if (item_to_delete.has_value())
            item_delete(item_to_delete.value());
        else if (items_to_swap.has_value())
        {
            auto [it1, it2] = items_to_swap.value();
            auto temp = *it1;
            *it1 = *it2;
            *it2 = temp;
        }
    }

    // A convenience class to read keyboard input
    class Keyboard
    {
    private:
        bool enabled; // Is this class enabled (allowed to read user input)
        bool currentKeyStates[GLFW_KEY_LAST + 1];
        bool previousKeyStates[GLFW_KEY_LAST + 1];

    public:
        // Enable this object and capture current keyboard state from window
        void enable(GLFWwindow *window)
        {
            enabled = true;
            for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++)
            {
                currentKeyStates[key] = previousKeyStates[key] = glfwGetKey(window, key);
            }
        }

        // Disable this object and clear the state
        void disable()
        {
            for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++)
            {
                currentKeyStates[key] = previousKeyStates[key] = false;
            }
        }

        // update the keyboard state (moves current frame state to become the previous frame state)
        void update()
        {
            if (!enabled)
                return;
            std::memcpy(previousKeyStates, currentKeyStates, sizeof(previousKeyStates));
        }

        // Event functions called from GLFW callbacks in "application.cpp"
        void keyEvent(int key, int, int action, int)
        {
            if (!enabled)
                return;
            if (action == GLFW_PRESS)
            {
                currentKeyStates[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                currentKeyStates[key] = false;
            }
        }

        // Is the key currently pressed
        [[nodiscard]] bool isPressed(int key) const { return currentKeyStates[key]; }
        // Was the key unpressed in the previous frame but became pressed in the current frame
        [[nodiscard]] bool justPressed(int key) const { return currentKeyStates[key] && !previousKeyStates[key]; }
        // Was the key pressed in the previous frame but became unpressed in the current frame
        [[nodiscard]] bool justReleased(int key) const { return !currentKeyStates[key] && previousKeyStates[key]; }

        [[nodiscard]] bool isEnabled() const { return enabled; }
        void setEnabled(bool enabled, GLFWwindow *window)
        {
            if (this->enabled != enabled)
            {
                if (enabled)
                    enable(window);
                else
                    disable();
            }
        }
    };

}
