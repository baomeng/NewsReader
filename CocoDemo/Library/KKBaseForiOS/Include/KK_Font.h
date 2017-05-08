/*
 * KK_Font.h
 *
 * KK_Font class declaration, 
 *
 * KK Library
 *
 * Copyright (c) 2002- KK. Ltd.
 *
 * All Rights Reserved.
 *
 * The Initial Developer of the Original Code is KK. Ltd.
 *
 * Contributor(s): ______________________________________.
 *
 */


#ifndef __KK_FONT_H__
#define __KK_FONT_H__


#include "KK_ByteStream.h"


#define KK_FONT_INVALID		0
#define KK_FONT_WINDOWS		1

typedef struct __USERFONT__{
    
    long lfHeight;
    long lfWidth;
    long lfEscapement;
    long lfOrientation;
    long lfWeight;
    u_char lfItalic;
    u_char lfUnderline;
    u_char lfStrikeOut;
    u_char lfCharSet;
    u_char lfOutPrecision;
    u_char lfClipPrecision;
    u_char lfQuality;
    u_char lfPitchAndFamily;
    char lfFaceName[32];
    
    long lfColor;    
	long lPointSize;
    
} USERFONT;

//////////////////////////////////////////////////////////////////////////////////
//
// class KK_Font declaration
//
class KK_Font : public KK_StreamObject
{
public:
	KK_Font() { m_ucFlag = KK_FONT_INVALID; }
	~KK_Font() {}

public:
	virtual void SerializeFrom(KK_ByteStream& is) {
		assert(is.IsLoading());

		u_short usSize;

		is >> m_ucFlag;
		is >> usSize;

		if (m_ucFlag != KK_FONT_WINDOWS)
		{
			u_char ucaBuf[1024];
			if (usSize > sizeof(ucaBuf)) {
				long lError = ERROR_READ_BUF;
				throw lError;
			}
			is.ReadBytes(ucaBuf, usSize);
		}
		else
		{
			memset(&m_font, 0, sizeof(m_font));
			
			is >> m_font.lfHeight; 
			is >> m_font.lfWidth; 
			is >> m_font.lfEscapement; 
			is >> m_font.lfOrientation; 
			is >> m_font.lfWeight; 
			is >> m_font.lfItalic; 
			is >> m_font.lfUnderline; 
			is >> m_font.lfStrikeOut; 
			is >> m_font.lfCharSet; 
			is >> m_font.lfOutPrecision; 
			is >> m_font.lfClipPrecision; 
			is >> m_font.lfQuality; 
			is >> m_font.lfPitchAndFamily; 

			KK_StringA strTemp;
			is.ReadString(strTemp, true);
			strcpy(m_font.lfFaceName, strTemp);

			is >> m_font.lfColor;
			is >> m_font.lPointSize;
		}
	}

	virtual void SerializeTo(KK_ByteStream& os) const {
		assert(os.IsStoring());
		assert(m_ucFlag == KK_FONT_WINDOWS);

		os << m_ucFlag;

		u_char ucaBuf[1024];
		KK_ByteStream temp_os(ucaBuf, CountOf(ucaBuf), false);

		temp_os << m_font.lfHeight; 
		temp_os << m_font.lfWidth; 
		temp_os << m_font.lfEscapement; 
		temp_os << m_font.lfOrientation; 
		temp_os << m_font.lfWeight; 
		temp_os << m_font.lfItalic; 
		temp_os << m_font.lfUnderline; 
		temp_os << m_font.lfStrikeOut; 
		temp_os << m_font.lfCharSet; 
		temp_os << m_font.lfOutPrecision; 
		temp_os << m_font.lfClipPrecision; 
		temp_os << m_font.lfQuality; 
		temp_os << m_font.lfPitchAndFamily; 

		temp_os.WriteString(KK_StringU(m_font.lfFaceName), true);

		temp_os << m_font.lfColor;
		temp_os << m_font.lPointSize;

		u_short usSize = (u_short)temp_os.Tell();

		os << usSize;
		os.WriteBytes(ucaBuf, usSize);
	}

public:
	void SetFont(USERFONT * pFont) { 
		m_ucFlag = KK_FONT_WINDOWS;
		m_font = *pFont; 
	}
    void SetFont() { 
        USERFONT font;
		font.lfHeight = 16;
        font.lfWidth = 0;
        font.lfEscapement = 0;
        font.lfOrientation = 0;
        font.lfWeight = 400;
        font.lfItalic = 0;
        font.lfUnderline = 0;
        font.lfStrikeOut = 0;
        font.lfCharSet = 1;
        font.lfOutPrecision = 0;
        font.lfClipPrecision = 0;
        font.lfQuality = 1;
        font.lfPitchAndFamily = 0;
        sprintf(font.lfFaceName ,"%s","宋体");
        
        font.lfColor = 0;
        font.lPointSize = 100;
        SetFont(&font);
	}

	u_char GetFontFlag() { return m_ucFlag; }
	USERFONT * GetFont() { return &m_font; }

protected:
	u_char m_ucFlag;
	USERFONT m_font;
};

#endif // __KK_FONT_H__
