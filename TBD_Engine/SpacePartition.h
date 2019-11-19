#ifndef __SPACEPARTITION_H__
#define __SPACEPARTITION_H__

#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class TreeNode;
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

	void DrawNode();

	void CleanUp(TreeNode* node);

public:

	AABB box;
	std::vector<TreeNode*> childs;
	std::vector<GameObject*> my_GOs;

	bool isNode_bottom;
};


#endif //!__SPACEPARTITION_H__