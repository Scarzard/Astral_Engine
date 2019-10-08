#ifndef _FRAMEBUFFFEROBJECT_H_
#define _FRAMEBUFFFEROBJECT_H_
#include "Application.h"
#include "Module.h"

class FrameBuffferObject : public Module
{
public:
	FrameBuffferObject(Application* app, bool start_enabled = true);
	~FrameBuffferObject();
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:
	uint fbo;
	uint texture;

};

#endif // !_FRAMEBUFFFEROBJECT_H_

