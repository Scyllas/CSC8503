
#pragma once

#include <ncltech\Scene.h>
#include <ncltech\NetworkBase.h>
#include <nclgl\OBJMesh.h>
#include <ncltech\MazeRenderer.h>
#include <ncltech\EnumPackets.h>

//Basic Network Example

class Net1_Client : public Scene
{
public:
	Net1_Client(const std::string& friendly_name);

	virtual void OnInitializeScene() override;
	virtual void OnCleanupScene() override;
	virtual void OnUpdateScene(float dt) override;


	void ProcessNetworkEvent(const ENetEvent& evnt);

	void createMazeFromServer();

	Vector3 ConvertWorldPosition(Vector3 gridPos);

	void velAndPosUpdate();




protected:
	GameObject* box;
	GameObject* player;
	GameObject* ground;
	MazeGenerator* maze;
	MazeRenderer*	mazeRender;

	Vector3* astar_path;

	SearchHistory search_as;

	Mesh* wallmesh;
	int playerNum;
	int pathPoint = 0;
	int path_size = 1;
	int grid_size = 1;
	int maze_size = 1;
	float density = 0.5f;

	bool* isWall;
	bool canBuild = false;
	bool drawAstar = false;

	Matrix4 maze_scalar;

	NetworkBase network;
	ENetPeer*	serverConnection;
};