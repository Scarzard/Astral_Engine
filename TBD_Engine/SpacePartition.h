#ifndef __SPACEPARTITION_H__
#define __SPACEPARTITION_H__

#include "MathGeoLib/include/MathGeoLib.h"

#define BUCKET 10

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

public:
	TreeNode* Root;
	float3 min_point, max_point;

	bool update_tree = true;
};


// ----------------------------------------------- TREENODE

class TreeNode
{
public:
	TreeNode(AABB aabb);
	~TreeNode();

	void Split();

	void DrawNode() const;
	void Node_Insert(ComponentMesh* mesh);

	void CleanUp(TreeNode* node);

	bool isNodeFull();

public:

	AABB box;
	std::vector<TreeNode*> childs;
	std::vector<ComponentMesh*> meshes;
	int level = 0;

	bool is_leaf = true;
};


#endif //!__SPACEPARTITION_H__