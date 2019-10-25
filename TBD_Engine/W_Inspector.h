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
};

#endif // !_W_INSPECTOR_H_

