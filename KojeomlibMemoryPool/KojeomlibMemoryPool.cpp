// KojeomlibMemoryPool.cpp : Defines the exported functions for the DLL application.
//
#include "KojeomlibMemoryPool.h"


void * KojeomlibMemoryPool::KojeomMemoryPool::GetMemory(unsigned int memorySize) {
	return nullptr;
}

void KojeomlibMemoryPool::KojeomMemoryPool::FreeMemory(void * memroyBlock) {
}

KojeomlibMemoryPool::KojeomMemoryPool::KojeomMemoryPool(unsigned int poolByteSize) {
	memBlockHead = nullptr;
	memoryPoolSize = poolByteSize;
	memBlockCount = memoryPoolSize / DEFAULT_BLOCK_SIZE_BYTE;
	
	MemoryBlock* traverse = nullptr;
	for (unsigned int idx = 0; idx < memBlockCount; idx++) {
		MemoryBlock* blk = new MemoryBlock();
		blk->idx = idx;
		blk->data = (void*)malloc(sizeof(MemoryByte) * DEFAULT_BLOCK_SIZE_BYTE);
		blk->dataByteLength = DEFAULT_BLOCK_SIZE_BYTE;
		blk->usedDataByte = 0;
		blk->nextBlock = nullptr;
		if (memBlockHead == nullptr) {
			memBlockHead = blk;
			traverse = memBlockHead;
		} else {
			traverse->nextBlock = blk;
			traverse = blk;
		}
	}
}

KojeomlibMemoryPool::KojeomMemoryPool::~KojeomMemoryPool() {
}
