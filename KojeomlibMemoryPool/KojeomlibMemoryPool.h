#pragma once

#include <iostream>
#include "IMemoryChunk.h"
#ifdef KOJEOMLIB_MEMORYPOOL_EXPORTS  
#define KOJEOMLIB_MEMORYPOOL_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_MEMORYPOOL_API __declspec(dllimport)   
#endif 

// reference document link : https://www.codeproject.com/Articles/15527/C-Memory-Pool

namespace KojeomlibMemoryPool {

// �޸𸮰� �����Ǹ� 0xAA������ �ʱ�ȭ��Ų��.
#define RELEASED_MEMORY_VALUE 0xAA
// �޸𸮰� �Ҵ�� ���� 0xFF������ �ʱ�ȭ ��Ų��.
#define NEW_MEMORY_VALUE 0xFF
#define DEFAULT_BLOCK_SIZE_BYTE 100
	typedef unsigned char MemoryByte;
	struct MemoryBlock {
	public:
		unsigned int idx;
		unsigned int dataByteLength;
		unsigned int usedDataByte;
		void* data;
		MemoryBlock* nextBlock;
	};

	class KojeomMemoryPool : public IMemoryChunk {
	public:
		// Inherited via IMemoryChunk
		virtual void* GetMemory(unsigned int memorySize) override;
		virtual void FreeMemory(void * memroyBlock) override;
		KojeomMemoryPool(unsigned int poolByteSize);
		~KojeomMemoryPool();
	private:
		unsigned int memoryPoolSize;
		unsigned int memBlockTotalCount;
		MemoryBlock* memBlockHead;
	};
}