#include "TextureLoader.h"
#include "Application.h"

#include "glew/include/GL/glew.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

#include "mmgr/mmgr.h"

#define CHECKERS_HEIGHT  16
#define CHECKERS_WIDTH 16

TextureLoader::TextureLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

TextureLoader::~TextureLoader() 
{}

bool TextureLoader::Init()
{
	bool ret = true;

	// Checking current version
	if(ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		App->LogInConsole("DevIL version is different...exiting!\n");
		ret = false;
	}
	else
	{
		App->LogInConsole("Initializing DevIL");
	
		ilInit();
		iluInit();
		ilutInit();

		// Initialize DevIL's OpenGL access
		ilutRenderer(ILUT_OPENGL);
	}

	return ret;
}

bool TextureLoader::Start()
{
	id_checkersTexture = CreateCheckersTexture(CHECKERS_WIDTH, CHECKERS_HEIGHT);

	return true;
}

update_status TextureLoader::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool TextureLoader::CleanUp()
{

	return true;
}

uint TextureLoader::CreateCheckersTexture(uint width, uint height) const
{

	// creating procedurally a checkered texture
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	GLuint ImageName = 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);


	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);


	return ImageName;
}