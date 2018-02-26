#pragma once
class IMemoryChunk {
public:
	IMemoryChunk();
	virtual ~IMemoryChunk();
	virtual void* GetMemory(unsigned int memorySize) = 0;
	virtual void FreeMemory(void* memroyBlock) = 0;
};

