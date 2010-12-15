// ======================================================================================
// File         : GameWorldService.h
// Author       : Zhang yuzhong 
// Last Change  : 12/2/2010
// Description  : 网络消息处理
//				  
// ======================================================================================

#pragma once
#include <efd\ISystemService.h>
#include <NiRefObject.h>

#include "IClientNetInterface.h"
#include "demoproto.h"


class NetWorkService : 	public efd::ISystemService, public IConnectorSink
{
	EE_DECLARE_CLASS1(NetWorkService, game::SERVICE_ID_NetWorkService, ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	NetWorkService(void);
	virtual ~NetWorkService(void);

	virtual const char *GetDisplayName() const;

protected:
	// 声明OnInit（）需求的服务 
	virtual efd::SyncResult OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);

	// 
	virtual efd::AsyncResult OnInit();

	// 
	virtual efd::AsyncResult OnShutdown();

	virtual efd::AsyncResult OnTick();

	// 网络消息
	bool OnError(INT32 iModuleError, INT32 iSysError);
	bool OnConnect(INT32 iModuleError, INT32 iSysError);
	bool OnDisConnect(INT32 iModuleError, INT32 iSysError);
	bool OnRecv(const char* pszBuff, UINT32 dwBufLen);

public:
	void Send(UINT16 wMsgID, void *pData, INT32 nLen);

public:
	static const DWORD NET_BUF_SIZE;

	typedef void (NetWorkService::*MsgNotifyPtr)(char*, int);
	typedef	efd::hash_map<int, MsgNotifyPtr>	MsgHandlerMap;

	MsgHandlerMap  m_mapMsgHandler; 

	void RegisterNetEventFunc();

	void GetCharData(char* pMsg, int len);
	void GetSceneNtf(char* pMsg, int len);
	void LoadScene(char* pMsg, int len);
	void EnterGame(char* pMsg, int len);

private:
	struct _NETDATA : public NiRefObject
	{
		int		nLen;
		char*	pchData;
		_NETDATA()
		{
			nLen = 0;
			pchData = NULL;
		}
		_NETDATA(int nNet,const char* pchNet)
		{
			nLen = nNet;
			pchData = NiAlloc(char,nLen);
			NiMemcpy(pchData,pchNet,nLen);
		}
		~_NETDATA()
		{
			nLen = 0;
			if(pchData)
			{
				NiFree(pchData);
				pchData = NULL;
			}
		}
	};
	typedef efd::SmartPointer<_NETDATA>		_NETDATAPTR;
	typedef	efd::list<_NETDATAPTR>			VTDATA;

	VTDATA	m_vtRecv;
	char*   m_pNetBuf;

private:
	CDemoProto m_demoProto;
};

typedef efd::SmartPointer<NetWorkService> NetWorkServicePtr;