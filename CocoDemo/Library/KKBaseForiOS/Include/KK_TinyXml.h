//
//  KK_TinyXml.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 12-8-21.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef __KK_TINYXML_H__
#define __KK_TINYXML_H__

#include "KK_Includes.h"
#include "KK_File.h"
#include "KK_Buffer.h"

#ifdef _DEBUG
#define _XML_NAME
#endif

struct _TinyXmlAttribute
{
	u_short usNameID;
#ifdef _XML_NAME
	char *lpName;
#endif
	char szValue[1];
};

class KK_TinyXml;

class KK_TinyXmlElement
{
public:
	KK_TinyXmlElement(KK_TinyXml *lpXmlDoc, KK_TinyXmlElement *lpParent);
	~KK_TinyXmlElement();
    
public:
	void SetElementName(const KK_StringU& strName);
	void GetElementName(KK_StringU &strName);
	void GetElementName(KK_StringW &strName);
    
	void SetElementValue(const KK_StringU& strValue);
	void GetElementValue(KK_StringU &strValue);
    
	KK_TinyXmlElement* GetParentElement() { return m_lpParentElement; }
	bool IsSameElementName(const KK_StringU &strName);
    
	void Clean();
	void SaveUtf8(IFile *lpFile, int iLevel);
	void SaveUnicode(IFile *lpFile, int iLevel);
    
	KK_TinyXmlElement* AddChildElement(const KK_StringU& strElementName);
	int GetChildElementSize();
	KK_TinyXmlElement* GetChildElement(int iIndex);
	bool RemoveChildElement(int iIndex);
	bool GetChildElement(const KK_StringU& strElementName, KK_Array<KK_TinyXmlElement*, KK_TinyXmlElement*> & arrayElement);
	int RemoveChildElement(const KK_StringU& strElementName);
    
	bool AddAttribute(const KK_StringU& strAttributeName, const KK_StringU& strAttributeValue);
	bool AddAttribute(const KK_StringU& strAttributeName, int iValue);
	bool GetAttribute(const KK_StringU& strAttributeName, KK_StringU& strAttributeValue);
	bool GetAttribute(const KK_StringU& strAttributeName, int& iValue);
	bool RemoveAttribute(const KK_StringU& strAttributeName);
    
protected:
	u_short m_usNameID;
	char *m_lpElementValue;
	KK_TinyXml *m_lpXmlDoc;
	KK_TinyXmlElement *m_lpParentElement;
	KK_Array<_TinyXmlAttribute*, _TinyXmlAttribute*> m_arrayAttribute;
	KK_Array<KK_TinyXmlElement*, KK_TinyXmlElement*> *m_lpArrayElement;
#ifdef _XML_NAME
	char *m_lpElementName;
#endif
};

class KK_BlockSaveFile : public IFile
{
public:
	KK_BlockSaveFile(int nBlockSize = 64*1024) { m_nBlockSize = nBlockSize; }
	~KK_BlockSaveFile() { Close(); }
    
public:
    BOOL Open(const KK_StringWA& strFileName, unsigned int uOpenFlag) { return m_file.Open(strFileName, uOpenFlag); }
	BOOL IsOpen() { return m_file.IsOpen(); }
	BOOL Close() { 
		if (IsOpen()) { Save(); }
		m_buf.Clean();
		return m_file.Close(); 
	}
    
	virtual size_t Read(void *lpReadBuf, size_t nCount) {	return -1; }
	virtual size_t Write(const void *lpWriteBuf, size_t nCount) {
		KK_Assert(m_file.IsOpen());
		m_buf.AppendData((const u_char*)lpWriteBuf, nCount);
		if (m_buf.GetDataLen() >= m_nBlockSize) Save();
		return nCount;
	}
    
protected:
	int Save() {
		int iWrite = m_file.Write(m_buf.GetBuffer(), m_buf.GetDataLen());
		KK_Assert(iWrite == m_buf.GetDataLen());
		m_buf.SetDataLen(0);
		return iWrite;
	}
    
protected:
	int m_nBlockSize;
	KK_Buffer m_buf;
	KK_File m_file;
};

class KK_TinyXml
{
    friend class KK_TinyXmlElement;
public:
	KK_TinyXml();
	virtual ~KK_TinyXml();
	void Clean();
    
public:
	bool Load(const char *lpszFilename);
	bool Load(IFile *lpFile);
    
	bool Save(const char *lpszFilename, bool bUnicode = false);
	bool Save(IFile *lpFile, bool bUnicode = false);
    
	KK_TinyXmlElement* CreateRootElement(const KK_StringU& strElementName);
	KK_TinyXmlElement* GetRootElement() { return m_lpRootElement; }
    
protected:
	void OnAddAttribute() { m_nAttributeCount++; }
	void OnRemoveAttribute() { m_nAttributeCount--; }
	void OnAddElement() { m_nElementCount++; }
	void OnRemoveElement() { m_nElementCount--; }
	bool GetNameByID(u_short usNameID, KK_StringW & strName) {
		KK_StringU strTemp;
		bool bRet = GetNameByID(usNameID, strTemp);
		if (bRet) strName = strTemp;
		return bRet;
	}
	bool GetNameByID(u_short usNameID, KK_StringU & strName) { return m_mapID2Name.Lookup(usNameID, strName); }
	bool LookupNameID(const KK_StringU& strName, u_short &usNameID) { return m_mapName2ID.Lookup(strName, usNameID); }
	u_short GetNameID(const KK_StringU& strName);
    
protected:
	u_short m_usNameIDBase;
	int m_nLine; // 当前处理的行
	int m_nColumn; // 当前处理的列
	int m_nElementCount; // XML中对象个数
	int m_nAttributeCount; // XML中参数个数
	int m_nFileLen; // XML文件长度
	KK_TinyXmlElement *m_lpRootElement;
	KK_Map<KK_StringU, const KK_StringU&, u_short, u_short> m_mapName2ID;
	KK_Map<u_short, u_short, KK_StringU, const KK_StringU&> m_mapID2Name;
    
public:
	int GetXMLLen() { return m_nFileLen; }
	int GetLine() { return m_nLine; }
	int GetColumn() { return m_nColumn; }
	int GetElementCount() { return m_nElementCount; }
	int GetAttributeCount() { return m_nAttributeCount; }
};


#endif // __KK_TINYXML_H__