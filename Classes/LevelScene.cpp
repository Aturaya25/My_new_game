/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "LevelScene.h"
#include "Player.h"
#include "Ball.h"
#include "GameBlock.h"
#include "BlockManager.h"

USING_NS_CC;

Scene* Level::createScene()
{
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, 0.0f));

	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

	auto layer = Level::create();

	scene->addChild(layer);

	return scene;
}

bool Level::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	_start = false;
	_reset = false;
	_isFirstScreen = true;
	_firstBoost = false;
	_secondBoost = false;
	_orangeHit = false;
	_redHit = false;
	_halvedPlayer = false;
	_blockHit = false;

	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize * 2, Color4F(0.6f, 0.6f, 0.6f, 1.0f));
	this->addChild(background);

	spawnLabels(origin, winSize);

	_player = Player::create();
	_player->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.4f));
	this->addChild(_player);

	_blocks = new BlockManager();
	spawnBlocks();

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(Level::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();

    return true;
}

void Level::update(float dt)
{
	if (_start)
	{
		if (_ball->isOutOfBounds())
		{
			_lives -= 1;
			updateLabelText(_livesLabel, "Lives: ", _lives);
			resetModifiers();
			
			if (_lives)
			{
				_ball->setup();
			}
		}

		if (!_lives)
		{
			displayResultLabels(false);
		}
		else
		{
			if (_blockCollisions == 112)
			{
				if (_isFirstScreen)
				{
					_blocks->revealBlocks();
					_isFirstScreen = false;
				}
				else
				{
					displayResultLabels(true);
				}

				_blockCollisions = 0;
			}
		}
	}
}

void Level::onMouseUp(Event* event) 
{
	if (_start)
	{
		return;
	}

	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

	if (!mouseEvent)
	{
		log("Could not start game due to bad cast");
		return;
	}
	if (EventMouse::MouseButton::BUTTON_LEFT == mouseEvent->getMouseButton() || EventMouse::MouseButton::BUTTON_RIGHT == mouseEvent->getMouseButton())
	{
		_score = 0;
		_lives = 3;
		_start = true;

		if (_reset)
		{
			_blocks->revealBlocks();
			resetModifiers();
			updateLabelText(_scoreLabel, "Score: ", _score);
			updateLabelText(_livesLabel, "Lives: ", _lives);
			_reset = false;
			_isFirstScreen = true;
			_blockCollisions = 0;
		}

		if (!_ball)
		{
			_ball = Ball::create();
			this->addChild(_ball);
		}
		_ball->setup();

		if (_startLabel->isVisible())
		{
			_startLabel->setVisible(false);
		}

		if (_resultLabel->isVisible())
		{
			_resultLabel->setVisible(false);
		}
	}
}

bool Level::onContactBegin(PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
	{
		if (a->getTag() == 1)
		{
			if (b->getTag() == 0)
			{
				_ball->handlePlayerCollision(_player);

				_blockHit = false;
			}

			if (b->getTag() >= 2 && !_blockHit)
			{
				_ball->handleBlockCollision();
				
				_bodiesHit.push_back(b);
				scheduleOnce(schedule_selector(Level::updateBlockHit), 0.1f);

				b->getOwner()->setVisible(false);
				b->setEnabled(false);

				_blockHit = true;
			}

			return true;
		}
	}

	return false;
}

void Level::updateBlockHit(float dt)
{
	if (!_bodiesHit.size())
	{
		return;
	}

	while (_bodiesHit.size())
	{
		const PhysicsBody* block = _bodiesHit.back();
		_bodiesHit.pop_back();

		_score += (block->getTag() - 1);

		_blockCollisions++;
		_activeCollisions++;

		updateLabelText(_scoreLabel, "Score: ", _score);

		checkBallModifiers(block->getTag());
		checkPlayerModifiers();
	}
}

bool Level::spawnLabels(const Vec2& origin, const Size& winSize)
{
	_titleLabel = Label::createWithTTF("Breakout Demo", "fonts/Marker Felt.ttf", 24);
	if (!_titleLabel)
	{
		return false;
	}

	_titleLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height - _titleLabel->getContentSize().height));

	_scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	if (!_scoreLabel)
	{
		return false;
	}

	_scoreLabel->setPosition(Vec2(origin.x + winSize.width * 0.25f,
		origin.y + winSize.height - _scoreLabel->getContentSize().height - 100.0f));

	_livesLabel = Label::createWithTTF("Lives: 3", "fonts/Marker Felt.ttf", 24);
	if (!_livesLabel)
	{
		return false;
	}

	_livesLabel->setPosition(Vec2(origin.x + winSize.width * 0.75f,
		origin.y + winSize.height - _livesLabel->getContentSize().height - 100.0f));

	_resultLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (!_resultLabel)
	{
		return false;
	}

	_resultLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height / 3 - _resultLabel->getContentSize().height));

	_resultLabel->setVisible(false);

	_startLabel = Label::createWithTTF("Click to start", "fonts/Marker Felt.ttf", 24);
	if (!_startLabel)
	{
		return false;
	}

	_startLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height / 3 - (_resultLabel->getContentSize().height + _startLabel->getContentSize().height)));

	this->addChild(_titleLabel, 1);
	this->addChild(_scoreLabel, 1);
	this->addChild(_livesLabel, 1);
	this->addChild(_resultLabel, 1);
	this->addChild(_startLabel, 1);

	return true;
}

bool Level::spawnBlocks() 
{
	if (!_blocks->spawnBlocks(this))
	{
		log("we have failed to spawn the blocks");
		return false;
	}

	return true;
}

void Level::checkBallModifiers(int tag)
{
	if (!_redHit || !_orangeHit)
	{
		if (tag == 8)
		{
			_redHit = true;
		}

		if (tag == 6)
		{
			_orangeHit = true;
		}

		if (_redHit && _orangeHit)
		{
			_ball->increaseSpeed();
		}
	}

	if (!_firstBoost)
	{
		if (_activeCollisions == 4)
		{
			_firstBoost = _ball->increaseSpeed();
		}
	}

	if (!_secondBoost) {
		if (_activeCollisions == 12)
		{
			_secondBoost = _ball->increaseSpeed();
		}
	}
}

void Level::checkPlayerModifiers()
{

	if (!_halvedPlayer)
	{
		if (_redHit && _ball->hasTouchedTop())
		{
			_halvedPlayer = _player->scale();
		}
	}
}

void Level::resetModifiers()
{
	_firstBoost = false;
	_secondBoost = false;
	_orangeHit = false;
	_redHit = false;
	_halvedPlayer = false;

	_player->scale(true);

	_activeCollisions = 0;
}

void Level::updateLabelText(Label* label, std::string text, int value)
{
	text += std::to_string(value);
	label->setString(text);
}

void Level::displayResultLabels(bool didWin)
{
	std::string resultText;
	if (didWin)
	{
		resultText = "Congratulations! You won!";
	}
	else
	{
		resultText = "Game Over";
	}

	std::string startText = "Click to play again.";

	_startLabel->setString(startText);
	_resultLabel->setString(resultText);

	_startLabel->setVisible(true);
	_resultLabel->setVisible(true);

	_ball->setVisible(false);

	_start = false;
	_reset = true;
}
