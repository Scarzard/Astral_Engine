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

#define CHECKERS_HEIGHT  128
#define CHECKERS_WIDTH 128

TextureLoader::TextureLoader(bool start_enabled) : Module(start_enabled)
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
	CheckersTexture = CreateCheckersTexture();
	DefaultTexture = CreateDefaultTexture();

	return true;
}

update_status TextureLoader::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool TextureLoader::CleanUp()
{

	glDeleteTextures(1, (GLuint*)&CheckersTexture.id);
	glDeleteTextures(1, (GLuint*)&DefaultTexture.id);

	return true;
}

Texture TextureLoader::CreateCheckersTexture() const
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

	Texture tex;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	tex.height = CHECKERS_HEIGHT;
	tex.width = CHECKERS_WIDTH;
	tex.path = "No path";


	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);


	return tex;
}

uint TextureLoader::CreateTexture(const void* img, uint width, uint height, int internalFormat, uint format) const
{
	uint id_texture = 0;

	//Generate the texture ID
	glGenTextures(1, (GLuint*)&id_texture);
	//Bind the texture 
	glBindTexture(GL_TEXTURE_2D, id_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Enabling anisotropic filtering
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		float max_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	App->LogInConsole("Loaded Texture(id:%i) with Width: %i and Height: %i ", id_texture, width, height);

	return id_texture;
}

Texture TextureLoader::LoadTextureFromPath(const char* path) const
{
	Texture tex;
	uint id_img = 0;

	if (path != nullptr)
	{
		// Generate image 
		ilGenImages(1, (ILuint*)&id_img);	
		// Bind image
		ilBindImage(id_img);			

		if (ilLoadImage(path))
		{
			
			ILinfo ImgInfo;
			iluGetImageInfo(&ImgInfo);

			//FLip image in case it is flipped 
			if (ImgInfo.Origin == IL_ORIGIN_UPPER_LEFT)
				iluFlipImage();

			if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				//Create TExture
				tex.id = CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_FORMAT));
				tex.height = ilGetInteger(IL_IMAGE_HEIGHT);
				tex.width = ilGetInteger(IL_IMAGE_WIDTH);
				tex.path = path;

				CreateFileDDS(path);
			}
			else
				App->LogInConsole("Failed converting image: %s", iluErrorString(ilGetError()));
		}
		else
		{
			App->LogInConsole("Failed loading image: %s", iluErrorString(ilGetError()));
		}
		
	}
	else
	{
		App->LogInConsole("Could not load image from path! Path %s was nullptr", path);
	}


	return tex;
}

Texture TextureLoader::CreateDefaultTexture() const
{
	Texture tex;
	tex.id = 0;
	tex.width = 0;
	tex.height = 0;
	tex.path = "none";

	return tex;
}

bool TextureLoader::CreateFileDDS(const char * path) const
{
	bool ret = false;

	std::string output_file, name;

	name = App->GetNameFromPath(path);

	ILuint   size;
	ILubyte *data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0)
	{
		data = new ILubyte[size]; // allocate data buffer   
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function        
			ret = App->file_system->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, name.data(), "dds");
		RELEASE_ARRAY(data);
	}


	return ret;
}
