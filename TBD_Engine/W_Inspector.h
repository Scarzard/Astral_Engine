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
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	GameObject* selected_GO = nullptr;
};

#endif // !_W_INSPECTOR_H_

