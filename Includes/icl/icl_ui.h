#pragma once
#include <ImGUI/imgui.h>

namespace icl {
	static void setImGUIStuff() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0.75);

		//-------------------------------------------------------

		/*ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.Fonts->AddFontFromFileTTF("C:\\WIndows\\Fonts\\segoeui.ttf", 15.0f);*/

	}
}
