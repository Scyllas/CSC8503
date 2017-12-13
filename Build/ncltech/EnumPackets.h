#pragma once
#include <nclgl\Vector3.h>

enum PacketsID
{
	boxVector = 0,
	mazeVariables = 1,
	mazeWalls = 2

};

struct vector3Packet {

	int ID = boxVector;
	Vector3 vec;

};

struct mazeVarPacket {

	int ID = mazeVariables;
	int gridSize;
	int size;

};
struct mazeWallPacket {

	int ID = mazeWalls;
	char* mazeWall;

};