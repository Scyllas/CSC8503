#pragma once
#include <nclgl\Vector3.h>
#include <ncltech\SearchAStar.h>

enum PacketsID
{
	boxVector = 0,
	mazeVariables = 1,
	mazeWalls = 2,
	astar = 3,
	graph = 4,
	create = 5

};

struct vector3Packet {

	int ID = boxVector;
	Vector3 vec = Vector3();

};

struct mazeVarPacket {

	int ID = mazeVariables;
	bool toggle = true;
	int gridSize = 1;
	int arraySize = 1;
	float density = 0.5f;
};

struct mazeWallPacket {

	int ID = mazeWalls;
	int mazeSize = 1;
	bool mazeWall[1000];

};

struct aStarPacket {

	int ID = astar;
	SearchAStar astarPath;
};

struct graphNodePacket {
	int ID = graph;
	GraphNode* start_node;
	GraphNode* end_node;
};

struct makeMazePacket{
	int ID = create;
	bool toggle = true;

};