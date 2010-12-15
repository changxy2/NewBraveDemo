#include "game_pch.h"
#include "NetWorkService.h"
#include "sdpkg.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(NetWorkService);

const DWORD NetWorkService::NET_BUF_SIZE =	20480;

NetWorkService::NetWorkService(void)
{
}

NetWorkService::~NetWorkService(void)
{
}

const char * NetWorkService::GetDisplayName() const
{
	return "NetWorkService";
}

	// 声明OnInit（）需求的服务 
efd::SyncResult NetWorkService::OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar)
{
	return efd::SyncResult_Success;
}

	// 
efd::AsyncResult NetWorkService::OnInit()
{
	// 注册各个网络消息的处理
	return efd::AsyncResult_Complete;
}

	// 
efd::AsyncResult NetWorkService::OnShutdown()
{
	return efd::AsyncResult_Complete;
}

efd::AsyncResult NetWorkService::OnTick()
{
	// 处理每个网络消息
	// 现在还是单线程 以后改为多线程接收消息了 再来加个锁
	for (VTDATA::iterator iteRecv = m_vtRecv.begin(); iteRecv != m_vtRecv.end(); ++ iteRecv)
 	{  
   		_NETDATAPTR ptNet = *iteRecv; 		
   
  		UINT16 wMsgID = *(UINT16*)ptNet->pchData;
  		char *pData = (char*)ptNet->pchData + sizeof(UINT16);
		int dwLength = m_demoProto.Decode(wMsgID, pData, ptNet->nLen-sizeof(UINT16), m_pNetBuf);
  		if( dwLength < 0)
  		{
  			EE_ASSERT(FALSE && "网络包解析出错");
  			continue;
  		}
  
		MsgHandlerMap::iterator iteMap = m_mapMsgHandler.find(wMsgID);
   		if( m_mapMsgHandler.end() != iteMap)
   		{
   			(this->*iteMap->second)( m_pNetBuf, dwLength );
   		}
   	}

	m_vtRecv.clear();

	return efd::AsyncResult_Pending;
}

// net message
bool NetWorkService::OnError(INT32 iModuleError, INT32 iSysError)
{
	return true;
}

bool NetWorkService::OnConnect(INT32 iModuleError, INT32 iSysError)
{
	return true;
}

bool NetWorkService::OnDisConnect(INT32 iModuleError, INT32 iSysError)
{
	return true;
}

bool NetWorkService::OnRecv(const char* pszBuff, UINT32 dwBufLen)
{
	// 先压栈 现在还是单线程 以后改为多线程了 再来加个锁
	_NETDATAPTR pkInfo = NiNew _NETDATA(dwBufLen,pszBuff);	
	m_vtRecv.push_back(pkInfo);

	return true;
}

void NetWorkService::Send(UINT16 wMsgID, void *pData, INT32 nLen)
{
	int nEncLen = m_demoProto.Encode(wMsgID, pData, (char*)m_pNetBuf + sizeof(SSDPkgHead16), NET_BUF_SIZE - sizeof(SSDPkgHead16));
	if(nEncLen < 0)
		return;

	SSDPkgHead16* pstHdr = (SSDPkgHead16*)m_pNetBuf;
	INT32 nPkgLen = BuildSDPkgHead16(pstHdr, (UINT16)(nEncLen));
	//Send(m_pNetBuf, nPkgLen);
}

void NetWorkService::RegisterNetEventFunc()
{
	bool ret = false;
#define MAKE_MAP(header, funPtr) \
	ret = m_mapMsgHandler.insert(MsgHandlerMap::value_type(header, funPtr)).second;\
	assert(ret);

	MAKE_MAP(PKGID_GETCHAR_ACK, &NetWorkService::GetCharData);
	MAKE_MAP(PKGID_SCENE_NTF, &NetWorkService::GetSceneNtf);
	MAKE_MAP(PKGID_LOADSCENE_ACK, &NetWorkService::LoadScene);
	MAKE_MAP(PKGID_ENTERGAME_ACK, &NetWorkService::EnterGame);
}

void NetWorkService::GetCharData(char* pMsg, int len)
{
}
void NetWorkService::GetSceneNtf(char* pMsg, int len)
{
}
void NetWorkService::LoadScene(char* pMsg, int len)
{
}
void NetWorkService::EnterGame(char* pMsg, int len)
{
}