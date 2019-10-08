#include "Application.h"
#include "FrameBufferObject.h"
#include "glew/include/GL/glew.h"

FrameBuffferObject::FrameBuffferObject(Application * app, bool start_enabled) : Module(app, start_enabled)
{

}

FrameBuffferObject::~FrameBuffferObject()
{
}

bool FrameBuffferObject::Init()
{
	bool ret = false;

	//Generate the FBO and bind it, continue if FBO is complete
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		ret = true;
	}
	else 
		return false;

	return ret;
}

bool FrameBuffferObject::Start()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

	return true;
}

update_status FrameBuffferObject::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status FrameBuffferObject::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool FrameBuffferObject::CleanUp()
{
	glDeleteFramebuffers(1, &fbo);

	return true;
}
