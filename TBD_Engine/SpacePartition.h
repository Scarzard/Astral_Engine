#ifndef __SPACEPARTITION_H__
#define __SPACEPARTITION_H__

#include "MathGeoLib/include/MathGeoLib.h"
#include <map>

#define BUCKET 3

class GameObject;
class TreeNode;
class ComponentMesh;
// ----------------------------------------------- TREE
class Tree
{
public:
	Tree(AABB aabb);
	~Tree();

	void DrawTree(TreeNode* node);
	void CleanUp();

	void UpdateTree();
	void CalculateNewSize(float3 min_point, float3 max_point);

	void Insert(ComponentMesh* mesh);

	void Intersects(std::vector<GameObject*>& collector, const AABB& area);
	void Intersects(std::vector<GameObject*>& collector, const Frustum& frustum);
	void Intersects(std::map<float, GameObject*>& collector, const LineSegment& line, float closer = true);

public:
	TreeNode* Root;
	float3 min_point, max_point;
	bool update_tree = true;
};


// ----------------------------------------------- TREENODE

class TreeNode
{
public:
	TreeNode(AABB aabb, TreeNode* parent = nullptr);
	~TreeNode();

	void OctSplit();
	void QuadSplit();

	void DrawNode() const;
	bool Node_Insert(ComponentMesh* mesh);

	void CleanUp(TreeNode* node);
	void PruneEmptyLeafs();

	bool isNodeFull();

	void Intersects(std::vector<GameObject*>& collector, const AABB& area);
	void Intersects(std::vector<GameObject*>& collector, const Frustum& frustum);
	void Intersects(std::map<float, GameObject*>& collector, const LineSegment& line, float closer = true);

public:

	AABB box;
	std::vector<TreeNode*> childs;
	std::vector<ComponentMesh*> meshes;
	int level = 0;

	bool is_leaf = true;
	TreeNode* parent = nullptr;
};


#endif //!__SPACEPARTITION_H__