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

	void DeleteBuffers();

public:
	unsigned int fbo;
	unsigned int rbo;
	unsigned int texture;
	ImVec2 size;

};

#endif // !_FRAMEBUFFEROBJECT_H_

