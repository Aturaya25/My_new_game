#include "Player.h"
#include "Ball.h"

USING_NS_CC;

bool Player::init()
{
	if (!Sprite::initWithFile("player.png"))
	{
		return false;
	}

	_size = getContentSize();
	_speed = 600.0f;

	auto body = PhysicsBody::createBox(_size);
	body->setDynamic(false);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(true);
	body->setTag(0);
	setPhysicsBody(body);

	auto kbListener = EventListenerKeyboard::create();
	kbListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	kbListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kbListener, this);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	_leftSide = origin.x;
	_rightSide = origin.x + winSize.width;

	scheduleUpdate();

	_moving = false;
	_pause = false;

	return true;
}

void Player::update(float dt) 
{
	if (_moving)
	{
		move(dt);
	}
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			_direction = 0;
			_moving = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			_direction = 1;
			_moving = true;
			break;
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
		default:
			_moving = false;
			break;
	}
}

bool Player::move(float dt)
{
	if (!_moving)
	{
		return false;
	}

	Vec2 currentPosition = getPosition();

	if (_direction)
	{
		currentPosition.x = clampf(currentPosition.x + _speed * dt, _leftSide, _rightSide);
	}
	else
	{
		currentPosition.x = clampf(currentPosition.x + -_speed * dt, _leftSide, _rightSide);
	}

	setPosition(currentPosition);

	return true;
}


bool Player::scale(bool full)
{
	if (full)
	{
		setContentSize(_size);
	}
	else
	{
		setContentSize(_size / 2);
	}
	return true;
}
