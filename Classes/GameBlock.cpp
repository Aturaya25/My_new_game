#include "GameBlock.h"

USING_NS_CC;

bool GameBlock::setupPhysics()
{
	auto body = PhysicsBody::createBox(getContentSize());

	if (!body)
	{
		return false;
	}

	body->setDynamic(false);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(1);
	setPhysicsBody(body);

	return true;
}

bool GameBlock::init() 
{
	return true;
}

bool GameBlock::init(std::string filename)
{
	if (filename.empty())
	{
		filename = "block.png";
	}

	if (!Sprite::initWithFile(filename)) 
	{
		return false;
	}

	setupPhysics();

	return true;
}

bool RedBlock::init()
{
	if (!GameBlock::init("redscore.png"))
	{
		return false;
	}

	value = 7;
	getPhysicsBody()->setTag(8);

	return true;
}

bool OrangeBlock::init()
{
	if (!GameBlock::init("orangescore.png"))
	{
		return false;
	}

	value = 5;
	getPhysicsBody()->setTag(6);

	return true;
}

bool GreenBlock::init()
{
	if (!GameBlock::init("greenscore.png"))
	{
		return false;
	}

	value = 3;
	getPhysicsBody()->setTag(4);

	return true;
}

bool YellowBlock::init()
{
	if (!GameBlock::init("yellowscore.png"))
	{
		return false;
	}

	value = 1;
	getPhysicsBody()->setTag(2);

	return true;
}
