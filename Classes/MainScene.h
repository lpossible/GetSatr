#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_
#include"cocos2d.h"
#include"AudioEngine.h";
using namespace cocos2d::experimental;
USING_NS_CC;
class MainScene:public Layer
{
public:
	bool init()override;
	static Scene* createScene();
	CREATE_FUNC(MainScene);
	void playMusic();
	void removeScoringLabel(Node* node);
	void playerMove(float dt);
	void starArise(float dt);
	void detectScore(float dt);
private:
	Sprite* player;
	std::map<EventKeyboard::KeyCode, bool> keyMap;
	float speed=0;
	float maxMoveSpeed = 3;
	Sprite* star;
	int score = 0;
	Label* scoreLabel;
};
#endif // !MAIN_SCENE_H_

