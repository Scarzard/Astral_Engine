#ifndef _W_INSPECTOR_H_
#define _W_INSPECTOR_H_

#include "Module.h"
#include "Application.h"

class W_Inspector : public Module
{
public:
	W_Inspector(Application* app, bool start_enabled = true);
	~W_Inspector();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
};

#endif // !_W_INSPECTOR_H_

