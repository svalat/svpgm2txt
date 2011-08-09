/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_CHAR_H
#define SV_CHAR_H

/********************  HEADERS  *********************/
#include "svOCRImage.h"
#include "svOCRLine.h"
#include "svOCRGlobalConfig.h"

/*********************  CLASS  **********************/
class svOCRChar
{
	public:
		svOCRChar();
		bool buildChar(svOCRLine & line,int start);
		void drawBorderOnPicture(void);
		int getStart(void) const;
		int getEnd(void) const;
		void reset(void);
		void whiteTheChar(SVOCR_COLOR color=SVOCR_WHITE_CHAR_WITH_COLOR);
		svOCRImage * getImage(void);
		bool getIsOk(void) const;
		int getWidth(void) const;
		int getHeight(void) const;
		svOCRLine * getLine(void);
		int getMStart(void);
	private:
		void init();
		int buildChar(int start,int angle);
		bool isEmptyLine(int pos,int angle);
		int angle;
		svOCRImage * img;
		svOCRLine * line;
		int start;
		int end;
		bool isOk;
};

#endif
