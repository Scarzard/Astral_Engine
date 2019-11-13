#ifndef _W_INSPECTOR_H_
#define _W_INSPECTOR_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"

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

private:
	/*float3 pos = float3::zero;
	Quat rot = Quat::identity;
	float3 sc = float3::zero;*/
};

#endif // !_W_INSPECTOR_H_

