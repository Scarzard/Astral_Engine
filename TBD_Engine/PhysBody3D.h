//#ifndef _PHYSBODY_H_
//#define _PHYSBODY_H_
//
//#include "Module.h"
//
//class btRigidBody;
//class Module;
//
//// =================================================
//struct PhysBody3D
//{
//	friend class ModulePhysics3D;
//public:
//	PhysBody3D(btRigidBody* body);
//	~PhysBody3D();
//
//	void Push(float x, float y, float z);
//	void GetTransform(float* matrix) const;
//	void SetTransform(const float* matrix) const;
//	void SetPos(float x, float y, float z);
//	void SetAsSensor(bool is_sensor);
//
//private:
//	btRigidBody* body = nullptr;
//
//public:
//	bool is_sensor = false;
//	list<Module*> collision_listeners;
//};
//#endif // !_PHYSBODY_H_
