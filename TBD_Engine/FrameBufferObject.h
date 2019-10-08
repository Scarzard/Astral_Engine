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

private:
	unsigned int fbo;
	unsigned int texture;

};

#endif // !_FRAMEBUFFEROBJECT_H_

