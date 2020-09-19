#include "MainScene.h"


bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//get winsize
	const auto size = Director::getInstance()->getWinSize();
	//create the background
	auto bgSprite = Sprite::create("background.png");
	bgSprite->setScale(2.5, 2);
	bgSprite->setPosition(size / 2);
	this->addChild(bgSprite);
	//create the ground
	auto groungSprite = Sprite::create("ground.png");
	groungSprite->setContentSize(Size(1280, 256));
	groungSprite->setPosition(Vec2(size.width / 2, 128));
	this->addChild(groungSprite);
	//create the initpos of player
	player = Sprite::create("PurpleMonster.png");
	player->setAnchorPoint(Vec2(0.5, 0));
	player->setPosition(Vec2(size.width / 2, 256));
	this->addChild(player);
	//add score Label
	std::string scoreString = "Score:";
	scoreString.append(std::to_string(score));
	scoreLabel = Label::createWithBMFont("mikado_outline_shadow.fnt", scoreString);
	scoreLabel->setPosition(Vec2(size.width / 2, size.height / 2 + 200));
	this->addChild(scoreLabel);
	//init the action of player
	auto moveby = MoveBy::create(0.6, Vec2(0, 200));
	auto sequence = Sequence::create(moveby, moveby->reverse(), CallFunc::create(CC_CALLBACK_0(MainScene::playMusic, this)), nullptr);
	auto repeatAction = RepeatForever::create(sequence);
	player->runAction(repeatAction);
	//add KeyBoardEvent
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [&](EventKeyboard::KeyCode keycode, Event* event)
	{
		keyMap[keycode] = true;
	};
	listener->onKeyReleased = [&](EventKeyboard::KeyCode keycode, Event* event)
	{
		keyMap[keycode] = false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, player);
	star = Sprite::create("star.png");
	const auto pos_x = random() % 1280;
	const auto pos_y = random() % 268 + 256;
	star->setPosition(Vec2(pos_x, pos_y));
	this->addChild(star);
	auto fadeout = FadeOut::create(5);
	star->runAction(fadeout);
	//add scheduler
	schedule(schedule_selector(MainScene::playerMove));
	schedule(CC_SCHEDULE_SELECTOR(MainScene::starArise), 5);
	schedule(CC_SCHEDULE_SELECTOR(MainScene::detectScore));
	return true;
}

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

void MainScene::playMusic()
{
	AudioEngine::play2d("audio/jump.mp3");
}

void MainScene::removeScoringLabel(Node* node)
{
	this->removeChild(node);
}

void MainScene::playerMove(float dt)
{
	const auto left = EventKeyboard::KeyCode::KEY_A;
	const auto right = EventKeyboard::KeyCode::KEY_D;
	if (keyMap[left])
	{
		this->speed -= 0.05;
	}
	if (keyMap[right])
	{
		this->speed += 0.05;
	}
	// 限制主角的速度不能超过最大值
	if (abs(this->speed) > this->maxMoveSpeed) {
		this->speed = this->maxMoveSpeed * this->speed / abs(this->speed);
	}
	player->setPositionX(player->getPositionX() + this->speed);
}

void MainScene::starArise(float dt)
{
	if (!star->getOpacity())
	{
		star->setOpacity(255);
		const auto pos_x = random() % 1280;
		const auto pos_y = random() % 268 + 256;
		star->setPosition(Vec2(pos_x, pos_y));
		auto fadeout = FadeOut::create(5);
		star->runAction(fadeout);
	}
}

void MainScene::detectScore(float dt)
{
	if (player->getBoundingBox().intersectsRect(star->getBoundingBox()))
	{
		AudioEngine::play2d("audio/score.mp3", false);
		auto scoringLabel = Label::createWithBMFont("mikado_outline_shadow.fnt", "+1");
		scoringLabel->setPosition(star->getPosition());
		this->addChild(scoringLabel);
		score += 100;
		auto sequence = Sequence::create(DelayTime::create(0.5), CallFuncN::create(CC_CALLBACK_1(MainScene::removeScoringLabel, this)), nullptr);
		scoringLabel->runAction(sequence);
		star->stopAllActions();
		star->setOpacity(255);
		const auto pos_x = random() % 1280;
		const auto pos_y = random() % 268 + 256;
		star->setPosition(Vec2(pos_x, pos_y));
		auto fadeout = FadeOut::create(5);
		star->runAction(fadeout);
	}
	std::string scoreString = "Score:";
	scoreString.append(std::to_string(score));
	scoreLabel->setString(scoreString);
}
