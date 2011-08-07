/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_CHAR_DB_H
#define SV_CHAR_DB_H

/********************  HEADERS  *********************/
#include <list>
#include <string>
#include "svOCRHeuristic.h"

/********************  MACRO  ***********************/
#define SV_DB_NOT_FOUND "--**== HASH NOT FOUND ==**--"

/*********************  STRUCT  *********************/
struct svDicEntry
{
	char hpos;
	std::string hash;
	std::string value;
	int majSize;
};

/*********************  CLASS  **********************/
class svCharDb
{
	public:
		svCharDb();
		void addEntry(std::string hash,int majSize,std::string value,bool addToHeur=false);
		std::string getValue(std::string hash);
		bool save(std::string filename);
		bool load(std::string filename);
		void addEmptyMark(void);
		std::list<svDicEntry> getDic();
		svOCRHeuristicAnswer askToGodOfChar(svOCRHeuristic & heur,bool testMode=false);
		void displayDistWithAll(svOCRHeuristic & heur);
	protected:
		void addHeuristique(std::string hash,int majSize,std::string value);
		void loadInternalHeurDb(void);
		std::list<svDicEntry> dic;
		std::list<svOCRHeuristicSimple> heurDb;
};

#endif
