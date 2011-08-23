/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SCOCR_OCR
#define SCOCR_OCR

/********************  HEADERS  *********************/
#include "svOCRCharDb.h"
#include "svOCRExtractedChar.h"
#include "svOCROptions.h"
#include "svOCRILFixerBase.h"

/*********************  CLASS  **********************/
class svOCR
{
	public:
		svOCR(const svOCROptions & options);
		virtual ~svOCR(void);
		std::string runOnImage(std::string path);
		bool loadDb(std::string filename);
		bool saveDb(std::string filename);
		void setFixer(svOCRILFixerBase * fixer);
	private:
		void buildDefaultFixer(void);
		std::string requestUnknown(svOCRExtractedChar & extrChr,std::string & hash);
		bool isMaj(std::string & value);
		svOCRCharDb db;
		const svOCROptions * options;
		int majSize;
		svOCRILFixerBase * defaultFixer;
		svOCRILFixerBase * fixer;
};

#endif
