#include "Octree.h"

Octree::Octree(BoundingBox box, vector<PhysicsNode*>* physicsNodes)
{

	dimensions = box;

	passedNodes = physicsNodes;

	DebugDraw();

	if (physicsNodes != nullptr && physicsNodes->size() >= octreeMax) createChildren();

}

Octree::~Octree()
{
}

void Octree::DebugDraw()
{
	dimensions.DebugDraw();
}

void Octree::createChildren()
{
	for (int i = 0; i < 8; i++) {
		childNodes[i] = new vector<PhysicsNode*>();
	}
	vector<PhysicsNode*>* temp;

	temp = checkOverlap();
	pushBackNodeToChild(temp);


	//childNodes
	{
		m_childNode[0] = Octree(
			BoundingBox(
				dimensions._min,
				dimensions._center),
			childNodes[0]);

		m_childNode[1] = Octree(
			BoundingBox(
				Vector3(dimensions._center.x, dimensions._min.y, dimensions._min.z),
				Vector3(dimensions._max.x, dimensions._center.y, dimensions._center.z)),
			childNodes[1]);

		m_childNode[2] = Octree(
			BoundingBox(
				Vector3(dimensions._min.x, dimensions._center.y, dimensions._min.z),
				Vector3(dimensions._center.x, dimensions._max.y, dimensions._center.z)),
			childNodes[2]);

		m_childNode[3] = Octree(
			BoundingBox(
				Vector3(dimensions._min.x, dimensions._min.y, dimensions._center.z),
				Vector3(dimensions._center.x, dimensions._center.y, dimensions._max.z)),
			childNodes[3]);

		m_childNode[4] = Octree(
			BoundingBox(
				Vector3(dimensions._center.x, dimensions._center.y, dimensions._min.z),
				Vector3(dimensions._max.x, dimensions._max.y, dimensions._center.z)),
			childNodes[4]);

		m_childNode[5] = Octree(
			BoundingBox(
				Vector3(dimensions._center.x, dimensions._min.y, dimensions._center.z),
				Vector3(dimensions._max.x, dimensions._center.y, dimensions._max.z)),
			childNodes[5]);

		m_childNode[6] = Octree(
			BoundingBox(
				Vector3(dimensions._min.x, dimensions._center.y, dimensions._center.z),
				Vector3(dimensions._center.x, dimensions._max.y, dimensions._max.z)),
			childNodes[6]);

		m_childNode[7] = Octree(
			BoundingBox(
				dimensions._center,
				dimensions._max),
			childNodes[7]);
	}
}


vector<PhysicsNode*>* Octree::checkOverlap() {

	vector<PhysicsNode*>* temp;


	for (vector<PhysicsNode*>::iterator it = passedNodes->begin(); it != passedNodes->end(); it++) {
		float x = (*it)->GetPosition().x;
		float y = (*it)->GetPosition().y;
		float z = (*it)->GetPosition().z;

		float rad = (*it)->GetBoundingRadius();
		bool overlap = false;

		//if x radius overlaps the bounds
		if (x > dimensions._min.x) {
			if (x - rad <= dimensions._min.x) {				
				overlap = true;
			}
		}
		else if (x < dimensions._min.x) {
			if (x + rad >= dimensions._min.x) {				
				overlap = true;
			}
		}
		else if (x == dimensions._min.x) {			
			overlap = true;
		}
		//if y radius overlaps the bounds
		if (y > dimensions._min.y) {
			if (y - rad <= dimensions._min.y) {				
				overlap = true;
			}
		}
		else if (y < dimensions._min.y) {
			if (y + rad >= dimensions._min.y) {				
				overlap = true;
			}
		}
		else if (y == dimensions._min.y) {			
			overlap = true;
		}
		//if z radius overlaps the bounds
		if (z > dimensions._min.z) {
			if (z - rad <= dimensions._min.z) {				
				overlap = true;
			}
		}
		else if (z < dimensions._min.z) {
			if (z + rad >= dimensions._min.z) {			
				overlap = true;
			}
		}
		else if (z == dimensions._min.z) {
			overlap = true;
		}


		if (overlap == true) {
			myNodes->push_back(*it);
		}
		else {
			temp->push_back(*it);
		}

	}
	return temp;
}

void Octree::pushBackNodeToChild(vector<PhysicsNode*>* temp) {

	for (vector<PhysicsNode*>::iterator it = temp->begin(); it != temp->end(); it++) {

		if ((*it)->GetPosition().x > dimensions._center.x) {
			if ((*it)->GetPosition().y > dimensions._center.y) {
				if ((*it)->GetPosition().z > dimensions._center.z) {
					//if x y and z are past the middle
					childNodes[7]->push_back(*it);
				}
				else {
					//if x and y are past the middle but not z
					childNodes[4]->push_back(*it);
				}
			}
			else {
				if ((*it)->GetPosition().z > dimensions._center.z) {
					//if x and z are past the middle but not y
					childNodes[5]->push_back(*it);
				}
				else {
					//if x is past the middle but not y and z	
					childNodes[1]->push_back(*it);
				}
			}
		}
		else {
			if ((*it)->GetPosition().y > dimensions._center.y) {
				if ((*it)->GetPosition().z > dimensions._center.z) {
					//if y and z are past the middle but not x
					childNodes[6]->push_back(*it);
				}
				else {
					//if y is past the middle but not x
					childNodes[2]->push_back(*it);
				}
			}
			else {
				if ((*it)->GetPosition().z > dimensions._center.z) {
					//if z is past the middle but not x and y
					childNodes[3]->push_back(*it);
				}
				else {
					//if none are past the middle
					childNodes[0]->push_back(*it);
				}
			}
		}
	}

}