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
};

#endif // !_W_CONSOLE_H_


