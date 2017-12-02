#pragma once

#include "PhysicsEngine.h"
#include "BoundingBox.h"

class Octree
{
public:
	Octree(BoundingBox box, vector<PhysicsNode*> * physicsNodes = nullptr);
	~Octree();

	void DebugDraw();
	void createChildren();

	vector<PhysicsNode*>*  checkOverlap();

	void pushBackNodeToChild(vector<PhysicsNode*>* temp);


private:

	float octreeMax = 5;

	BoundingBox dimensions;

	Octree* m_childNode;


	vector<PhysicsNode*>* passedNodes;
	vector<PhysicsNode*>* myNodes;
	vector<PhysicsNode*>* childNodes[8];

};

