#ifndef __Window_H__
#define __Window_H__

#include "Globals.h"

class Window 
{
public:

	Window(){}
	virtual ~Window(){}

	virtual bool Start()							{ return true; }
	virtual bool Draw()								{  return true; }
	virtual update_status PreUpdate(float dt)		{ return UPDATE_CONTINUE; }
	virtual update_status Update(float dt)			{ return UPDATE_CONTINUE; }
	virtual update_status PostUpdate(float dt)		{ return UPDATE_CONTINUE; }
	virtual bool CleanUp()							{ return true; }
};

#endif