#include "Ball.h"

USING_NS_CC;

bool Ball::init()
{
	if (!Sprite::initWithFile("Ball.png"))
	{
		return false;
	}

	auto body = PhysicsBody::createCircle(getContentSize().width / 2);
	body->setDynamic(true);
	body->setCollisionBitmask(2);
	body->setContactTestBitmask(true);
	body->setTag(1);
	setPhysicsBody(body);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	_moving = false;
	_startPosition = Vec2(winSize.width * 0.5f, winSize.height * 0.6f);

	_radius = getContentSize().width / 2;

	_leftSide = origin.x;
	_rightSide = origin.x + winSize.width;
	_topSide = origin.y + winSize.height;
	_bottomSide = origin.y;

	scheduleUpdate();

	return true;
}

void Ball::update(float dt)
{
	if (_moving)
	{
		checkBounds();
		_currentPosition = Vec2(clampf(_currentPosition.x + (_magnitude * _velocity.x * dt), _leftSide, _rightSide), 
			clampf(_currentPosition.y + (_magnitude * _velocity.y * dt), _bottomSide, _topSide));
		setPosition(_currentPosition);
	}
}

void Ball::setup()
{
	if (!isVisible())
	{
		setVisible(true);
	}

	_currentPosition = _startPosition;
	setPosition(_currentPosition);

	_hitTop = false;
	_outOfBounds = false;
	_magnitude = 200.0f;

	if (std::round(rand_0_1()))
	{
		_velocity = Vec2(1.0f, -1.0f);
	}
	else
	{
		_velocity = Vec2(-1.0f, -1.0f);
	}
	_moving = true;
}

void const Ball::clampVelocityX(float adjust)
{
	_velocity.x = clampf(_velocity.x * adjust, -1.0f, 1.0f);
}

void Ball::handlePlayerCollision(const cocos2d::Sprite* player)
{
	const cocos2d::Size halfWidth = player->getBoundingBox().size;
	const cocos2d::Vec2 playerPos = player->convertToWorldSpace(player->getPosition());

	const float playerLeft = playerPos.x + halfWidth.width / 3;
	const float playerRight = playerPos.x + (halfWidth.width / 3) * 2;

	const cocos2d::Size ballSize = getBoundingBox().size;
	const cocos2d::Vec2 ballPos = convertToWorldSpace(getPosition());

	const float ballCenter = ballPos.x + ballSize.width / 2;

	if (ballCenter > playerLeft && ballCenter < playerRight)
	{
		clampVelocityX(1.0f);
	}
	else if (ballCenter < playerLeft || ballCenter > playerRight)
	{
		if (ballCenter < playerLeft)
		{
			if (_velocity.x < 0.0f)
			{
				clampVelocityX(0.5f);
			}
			else
			{
				clampVelocityX(-2.0f);
			}
		}
		else
		{
			if (_velocity.x < 0.0f)
			{
				clampVelocityX(-2.0f);
			}
			else
			{
				clampVelocityX(0.5f);
			}
		}
	}

	_velocity.y *= -1.0f;
}

void Ball::handleBlockCollision()
{
	clampVelocityX(-1.0f);
	_velocity.y *= -1.0f;
}

bool Ball::checkBounds() {
	if (_currentPosition.x >= _rightSide || _currentPosition.x <= _leftSide)
	{
		clampVelocityX(-1.0f);
	}
	else if (_topSide <= _currentPosition.y)
	{
		_velocity.y *= -1.0f;
		if (!_hitTop)
		{
			_hitTop = true;
		}
	}
	else if (_bottomSide >= _currentPosition.y)
	{
		_outOfBounds = true;
		return (_moving = false);
	}

	return true;
}
