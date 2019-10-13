#ifndef _W_CONSOLE_H_
#define _W_CONSOLE_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"

class W_Console : public Window
{
public:
	W_Console();
	~W_Console();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};

#endif // !_W_CONSOLE_H_


