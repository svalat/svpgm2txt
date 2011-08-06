/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_OPTIONS_H
#define SV_OPTIONS_H

/********************  HEADERS  *********************/
#include <string>
#include <list>
#include <argp.h>

/*********************  CLASS  **********************/
class svOptions
{
	public:
		svOptions();
		void loadParameters(int argc, char * argv[]);
		void displayOptions(void) const;
		const std::list<std::string> & getBatch(void) const;
		const std::list<std::string> & getDbs(void) const;
		std::string getOutputDb(void) const;
		bool hasOutputDb(void) const;
		bool hasSaveEach(void) const;
		bool hasSkipUnknown(void) const;
		std::string getSkipString(void) const;
		bool hasAddMark(void) const;
		std::string getFileToMark(void) const;
		bool hasCalcHeuristique(void) const;
		bool hasHeuristiqueChar(void) const;
		std::string getHeuristiqueChar(void) const;
		bool hasDisplayDist(void) const;
		bool hasTestHeuristique(void) const;
		const std::list<std::string> & getDbToTestHeuristique(void) const;
		bool hasOptimizeCoefs(void) const;
		float getOptimizeMaxParam(void) const;
		std::string getCoefs(void) const;
		bool hasCoefs(void) const;
		bool hasUseHeuristiques(void) const;
	private:
		static error_t parseOptions(int key, char *arg, struct argp_state *state);
		void doInternal(void);
		void loadInputFileList(void);
		void init(void);
		std::list<std::string> dbs;
		std::string outputDb;
		std::list<std::string> batch;
		std::string inputFileList;
		bool paramSaveEach;
		bool paramSkipUnknown;
		std::string paramSkipWith;
		std::string addEmptyMarkTo;
		bool calcHeuristique;
		std::string ch;
		bool displayDist;
		bool testHeuristique;
		std::list<std::string> dbsToTests;
		bool optimiseCoefs;
		float optimiseWithMax;
		std::string coefs;
		bool useHeuristiques;
};

#endif
