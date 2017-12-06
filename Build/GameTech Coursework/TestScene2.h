#pragma once

#include <ncltech\Scene.h>



class TestScene2 : public Scene
{
public:
	TestScene2(const std::string& friendly_name);
	virtual ~TestScene2();

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