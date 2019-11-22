#include "SpacePartition.h"
#include "SDL/include/SDL_opengl.h"
#include "Color.h"
#include "Application.h"
#include "Component_Camera.h"

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

	if (!Root->Node_Insert(mesh))
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

// Intersects
void Tree::Intersects(std::vector<GameObject*>& collector, const AABB& area)
{
	Root->Intersects(collector, area);
}

void Tree::Intersects(std::vector<GameObject*>& collector, const Frustum& frustum)
{
	Root->Intersects(collector, frustum);
}

void Tree::Intersects(std::vector<GameObject*>& collector, const LineSegment& line)
{
	Root->Intersects(collector, line);
}

//--------------------------------------------------- TREE NODE
TreeNode::TreeNode(AABB aabb, TreeNode* node)
{
	box = aabb;
	is_leaf = true;
	parent = node;
}

TreeNode::~TreeNode()
{
}

void TreeNode::DrawNode() const
{
	if (is_leaf)
	{
		glLineWidth(3.0f);
		

		bool draw_red = false;

		if(App->gui->ins_window->selected_GO != nullptr)
			if (App->gui->ins_window->selected_GO->GetComponentMesh() != nullptr)
			{
				for (int i = 0; i < meshes.size(); i++)
				{
					if (meshes[i]->my_GO == App->gui->ins_window->selected_GO)
						draw_red = true;
				}
			}
		
		if(!draw_red)
			glColor4f(Blue.r, Blue.g, Blue.b, Blue.a);
		else
		{
			glLineWidth(5.0f);
			glColor4f(Red.r, Red.g, Red.b, Red.a);
		}
		
		glBegin(GL_LINES);

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


// Intersects
void TreeNode::Intersects(std::vector<GameObject*>& collector, const AABB& area)
{
	if (box.Intersects(area)) {
		for (int i = 0; i < meshes.size(); i++)
				collector.push_back(meshes[i]->my_GO);

		if (childs.size() > 0)
			for (int i = 0; i < childs.size(); i++)
				childs[i]->Intersects(collector, area);
	}
}

void TreeNode::Intersects(std::vector<GameObject*>& collector, const Frustum& frustum)
{
	if (ComponentCamera::ContainsAABB(frustum, box))
	{
		for (int i = 0; i < meshes.size(); i++)
			collector.push_back(meshes[i]->my_GO);

		if (childs.size() > 0)
			for (int i = 0; i < childs.size(); i++)
				childs[i]->Intersects(collector, frustum);
	}
}

void TreeNode::Intersects(std::vector<GameObject*>& collector, const LineSegment& line)
{
	if (box.Intersects(line)) {
		for (int i = 0; i < meshes.size(); i++) {
			float nearHit, farHit;
			if (meshes[i]->global_aabb.Intersects(line, nearHit, farHit))
				collector[nearHit] = meshes[i]->my_GO;
		}

		if (childs.size() > 0)
			for (int i = 0; i < childs.size(); i++)
				childs[i]->Intersects(collector, line);
	}
}

// -----------------------------------------------------------------

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
	TreeNode* node1 = new TreeNode(temp_aabb, this);
	childs.push_back(node1);


	//NORTH-EAST subnode
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2, box.MaxY(), box.MaxZ() };
	temp_aabb.minPoint = { box.MinX(), box.MinY(), (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node2 = new TreeNode(temp_aabb, this);
	childs.push_back(node2);

	//SOUTH-WEST subnode
	temp_aabb.maxPoint = { box.MaxX(), box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.minPoint = { (box.MaxX() + box.MinX()) / 2, box.MinY(), box.MinZ() };
	TreeNode* node3 = new TreeNode(temp_aabb, this);
	childs.push_back(node3);

	//SOUTH-EAST subnode
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2, box.MaxY(),(box.MaxZ() + box.MinZ()) / 2 };
	temp_aabb.minPoint = { box.MinX(), box.MinY(), box.MinZ() };
	TreeNode* node4 = new TreeNode(temp_aabb, this);
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

	}

}

void TreeNode::PruneEmptyLeafs()
{
	if (is_leaf)
	{
		if (meshes.size() == 0)
		{
			for (std::vector<TreeNode*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
			{
				if ((*it) == this)
				{
					parent->childs.erase(it);
					delete this;
					break;
				}
			}
		}
	}
	else
	{
		for (int i=0; i < childs.size(); i++)
		{
			if(childs[i] != nullptr)
				childs[i]->PruneEmptyLeafs();
		}
	}

}

void TreeNode::OctSplit()
{
	is_leaf = false;

	//OCTREE --> Making an AABB for each Node (8 nodes)
	AABB temp_aabb;
	//TOP FAR LEFT - NODE 1
	temp_aabb.minPoint = { box.MaxX(), box.MaxY(), box.MaxZ() };
	temp_aabb.maxPoint = { (box.MaxX() + box.MinX()) / 2 , (box.MinY() + box.MaxY()) / 2, (box.MaxZ() + box.MinZ()) / 2 };
	TreeNode* node1 = new TreeNode(temp_aabb);
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

	}
}