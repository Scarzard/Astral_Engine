#ifndef _W_INSPECTOR_H_
#define _W_INSPECTOR_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"
#include "ImGui/imgui.h"

class GameObject;
class W_Inspector : public Window
{
public:
	W_Inspector();
	~W_Inspector();

	bool Start();
	bool Draw();

public:
	GameObject* selected_GO = nullptr;
	ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_None;

	bool active_bool = true;
};

#endif // !_W_INSPECTOR_H_

