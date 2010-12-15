#include "game_pch.h"
#include "demoproto.h"

CDemoProto::CDemoProto()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	Init();
}

CDemoProto::~CDemoProto()
{
}

int CDemoProto::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);

	EnFuncCDemoProto pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

int CDemoProto::Decode(int iMsgID, char *pNet,int iNetSize, void *pHost)
{
	m_oData.Prepare(pNet, iNetSize);

	DeFuncCDemoProto pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCDemoProto CDemoProto::FindEncodeFunc(int iMsgID)
{
	if(iMsgID >= 8192)
		return NULL;
	return m_EncodeFuncArray[iMsgID];
}

DeFuncCDemoProto CDemoProto::FindDecodeFunc(int iMsgID)
{
	if(iMsgID >= 8192)
		return NULL;
	return m_DecodeFuncArray[iMsgID];
}

bool CDemoProto::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[PKGID_GETCHAR_REQ] = &EncodePKG_GETCHAR_REQ;
	m_DecodeFuncArray[PKGID_GETCHAR_REQ] = &DecodePKG_GETCHAR_REQ;

	m_EncodeFuncArray[PKGID_GETCHAR_ACK] = &EncodePKG_GETCHAR_ACK;
	m_DecodeFuncArray[PKGID_GETCHAR_ACK] = &DecodePKG_GETCHAR_ACK;

	m_EncodeFuncArray[PKGID_LOADSCENE_REQ] = &EncodePKG_LOADSCENE_REQ;
	m_DecodeFuncArray[PKGID_LOADSCENE_REQ] = &DecodePKG_LOADSCENE_REQ;

	m_EncodeFuncArray[PKGID_SCENE_NTF] = &EncodePKG_SCENE_NTF;
	m_DecodeFuncArray[PKGID_SCENE_NTF] = &DecodePKG_SCENE_NTF;

	m_EncodeFuncArray[PKGID_LOADSCENE_ACK] = &EncodePKG_LOADSCENE_ACK;
	m_DecodeFuncArray[PKGID_LOADSCENE_ACK] = &DecodePKG_LOADSCENE_ACK;

	m_EncodeFuncArray[PKGID_ENTERGAME_REQ] = &EncodePKG_ENTERGAME_REQ;
	m_DecodeFuncArray[PKGID_ENTERGAME_REQ] = &DecodePKG_ENTERGAME_REQ;

	m_EncodeFuncArray[PKGID_ENTERGAME_ACK] = &EncodePKG_ENTERGAME_ACK;
	m_DecodeFuncArray[PKGID_ENTERGAME_ACK] = &DecodePKG_ENTERGAME_ACK;

	return true;
}

int EncodeCHAR_DATE(void *pHost, CNetData* poNetData)
{
	CHAR_DATE *pstIn = (CHAR_DATE*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwid))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szname, sizeof(pstIn->szname)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwscene_id))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwpos_x))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwpos_y))
		return -1;

	if(-1 == poNetData->AddInt(pstIn->nhp))
		return -1;

	if(-1 == poNetData->AddInt(pstIn->nmp))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeCHAR_DATE(void *pHost, CNetData* poNetData)
{
	CHAR_DATE *pstOut = (CHAR_DATE*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwid))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szname, sizeof(pstOut->szname)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwscene_id))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwpos_x))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwpos_y))
		return -1;

	if(-1 == poNetData->DelInt(pstOut->nhp))
		return -1;

	if(-1 == poNetData->DelInt(pstOut->nmp))
		return -1;

	return sizeof(*pstOut);
}

int EncodeMONSTER_DATE(void *pHost, CNetData* poNetData)
{
	MONSTER_DATE *pstIn = (MONSTER_DATE*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwid))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwscene_id))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwpos_x))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwpos_y))
		return -1;

	if(-1 == poNetData->AddInt(pstIn->nhp))
		return -1;

	if(-1 == poNetData->AddInt(pstIn->nmp))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeMONSTER_DATE(void *pHost, CNetData* poNetData)
{
	MONSTER_DATE *pstOut = (MONSTER_DATE*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwid))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwscene_id))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwpos_x))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwpos_y))
		return -1;

	if(-1 == poNetData->DelInt(pstOut->nhp))
		return -1;

	if(-1 == poNetData->DelInt(pstOut->nmp))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GETCHAR_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GETCHAR_REQ *pstIn = (PKG_GETCHAR_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwuser_id))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GETCHAR_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GETCHAR_REQ *pstOut = (PKG_GETCHAR_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwuser_id))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GETCHAR_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GETCHAR_ACK *pstIn = (PKG_GETCHAR_ACK*)pHost;

	if(-1 == EncodeCHAR_DATE(&pstIn->stchar_date, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GETCHAR_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GETCHAR_ACK *pstOut = (PKG_GETCHAR_ACK*)pHost;

	if(-1 == DecodeCHAR_DATE(&pstOut->stchar_date, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_LOADSCENE_REQ(void *pHost, CNetData* poNetData)
{
	PKG_LOADSCENE_REQ *pstIn = (PKG_LOADSCENE_REQ*)pHost;

	return poNetData->GetDataLen();
}

int DecodePKG_LOADSCENE_REQ(void *pHost, CNetData* poNetData)
{
	PKG_LOADSCENE_REQ *pstOut = (PKG_LOADSCENE_REQ*)pHost;

	return sizeof(*pstOut);
}

int EncodePKG_SCENE_NTF(void *pHost, CNetData* poNetData)
{
	PKG_SCENE_NTF *pstIn = (PKG_SCENE_NTF*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwMonsterCount))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwCharCount))
		return -1;

	int i;
	for(i = 0; i < MAX_MONSTER_COUNT; i++)
	{
		if(i >= (int)pstIn->dwMonsterCount)
			break;
		if(-1 == EncodeCHAR_DATE(&pstIn->astMonsterInfos[i], poNetData))
			return -1;
	}

	for(i = 0; i < MAX_CHAR_COUNT; i++)
	{
		if(i >= (int)pstIn->dwCharCount)
			break;
		if(-1 == EncodeCHAR_DATE(&pstIn->astCharInfos[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

int DecodePKG_SCENE_NTF(void *pHost, CNetData* poNetData)
{
	PKG_SCENE_NTF *pstOut = (PKG_SCENE_NTF*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwMonsterCount))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwCharCount))
		return -1;

	int i;
	for(i = 0; i < MAX_MONSTER_COUNT; i++)
	{
		if(i >= (int)pstOut->dwMonsterCount)
			break;
		if(-1 == DecodeCHAR_DATE(&pstOut->astMonsterInfos[i], poNetData))
			return -1;
	}

	for(i = 0; i < MAX_CHAR_COUNT; i++)
	{
		if(i >= (int)pstOut->dwCharCount)
			break;
		if(-1 == DecodeCHAR_DATE(&pstOut->astCharInfos[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

int EncodePKG_LOADSCENE_ACK(void *pHost, CNetData* poNetData)
{
	PKG_LOADSCENE_ACK *pstIn = (PKG_LOADSCENE_ACK*)pHost;

	return poNetData->GetDataLen();
}

int DecodePKG_LOADSCENE_ACK(void *pHost, CNetData* poNetData)
{
	PKG_LOADSCENE_ACK *pstOut = (PKG_LOADSCENE_ACK*)pHost;

	return sizeof(*pstOut);
}

int EncodePKG_ENTERGAME_REQ(void *pHost, CNetData* poNetData)
{
	PKG_ENTERGAME_REQ *pstIn = (PKG_ENTERGAME_REQ*)pHost;

	return poNetData->GetDataLen();
}

int DecodePKG_ENTERGAME_REQ(void *pHost, CNetData* poNetData)
{
	PKG_ENTERGAME_REQ *pstOut = (PKG_ENTERGAME_REQ*)pHost;

	return sizeof(*pstOut);
}

int EncodePKG_ENTERGAME_ACK(void *pHost, CNetData* poNetData)
{
	PKG_ENTERGAME_ACK *pstIn = (PKG_ENTERGAME_ACK*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwerror))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_ENTERGAME_ACK(void *pHost, CNetData* poNetData)
{
	PKG_ENTERGAME_ACK *pstOut = (PKG_ENTERGAME_ACK*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwerror))
		return -1;

	return sizeof(*pstOut);
}

