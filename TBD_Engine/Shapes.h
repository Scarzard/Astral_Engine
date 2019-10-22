#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "Globals.h"


struct par_shapes_mesh_s;

struct Position
{
	float x, y, z;
};

class Shapes{

public:

	Shapes();
	~Shapes();

	void LoadPrimitiveMesh(const par_shapes_mesh_s* m);

	void CreateCube(float x, float y, float z, float size = 1);
	void CreateSphere(float x, float y, float z, int slices, int stacks); 
	void CreateTrefoil(float x, float y, float z, int slices, int stacks, int rad);
	void CreateTorus(float x, float y, float z, int slices, int stacks, int rad);
	
public: 

	par_shapes_mesh_s* obj = nullptr;

	Position position;
};



#endif // !__SHAPES_H__