/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCR_HEURISTIQUE_H
#define SVOCR_HEURISTIQUE_H

/********************  HEADERS  *********************/
#include "svOCRExtractedChar.h"
#include <ostream>

/********************  MACRO  ***********************/
#define SVOCR_HEUR_NB_COORD 36
#define SVOCR_HEUR_INTERNAL_DB_SIZE 0
#define SVOCR_HEUR_NB_COEFS SVOCR_HEUR_NB_COORD+1

/*********************  STRUCT  *********************/
struct svOCRHeuristicSimpleBase
{
	float coord[SVOCR_HEUR_NB_COORD];
	const char * value;
};

/*********************  STRUCT  *********************/
struct svOCRHeuristicSimple
{
	float coord[SVOCR_HEUR_NB_COORD];
	std::string value;
};

/*********************  STRUCT  *********************/
struct svOCRHeuristicAnswer
{
	float dist1;
	std::string ans1;
	const svOCRHeuristicSimple * heur1;
	float dist2;
	std::string ans2;
	const svOCRHeuristicSimple * heur2;
	bool hasSome;
	float coordDist[SVOCR_HEUR_NB_COORD];
};

/*********************  CLASS  **********************/
/**
 * Class used to implement unknwon character regnition base on some heuristics which evaluate
 * the distance of an unknown char to those provided by char DB. It may permit to autoselect
 * new chars translation. (not perfect but interesting test).
 * It may be better to use fourrier transform recognition, but it was funny to test this approach.
 * @brief Implementation of an heuristic for unknown character recognition.
 * @author Sébastien Valat.
**/
class svOCRHeuristic
{
	public:
		svOCRHeuristic();
		void buildFromExtractedChar( svOCRExtractedChar & ch,int majSize);
		void buildFromHash(std::string hash,int majSize);
		std::string getCDefinition(void);
		friend std::ostream & operator << (ostream & out, const svOCRHeuristic &heur);
		std::string getValue(void);
		void setValue(std::string value);
		void setCoord(float coord[15]);
		svOCRHeuristicSimple getSimple();
		float getDistanceTo(const svOCRHeuristicSimple & simple,float * coordDist);
		void displayDistWithAll(void);
		static void setRandomCoefs(float max);
		static void copyCoefs(float * dest);
		static void setCoefs(std::string value);
		static void setCoefs(float * value);
		bool isAccepted(svOCRHeuristicAnswer & ans);
		void printCoordDists(const svOCRHeuristicSimple & simple);
		bool applyCut(float * coordDist);
	private:
		void init();
		void buildFromImage(svOCRImage & img,int majSize);
		void calcFillingLevel(svOCRImage & img);
		void calcCell(svOCRImage & img);
		void calcHSegm(svOCRImage & img);
		void calcVSegm(svOCRImage & img);
		void calcHPix(svOCRImage & img);
		void calcVPix(svOCRImage & img);
		void calcAngle(svOCRImage & img);
		void calcVertPos(std::string hash);
		void fit(int pos,int max,int nbSegm,float * res,float value=1.0);
		float fillingLevel;//Ratio of black pixel on white pixel.
		float hsegm[8];//Number of horizontal segments.
		float vsegm[8];//Number of vertical segments.
		float hpix[4];//Number of black pixel on the selected line.
		float vpix[4];//Number of black pixel on the selected column.
		float cell[4];//Number of block pixel on each parts of the image by spliting it in 4 parts.
		float angle[4];
		float aspect;
		float vertpos;
		float ratioToMaj;
		std::string value;
};

#endif
