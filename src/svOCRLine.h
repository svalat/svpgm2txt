/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_LINE_H
#define SV_LINE_H

/********************  HEADERS  *********************/
#include "image.h"

/*********************  CLASS  **********************/
class svOCRLine
{
	public:
		svOCRLine();
		bool buildLine(image & img,unsigned int startPoint);
		void drawBorderOnPicture(void);
		int getStart(void) const;
		int getEnd(void) const;
		image * getImage(void);
		bool getIsOk(void) const;
		int getHeight(void) const;
	private:
		void init();
		image * img;
		int ystart;
		int yend;
		bool isOk;
};

#endif
