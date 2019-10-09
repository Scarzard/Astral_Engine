#ifndef _W_HIERARCHY_H_
#define _W_HIERARCHY_H_

#include "Module.h"
#include "Application.h"

class W_Hierarchy : public Module
{
public:
	W_Hierarchy(Application* app, bool start_enabled = true);
	~W_Hierarchy();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};

#endif // !_W_HIERARCHY_H_