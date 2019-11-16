#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Module.h"

class Importer 
{
public:
	Importer();

	~Importer();
	
	bool Export(const char * name, std::string & output_file, ComponentMesh* mesh);
};

#endif // __IMPORTER_H__
