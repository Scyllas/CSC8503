#pragma once
class Octree
{
public:
	Octree();
	~Octree();

private:

	Octree* m_childNode = new Octree[8];
};

