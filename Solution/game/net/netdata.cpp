#include "game_pch.h"
#include "netdata.h"

CNetData::CNetData()
{
	m_pBuf	= NULL;
	m_iSize = 0;
	m_iPos	= 0;
}

CNetData::~CNetData()
{
}

void CNetData::Prepare(char *pNetData, int iSize)
{
	m_pBuf	= pNetData;
	m_iSize	= iSize;
	m_iPos	= 0;
}

void CNetData::Reset()
{
	m_iPos = 0;
}

char * CNetData::GetData()
{
	return m_pBuf;
}

int CNetData::GetDataLen()
{
	return m_iPos;
}

int CNetData::AddByte(unsigned char byByte)
{
	if(m_iPos + (int)sizeof(byByte) > m_iSize)
		return -1;

	*(unsigned char*)(m_pBuf + m_iPos) = byByte;
	m_iPos += sizeof(byByte);

	return m_iPos; 
}

int CNetData::DelByte(unsigned char &byByte)
{
	if(m_iPos + (int)sizeof(byByte) > m_iSize)
		return -1;

	byByte = *(unsigned char*)(m_pBuf + m_iPos);
	m_iPos += sizeof(byByte);

	return m_iPos; 
	return -1;
}

int CNetData::AddChar(char chChar)
{
	if(m_iPos + (int)sizeof(chChar) > m_iSize)
		return -1;

	*(char*)(m_pBuf + m_iPos) = chChar;
	m_iPos += sizeof(chChar);

	return m_iPos; 
}

int CNetData::DelChar(char &chChar)
{
	if(m_iPos + (int)sizeof(chChar) > m_iSize)
		return -1;

	chChar = *(char*)(m_pBuf + m_iPos);
	m_iPos += sizeof(chChar);

	return m_iPos; 
}

int CNetData::AddWord(unsigned short wWord)
{
	if(m_iPos + (int)sizeof(wWord) > m_iSize)
		return -1;

	*(unsigned short*)(m_pBuf + m_iPos) = htons(wWord);
	m_iPos += sizeof(wWord);

	return m_iPos; 
}

int CNetData::DelWord(unsigned short &wWord)
{
	if(m_iPos + (int)sizeof(wWord) > m_iSize)
		return -1;

	wWord = ntohs(*(unsigned short*)(m_pBuf + m_iPos));
	m_iPos += sizeof(wWord);

	return m_iPos; 
}

int CNetData::AddShort(short shShort)
{
	if(m_iPos + (int)sizeof(shShort) > m_iSize)
		return -1;

	*(short*)(m_pBuf + m_iPos) = htons(shShort);
	m_iPos += sizeof(shShort);

	return m_iPos;
}

int CNetData::DelShort(short &shShort)
{
	if(m_iPos + (int)sizeof(shShort) > m_iSize)
		return -1;

	shShort = ntohs(*(short*)(m_pBuf + m_iPos));
	m_iPos += sizeof(shShort);

	return m_iPos;
}

int CNetData::AddDword(unsigned int dwDword)
{
	if(m_iPos + (int)sizeof(dwDword) > m_iSize)
		return -1;

	*(unsigned int*)(m_pBuf + m_iPos) = htonl(dwDword);
	m_iPos += sizeof(dwDword);

	return m_iPos;
}

int CNetData::DelDword(unsigned int &dwDword)
{
	if(m_iPos + (int)sizeof(dwDword) > m_iSize)
		return -1;

	dwDword = ntohl(*(unsigned int*)(m_pBuf + m_iPos));
	m_iPos += sizeof(dwDword);

	return m_iPos;
}

int CNetData::AddInt(int iInt)
{
	if(m_iPos + (int)sizeof(iInt) > m_iSize)
		return -1;

	*(int*)(m_pBuf + m_iPos) = htonl(iInt);
	m_iPos += sizeof(iInt);

	return m_iPos;
}

int CNetData::DelInt(int &iInt)
{
	if(m_iPos + (int)sizeof(iInt) > m_iSize)
		return -1;

	iInt = ntohl(*(int*)(m_pBuf + m_iPos));
	m_iPos += sizeof(iInt);

	return m_iPos;
}

int CNetData::AddString(const char *pszString, int iSize)
{
	int iLen = Strnlen(pszString, iSize-1);

	if(m_iPos + (int)sizeof(unsigned short) + iLen > m_iSize)
		return -1;

	if(-1 == AddWord(iLen))
		return -1;

	memcpy(m_pBuf + m_iPos, pszString, iLen);
	m_iPos += iLen;

	return m_iPos;
}

int CNetData::DelString(char *pszOut, int iSize)
{
	unsigned short wLen = 0;
	if(-1 == DelWord(wLen))
		return -1;

	if(m_iPos + (int)wLen > m_iSize)
		return -1;

	if( int(wLen + 1) > iSize )
		return -1;

	memcpy(pszOut, m_pBuf+m_iPos, wLen);
	pszOut[wLen] = '\0';
	m_iPos += wLen;

	return m_iPos;
}

int CNetData::Strnlen(const char *pszString, int iSize)
{
	int i;
	const char *ptr = pszString;

	for(i = 0; i < iSize; i++)
	{
		if('\0' == *ptr)
		{
			return i;
		}
		ptr++;
	}

	return iSize;
}

