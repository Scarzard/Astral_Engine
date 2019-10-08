#include "Application.h"
#include "FrameBufferObject.h"
#include "glew/include/GL/glew.h"

FrameBufferObject::FrameBufferObject()
{

}

FrameBufferObject::~FrameBufferObject()
{
}

bool FrameBufferObject::Start(ImVec2 size)
{
	bool ret = false;
	//Generate the FBO and bind it, continue if FBO is complete
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	//	App->LogInConsole("Frame buffer object creation is successful");
	//else
	//	App->LogInConsole("Frame buffer object creation wasn't successful");

	//Generate the texture used to render our scene
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	//Generate RenderBufferObject
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	App->LogInConsole("Frame buffer object creation is successful");
	//else
	//	App->LogInConsole("Frame buffer object creation wasn't successful");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

update_status FrameBufferObject::PreUpdate()
{
	// first pass
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	return UPDATE_CONTINUE;
}

update_status FrameBufferObject::PostUpdate()
{
	// second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

bool FrameBufferObject::CleanUp()
{
	glDeleteFramebuffers(1, &fbo);

	return true;
}
