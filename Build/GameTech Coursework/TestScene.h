#pragma once

#include <ncltech\Scene.h>



class TestScene : public Scene
{
public:
	TestScene(const std::string& friendly_name);
	virtual ~TestScene();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;

protected:
	float m_AccumTime;
	GameObject* m_pPlayer;

	GameObject* sphere[MAX_BALLS];

	int currentBallNum = 0;
	bool allBallsInPlay = false;
};