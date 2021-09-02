#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Ball;

class Player : public cocos2d::Sprite
{
private:
	cocos2d::Size _size;

	bool _moving;
	bool _pause;

	float _leftSide;
	float _rightSide;

	int _direction;
	float _speed;

	bool move(float dt);
protected:
	void update(float dt) override;
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
public:
	CREATE_FUNC(Player);
	bool init() override;
	bool scale(bool full = false);
};

#endif // __PLAYER_H__
