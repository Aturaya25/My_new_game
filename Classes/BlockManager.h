#ifndef __BLOCKMANAGER_H__
#define __BLOCKMANAGER_H__

#include "cocos2d.h"

class GameBlock;

class BlockManager
{
private:
	std::vector<std::vector<GameBlock*>> _blocks;
public:
	BlockManager();
	~BlockManager();

	inline const int getSize() { int size = 0; for (const std::vector<GameBlock*> vector : _blocks) { size += vector.size(); }  return size; }
	const std::vector<std::vector<GameBlock*>> getBlocks();

	bool spawnBlocks(cocos2d::Scene* parent);
	void revealBlocks();
};

#endif __BLOCKMANAGER_H__
