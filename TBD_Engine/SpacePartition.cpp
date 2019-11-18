#include "SpacePartition.h"
#include "SDL/include/SDL_opengl.h"
#include "Color.h"

#include "mmgr/mmgr.h"

//--------------------------------------------------- TREE
Tree::Tree(AABB aabb)
{
	Root = new TreeNode(aabb);
}

Tree::Tree(float3 minPoint, float3 maxPoint) 
{
	Root = new TreeNode();
	Root->box.minPoint = minPoint;
	Root->box.maxPoint = maxPoint;
}

Tree::~Tree()
{
}

void Tree::DrawTree(TreeNode* node)
{
	node->DrawNode();

	//recursive draw
	if (node->childs.size() > 0)
	{
		for (std::vector<TreeNode*>::iterator it = node->childs.begin(); it != node->childs.end(); ++it)
		{
			DrawTree((*it));
		}
	}
}


void Tree::CleanUp()
{
	Root->CleanUp(Root);

	delete this;
}

//--------------------------------------------------- TREE NODE
TreeNode::TreeNode(AABB aabb)
{
	box = aabb;
}

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
}

void TreeNode::DrawNode()
{
	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor4f(Red.r, Red.g, Red.b, Red.a);

	for (int i = 0; i < 12; i++)
	{
		glVertex3f(box.Edge(i).a.x, box.Edge(i).a.y, box.Edge(i).a.z);
		glVertex3f(box.Edge(i).b.x, box.Edge(i).b.y, box.Edge(i).b.z);
	}
	glEnd();
	glLineWidth(1.0f);

}

void TreeNode::CleanUp(TreeNode* node) 
{
	//delete its childrens (if it has)
	if (node->childs.size() > 0)
	{
		for (std::vector<TreeNode*>::iterator it = node->childs.begin(); it != node->childs.end(); ++it)
		{
			CleanUp(*it);
		}

		node->childs.clear();
	}
	//delete this node
	delete node;
}

void TreeNode::Split()
{
	//OCTREE --> Making an AABB for each Node (8 nodes)
	AABB temp_aabb;
	//TOP FAR LEFT - NODE 1
	temp_aabb.minPoint = { box.MaxX(), box.MaxY(), box.MaxZ() };
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2 , (box.MinY() + box.MaxY()) / 2, (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node1  = new TreeNode(temp_aabb);
	childs.push_back(node1);

	//TOP FAR RIGHT - NODE 2
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2, box.MaxY(), box.MaxZ() };
	temp_aabb.maxPoint = { box.MinX(), (box.MinY() + box.MaxY()) / 2, (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node2 = new TreeNode(temp_aabb);
	childs.push_back(node2);

	//DOWN FAR LEFT - NODE 3
	temp_aabb.minPoint = { box.MaxX(), (box.MinY() + box.MaxY()) / 2, box.MaxZ() };
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2 ,box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node3 = new TreeNode(temp_aabb);
	childs.push_back(node3);

	//DOWN FAR RIGHT - NODE 4
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2,(box.MinY() + box.MaxY()) / 2, box.MaxZ() };
	temp_aabb.maxPoint = { box.MinX(), box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node4 = new TreeNode(temp_aabb);
	childs.push_back(node4);

	//TOP CLOSE LEFT - NODE 5
	temp_aabb.minPoint = { box.MaxX(), box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2, (box.MinY() + box.MaxY()) / 2, box.MinZ() };
	TreeNode* node5 = new TreeNode(temp_aabb);
	childs.push_back(node5);

	//TOP CLOSE RIGHT - NODE 6
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2, box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.maxPoint = { box.MinX(), (box.MinY() + box.MaxY()) / 2, box.MinZ() };
	TreeNode* node6 = new TreeNode(temp_aabb);
	childs.push_back(node6);

	//DOWN CLOSE LEFT - NODE 7
	temp_aabb.minPoint = { box.MaxX(), (box.MinY() + box.MaxY()) / 2, (box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2, box.MinY(), box.MinZ() };
	TreeNode* node7 = new TreeNode(temp_aabb);
	childs.push_back(node7);

	//DOWN CLOSE RIGHT - NODE 8
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2,(box.MinY() + box.MaxY()) / 2,(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.maxPoint = { box.MinX(), box.MinY(), box.MinZ() };
	TreeNode* node8 = new TreeNode(temp_aabb);
	childs.push_back(node8);
}