#ifndef _FRAMEBUFFEROBJECT_H_
#define _FRAMEBUFFEROBJECT_H_

class FrameBufferObject
{
public:
	FrameBufferObject();
	~FrameBufferObject();
	bool Start(ImVec2 size);
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

public:
	unsigned int fbo;
	unsigned int texture;
	ImVec2 size;

};

#endif // !_FRAMEBUFFEROBJECT_H_

