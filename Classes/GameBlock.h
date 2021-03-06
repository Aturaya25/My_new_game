#ifndef __GAMEBLOCK_H__
#define __GAMEBLOCK_H__

#include "cocos2d.h"

class GameBlock : public cocos2d::Sprite
{
protected:
	int value;
	cocos2d::Vec2 index;

	bool setupPhysics();
public:
	CREATE_FUNC(GameBlock);
	bool init() override;
	virtual bool init(std::string filename);
	inline const int getValue() {return value;}

	inline void setIndex(const cocos2d::Vec2& newIndex) { index = newIndex; }
	inline const cocos2d::Vec2 getIndex() { return index; }
};

class RedBlock : public GameBlock
{
public:
	CREATE_FUNC(RedBlock);
	bool init() override;
};

class OrangeBlock : public GameBlock
{
public:
	CREATE_FUNC(OrangeBlock);
	bool init() override;
};

class GreenBlock : public GameBlock
{
public:
	CREATE_FUNC(GreenBlock);
	bool init() override;
};

class YellowBlock : public GameBlock
{
public:
	CREATE_FUNC(YellowBlock);
	bool init() override;
};


#endif // __GAMEBLOCK_H__
