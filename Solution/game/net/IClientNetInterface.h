#ifndef _ICLIENT_NET_INTERFACE_H_
#define _ICLIENT_NET_INTERFACE_H_

#include "sdtype.h"
#define MAX_NET_BUFF	1024*64

#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")

class IPacketParser
{
public:
	virtual INT32 ParsePacket(const char* pBuf, UINT32 dwLen) = 0;
};

class IConnectorSink
{
public:
	virtual bool OnError(INT32 iModuleError, INT32 iSysError) = 0;
	virtual bool OnConnect(INT32 iModuleError, INT32 iSysError) = 0;
	virtual bool OnDisConnect(INT32 iModuleError, INT32 iSysError) = 0;
	virtual bool OnRecv(const char* pszBuff, UINT32 dwBufLen) = 0;
};

class IConnector
{
public:
	virtual bool Connect(const char* pszIP, UINT16 wPort) = 0;
	virtual void DisConnect() = 0;
	virtual void Release() = 0;

	virtual INT32 Send(const char* pszBuff, INT32 iLens) = 0;
	
	virtual const char* GetLocolIP() = 0;
	virtual UINT16 GetLocolPort() = 0;
	
	virtual const char* GetRemoteIP() = 0;
	virtual UINT16 GetRemotePort() = 0;
};

class IClientNetModule
{
public:
	virtual IConnector* CreateConnector(IConnectorSink* poSink, IPacketParser* poPacketParser) = 0;
	virtual bool Run() = 0;
	virtual void Release() = 0;
};

IClientNetModule* CreateNetModule();

#endif