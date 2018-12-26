#pragma once
class IServer
{
public:
	IServer() {};
	virtual ~IServer() {};
	virtual void Init() = 0;
	virtual void Start() = 0;
};

