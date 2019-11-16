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

	if (ret)
		App->LogInConsole("Exported %s.mesh into Library/mesh floder", name);
	else
		App->LogInConsole("Failed exporting %s.mesh into Library/mesh floder", name);

	return ret;
}

