/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOptions.h"
#include <argp.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>

/**********************  USING  *********************/
using namespace std;

/*********************  CONSTS  *********************/
const char * argp_program_version="svPgm2Txt version " VERSION;
const char *argp_program_bug_address = "<sebastien.valat@gmail.com>";
/* Program documentation. */
static const char RS_ARGP_DOC[] ="svPgm1Txt -- Utilitaire d'extraction de texte depuis un fichier PGM.";
/* A description of the arguments we accept. */
static const char RS_ARGS_DOC[] = "file1 file2...";
/* The options we understand. */
static const struct argp_option RS_OPTIONS[] = {
	{"save-each",    's',      NULL,      0,  "Sauvegarde le dictionnaire à la fin de chaque fichier." },
	{"unknown",      'u',  "STRING",      0,  "Remplace toutes les lettre inconnues par STRING plutôt que demander à l'utilisateur." },
	{"db",           'd',    "FILE",      0,  "Ajoute la base de donner à la liste des bases de recherche de caractères." },
	{"output-db",    'o',    "FILE",      0,  "Definit le fichier dans lequel sauvegarder la nouvelle base de donnée "
	                                          "(elle cumulera toutes les bases utilisées en entrée)." },
	{"list",         'l',    "FILE",      0,  "Fournit une fichier contenant la liste des fichiers à traiter." },
	{"mark",         'm',    "FILE",      0,  "Ajoute simplement une entrée vide à la fin de la base de donnée pour servir de repère"
	                                          "(par exemple pour de la validation de base)."},
	{"calc",         'c',      NULL,      0,  "Calcule les heuristiques des lettres de la base de donnée"},
	{"char",         'C',    "CHAR",      0,  "N'affiche que les heuristiques de la lettre demandée"},
	{"dist",         'D',      NULL,      0,  "Affiche la distance du caractère sélectionné par C à la liste des heuristiques disponibles"},
	{"test",         'T',    "FILE",      0,  "Test les lettres de la base de donnée avec les heuristiques"},
	{"optimize",     'O',     "MAX",      0,  "Applique les test d'heuristiques en boucle pour optimiser les coefs (utilisé avec -T pour "
	                                          "founir les cas test et -d pour les références supplémentaires au références internes.)"},
	{"coefs",        'E',  "STRING",      0,  "Fournit les 35 coefs pour l'heuristique (générés avec -O)"},
	{"heuristiques", 'e',      NULL,      0,  "Active l'utilisation des heuristiques pour les nouveaux caractères"},
	{ 0 }
};

/*******************  FUNCTION  *********************/
svOptions::svOptions()
{
	this->init();
}

/*******************  FUNCTION  *********************/
void svOptions::init(void)
{
	this->paramSaveEach = false;
	this->paramSkipUnknown = false;
	this->calcHeuristique = false;
	this->displayDist = false;
	this->testHeuristique = false;
	this->optimiseCoefs = false;
}

/*******************  FUNCTION  *********************/
void svOptions::loadParameters(int argc, char * argv[])
{
	const argp RS_ARGP_OPTS = { RS_OPTIONS, svOptions::parseOptions, RS_ARGS_DOC, RS_ARGP_DOC };
	argp_parse (&RS_ARGP_OPTS, argc, argv, 0, 0, this);
	doInternal();
}

/*******************  FUNCTION  *********************/
/* Parse a single option. */
error_t svOptions::parseOptions(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	know is a pointer to our arguments structure. */
	struct svOptions *options = (svOptions*)state->input;

	switch (key)
	{
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
			options->calcHeuristique = true;
			break;
		case 'C':
			options->ch = arg;
			break;
		case 'D':
			options->displayDist = true;
			break;
		case 'T':
			options->testHeuristique = true;
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
			options->useHeuristiques = true;
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
void svOptions::displayOptions(void) const
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
void svOptions::doInternal(void)
{
	if (this->inputFileList != "")
		this->loadInputFileList();
}

/*******************  FUNCTION  *********************/
void svOptions::loadInputFileList(void)
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
const std::list<std::string>& svOptions::getBatch(void) const
{
	return this->batch;
}

/*******************  FUNCTION  *********************/
const std::list<std::string> &svOptions::getDbs(void) const
{
	return this->dbs;
}

/*******************  FUNCTION  *********************/
std::string svOptions::getOutputDb(void) const
{
	return this->outputDb;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasOutputDb(void) const
{
	return (this->outputDb!="");
}

/*******************  FUNCTION  *********************/
bool svOptions::hasSaveEach(void) const
{
	return this->paramSaveEach;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasSkipUnknown(void) const
{
	return this->paramSkipUnknown;
}

/*******************  FUNCTION  *********************/
std::string svOptions::getSkipString(void) const
{
	return this->paramSkipWith;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasAddMark(void) const
{
	return (this->addEmptyMarkTo!="");
}

/*******************  FUNCTION  *********************/
std::string svOptions::getFileToMark(void) const
{
	return this->addEmptyMarkTo;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasCalcHeuristique(void) const
{
	return this->calcHeuristique;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasHeuristiqueChar(void) const
{
	return ch!="";
}

/*******************  FUNCTION  *********************/
float svOptions::getOptimizeMaxParam(void) const
{
	return this->optimiseWithMax;
}

/*******************  FUNCTION  *********************/
std::string svOptions::getHeuristiqueChar(void) const
{
	return ch;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasDisplayDist(void) const
{
	return this->displayDist;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasTestHeuristique(void) const
{
	return this->testHeuristique;
}

/*******************  FUNCTION  *********************/
const std::list<std::string> & svOptions::getDbToTestHeuristique(void) const
{
	return this->dbsToTests;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasOptimizeCoefs(void) const
{
	return this->optimiseCoefs;
}

/*******************  FUNCTION  *********************/
std::string svOptions::getCoefs(void) const
{
	return coefs;
}

/*******************  FUNCTION  *********************/
bool svOptions::hasCoefs(void) const
{
	return (coefs!="");
}

/*******************  FUNCTION  *********************/
bool svOptions::hasUseHeuristiques(void) const
{
	return useHeuristiques;
}

