/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 2009
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCR_CHAR_DB_H
#define SVOCR_CHAR_DB_H

/********************  HEADERS  *********************/
#include <list>
#include <map>
#include <string>
#include "svOCRHeuristic.h"

/********************  MACRO  ***********************/
#define SVOCR_DB_NOT_FOUND "--**== HASH NOT FOUND ==**--"

/*********************  STRUCT  *********************/
struct svOCRDicEntry
{
	char hpos;
	std::string hash;
	std::string value;
	int majSize;
};

/*********************  TYPES  **********************/
typedef std::map<std::string,svOCRDicEntry> svOCRCharDbEntryList;

/*********************  CLASS  **********************/
/**
 * Database of known character, this is done to search exact identification via hashing methods.
 * @brief Database of known characters.
 * @author Valat Sébastien
**/
class svOCRCharDb
{
	public:
		svOCRCharDb();
		void addEntry(std::string hash,int majSize,std::string value,bool addToHeur=false);
		std::string getValue(std::string hash);
		bool save(std::string filename);
		bool load(std::string filename);
		void addEmptyMark(void);
		svOCRCharDbEntryList getDic();
		svOCRHeuristicAnswer askToGodOfChar(svOCRHeuristic & heur,bool testMode=false);
		void displayDistWithAll(svOCRHeuristic & heur);
	protected:
		void addHeuristic(std::string hash,int majSize,std::string value);
		void loadInternalHeurDb(void);
		svOCRCharDbEntryList dic;
		std::list<svOCRHeuristicSimple> heurDb;
};

#endif
