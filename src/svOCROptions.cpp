/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 2009
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCROptions.h"
#include "svOCRGlobalConfig.h"
#include <argp.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>

/**********************  USING  *********************/
using namespace std;

/*********************  CONSTS  *********************/
const char * argp_program_version="svPgm2Txt version " VERSION;
const char *argp_program_bug_address = "<sebastien.valat.dev@orange.fr>";
/* Program documentation. */
static const char RS_ARGP_DOC[] ="svPgm1Txt -- Tools for text extarction from PGM files (OCR). "
"It's based on perfect text formatting to simplify implementation so didn't support scanned documents.";
/* A description of the arguments we accept. */
static const char RS_ARGS_DOC[] = "file1 file2...";
/* The options we understand. */
static const struct argp_option RS_OPTIONS[] = {
	{"save-each",    's',      NULL,      0,  "Save char db after each PGM files." },
	{"unknown",      'u',  "STRING",      0,  "Replace all unknown letters by 'STRING' instead of asking to the user." },
	{"db",           'd',    "FILE",      0,  "Add the given DB file for character recognition." },
	{"output-db",    'o',    "FILE",      0,  "Define the file in which to save the final DB which will cumulate all input "
	                                          "DB plus new entries created by the scan." },
	{"list",         'l',    "FILE",      0,  "Provide a file listing PGM files to process.." },
	{"mark",         'm',    "FILE",      0,  "Add an empty entry at the end of the DB. "
	                                          "(For exemple to mark DB validation)."},
	{"calc",         'c',      NULL,      0,  "Generate heuristics parameter for the letters given in the DB."},
	{"char",         'C',    "CHAR",      0,  "Only display heuristics from the given character."},
	{"dist",         'D',      NULL,      0,  "Display the distance between the char selected with -C and the list of "
	                                          "available heuristics."},
	{"test",         'T',    "FILE",      0,  "Test all letters from the given file to validate an heuristics."},
	{"optimize",     'O',     "MAX",      0,  "Apply heuristics tests in loop to optimise coefs. It can be used with -T "
	                                          "option to provide test case and -d to provide more references than internal once."},
	{"coefs",        'E',  "STRING",      0,  "Provide the 35 coefs for heuristics (generated with -O option)"},
	{"heuristics",   'e',      NULL,      0,  "Enable usage of heusitics for new character instead of requesting to the user."},
	{"whitespace",   'w', "INTEGER",      0,  "Setup width threshold for white space detection (default is 5)."},
	{ 0 }
};

/*******************  FUNCTION  *********************/
svOCROptions::svOCROptions()
{
	this->init();
}

/*******************  FUNCTION  *********************/
void svOCROptions::init(void)
{
	this->paramSaveEach = false;
	this->paramSkipUnknown = false;
	this->calcHeuristic = false;
	this->displayDist = false;
	this->testHeuristic = false;
	this->optimiseCoefs = false;
	this->whitespaceWidth = SVOCR_OCR_SPACE_DETECTION;
}

/*******************  FUNCTION  *********************/
void svOCROptions::loadParameters(int argc, char * argv[])
{
	const argp RS_ARGP_OPTS = { RS_OPTIONS, svOCROptions::parseOptions, RS_ARGS_DOC, RS_ARGP_DOC };
	argp_parse (&RS_ARGP_OPTS, argc, argv, 0, 0, this);
	doInternal();
}

/*******************  FUNCTION  *********************/
/* Parse a single option. */
error_t svOCROptions::parseOptions(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	know is a pointer to our arguments structure. */
	struct svOCROptions *options = (svOCROptions*)state->input;

	switch (key)
	{
		case 'b':
			if (arg!=NULL)
				options->whitespaceWidth = atoi(arg);
			else
				return ARGP_ERR_UNKNOWN;
			break;
		case 's'://progress bar
			options->paramSaveEach = true;
			break;
		case 'u':
			options->paramSkipUnknown = true;
			options->paramSkipWith = arg;
			break;
		case 'd':
			if (arg!=NULL)
				options->dbs.push_back(arg);
			else
				return ARGP_ERR_UNKNOWN;
			break;
		case 'o':
			options->outputDb = arg;
			break;
		case 'l':
			options->inputFileList = arg;
			break;
		case 'm':
			options->addEmptyMarkTo = arg;
			break;
		case 'c':
			options->calcHeuristic = true;
			break;
		case 'C':
			options->ch = arg;
			break;
		case 'D':
			options->displayDist = true;
			break;
		case 'T':
			options->testHeuristic = true;
			options->dbsToTests.push_back(arg);
			break;
		case 'O':
			options->optimiseCoefs = true;
			options->optimiseWithMax = atof(arg);
			break;
		case 'E':
			options->coefs = arg;
			break;
		case 'e':
			options->useHeuristics = true;
			break;
		case ARGP_KEY_ARG:
			options->batch.push_back(arg);
			break;
		case ARGP_KEY_END:
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/*******************  FUNCTION  *********************/
void svOCROptions::displayOptions(void) const
{
	cout << "=============== OPTIONS =================" << endl;
	cout << "save_each     (-s) : " << this->paramSaveEach << endl;
	cout << "skip_unknown  (-u) : " << this->paramSkipUnknown << endl;
	cout << "skip_with     (-u) : " << this->paramSkipWith << endl;
	cout << "dbs           (-d) : ";
	for (list<string>::const_iterator it=dbs.begin();it!=dbs.end();it++)
	{
		if (it!=dbs.begin())
			cout << "                     ";
		cout << *it << endl;
	}
	cout << endl;
	cout << "output_db     (-o) : " << this->outputDb << endl;
	cout << "input_list    (-l) : " << this->inputFileList << endl;
	cout << "mark_db       (-m) : " << this->addEmptyMarkTo << endl;
	cout << "whitespace    (-w) : " << this->whitespaceWidth << endl;
	cout << "files              : ";
	for (list<string>::const_iterator it=batch.begin();it!=batch.end();it++)
	{
		if (it!=batch.begin())
			cout << "                     ";
		cout << *it << endl;
	}
	cout << endl;
 	cout << "=========================================" << endl;
}

/*******************  FUNCTION  *********************/
void svOCROptions::doInternal(void)
{
	if (this->inputFileList != "")
		this->loadInputFileList();
}

/*******************  FUNCTION  *********************/
void svOCROptions::loadInputFileList(void)
{
	char buffer[4096];
	char * res;
	FILE * fp = fopen(this->inputFileList.c_str(),"r");
	if (fp==NULL)
	{
		perror("Impossible d'ouvrir le fichier\n");
		exit(1);
	}

	while(!feof(fp))
	{
		res = fgets(buffer,sizeof(buffer),fp);
		if (res!=NULL)
		{
			while (*res!='\0' && *res!='\n')
				res++;
			*res='\0';
			if (*buffer!='\0')
				this->batch.push_back(buffer);
		}
	}

	fclose(fp);
}

/*******************  FUNCTION  *********************/
const std::list<std::string>& svOCROptions::getBatch(void) const
{
	return this->batch;
}

/*******************  FUNCTION  *********************/
const std::list<std::string> &svOCROptions::getDbs(void) const
{
	return this->dbs;
}

/*******************  FUNCTION  *********************/
std::string svOCROptions::getOutputDb(void) const
{
	return this->outputDb;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasOutputDb(void) const
{
	return (this->outputDb!="");
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasSaveEach(void) const
{
	return this->paramSaveEach;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasSkipUnknown(void) const
{
	return this->paramSkipUnknown;
}

/*******************  FUNCTION  *********************/
std::string svOCROptions::getSkipString(void) const
{
	return this->paramSkipWith;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasAddMark(void) const
{
	return (this->addEmptyMarkTo!="");
}

/*******************  FUNCTION  *********************/
std::string svOCROptions::getFileToMark(void) const
{
	return this->addEmptyMarkTo;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasCalcHeuristic(void) const
{
	return this->calcHeuristic;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasHeuristicChar(void) const
{
	return ch!="";
}

/*******************  FUNCTION  *********************/
float svOCROptions::getOptimizeMaxParam(void) const
{
	return this->optimiseWithMax;
}

/*******************  FUNCTION  *********************/
std::string svOCROptions::getHeuristicChar(void) const
{
	return ch;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasDisplayDist(void) const
{
	return this->displayDist;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasTestHeuristic(void) const
{
	return this->testHeuristic;
}

/*******************  FUNCTION  *********************/
const std::list<std::string> & svOCROptions::getDbToTestHeuristic(void) const
{
	return this->dbsToTests;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasOptimizeCoefs(void) const
{
	return this->optimiseCoefs;
}

/*******************  FUNCTION  *********************/
std::string svOCROptions::getCoefs(void) const
{
	return coefs;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasCoefs(void) const
{
	return (coefs!="");
}

/*******************  FUNCTION  *********************/
bool svOCROptions::hasUseHeuristics(void) const
{
	return useHeuristics;
}

/*******************  FUNCTION  *********************/
int svOCROptions::getWhitespaceWidth(void ) const
{
	return whitespaceWidth;
}
