/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 2009
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SCOCR_OCR
#define SCOCR_OCR

/********************  HEADERS  *********************/
#include "svOCRCharDb.h"
#include "svOCRExtractedChar.h"
#include "svOCROptions.h"

/*********************  CLASS  **********************/
class svOCR
{
	public:
		svOCR(const svOCROptions & options);
		std::string runOnImage(std::string path);
		bool loadDb(std::string filename);
		bool saveDb(std::string filename);
	private:
		std::string requestUnknown(svOCRExtractedChar & extrChr,std::string & hash);
		bool isMaj(std::string & value);
		svOCRCharDb db;
		const svOCROptions * options;
		int majSize;
};

#endif
