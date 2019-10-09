#ifndef _SCENEGAME_H_
#include "Module.h"
#include "Application.h"
#include "FrameBufferObject.h"

class SceneGame : public Module
{
public:
	SceneGame(Application* app, bool start_enabled = true);
	~SceneGame();

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

#endif // !_SCENEGAME_H_

