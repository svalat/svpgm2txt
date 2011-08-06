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
#include "chaine.h"

/********************  MACRO  ***********************/
#define WHITESPACE " \t\r\n"
#define IS_WHITESPACE(c) chaine::contain(c,WHITESPACE)
#define PGM_BUFFER_SIZE 256

/********************  ENUM  ************************/
enum magiqueNumber {BAD_FORMAT=170,PLAINE_MODE='2',BINARY_MODE='5'};

/*********************  CLASS  **********************/
class pgmHeader
{
	public:
		pgmHeader(void);
		pgmHeader(const pgmHeader & value);
		~pgmHeader(void);
		bool load(FILE *fp);
		bool save(FILE *fp);
		static chaine getNextParam(char *s,int &pos);
		
		magiqueNumber magNumber;
		int width,height;
		int greyMax;
		chaine comment;		
	private:
		magiqueNumber checkMagiqueNumber(FILE *fp);
		

};

#endif
