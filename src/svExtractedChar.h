/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_EXTRACTED_CHAR
#define SV_EXTRACTED_CHAR

/********************  HEADERS  *********************/
#include "svChar.h"
#include <string>

/*********************  CLASS  **********************/
class svExtractedChar
{
	public:
		svExtractedChar(void);
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
