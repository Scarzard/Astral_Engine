#ifndef _W_GAME_H_
#define _W_GAME_H_
#include "Module.h"
#include "Application.h"
#include "FrameBufferObject.h"
#include "Window.h"

class W_Game : public Window
{
public:
	W_Game();
	~W_Game();

	bool Start();
	bool Draw();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	ImVec2 current_size, new_size; 
	bool in_editor = true;
private:
	FrameBufferObject* fbo = nullptr;
};

#endif // !_W_GAME_H_

