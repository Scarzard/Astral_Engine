#include "GeometryLoader.h"
#include "Application.h"


#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

GeometryLoader::GeometryLoader(Application* app,  bool start_enabled) : Module(app, start_enabled)
{
}

GeometryLoader::~GeometryLoader()
{}



bool GeometryLoader::Init()
{
	// Stream log messages to Debug window 
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool GeometryLoader::Start()
{
	return true;
}

update_status GeometryLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool GeometryLoader::CleanUp()
{

	// detach log stream 
	aiDetachAllLogStreams();

	return true;
}