#pragma once
#include <ImGUI/imgui.h>

namespace icl {
	static void setImGUIStuff() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0.5);
	}
}
