#ifndef _W_RESOURCES_H_
#define _W_RESOURCES_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"

class W_Resources : public Window
{
public:
	W_Resources();
	~W_Resources();

	bool Start();
	bool Draw();

};

#endif // !_W_RESOURCES_H_