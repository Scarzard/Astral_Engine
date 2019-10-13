#ifndef __Window_H__
#define __Window_H__

#include "Globals.h"

class Window 
{
public:

	Window();
	virtual ~Window();

	virtual bool Start();
	virtual bool Draw();
	virtual update_status PreUpdate(float dt);
	virtual update_status Update(float dt);
	virtual update_status PostUpdate(float dt);
	virtual bool CleanUp();
};

#endif