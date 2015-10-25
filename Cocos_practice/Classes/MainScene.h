#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__


USING_NS_CC;
//using namespace cocos2d

class MainScene : public Layer
{
private:
	void	menuClickCallback(Ref* pSender);
	void	menuCloseCallback(Ref* pSender);

	void	onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void	onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

public:
	//다른 화면에서 해당 화면으로 전환할 때 등에 사용되는 전역 메소드
    static Scene* createScene();

	//Layer를 상속받은 클래스가 생성될 때 생성자 메소드 다음으로 가장 먼저 호출되는 메소드
    virtual bool init();

	//해당 클래스를 생성할 때 new 명령어가 아닌 코코스2d-x에서 제공하는 create()메소드를 사용할 수 있게 해주는 전역 메소드(매크로?)
	CREATE_FUNC(MainScene);
};
#endif
