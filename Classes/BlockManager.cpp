#include "BlockManager.h"
#include "GameBlock.h"

USING_NS_CC;

BlockManager::BlockManager()
{
	_blocks = std::vector<std::vector<GameBlock*>>(8);
	for (int i = 0; i < 8; i++) {
		_blocks[i] = std::vector<GameBlock*>(14);
	}
}

BlockManager::~BlockManager()
{

}

bool BlockManager::spawnBlocks(Scene* parent)
{
	if (!parent)
	{
		return false;
	}

	float startingX = 40.0f;
	float startingY = 800.0f;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 14; j++) {
			GameBlock* block = _blocks[i][j];

			if (block)
			{
				continue;
			}

			Vec2 location = Vec2(startingX + (60.0f * j), startingY + (-20.0f * i));

			if (i < 2) {
				block = RedBlock::create();
			}
			else if (i >= 2 && i < 4) {
				block = OrangeBlock::create();
			}
			else if (i >= 2 && i < 6) {
				block = GreenBlock::create();
			}
			else {
				block = YellowBlock::create();
			}
			if (!block)
			{
				return false;
			}
			block->setPosition(location);
			block->setIndex(Vec2(i, j));
			_blocks[i][j] = block;
			parent->addChild(block);
		}
	}

	if (!_blocks.size())
	{
		return false;
	}

	return true;
}

const std::vector<std::vector<GameBlock*>> BlockManager::getBlocks()
{
	return _blocks;
}

void BlockManager::revealBlocks()
{
	for(const std::vector<GameBlock*> blockRow : _blocks)
	{
		for (GameBlock* block : blockRow)
		{
			block->setVisible(true);
			block->getPhysicsBody()->setEnabled(true);
		}
	}
}
