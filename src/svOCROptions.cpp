/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
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
#include <cstring>
#include <unistd.h>

/**********************  USING  *********************/
using namespace std;

/*********************  CONSTS  *********************/
const char * argp_program_version="svpgm2txt version " VERSION;
const char *argp_program_bug_address = "<sebastien.valat.dev@orange.fr>";
/* Program documentation. */
static const char RS_ARGP_DOC[] ="svPgm1Txt -- Tools for text extraction from PGM files (OCR). "
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
	{"calc",         'c',      NULL,      0,  "Generate heuristics parameter for the letters given in the DB."},
	{"char",         'C',    "CHAR",      0,  "Only display heuristics from the given character."},
	{"dist",         'D',      NULL,      0,  "Display the distance between the char selected with -C and the list of "
	                                          "available heuristics."},
	{"test",         'T',    "FILE",      0,  "Test all letters from the given file to validate an heuristics."},
	{"optimize",     'O',     "MAX",      0,  "Apply heuristics tests in loop to optimize coefs. It can be used with -T "
	                                          "option to provide test case and -d to provide more references than internal once."},
	{"coefs",        'E',  "STRING",      0,  "Provide the 35 coefs for heuristics (generated with -O option)"},
	{"heuristics",   'e',      NULL,      0,  "Enable usage of heuristics for new character instead of requesting to the user."},
	{"whitespace",   'w', "INTEGER",      0,  "Setup width threshold for white space detection (default is 5)."},
	{"userdb",       'U',      NULL,      0,  "Use default user DB : equivalent to -o and -d on $HOME/.svpgm2txt.db."},
	{"ilfix",        'i',  "STRING",      0,  "Enable fix mode for I and l distinction, value can be : "
	                                          "'none' for disabling the fix, "
	                                          "'always_ask' to ignore DB and ask every time, "
	                                          "'force_l' or 'force_i' to force using one letter, "
	                                          "'aspell' use apsell as spell checker for I/l distinction, "
	                                          "'upper_case' to detect if we are at begening of a work, an upper cased word...,"},
	{"lang",          'L', "STRING",      0,  "Define the language to use for spell checking in conjunction of -i aspell only."},
	{"mode",          'm', "STRING",      0,  "Define the terminal mode to use to enable or disable color support : color, std, auto."},
	{ 0 }
};
static const char * SVOCR_ILFIX_NAMES[]={"none","always_ask","force_l","force_i","upper_case","aspell"};

/*******************  FUNCTION  *********************/
svOCROptions::svOCROptions()
{
	this->init();
}

/*******************  FUNCTION  *********************/
void svOCROptions::init(void)
{
	this->paramSaveEach    = false;
	this->paramSkipUnknown = false;
	this->calcHeuristic    = false;
	this->displayDist      = false;
	this->testHeuristic    = false;
	this->optimiseCoefs    = false;
	this->whitespaceWidth  = SVOCR_OCR_SPACE_DETECTION;
	this->useHeuristics    = false;
	this->ilfix            = SVOCR_IL_FIX_NONE;
	this->spellLang        = "Undefined";
	this->setTermColorAuto();
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
		case 'w':
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
		case 'L':
			if (arg!=NULL)
				options->spellLang = arg;
			else
				return ARGP_ERR_UNKNOWN;
			break;
		case 'm':
			if (arg==NULL)
			{
				return ARGP_ERR_UNKNOWN;
			} else if (strcmp(arg,"auto")==0) {
				options->setTermColorAuto();
			} else if (strcmp(arg,"color")==0) {
				options->termColor = true;
			} else if (strcmp(arg,"std")==0) {
				options->termColor = false;
			} else {
				cerr << "Unknown terminal color mode : " << arg << endl;
				return ARGP_ERR_UNKNOWN;
			}
			break;
		case 'i':
			if (arg == NULL)
			{
				return ARGP_ERR_UNKNOWN;
			} else {
				bool find = false;
				for (unsigned int i = 0 ; i < sizeof(SVOCR_ILFIX_NAMES)/sizeof(char *) ; ++i)
				{
					if (strcmp(SVOCR_ILFIX_NAMES[i],arg) == 0)
					{
						find = true;
						options->ilfix = (svOCRILFix)i;
						break;
					}
				}
				if (find == false)
				{
					cerr << "Invalid value for argument -i/--ilfix : " << arg << endl;
					return ARGP_ERR_UNKNOWN;
				}
			}
						
			break;
		case 'o':
			options->outputDb = arg;
			break;
		case 'U':
			options->outputDb = svOCROptions::getUserDbFile();
			options->dbs.push_back(options->outputDb);
			break;
		case 'l':
			options->inputFileList = arg;
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
	cout << "whitespace    (-w) : " << this->whitespaceWidth << endl;
	cout << "ilfix         (-i) : " << SVOCR_ILFIX_NAMES[this->ilfix] << endl;
	cout << "lang          (-L) : " << this->spellLang << endl;
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

/*******************  FUNCTION  *********************/
string svOCROptions::getUserDbFile(void )
{
	string tmp = getenv("HOME");
	tmp += "/.svpgm2txt.db";
	return tmp;
}

/*******************  FUNCTION  *********************/
svOCRILFix svOCROptions::getILFix(void ) const
{
	return ilfix;
}

/*******************  FUNCTION  *********************/
string svOCROptions::getSpellLang(void ) const
{
	return spellLang;
}

/*******************  FUNCTION  *********************/
void svOCROptions::setSpellLang(string lang)
{
	this->spellLang = lang;
}

/*******************  FUNCTION  *********************/
bool svOCROptions::getTermColor(void ) const
{
	return this->termColor;
}

/*******************  FUNCTION  *********************/
void svOCROptions::setTermColorAuto(void )
{
	if (isatty(STDOUT_FILENO))
		this->termColor = true;
	else
		this->termColor = false;
}
