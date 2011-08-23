/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCR_LINE_H
#define SVOCR_LINE_H

/********************  HEADERS  *********************/
#include "svOCRImage.h"

/*********************  CLASS  **********************/
/**
 * Provide a representatino of a text line detected on a given picture.
 * @brief Class used to represent a text line detected on a picture.
 * @author Valat Sébastien
**/
class svOCRLine
{
	public:
		svOCRLine();
		bool buildLine(svOCRImage & img,unsigned int startPoint);
		void drawBorderOnPicture(void);
		int getStart(void) const;
		int getEnd(void) const;
		svOCRImage * getImage(void);
		bool getIsOk(void) const;
		int getHeight(void) const;
	private:
		void init();
		svOCRImage * img;
		int ystart;
		int yend;
		bool isOk;
};

#endif
