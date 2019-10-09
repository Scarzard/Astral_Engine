#ifndef _W_GAME_H_
#define _W_GAME_H_
#include "Module.h"
#include "Application.h"
#include "FrameBufferObject.h"

class W_Game : public Module
{
public:
	W_Game(Application* app, bool start_enabled = true);
	~W_Game();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool Cleanup();

public:
	ImVec2 current_size, new_size;
private:
	FrameBufferObject* fbo = nullptr;
};

#endif // !_W_GAME_H_

