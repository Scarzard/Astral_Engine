#include "SpacePartition.h"
#include "SDL/include/SDL_opengl.h"
#include "Color.h"
#include "Application.h"

#include "mmgr/mmgr.h"

//--------------------------------------------------- TREE
Tree::Tree(AABB aabb)
{
	Root = new TreeNode(aabb);
}

Tree::~Tree()
{
}

void Tree::UpdateTree()
{
	min_point = float3::zero;
	max_point = float3::zero;

	if(Root != nullptr)
		Root->CleanUp(Root);

	for (std::vector<ComponentMesh*>::iterator it = App->scene_intro->static_meshes.begin(); it != App->scene_intro->static_meshes.end(); it++)
	{
		if ((*it) != nullptr)
		{
			CalculateNewSize((*it)->global_aabb.minPoint, (*it)->global_aabb.maxPoint);
		}
			
	}

	AABB aabb(min_point, max_point);
	Root = new TreeNode(aabb);

}

void Tree::CalculateNewSize(float3 AABB_min_point, float3 AABB_max_point)
{
	if (min_point.x == 0 && min_point.y == 0 && min_point.z == 0 && max_point.x == 0 && max_point.y == 0 && max_point.z == 0)
	{
		min_point = AABB_min_point;
		max_point = AABB_max_point;
	}
	else 
	{
		if (AABB_min_point.x < min_point.x)
		{
			min_point.x = AABB_min_point.x;
		}
		if (AABB_min_point.y < min_point.y)
		{
			min_point.y = AABB_min_point.y;
		}
		if (AABB_min_point.z < min_point.z)
		{
			min_point.z = AABB_min_point.z;
		}
		if (AABB_max_point.x > max_point.x)
		{
			max_point.x = AABB_max_point.x;
		}
		if (AABB_max_point.y > max_point.y)
		{
			max_point.y= AABB_max_point.y;
		}
		if (AABB_max_point.z > max_point.z)
		{
			max_point.z = AABB_max_point.z;
		}
	}
}

void Tree::Insert(ComponentMesh * mesh)
{
	if (mesh == nullptr)
	{
		App->LogInConsole("Can't Insert mesh in tree. Mesh is nullptr");
		return;
	}

	num_meshes++;

	if (Root->Node_Insert(mesh))
	{
		App->LogInConsole("inserted mesh succes %i", num_meshes);
	}
	else
	{
		Root->meshes.push_back(mesh);
	}
		


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
	Root = nullptr;

	delete this;
}

//--------------------------------------------------- TREE NODE
TreeNode::TreeNode(AABB aabb)
{
	box = aabb;
	this->is_leaf = true;
}

TreeNode::~TreeNode()
{
}

void TreeNode::DrawNode() const
{
	if (is_leaf)
	{
		glLineWidth(3.0f);
		glBegin(GL_LINES);
		glColor4f(Blue.r, Blue.g, Blue.b, Blue.a);

		for (int i = 0; i < 12; i++)
		{
			glVertex3f(box.Edge(i).a.x, box.Edge(i).a.y, box.Edge(i).a.z);
			glVertex3f(box.Edge(i).b.x, box.Edge(i).b.y, box.Edge(i).b.z);
		}
		glEnd();
		glLineWidth(1.0f);
	}
}

bool TreeNode::Node_Insert(ComponentMesh* mesh)
{
	bool ret = false;
	if (box.Contains(mesh->global_aabb.CenterPoint()))
	{
		if (is_leaf)
		{
			if (isNodeFull() == false)
			{
				meshes.push_back(mesh);
				ret = true;
			}
			else
			{
				meshes.push_back(mesh);
				QuadSplit();
			}
		}
		else
		{
			bool inserted = false;
			for (std::vector<TreeNode*>::iterator it = childs.begin(); it != childs.end(); ++it)
			{
				if ((*it)->Node_Insert(mesh))
				{
					inserted = true;
					ret = true;
					break;
				}
			}
			if (!inserted)
			{
				meshes.push_back(mesh);
				ret = true;
			}
				
		}

	}

	return ret;
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

bool TreeNode::isNodeFull()
{
	return (meshes.size() == BUCKET);
}



void TreeNode::QuadSplit()
{
	is_leaf = false;

	AABB temp_aabb;
	//NORTH-WEST subnode
	temp_aabb.maxPoint = { box.MaxX(), box.MaxY(), box.MaxZ() };
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2 , box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node1 = new TreeNode(temp_aabb);
	childs.push_back(node1);
	

	//NORTH-EAST subnode
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2, box.MaxY(), box.MaxZ() };
	temp_aabb.minPoint = { box.MinX(), box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node2 = new TreeNode(temp_aabb);
	childs.push_back(node2);

	//SOUTH-WEST subnode
	temp_aabb.maxPoint = { box.MaxX(), box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2, box.MinY(), box.MinZ() };
	TreeNode* node3 = new TreeNode(temp_aabb);
	childs.push_back(node3);

	//SOUTH-EAST subnode
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2, box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.minPoint = { box.MinX(), box.MinY(), box.MinZ() };
	TreeNode* node4 = new TreeNode(temp_aabb);
	childs.push_back(node4);

	for (std::vector<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
		bool is_assigned = false;

		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->level = level + 1;

			if (childs[i]->Node_Insert(*it))
			{
				//App->LogInConsole("Passed mesh to child");
				is_assigned = true;
			}

		}

		if (!is_assigned)
			App->LogInConsole("Mesh doesn't fit in any child");
		else
		{
			is_assigned = false;
		}
			is_assigned = false;

	}
}