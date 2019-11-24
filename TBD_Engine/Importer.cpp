#include "Application.h"
#include "Importer.h"

#include "Component_Mesh.h"
#include "Component_Transform.h"
#include "ModuleFileSystem.h"

#include "mmgr/mmgr.h"

Importer::Importer() 
{}

Importer::~Importer()
{}


bool Importer::Export(const char * name, std::string & output_file, ComponentTransform* transf) // Create .transf own file format
{
	bool ret = false;
	//position, scale, rotation + local_matrix, global_matrix
	uint size = sizeof(float3) * 3 + sizeof(float) * 32; 

	char* data = new char[size];
	char* cursor = data;

	// Preparing data to be stored ---------------------------------------------------
	float3 p_r_s[3] = { transf->position, transf->rotation_euler, transf->scale }; 

	float l_matrix[16], g_matrix[16];
	int counter = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			l_matrix[counter] = transf->local_matrix.At(i, j);
			g_matrix[counter] = transf->global_matrix.At(i, j);
			counter++;
		}
	//----------------------------------------------------------------------------------

	uint bytes = sizeof(float3)*3;// Store position, rotation and scale
	memcpy(cursor, p_r_s, bytes);

	cursor += bytes;
	bytes = sizeof(float)*16; // Store Local Transformation
	memcpy(cursor, l_matrix, bytes);

	cursor += bytes;
	bytes = sizeof(float) * 16; // Store Global Transformation
	memcpy(cursor, g_matrix, bytes);

	ret = App->file_system->SaveUnique(output_file, data, size, LIBRARY_TRANSF_FOLDER, name, "transf");

	if (!ret)
		App->LogInConsole("Failed exporting %s.transf into Library/Transforms  floder", name);

	return ret;
}

bool Importer::Load(const char * full_path, ComponentTransform * transf)
{

	char* buffer = nullptr;
	App->file_system->Load(full_path, &buffer); //put data file in buffer
	char* cursor = buffer;

	// position / rotation / scale
	float3 p_r_s[3];
	uint bytes = sizeof(p_r_s);
	memcpy(p_r_s, cursor, bytes);

	transf->position = p_r_s[0]; //position
	transf->rotation_euler = p_r_s[1]; //rotation
	transf->scale = p_r_s[2]; //scale


	// ---------------------- Local_matrix ------------------------
	float transform_matrix[16];
	cursor += bytes;
	bytes = sizeof(float)*16;
	memcpy(transform_matrix, cursor, bytes);

	int counter = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			transf->local_matrix[i][j] = transform_matrix[counter]; 
			counter++;
		}


	//--------------------------Global_matrix-----------------------
	cursor += bytes;
	bytes = sizeof(float)*16;
	memcpy(transform_matrix, cursor, bytes);

	counter = 0;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			transf->global_matrix[i][j] = transform_matrix[counter];
			counter++;
		}

	return true;
}
