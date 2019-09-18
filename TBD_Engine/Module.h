#ifndef _MODULE_H_
#define _MODULE_H_

class Application;
struct PhysBody3D;

using namespace std;
#include <iostream>
#include <utility>
#include <list> 
#include <iterator>
#include <string>
#include <vector>
#include <queue>

class Module
{
public:

	bool enabled = false;
	Application* App;

public:

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{ 

	}
};

#endif // !_MODULE_H_


