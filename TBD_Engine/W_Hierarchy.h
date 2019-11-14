#ifndef _W_HIERARCHY_H_
#define _W_HIERARCHY_H_

#include "Module.h"
#include "Application.h"
#include "Window.h"

class W_Hierarchy : public Window
{
public:
	W_Hierarchy();
	~W_Hierarchy();

	bool Start();
	bool Draw();
	void DrawRecursively(GameObject* GO);

};

#endif // !_W_HIERARCHY_H_