#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Module.h"

class Importer 
{
public:
	Importer();

	~Importer();

	//export mesh
	bool Export(const char * name, std::string & output_file, ComponentMesh* mesh);
	//export transform
	bool Export(const char * name, std::string & output_file, ComponentTransform* trans);

	bool Load(const char* full_path, ComponentMesh* mesh);
};

#endif // __IMPORTER_H__
