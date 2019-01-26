#pragma once

#include <iostream>
#include "IMemoryChunk.h"
#ifdef KOJEOMLIB_MEMORYPOOL_EXPORTS  
#define KOJEOMLIB_MEMORYPOOL_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_MEMORYPOOL_API __declspec(dllimport)   
#endif 

/// reference document link : https://www.codeproject.com/Articles/15527/C-Memory-Pool

namespace KojeomlibMemoryPool {

// �޸𸮰� �����Ǹ� 0xAA������ �ʱ�ȭ��Ų��.
#define RELEASED_MEMORY_VALUE 0xAA
// �޸𸮰� �Ҵ�� ���� 0xFF������ �ʱ�ȭ ��Ų��.
#define NEW_MEMORY_VALUE 0xFF
#define DEFAULT_BLOCK_SIZE_BYTE 100

	typedef unsigned char MemoryUnit;
	struct MemoryBlock {
	public:
		bool isUsing;
		unsigned int idx;
		unsigned int dataByteLength;
		unsigned int usingDataByte;
		void* data;
		MemoryBlock* nextBlock;
	};

	// Non thread safe Memory pool.
	class KojeomMemoryPool : public IMemoryChunk {
	public:
		// Inherited via IMemoryChunk
		virtual void* GetMemory(unsigned int memorySizeByte) override;
		virtual void FreeMemory(void * memroyBlock) override;
		KojeomMemoryPool(unsigned int poolByteSize);
		~KojeomMemoryPool();
	private:
		unsigned int memoryPoolSize;
		unsigned int memBlockTotalCount;
		MemoryBlock* head;
		MemoryBlock* tail;
	private:
		void MoreAllocateMemory();
		MemoryBlock* FindBestFitBlock(unsigned int memorySizeByte);
	};
}