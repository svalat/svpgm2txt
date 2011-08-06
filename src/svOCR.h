/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_OCR
#define SV_OCR

/********************  HEADERS  *********************/
#include "svCharDb.h"
#include "svExtractedChar.h"
#include "svOptions.h"

/*********************  CLASS  **********************/
class svOCR
{
	public:
		svOCR(const svOptions & options);
		std::string runOnImage(std::string path);
		bool loadDb(std::string filename);
		bool saveDb(std::string filename);
	private:
		std::string requestUnknown(svExtractedChar & extrChr,std::string & hash);
		bool isMaj(std::string & value);
		svCharDb db;
		const svOptions * options;
		int majSize;
};

#endif
