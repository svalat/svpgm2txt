/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCR_EXTRACTED_CHAR
#define SVOCR_EXTRACTED_CHAR

/********************  HEADERS  *********************/
#include "svChar.h"
#include <string>

/*********************  CLASS  **********************/
/**
 * Class used to manage extracted char from picture. It permit to manage them simply (displaty,
 * search in DB char, calc hash...)
 * @brief Represent a characte detected into a given picture.
 * @author Sébastien Valat
**/
class svOCRExtractedChar
{
	public:
		svOCRExtractedChar(void);
		void buildExtractedChar(svChar & source);
		void applyCrop(void);
		std::string getHash(int majSize);
		std::string askWhatItIs(void);
		int getXOffset();
		int getXEnd();
		image getImage();
		int getHeight();
		static void buildImgFromHash(std::string hash,image & img);
	private:
		static unsigned char traduct(char value);
		void init();
		svChar * source;
		image img;
		char hpos;
		bool isOk;
		int xoffset;
		int yoffset;
};

#endif
