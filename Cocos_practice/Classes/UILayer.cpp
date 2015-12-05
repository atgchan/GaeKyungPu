#include "pch.h"
#include "UILayer.h"
#include "GameSceneManager.h"

cocos2d::Scene* UILayer::scene()
{
	cocos2d::Scene *scene = Scene::create();
	UILayer* layer = UILayer::create();

	scene->addChild(layer);
	return scene;
}

bool UILayer::init()
{
	if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)))  //투명하게
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	SetUIBar();

	//backLayer추가
	BackLayer = LayerColor::create(ccc4(0, 0, 0, 0), 300, 50);
	BackLayer->setZOrder(10);
	BackLayer->setAnchorPoint(Vec2(0, 0));
	BackLayer->setPosition(50, visibleSize.height - 70);

	SetFoodValue(GM->getPlayerDataByPlayerInfo(PLAYER_RED), GM->getPlayerDataByPlayerInfo(PLAYER_BLUE));

	this->addChild(BackLayer);
	
	Sprite* endButton = Sprite::createWithSpriteFrameName("end_turn.png");
	Sprite* endButtonClicked = Sprite::createWithSpriteFrameName("end_turn_clicked.png");

	MenuItemSprite* toggleTurn = MenuItemSprite::create(endButton, endButtonClicked, CC_CALLBACK_1(GameSceneManager::ToggleTurn, GameSceneManager::getInstance()));
	MenuItemLabel* menu_toggle = MenuItemLabel::create(toggleTurn);
	menu_toggle->setPosition(Vec2(visibleSize.width * 1 / 2, visibleSize.height - 50));

	Menu* mainMenu = Menu::create(toggleTurn, NULL);
	mainMenu->setPosition(Vec2::ZERO);
	this->addChild(mainMenu);
	
	return true;
}

const void UILayer::SetFoodValue(PlayerData* pData1, PlayerData* pData2) const
{
	int pData1Food = pData1->getFood();
	int pData2Food = pData2->getFood();

	BackLayer->removeAllChildren();
	auto p1Food = Label::createWithTTF(std::to_string(pData1Food), "fonts/upheavtt.ttf", 50);
	auto p2Food = Label::createWithTTF(std::to_string(pData2Food), "fonts/upheavtt.ttf", 50);

	float width = BackLayer->getContentSize().width;
	float height = BackLayer->getContentSize().height;

	p1Food->setPosition(Vec2(100, height - 10));
	p2Food->setPosition(Vec2(100 + width / 2, height - 10));

	BackLayer->addChild(p1Food);
	BackLayer->addChild(p2Food);
}

void UILayer::SetUIBar()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* spriteBarArray[3];
	Sprite* spritecornerArray[4];
	for (int i = 0; i < 3; ++i)
	{
		spriteBarArray[i] = Sprite::createWithSpriteFrameName("bar.png");
		spriteBarArray[i]->setAnchorPoint(Vec2(0, 0));
		this->addChild(spriteBarArray[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		spritecornerArray[i] = Sprite::createWithSpriteFrameName("point.png");
		spritecornerArray[i]->setAnchorPoint(Vec2(0, 0));
		this->addChild(spritecornerArray[i]);
	}

	Sprite* grain = Sprite::createWithSpriteFrameName("grain.png");
	grain->setPosition(50, visibleSize.height - 30);
	grain->setZOrder(11);
	this->addChild(grain);

	spriteBarArray[0]->setPosition(0, 0);

	spriteBarArray[1]->setRotation(90);
	spriteBarArray[1]->setPosition(0, visibleSize.height);

	spriteBarArray[2]->setRotation(270);
	spriteBarArray[2]->setPosition(visibleSize.width, 0);

	spritecornerArray[0]->setPosition(0, 0);
	spritecornerArray[1]->setPosition(visibleSize.width-30, 0);
	spritecornerArray[2]->setPosition(0, visibleSize.height-30);
	spritecornerArray[3]->setPosition(visibleSize.width-30, visibleSize.height - 30);

	Sprite* UIBar = Sprite::createWithSpriteFrameName("ui_bar.png");
	UIBar->setAnchorPoint(Vec2(0, 0));
	UIBar->setPosition(0, visibleSize.height-70);
	this->addChild(UIBar);
}

void UILayer::SelectCharacter(Character* character)
{
	if (this->getChildByName("indicator"))
	{
		this->removeChildByName("indicator");
	}

	if (character)
	{
		float posX = character->getPositionX();
		float posY = character->getPositionY();

		Sprite* indicator = Sprite::createWithSpriteFrameName("indicator.png");
		indicator->setName("indicator");
		indicator->setAnchorPoint(Vec2(25, 0));
		indicator->setPosition(posX, posY + 120);

		this->addChild(indicator);
	}
}