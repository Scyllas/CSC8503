
/******************************************************************************
Class: Net1_Client
Implements:
Author: Pieran Marris <p.marris@newcastle.ac.uk> and YOU!
Description:

:README:
- In order to run this demo, we also need to run "Tuts_Network_Client" at the same time.
- To do this:-
	1. right click on the entire solution (top of the solution exporerer) and go to properties
	2. Go to Common Properties -> Statup Project
	3. Select Multiple Statup Projects
	4. Select 'Start with Debugging' for both "Tuts_Network_Client" and "Tuts_Network_Server"

- Now when you run the program it will build and run both projects at the same time. =]
- You can also optionally spawn more instances by right clicking on the specific project
and going to Debug->Start New Instance.




FOR MORE NETWORKING INFORMATION SEE "Tuts_Network_Client -> Net1_Client.h"



		(\_/)
		( '_')
	 /""""""""""""\=========     -----D
	/"""""""""""""""""""""""\
....\_@____@____@____@____@_/

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <enet\enet.h>
#include <nclgl\GameTimer.h>
#include <nclgl\Vector3.h>
#include <nclgl\common.h>
#include <ncltech\NetworkBase.h>

#include <nclgl\OBJMesh.h>

#include <ncltech\MazeGenerator.h>
#include <ncltech\SearchAStar.h>

#include <ncltech\EnumPackets.h>

//Needed to get computer adapter IPv4 addresses via windows
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")


#define SERVER_PORT 1234
#define UPDATE_TIMESTEP (1.0f / 30.0f) //send 30 position updates per second

NetworkBase server;
GameTimer timer;
float accum_time = 0.0f;
float rotation = 0.0f;

int grid_size = 16;
float density = 0.5f;
float weightingG, weightingH;

int astar_preset_idx;
int print_count = 0;

bool mazeToggle = false;
std::string astar_preset_text;

SearchAStar* search_as = new SearchAStar();

MazeGenerator* maze = new MazeGenerator();
GraphNode* startNode;
GraphNode* endNode;

const Vector3 status_color3 = Vector3(1.0f, 0.6f, 0.6f);
const Vector4 status_color = Vector4(status_color3.x, status_color3.y, status_color3.z, 1.0f);

void Win32_PrintAllAdapterIPAddresses();
void createMaze();
void UpdateAStarPreset();

int onExit(int exitcode)
{
	server.Release();
	system("pause");
	exit(exitcode);
}

int main(int arcg, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	//Initialize Server on Port 1234, with a possible 32 clients connected at any time
	if (!server.Initialize(SERVER_PORT, 32))
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		onExit(EXIT_FAILURE);
	}

	printf("Server Initiated\n");


	Win32_PrintAllAdapterIPAddresses();

	timer.GetTimedMS();



	while (true)
	{
		float dt = timer.GetTimedMS() * 0.001f;
		accum_time += dt;
		rotation += 0.5f * PI * dt;

		//Handle All Incoming Packets and Send any enqued packets
		server.ServiceNetwork(dt, [&](const ENetEvent& evnt)
		{
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("- New Client Connected\n");
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				if (evnt.packet->dataLength == sizeof(graphNodePacket) && evnt.packet->data[0] == graph)
				{
					graphNodePacket packet;
					memcpy(&packet, evnt.packet->data, sizeof(graphNodePacket));
					UpdateAStarPreset();
				}
				else if (evnt.packet->dataLength == sizeof(makeMazePacket) && evnt.packet->data[0] == create)
				{
					makeMazePacket packet;
					memcpy(&packet, evnt.packet->data, sizeof(makeMazePacket));
					mazeToggle = packet.toggle;
				}
				else {
					printf("\t Client %d says: %s\n", evnt.peer->incomingPeerID, evnt.packet->data);
				}
				enet_packet_destroy(evnt.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("- Client %d has disconnected.\n", evnt.peer->incomingPeerID);
				break;
			}
		});

		//Broadcast update packet to all connected clients at a rate of UPDATE_TIMESTEP updates per second
		if (accum_time >= UPDATE_TIMESTEP)
		{
			if (print_count >= 30) {
				printf("Incoming: %5.2fKbps, Outgoing: %5.2fKbps\n", server.m_IncomingKb, server.m_OutgoingKb);
				print_count = 0;
			}
			else {
				print_count++;
			}
			//Packet data
			// - At the moment this is just a position update that rotates around the origin of the world
			//   though this can be any variable, structure or class you wish. Just remember that everything 
			//   you send takes up valuable network bandwidth so no sending every PhysicsObject struct each frame ;)
			accum_time = 0.0f;
			Vector3 pos = Vector3(
				cos(rotation) * 2.0f,
				1.5f,
				sin(rotation) * 2.0f);

			vector3Packet temp;
			temp.vec = pos;
			//Create the packet and broadcast it (unreliable transport) to all clients
			ENetPacket* position_update = enet_packet_create(&temp, sizeof(vector3Packet), 0);
			enet_host_broadcast(server.m_pNetwork, 0, position_update);


		}
		if (mazeToggle == true) {
			createMaze();
			mazeToggle = false;
		}
		
		

		Sleep(0);
	}

	system("pause");
	server.Release();
}




//Yay Win32 code >.>
//  - Grabs a list of all network adapters on the computer and prints out all IPv4 addresses associated with them.
void Win32_PrintAllAdapterIPAddresses()
{
	//Initially allocate 5KB of memory to store all adapter info
	ULONG outBufLen = 5000;


	IP_ADAPTER_INFO* pAdapters = NULL;
	DWORD status = ERROR_BUFFER_OVERFLOW;

	//Keep attempting to fit all adapter info inside our buffer, allocating more memory if needed
	// Note: Will exit after 5 failed attempts, or not enough memory. Lets pray it never comes to this!
	for (int i = 0; i < 5 && (status == ERROR_BUFFER_OVERFLOW); i++)
	{
		pAdapters = (IP_ADAPTER_INFO *)malloc(outBufLen);
		if (pAdapters != NULL) {

			//Get Network Adapter Info
			status = GetAdaptersInfo(pAdapters, &outBufLen);

			// Increase memory pool if needed
			if (status == ERROR_BUFFER_OVERFLOW) {
				free(pAdapters);
				pAdapters = NULL;
			}
			else {
				break;
			}
		}
	}


	if (pAdapters != NULL)
	{
		//Iterate through all Network Adapters, and print all IPv4 addresses associated with them to the console
		// - Adapters here are stored as a linked list termenated with a NULL next-pointer
		IP_ADAPTER_INFO* cAdapter = &pAdapters[0];
		while (cAdapter != NULL)
		{
			IP_ADDR_STRING* cIpAddress = &cAdapter->IpAddressList;
			while (cIpAddress != NULL)
			{
				printf("\t - Listening for connections on %s:%u\n", cIpAddress->IpAddress.String, SERVER_PORT);
				cIpAddress = cIpAddress->Next;
			}
			cAdapter = cAdapter->Next;
		}

		free(pAdapters);
	}

}

void createMaze() {


	


	std::string astar_preset_text;

	maze->Generate(grid_size, density);
	const int mazeSize = (maze->GetSize() * (maze->GetSize() - 1) * 2);

	mazeVarPacket mazeInfo;
	mazeInfo.gridSize = grid_size;
	mazeInfo.arraySize = mazeSize;
	mazeInfo.density = density;

	ENetPacket* send_maze_info = enet_packet_create(&mazeInfo, sizeof(mazeVarPacket), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server.m_pNetwork, 0, send_maze_info);

	//const int wallRows = mazeSize / 8;
	//int temp = NULL;
	//int temp2 = NULL;
	//int current = NULL;
	//int concatCount = 0;
	//int* walls = new int[wallRows];
	//int wallIndex = 0;

	//for (int i = 0; i < mazeSize; i++) {
	//	current = temp2;
	//	//ensure my ints have only 8 bytes
	//	if (concatCount >= 7) {
	//		//allocate a full int to the array
	//		walls[wallIndex] = temp;
	//		//clear the int
	//		temp = NULL;
	//		//move to the next array pos
	//		wallIndex++;
	//		//restart the int count
	//		concatCount = 0;
	//	}
	//	if (maze->allEdges[i]._iswall) {
	//		//append a 1 to the int
	//		temp = 0b1;
	//	}
	//	else {
	//		//append a 0 to the int
	//		temp = 0b0;
	//	}
	//	//bitwise the int onto the end of the
	//	current |= (1UL << temp);
	//	//increment the int count
	//	concatCount++;
	//}

	mazeWallPacket mazeWallInfo;

	for (int i = 0; i < mazeSize; i++) {
		if (maze->allEdges[i]._iswall) {
			mazeWallInfo.mazeWall[i] = true;
		}
		else {
			mazeWallInfo.mazeWall[i] = false;
		}
	}


	mazeWallInfo.mazeSize = mazeSize;

	ENetPacket* send_mazeWall_info = enet_packet_create(&mazeWallInfo, sizeof(mazeWallPacket), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server.m_pNetwork, 0, send_mazeWall_info);

}

void UpdateAStarPreset()
{
	//Example presets taken from:
	// http://movingai.com/astar-var.html
	float weightingG, weightingH;
	switch (astar_preset_idx)
	{
	default:
	case 0:
		//Only distance from the start node matters - fans out from start node
		weightingG = 1.0f;
		weightingH = 0.0f;
		astar_preset_text = "Dijkstra - None heuristic search";
		break;
	case 1:
		//Only distance to the end node matters
		weightingG = 0.0f;
		weightingH = 1.0f;
		astar_preset_text = "Pure Hueristic search";
		break;
	case 2:
		//Equal weighting
		weightingG = 1.0f;
		weightingH = 1.0f;
		astar_preset_text = "Traditional A-Star";
		break;
	case 3:
		//Greedily goes towards the goal node where possible, but still cares about distance travelled a little bit
		weightingG = 1.0f;
		weightingH = 2.0f;
		astar_preset_text = "Weighted Greedy A-Star";
		break;
	}
	search_as->SetWeightings(weightingG, weightingH);

	startNode = maze->GetStartNode();
	endNode = maze->GetGoalNode();

	search_as->FindBestPath(startNode, endNode);

	aStarPacket astar;
	astar.astarPath = search_as;

	ENetPacket* send_mazeWall_info = enet_packet_create(&astar, sizeof(aStarPacket), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server.m_pNetwork, 0, send_mazeWall_info);

}