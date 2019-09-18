#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Color.h"
#include "glmath.h"

struct Light
{
	Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	vec3 position;

	int ref;
	bool on;
};

#endif // !_LIGHT_H_
