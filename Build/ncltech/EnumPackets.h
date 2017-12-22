#pragma once
#include <nclgl\Vector3.h>
#include <ncltech\SearchAStar.h>
//#include <ncltech\SearchAlgorithm.h>

static int playerCount = 0;

enum PacketsID
{
	boxVector = 0,
	mazeVariables = 1,
	mazeWalls = 2,
	astar = 3,
	graph = 4,
	create = 5,
	playerID = 6,
	playerPos = 7,
	playerVel = 8
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
	Vector3 start_node_pos = Vector3();;
	Vector3 end_node_pos = Vector3();;

};

struct mazeWallPacket {

	int ID = mazeWalls;
	int mazeSize = 1;
	bool mazeWall[1000];

};

struct aStarPacket {
	int ID = astar;
	int pathSize = 1;
	Vector3 astarPath[1000];
	bool toggle = true;
};

struct graphNodePacket {
	int ID = graph;
	Vector3 start_node_pos = Vector3();;
	Vector3 end_node_pos = Vector3();;
	bool nodesInit = true;
};

struct makeMazePacket {
	int ID = create;
	bool toggle = true;

};


struct playerPacket {
	int ID = playerID;
	int playerNo;
	Vector3 pos = Vector3();
	Vector3 vel = Vector3();
};

struct playerPosPacket {
	int ID = playerPos;
	int playerNo;
	Vector3 pos = Vector3();
};

struct playerVelPacket {
	int ID = playerVel;
	int playerNo;
	Vector3 vel = Vector3();
};