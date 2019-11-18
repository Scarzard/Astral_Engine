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
	Tree(float3 minPoint, float3 maxPoint);
	~Tree();

	void DrawTree(TreeNode* node);

	void CleanUp();

public:
	TreeNode* Root;
};


// ----------------------------------------------- TREENODE

class TreeNode
{
public:
	TreeNode();
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