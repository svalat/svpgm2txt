/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCR_EXTRACTED_CHAR
#define SVOCR_EXTRACTED_CHAR

/********************  HEADERS  *********************/
#include "svOCRChar.h"
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
		void buildExtractedChar(svOCRChar & source);
		void applyCrop(void);
		std::string getHash(int majSize);
		std::string askWhatItIs(bool useTermColor);
		int getXOffset();
		int getXEnd();
		svOCRImage getImage();
		int getHeight();
		static void buildImgFromHash(std::string hash,svOCRImage & img);
	private:
		static unsigned char traduct(char value);
		void init();
		svOCRChar * source;
		svOCRImage img;
		char hpos;
		bool isOk;
		int xoffset;
		int yoffset;
};

#endif
