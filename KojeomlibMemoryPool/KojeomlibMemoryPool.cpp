// KojeomlibMemoryPool.cpp : Defines the exported functions for the DLL application.
//

#include "KojeomlibMemoryPool.h"
#include "stdafx.h"

void * KojeomlibMemoryPool::KojeomMemoryPool::GetMemory(unsigned int memorySize) {
	return nullptr;
}

void KojeomlibMemoryPool::KojeomMemoryPool::FreeMemory(void * memroyBlock) {
}

KojeomlibMemoryPool::KojeomMemoryPool::KojeomMemoryPool(unsigned int poolSize) {
	memoryPoolSize = poolSize;

}

KojeomlibMemoryPool::KojeomMemoryPool::~KojeomMemoryPool() {
}
