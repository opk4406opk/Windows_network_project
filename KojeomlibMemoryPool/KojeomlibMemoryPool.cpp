// KojeomlibMemoryPool.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "KojeomlibMemoryPool.h"
#include <cmath>
using namespace KojeomlibMemoryPool;

void * KojeomMemoryPool::GetMemory(unsigned int memorySizeByte) {
	
	MemoryBlock* traverse = head;
	while (traverse != nullptr){
		if (traverse->isUsing == false){
			if (traverse->dataByteLength < memorySizeByte){
				MemoryBlock* fitMemory = FindBestFitBlock(memorySizeByte);
				if (fitMemory != nullptr) {
					return fitMemory->data;
				} else {
					// 알맞은 메모리블록이 없다면 추가로 블록을 요청한 뒤 이를 return.
					// to do.
				}
			} else if(traverse->dataByteLength == memorySizeByte) {
				traverse->isUsing = true;
				traverse->usingDataByte = memorySizeByte;
				return traverse->data;
			}
		}
		traverse = traverse->nextBlock;
	}
	return nullptr;
}

void KojeomMemoryPool::FreeMemory(void * memroyBlock) {
}

KojeomMemoryPool::KojeomMemoryPool(unsigned int poolByteSize) {
	head = nullptr;
	memoryPoolSize = poolByteSize;
	memBlockTotalCount = memoryPoolSize / DEFAULT_BLOCK_SIZE_BYTE;
	
	MemoryBlock* traverse = nullptr;
	for (unsigned int idx = 0; idx < memBlockTotalCount; idx++) {
		MemoryBlock* blk = new MemoryBlock();
		blk->idx = idx;
		blk->data = (void*)malloc(sizeof(MemoryUnit) * DEFAULT_BLOCK_SIZE_BYTE);
		memset(blk->data, NEW_MEMORY_VALUE, DEFAULT_BLOCK_SIZE_BYTE);
		blk->dataByteLength = DEFAULT_BLOCK_SIZE_BYTE;
		blk->usingDataByte = 0;
		blk->isUsing = false;
		blk->nextBlock = nullptr;
		if (head == nullptr) {
			head = blk;
			traverse = head;
		} else {
			traverse->nextBlock = blk;
			traverse = blk;
		}
	}
	tail = traverse;
}

KojeomMemoryPool::~KojeomMemoryPool() {
	MemoryBlock* traverse = head;
	while (traverse != nullptr) {
		free(traverse->data);
		MemoryBlock* next = traverse->nextBlock;
		delete traverse;
		traverse = next;
	}
}

void KojeomMemoryPool::MoreAllocateMemory(){
	MemoryBlock* blk = new MemoryBlock();
	blk->idx = tail->idx + 1;
	blk->data = (void*)malloc(sizeof(MemoryUnit) * DEFAULT_BLOCK_SIZE_BYTE);
	memset(blk->data, NEW_MEMORY_VALUE, DEFAULT_BLOCK_SIZE_BYTE);
	blk->dataByteLength = DEFAULT_BLOCK_SIZE_BYTE;
	blk->usingDataByte = 0;
	blk->isUsing = false;
	blk->nextBlock = nullptr;
	//
	tail->nextBlock = blk;
	tail = blk;
}

MemoryBlock * KojeomMemoryPool::FindBestFitBlock(unsigned int memorySizeByte){
	float rate = (float)memorySizeByte / (float)DEFAULT_BLOCK_SIZE_BYTE;
	int needBlockCount = ceil(rate);
	int countSeqeunce = 0;
	//
	MemoryBlock* traverse = head;
	while (traverse != nullptr) {
		if (traverse->isUsing == false) {
			traverse->isUsing = true;
			traverse->usingDataByte = DEFAULT_BLOCK_SIZE_BYTE;
			countSeqeunce++;
			// 연속된 메모리블럭들이 필요한 블럭 수와 동일하면.
			if (countSeqeunce == needBlockCount) {
				traverse->usingDataByte = ((float)needBlockCount - rate) * DEFAULT_BLOCK_SIZE_BYTE;
				return traverse;
			}
		} else {
			countSeqeunce = 0;
		}
		traverse = traverse->nextBlock;
	}

	// initialize.
	traverse = head;
	while (traverse != nullptr) {
		traverse->isUsing = false;
		traverse->usingDataByte = 0;
		traverse = traverse->nextBlock;
	}
	return nullptr;
}
