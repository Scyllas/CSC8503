#pragma once

#include "PhysicsEngine.h"
#include "BoundingBox.h"

class Octree
{
public:
	Octree(vector<Octree*>*, BoundingBox box, vector<PhysicsNode*> * physicsNodes = nullptr);
	~Octree();

	void DebugDraw();
	void createChildren();

	vector<PhysicsNode*>  checkOverlap();

	void pushBackNodeToChild(vector<PhysicsNode*> temp);

	vector<PhysicsNode*>* getNodes() { return myNodes;}
	Octree** getChildren() { return m_childOctree; }

	Octree* getParent() { return parent; }
	void setParent(Octree* o) { parent = o; }

private:

	float octreeMax = 5;

	BoundingBox dimensions;

	Octree* m_childOctree[8];

	Octree* parent = nullptr;

	vector<Octree*>* octreeMap;

	vector<PhysicsNode*>* passedNodes;
	vector<PhysicsNode*>* myNodes;
	vector<PhysicsNode*>* childNodes[8];


};

