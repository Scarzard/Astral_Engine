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

	void CreateBuffer();
	void RenderShape();

	void CreateCube(float x, float y, float z, float size = 1);
	void CreateSphere(float x, float y, float z, int slices, int stacks); 
	void CreateTrefoil(float x, float y, float z, int slices, int stacks, int rad);
	void CreateIcosahedron(float x, float y, float z);
	
public: 

	par_shapes_mesh_s* obj = nullptr;
	uint id_vertex = 0;
	uint id_indices = 0;

	Position position;
};



#endif // !__SHAPES_H__