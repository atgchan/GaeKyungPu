#include "pch.h"
#include "Phase_Volcano.h"
#include "TileMap.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <iterator>
#include "SimpleAudioEngine.h"

Phase_Volcano::Phase_Volcano()
{
	//리치 타일만 모아놓은 리스트를 생성한다.
	for (cocos2d::Node* iter : TileMap::getInstance()->getChildren())
	{
		if (static_cast<Self_Tile*>(iter)->getTypeOfTile() == TILE_RICH)
		{
			_VolcanoTileList.pushBack(iter);
		}
	}

	//생성한 리스트를 랜덤으로 섞는다.
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(_VolcanoTileList.begin(), _VolcanoTileList.end(), g);

	//화산으로 변화를 시작할 타일을 가르킨다.
	_VolcanoTileListIter = _VolcanoTileList.begin();
}


Phase_Volcano::~Phase_Volcano()
{
}

void Phase_Volcano::Tick()
{
	if (false == GM->getIsVolcanoActivated())
	{
		int randNum;
		std::srand(std::time(0));
		randNum = std::rand() % 5;
		if (randNum == 1)//5분의 1 확률로 이벤트 발생
		{
			GM->setVolcanoActivated(true);
			GM->setProgressVolcano(1);
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/lava-01.wav");
		}
	}

	switch (GM->getProgressVolcano())
	{
	case 1:
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/lava-01.wav");
		ChangeRichToLava(1);
		GM->setProgressVolcano(2);
		break;
	case 2:
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/lava-01.wav");
		ChangeRichToLava(2);
		GM->setProgressVolcano(3);
		break;
	case 3:
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Sound/lava-01.wav");
		ChangeRichToLava(3);
		GM->setProgressVolcano(4);
		break;
	case 4:
		ChangeAllLavaToPlane();
		GM->setProgressVolcano(5);
		break;
	case 5:
		GM->setVolcanoActivated(false);
		GM->setProgressVolcano(0);
		ChangePlanesToRich();
		_VolcanoTileListIter = _VolcanoTileList.begin();
		break;

	default:
		break;
	}

	_NextPhase = PHASE_ACTION;
}

void Phase_Volcano::ChangeRichToLava(int repeat)
{
	for (int i = 0; i < repeat; i++)
	{
		GM->ChangeRichToLava(static_cast<Self_Tile*>(*_VolcanoTileListIter));
		_VolcanoTileListIter++;
	}
}

void Phase_Volcano::ChangeAllLavaToPlane()
{
	for (cocos2d::Node* iter : _VolcanoTileList)
	{
		static_cast<Self_Tile*>(iter)->ChangeTile(TILE_PLAIN);
	}
}

void Phase_Volcano::ChangePlanesToRich()
{
	for (cocos2d::Node* iter : _VolcanoTileList)
	{
		static_cast<Self_Tile*>(iter)->ChangeTile(TILE_RICH);
	}
}
