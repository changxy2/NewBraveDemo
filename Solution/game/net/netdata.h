#ifndef CODER_H_INCLUDE
#define CODER_H_INCLUDE

class CNetData
{
public:
	CNetData();
	virtual ~CNetData();

	void Prepare(char *pNetData, int iSize);
	void Reset();

	char * GetData();
	int GetDataLen();

	int AddByte(unsigned char byByte);
	int DelByte(unsigned char &byByte);

	int AddChar(char chChar);
	int DelChar(char &chChar);

	int AddWord(unsigned short wWord);
	int DelWord(unsigned short &wWord);

	int AddShort(short shShort);
	int DelShort(short &shShort);

	int AddDword(unsigned int dwDword);
	int DelDword(unsigned int &dwDword);

	int AddInt(int iInt);
	int DelInt(int &iInt);

	int AddString(const char *pszString, int iSize);
	int DelString(char *pszOut, int iSize);

	int Strnlen(const char *pszString, int iSize);

//	int AddBuf(const char *pBuf, int iSize);
//	int DelBuf(char *pBuf, int iSize);

protected:
	char * m_pBuf;
	int m_iSize;
	int m_iPos;
};

#endif
