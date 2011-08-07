/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef PGM_HEADER_H
#define PGM_HEADER_H

/********************  HEADERS  *********************/
#include <stdio.h>
#include <string>

/********************  MACRO  ***********************/
#define WHITESPACE " \t\r\n"
#define PGM_BUFFER_SIZE 256

/********************  ENUM  ************************/
enum svOCRMagiqueNumber {BAD_FORMAT=170,PLAINE_MODE='2',BINARY_MODE='5'};

/*********************  CLASS  **********************/
class svOCRPgmHeader
{
	public:
		svOCRPgmHeader(void);
		svOCRPgmHeader(const svOCRPgmHeader & value);
		~svOCRPgmHeader(void);
		bool load(FILE *fp);
		bool save(FILE *fp);
		static std::string getNextParam(char *s,int &pos);
		
		svOCRMagiqueNumber magNumber;
		int width,height;
		int greyMax;
		std::string comment;		
	private:
		svOCRMagiqueNumber checkMagiqueNumber(FILE *fp);
		static bool isWhitespace(char value);
};

#endif
