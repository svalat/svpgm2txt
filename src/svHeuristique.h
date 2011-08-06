/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SV_HEURISTIQUE_H
#define SV_HEURISTIQUE_H

/********************  HEADERS  *********************/
#include "svExtractedChar.h"
#include <ostream>

#define SV_HEUR_NB_COORD 36
#define SV_HEUR_INTERNAL_DB_SIZE 0
#define SV_HEUR_NB_COEFS SV_HEUR_NB_COORD+1

/*********************  STRUCT  *********************/
struct svHeuristiqueSimpleBase
{
	float coord[SV_HEUR_NB_COORD];
	const char * value;
};

/*********************  STRUCT  *********************/
struct svHeuristiqueSimple
{
	float coord[SV_HEUR_NB_COORD];
	std::string value;
};

/*********************  STRUCT  *********************/
struct svHeuristiqueAnswer
{
	float dist1;
	std::string ans1;
	const svHeuristiqueSimple * heur1;
	float dist2;
	std::string ans2;
	const svHeuristiqueSimple * heur2;
	bool hasSome;
	float coordDist[SV_HEUR_NB_COORD];
};

/*********************  CLASS  **********************/
class svHeuristique
{
	public:
		svHeuristique();
		void buildFromExtractedChar( svExtractedChar & ch,int majSize);
		void buildFromHash(std::string hash,int majSize);
		std::string getCDefinition(void);
		friend std::ostream & operator << (ostream & out, const svHeuristique &heur);
		std::string getValue(void);
		void setValue(std::string value);
		void setCoord(float coord[15]);
		svHeuristiqueSimple getSimple();
		float getDistanceTo(const svHeuristiqueSimple & simple,float * coordDist);
		void displayDistWithAll(void);
		static void setRandomCoefs(float max);
		static void copyCoefs(float * dest);
		static void setCoefs(std::string value);
		static void setCoefs(float * value);
		bool isAccepted(svHeuristiqueAnswer & ans);
		void printCoordDists(const svHeuristiqueSimple & simple);
		bool applyCut(float * coordDist);
	private:
		void init();
		void buildFromImage(image & img,int majSize);
		void calcFillingLevel(image & img);
		void calcCell(image & img);
		void calcHSegm(image & img);
		void calcVSegm(image & img);
		void calcHPix(image & img);
		void calcVPix(image & img);
		void calcAngle(image & img);
		void calcVertPos(std::string hash);
		void fit(int pos,int max,int nbSegm,float * res,float value=1.0);
		float fillingLevel;//rapport pixel noir sur blanc
		float hsegm[8];//nombre de segment horizontal
		float vsegm[8];//nombre de segment vertical
		float hpix[4];//nombre de pixel noir sur la ligne;
		float vpix[4];//nombre de pixel noir sur la colonne;
		float cell[4];//nombre de pixel noir pour un découpage en 4 cellules
		float angle[4];
		float aspect;
		float vertpos;
		float ratioToMaj;
		std::string value;
};

#endif
