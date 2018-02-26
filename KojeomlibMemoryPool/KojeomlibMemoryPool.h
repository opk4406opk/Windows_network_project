#pragma once

#include <iostream>
#include "IMemoryChunk.h"
#ifdef KOJEOMLIB_MEMORYPOOL_EXPORTS  
#define KOJEOMLIB_MEMORYPOOL_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_MEMORYPOOL_API __declspec(dllimport)   
#endif 

namespace KojeomlibMemoryPool {
	
	struct MemoryBlock {
	public:
		unsigned int dataByteLength;
		void* data;
		MemoryBlock* nextBlock;
	};

	class KojeomMemoryPool : public IMemoryChunk {
	public:
		// Inherited via IMemoryChunk
		virtual void* GetMemory(unsigned int memorySize) override;
		virtual void FreeMemory(void * memroyBlock) override;
		KojeomMemoryPool(unsigned int poolSize);
		~KojeomMemoryPool();
	private:
		unsigned int memoryPoolSize;
		unsigned int memBlockCount;
		MemoryBlock* memBlockHead;
	};
}