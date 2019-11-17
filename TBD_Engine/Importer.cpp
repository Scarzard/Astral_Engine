#include "Application.h"
#include "Importer.h"

#include "Component_Mesh.h"

#include "mmgr/mmgr.h"

Importer::Importer() 
{}

Importer::~Importer()
{}


bool Importer::Export(const char * name, std::string & output_file, ComponentMesh* mesh) // Create .mesh own file format
{
	bool ret = false;

	// amount of indices / vertices / normals / texture_coords / AABB
	uint ranges[4] = { mesh->num_index, mesh->num_vertex,  mesh->num_normals, mesh->num_tex_coords};

	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_index + sizeof(float) * mesh->num_vertex * 3 + sizeof(float) * mesh->num_normals * 3 * 2 + sizeof(float) * mesh->num_tex_coords * 2;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices
	bytes = sizeof(uint) * mesh->num_index;
	memcpy(cursor, mesh->index, bytes);

	cursor += bytes; // Store Vertices
	bytes = sizeof(float) * mesh->num_vertex * 3;
	memcpy(cursor, mesh->vertex, bytes);

	cursor += bytes; // Store Normals Starting Point
	bytes = sizeof(float) * mesh->num_normals * 3;
	memcpy(cursor, mesh->face_center, bytes);

	cursor += bytes; // Store Normals Vector
	bytes = sizeof(float) * mesh->num_normals * 3;
	memcpy(cursor, mesh->face_normal, bytes);
	
	cursor += bytes; // Store Texture Coordinates
	bytes = sizeof(float) * mesh->num_tex_coords * 2;
	memcpy(cursor, mesh->tex_coords, bytes);


	ret = App->file_system->SaveUnique(output_file, data, size, LIBRARY_MESH_FOLDER, name, "mesh");

	if (!ret)
		App->LogInConsole("Failed exporting %s.mesh into Library/mesh floder", name);

	return ret;
}

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

bool Importer::Load(const char * full_path, ComponentMesh * mesh)
{
	bool ret = false;

	char* buffer = nullptr;
	App->file_system->Load(full_path, &buffer); //put data file in buffer

	char* cursor = buffer;

	// amount of indices / vertices / normals / texture_coords
	uint ranges[5];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	mesh->num_index = ranges[0];
	mesh->num_vertex = ranges[1];
	mesh->num_normals = ranges[2];
	mesh->num_tex_coords = ranges[3];

	cursor += bytes; // Load indices
	bytes = sizeof(uint) * mesh->num_index;
	mesh->index = new uint[mesh->num_index];
	memcpy(mesh->index, cursor, bytes);

	cursor += bytes; // Load vertex
	bytes = sizeof(float) * mesh->num_vertex * 3;
	mesh->vertex = new float3[mesh->num_vertex];
	memcpy(mesh->vertex, cursor, bytes);

	cursor += bytes; // Load Normals Starting Point
	bytes = sizeof(float) * mesh->num_normals * 3;
	mesh->face_center = new float3[mesh->num_normals];
	memcpy(mesh->face_center, cursor, bytes);

	cursor += bytes; // Load Normals Vector
	bytes = sizeof(float) * mesh->num_normals * 3;
	mesh->face_normal = new float3[mesh->num_normals];
	memcpy(mesh->face_normal, cursor, bytes);

	cursor += bytes; // Load Texture Coordinates
	bytes = sizeof(float) * mesh->num_tex_coords * 2;
	mesh->tex_coords = new float[mesh->num_tex_coords * 2];
	memcpy(mesh->tex_coords, cursor, bytes);

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
