/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_CHAR_H
#define SV_CHAR_H

/********************  HEADERS  *********************/
#include "image.h"
#include "svOCRLine.h"
#include "svGlobalConfig.h"

/*********************  CLASS  **********************/
class svChar
{
	public:
		svChar();
		bool buildChar(svOCRLine & line,int start);
		void drawBorderOnPicture(void);
		int getStart(void) const;
		int getEnd(void) const;
		void reset(void);
		void whiteTheChar(COLOR color=SV_WHITE_CHAR_WITH_COLOR);
		image * getImage(void);
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
		image * img;
		svOCRLine * line;
		int start;
		int end;
		bool isOk;
};

#endif
