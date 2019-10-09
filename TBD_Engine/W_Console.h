#ifndef _W_CONSOLE_H_
#define _W_CONSOLE_H_

#include "Module.h"
#include "Application.h"

class W_Console : public Module
{
public:
	W_Console(Application* app, bool start_enabled = true);
	~W_Console();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};

#endif // !_W_CONSOLE_H_


