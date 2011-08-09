/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.0
             DATE     : 2009
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRCharDb.h"
#include <iostream>
#include <cstdio>

/**********************  USING  *********************/
using namespace std;

/*********************  CONSTS  *********************/
extern const svOCRHeuristicSimpleBase SVOCR_INTERNAL_DB[SVOCR_HEUR_INTERNAL_DB_SIZE];

/*******************  FUNCTION  *********************/
svOCRCharDb::svOCRCharDb()
{
	this->loadInternalHeurDb();
}

/*******************  FUNCTION  *********************/
void svOCRCharDb::addEntry(std::string hash,int majSize,std::string value,bool addToHeur)
{
	string res;
	svOCRDicEntry entry;
	entry.hpos = hash[0];
	entry.hash = hash;
	entry.value = value;
	entry.majSize = majSize;
	if (majSize<=0)
		return;
	//test if hash is already present in DB
	res = this->getValue(hash);
	if (res == SVOCR_DB_NOT_FOUND)
	{
		dic[entry.hash] = entry;
		if (addToHeur==true)
			addHeuristic(hash,majSize,value);
	} else if (res!=value) {
		cerr << "Try de redefine an entry with different value : " << res << " != " << value << endl;
	}
}

/*******************  FUNCTION  *********************/
void svOCRCharDb::addHeuristic(std::string hash,int majSize,std::string value)
{
	if (hash[0]=='i')
		return;
	svOCRHeuristic heur;
	heur.buildFromHash(hash,majSize);
	heur.setValue(value);
	heurDb.push_back(heur.getSimple());
}

/*******************  FUNCTION  *********************/
string svOCRCharDb::getValue(std::string hash)
{
	if (hash[0]=='i')
		return SVOCR_DB_NOT_FOUND;
	
	svOCRCharDbEntryList::iterator it = dic.find(hash);
	if (it == dic.end())
		return SVOCR_DB_NOT_FOUND;
	else
		return it->second.value;
}

/*******************  FUNCTION  *********************/
bool svOCRCharDb::save(std::string filename)
{
	FILE * fp = fopen(filename.c_str(),"w");
	if (!fp)
	{
		perror(filename.c_str());
		return false;
	}
	for (svOCRCharDbEntryList::iterator it=dic.begin();it!=dic.end();it++)
	{
		fprintf(fp,"%s %04d %s\n",it->second.hash.c_str(),it->second.majSize,it->second.value.c_str());
	}
	fclose(fp);
	return true;
}

/*******************  FUNCTION  *********************/
bool svOCRCharDb::load(std::string filename)
{
	FILE * fp = fopen(filename.c_str(),"r");
	char buffer1[2048];
	char buffer2[2048];
	svOCRDicEntry entry;
	int res;
	int line=1;
	int majSize;
	if (!fp)
	{
		perror(filename.c_str());
		return false;
	}
	while (!feof(fp))
	{
		res = fscanf(fp,"%s %d %s\n",buffer1,&majSize,buffer2);
		if (res!=3)
		{
			cerr << "Bad format on line " << line << "of file " << filename << endl;
			return false;
		} else {
			entry.hpos=buffer1[0];
			entry.hash=buffer1;
			entry.value=buffer2;
			entry.majSize=majSize;
			this->dic[entry.hash] = entry;
			this->addHeuristic(entry.hash,majSize,entry.value);
		}
		line++;
	}

	fclose(fp);

	return true;
}

/*******************  FUNCTION  *********************/
void svOCRCharDb::addEmptyMark(void)
{
	this->addEntry("i0000000099999999",9999,"-EM-");
}

/*******************  FUNCTION  *********************/
svOCRCharDbEntryList svOCRCharDb::getDic()
{
	return dic;
}

/*******************  FUNCTION  *********************/
svOCRHeuristicAnswer svOCRCharDb::askToGodOfChar(svOCRHeuristic & heur,bool testMode)
{
	svOCRHeuristicAnswer res;
	string * val1=NULL;
	string * val2=NULL;
	res.dist1 = 10000000000000000.0;
	res.dist2 = 10000000000000000.0;
	res.heur1 = NULL;
	res.heur2 = NULL;
	res.hasSome = false;
	float cur = 0.0;
	float coordDist[SVOCR_HEUR_NB_COORD];
	for (list<svOCRHeuristicSimple>::iterator it=heurDb.begin();it!=heurDb.end();it++)
	//for (int i=0;i<sizeof(SV_INTERNAL_DB)/sizeof(svHeuristiqueSimple);i++)
	{
		cur = heur.getDistanceTo(*it,coordDist);
		if ((cur<res.dist1 || val1==NULL) && (testMode==false || cur>0.0))// && heur.applyCut(coordDist))
		{
			val2=val1;
			res.dist2=res.dist1;
			res.heur2=res.heur1;
			val1 = &it->value;
			res.dist1 = cur;
			res.heur1 = &(*it);
			res.hasSome = true;
			if (testMode)
				for (int i=0;i<SVOCR_HEUR_NB_COORD;i++)
					res.coordDist[i]=coordDist[i];
		}
	}
	if (val1!=NULL)
		res.ans1=*val1;
	if (val2!=NULL)
		res.ans2=*val2;
	return res;
}

/*******************  FUNCTION  *********************/
void svOCRCharDb::loadInternalHeurDb(void)
{
	svOCRHeuristicSimple simple;
	for (unsigned int i=0;i<sizeof(SVOCR_INTERNAL_DB)/sizeof(svOCRHeuristicSimpleBase);i++)
	{
		for (int j=0;j<SVOCR_HEUR_NB_COORD;j++)
			simple.coord[j]=SVOCR_INTERNAL_DB[i].coord[j];
		simple.value = SVOCR_INTERNAL_DB[i].value;
		this->heurDb.push_back(simple);
	}
}

/*******************  FUNCTION  *********************/
void svOCRCharDb::displayDistWithAll(svOCRHeuristic & heur)
{
	float min = 100000000000.0;
	string * val = NULL;
	float cur;
	int i=0;
	for (list<svOCRHeuristicSimple>::iterator it=heurDb.begin();it!=heurDb.end();it++)
	//for (int i=0;i<sizeof(SV_INTERNAL_DB)/sizeof(svHeuristiqueSimple);i++)
	{
		cur = heur.getDistanceTo(*it,NULL);

		printf("%i\t%s\t%f\n",i++,it->value.c_str(),cur);
		if ((cur<min || val==NULL))
		{
			val = &it->value;
			min = cur;
		}
	}
	printf("#Min was for %s, with %f\n",val->c_str(),min);
}
