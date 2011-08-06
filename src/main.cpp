/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include "svOptions.h"
#include "svHeuristique.h"
#include "svOCR.h"

/**********************  USING  *********************/
using namespace std;

/*********************  STRUCT  *********************/
struct TestResStruct
{
	int cntOk;
	int cntOk2;
	int cntNo;
	int cntTot;
	float meanOk;
	float meanBad;
	float meanDist;
	float minBad;
	float maxOk;
	float distMins;
	float maxGood[SV_HEUR_NB_COORD];
};

/*******************  FUNCTION  *********************/
bool writeOnFile(std::string fname,std::string value)
{
	FILE * fp = fopen(fname.c_str(),"w");
	if (!fp)
	{
		perror(fname.c_str());
		return false;
	}
	fwrite(value.c_str(),sizeof(char),value.size(),fp);
	fclose(fp);
	return true;
}

/*******************  FUNCTION  *********************/
void normalRun(const svOptions & options)
{
	//loading DBS
	svOCR ocr(options);
	const list<string> & dbs = options.getDbs();
	for(list<string>::const_iterator it=dbs.begin();it!=dbs.end();it++)
		ocr.loadDb(*it);

	//batch all files
	const list<string> & batch = options.getBatch();
	string res;
	for(list<string>::const_iterator it=batch.begin();it!=batch.end();it++)
	{
		cout << "============== "<< *it <<" ===========" << endl;
		res = ocr.runOnImage(*it);
		writeOnFile(*it+".txt",res);
		cout << res << endl;
		if (options.hasSaveEach() && options.hasOutputDb())
			ocr.saveDb(options.getOutputDb());
	}

	//end save
	if (options.hasOutputDb())
		 ocr.saveDb(options.getOutputDb());
}

/*******************  FUNCTION  *********************/
void addMark(const svOptions & options)
{
	cout << "Add mark to DB " << options.getFileToMark()  << endl;
	svCharDb db;
	db.load(options.getFileToMark());
	db.addEmptyMark();
	db.save(options.getFileToMark());
}

/*******************  FUNCTION  *********************/
void calcHeuristique(const svOptions & options)
{
	svCharDb db;
	//load dbs
	const list<string> & dbs = options.getDbs();
	for(list<string>::const_iterator it=dbs.begin();it!=dbs.end();it++)
		db.load(*it);

	//get list
	list<svDicEntry> lst = db.getDic();
	svHeuristique heur;
	for(list<svDicEntry>::const_iterator it=lst.begin();it!=lst.end();it++)
	{
		if (!options.hasHeuristiqueChar() || options.getHeuristiqueChar()==it->value || options.getHeuristiqueChar()==it->hash)
		{
			heur.buildFromHash(it->hash,it->majSize);
			cout << "#======== " << it->value << " ========" << endl << heur;
			cout << "#hash = " << it->hash << endl;
			cout << "#CDEF = {" << heur.getCDefinition() << ",\"" << it->value << "\"}," << endl;
			if (options.hasHeuristiqueChar() && options.hasDisplayDist())
				db.displayDistWithAll(heur);
		}
	}
}

/*******************  FUNCTION  *********************/
TestResStruct runTest(svCharDb & ref,svCharDb & test,bool verbose=true)
{
	//get list
	list<svDicEntry> lst = test.getDic();
	svHeuristique heur;
	svHeuristiqueAnswer res;
	TestResStruct stat={0,0,0,0,0.0,0.0,0.0,0.0,0.0};
	int i;
	if (verbose)
	{
		for (i=0;i<SV_HEUR_NB_COORD;i++)
			stat.maxGood[i]=0.0;
	}
	for(list<svDicEntry>::const_iterator it=lst.begin();it!=lst.end();it++)
	{
		if (it->hpos=='i')
			continue;
		heur.buildFromHash(it->hash,it->majSize);
		res = ref.askToGodOfChar(heur,true);
		if (res.dist2>10000000)
			res.dist2=res.dist1;
		if (res.hasSome==true)
		{
			if(res.ans1 == it->value)
			{
				//if (res.dist2-res.dist1>0.5)
				stat.cntOk++;
				stat.meanOk+=res.dist1;
				if (res.dist1 > stat.maxOk || stat.maxOk==0.0)
					stat.maxOk = res.dist1;
				if (verbose)
				{
					printf("Bonne réponse : %s ( %f ) or %s ( %f ) must be %s ",res.ans1.c_str(),res.dist1,res.ans2.c_str(),res.dist2,it->value.c_str());
					//heur.printCoordDists(*res.heur1);
					cout << endl;
					for (i=0;i<SV_HEUR_NB_COORD;i++)
						if (stat.maxGood[i] < res.coordDist[i])
							stat.maxGood[i] = res.coordDist[i];
				}
			} else {
				stat.meanBad+=res.dist1;
				if (res.dist1 < stat.minBad || stat.minBad==0.0)
					stat.minBad = res.dist1;
				if (res.ans2 == it->value)
					stat.cntOk2++;
				if (verbose)
				{
					printf("Mauvaise réponse : %s ( %f ) or %s ( %f ) must be %s ",res.ans1.c_str(),res.dist1,res.ans2.c_str(),res.dist2,it->value.c_str());
					//heur.printCoordDists(*res.heur1);
					cout << endl;
				}
			}
		} else {
			stat.cntNo++;
		}
		stat.cntTot++;
	}

	stat.meanOk /= (float)stat.cntTot;
	stat.meanBad /= (float)stat.cntTot;
	stat.meanDist = stat.meanBad - stat.meanOk;
	stat.distMins = stat.maxOk - stat.minBad;
	if (stat.meanDist<0.0)
		stat.meanDist=-stat.meanDist;

	return stat;
}

/*******************  FUNCTION  *********************/
void testHeuristique(const svOptions & options)
{
	svCharDb db;
	//load dbs
	const list<string> & dbs = options.getDbs();
	for(list<string>::const_iterator it=dbs.begin();it!=dbs.end();it++)
		db.load(*it);

	svCharDb dbToTest;
	//load dbs
	const list<string> & dbs2 = options.getDbToTestHeuristique();
	for(list<string>::const_iterator it=dbs2.begin();it!=dbs2.end();it++)
		dbToTest.load(*it);

	TestResStruct stat = runTest(db,dbToTest);

	for (int i=0;i<SV_HEUR_NB_COORD;i++)
		cout << stat.maxGood[i] << ",";
	cout << endl;

	printf("Total : %d/%d = %f %%, sec : %d/%d = %f %% , no : %d/%d = %f %%\n",
		   stat.cntOk,stat.cntTot,100.0*(float)stat.cntOk/(float)stat.cntTot,
		   stat.cntOk2,stat.cntTot,100.0*(float)stat.cntOk2/(float)stat.cntTot,
			stat.cntNo,stat.cntTot,100.0*(float)stat.cntNo/(float)stat.cntTot);
}

/*******************  FUNCTION  *********************/
float searchCutPoint(svOptions & options,float * coefs,svCharDb &db,svCharDb & dbToTest)
{
	float res=0.01;
	float pok=100;
	while (pok>90)
	{

		res+=0.01;
	}
	return res;
}

/*******************  FUNCTION  *********************/
void searchCoefs(svOptions & options)
{
	svCharDb db;
	//load dbs
	const list<string> & dbs = options.getDbs();
	for(list<string>::const_iterator it=dbs.begin();it!=dbs.end();it++)
		db.load(*it);

	svCharDb dbToTest;
	//load dbs
	const list<string> & dbs2 = options.getDbToTestHeuristique();
	for(list<string>::const_iterator it=dbs2.begin();it!=dbs2.end();it++)
		dbToTest.load(*it);

	TestResStruct stat,max;
	float coefs[SV_HEUR_NB_COEFS];
	timeval t;
	gettimeofday(&t,NULL);
	srand(t.tv_usec);
	max.cntOk=0;
	max.distMins = 10000000000000000.0;
	max.maxOk = 100000000000000000.0;
	for (int i=0;i<400;i++)
	{
		svHeuristique::setRandomCoefs(options.getOptimizeMaxParam());
	 	stat = runTest(db,dbToTest,false);
		if (stat.cntOk > max.cntOk*0.95 && stat.meanDist > max.meanDist)
		{
			max=stat;
			svHeuristique::copyCoefs(coefs);
			printf("Run %d, get new max %f %%, with mean dist %f, minBad=%f , maxOk=%f\n",i,100.0*max.cntOk/max.cntTot,max.meanDist,max.minBad,max.maxOk);
		}
	}

	printf("End with : %d/%d = %f %%\n",
		   max.cntOk,max.cntTot,100.0*(float)max.cntOk/(float)max.cntTot);
	//cout << "float SV_COEFS[SV_HEUR_NB_COORD] = {";
	for (int i=0;i<SV_HEUR_NB_COEFS;i++)
		cout << coefs[i] << ",";
	//cout << "};";
	cout << endl;

}

/*******************  FUNCTION  *********************/
int main(int argc, char *argv[])
{
	svOptions options;
	options.loadParameters(argc,argv);
	//options.displayOptions();

	if (options.hasCoefs())
		svHeuristique::setCoefs(options.getCoefs());

	if (options.hasAddMark())
		addMark(options);
	else if (options.hasCalcHeuristique())
		calcHeuristique(options);
	else if (options.hasTestHeuristique() && options.hasOptimizeCoefs())
		searchCoefs(options);
	else if (options.hasTestHeuristique())
		testHeuristique(options);
	else
		normalRun(options);
	return EXIT_SUCCESS;
}
