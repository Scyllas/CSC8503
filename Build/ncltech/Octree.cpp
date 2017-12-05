#include "Octree.h"

Octree::Octree(vector<Octree*>* deepestSubdivisions, BoundingBox box, vector<PhysicsNode*>* physicsNodes)
{

	deepestSubdivisions->push_back(this);

	octreeMap = deepestSubdivisions;

	dimensions = box;

	passedNodes = physicsNodes;

	myNodes = new vector<PhysicsNode*>();

	DebugDraw();

	if (physicsNodes != nullptr && physicsNodes->size() >= octreeMax) createChildren();
	else { myNodes = physicsNodes; }

}

Octree::~Octree()
{

	//	delete m_childOctree;


	//	delete passedNodes;
	//	delete myNodes;

	//	delete[] childNodes;
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
	vector<PhysicsNode*> temp;

	temp = checkOverlap();
	pushBackNodeToChild(temp);

	octreeMap->pop_back();

	//childNodes
	{
		BoundingBox b;
		if (childNodes[0]->size() != 0) {

			b = BoundingBox(dimensions._min, dimensions._center);
			m_childOctree[0] = new Octree(octreeMap, b, childNodes[0]);
			m_childOctree[0]->setParent(this);
		}

		if (childNodes[1]->size() != 0) {
			b = BoundingBox(
				Vector3(dimensions._center.x, dimensions._min.y, dimensions._min.z),
				Vector3(dimensions._max.x, dimensions._center.y, dimensions._center.z));
			m_childOctree[1] = new Octree(octreeMap, b, childNodes[1]);
			m_childOctree[1]->setParent(this);
		}

		if (childNodes[2]->size() != 0) {

			b = BoundingBox(
				Vector3(dimensions._min.x, dimensions._center.y, dimensions._min.z),
				Vector3(dimensions._center.x, dimensions._max.y, dimensions._center.z));
			m_childOctree[2] = new Octree(octreeMap, b, childNodes[2]);
			m_childOctree[2]->setParent(this);
		}

		if (childNodes[3]->size() != 0) {

			b = BoundingBox(
				Vector3(dimensions._min.x, dimensions._min.y, dimensions._center.z),
				Vector3(dimensions._center.x, dimensions._center.y, dimensions._max.z));
			m_childOctree[3] = new Octree(octreeMap, b, childNodes[3]);
			m_childOctree[3]->setParent(this);
		}

		if (childNodes[4]->size() != 0) {

			b = BoundingBox(
				Vector3(dimensions._center.x, dimensions._center.y, dimensions._min.z),
				Vector3(dimensions._max.x, dimensions._max.y, dimensions._center.z));
			m_childOctree[4] = new Octree(octreeMap, b, childNodes[4]);
			m_childOctree[4]->setParent(this);
		}

		if (childNodes[5]->size() != 0) {

			b = BoundingBox(
				Vector3(dimensions._center.x, dimensions._min.y, dimensions._center.z),
				Vector3(dimensions._max.x, dimensions._center.y, dimensions._max.z));
			m_childOctree[5] = new Octree(octreeMap, b, childNodes[5]);
			m_childOctree[5]->setParent(this);
		}

		if (childNodes[6]->size() != 0) {

			b = BoundingBox(
				Vector3(dimensions._min.x, dimensions._center.y, dimensions._center.z),
				Vector3(dimensions._center.x, dimensions._max.y, dimensions._max.z));
			m_childOctree[6] = new Octree(octreeMap, b, childNodes[6]);
			m_childOctree[6]->setParent(this);
		}

		if (childNodes[7]->size() != 0) {

			b = BoundingBox(dimensions._center, dimensions._max);
			m_childOctree[7] = new Octree(octreeMap, b, childNodes[7]);
			m_childOctree[7]->setParent(this);
		}
	}

}


vector<PhysicsNode*> Octree::checkOverlap() {

	vector<PhysicsNode*> temp;


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
		if (x < dimensions._min.x) {
			if (x + rad >= dimensions._min.x) {
				overlap = true;
			}
		}
		if (x < dimensions._max.x) {
			if (x - rad >= dimensions._max.x) {
				overlap = true;
			}
		}
		if (x > dimensions._max.x) {
			if (x + rad <= dimensions._max.x) {
				overlap = true;
			}
		}
		if (x == dimensions._min.x) {
			overlap = true;
		}
		//if y radius overlaps the bounds
		if (y > dimensions._min.y) {
			if (y - rad <= dimensions._min.y) {
				overlap = true;
			}
		}
		if (y < dimensions._min.y) {
			if (y + rad >= dimensions._min.y) {
				overlap = true;
			}
		}
		if (y < dimensions._max.y) {
			if (y - rad >= dimensions._max.y) {
				overlap = true;
			}
		}
		if (y > dimensions._max.y) {
			if (y + rad <= dimensions._max.y) {
				overlap = true;
			}
		}
		if (y == dimensions._min.y) {
			overlap = true;
		}
		//if z radius overlaps the bounds
		if (z > dimensions._min.z) {
			if (z - rad <= dimensions._min.z) {
				overlap = true;
			}
		}
		if (z < dimensions._min.z) {
			if (z + rad >= dimensions._min.z) {
				overlap = true;
			}
		}
		if (z < dimensions._max.z) {
			if (z - rad >= dimensions._max.z) {
				overlap = true;
			}
		}
		if (z > dimensions._max.z) {
			if (z + rad <= dimensions._max.z) {
				overlap = true;
			}
		}
		if (z == dimensions._min.z) {
			overlap = true;
		}


		if (overlap == true) {
			myNodes->push_back(*it);
		}
		else {
			temp.push_back(*it);
		}

	}
	return temp;
}

void Octree::pushBackNodeToChild(vector<PhysicsNode*> temp) {

	for (vector<PhysicsNode*>::iterator it = temp.begin(); it != temp.end(); it++) {

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