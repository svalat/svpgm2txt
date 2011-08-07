/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 2009
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCR.h"
#include "svOCRImage.h"
#include "svOCRGlobalConfig.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
svOCR::svOCR(const svOCROptions & options)
{
	this->options = & options;
	this->majSize = 0;
}

/*******************  FUNCTION  *********************/
std::string svOCR::runOnImage(std::string path)
{
	svOCRImage img(1,1);
	if (!img.load(path.c_str()))
	{
		perror("");
		cerr << "Error while reading file " << path << endl;
		return "";
	}

	svOCRLine line;
	svOCRChar chr;
	svOCRExtractedChar extrChr;
	string res;
	string hash;
	string cur;
	int lastm = -1;

	while (line.buildLine(img,line.getEnd()+1))
	{
		//line.drawBorderOnPicture();
		while (chr.buildChar(line,chr.getEnd()+1))
		{
			//extract
			chr.whiteTheChar((SVOCR_COLOR)1);
			extrChr.buildExtractedChar(chr);
			extrChr.applyCrop();

			//get the string
			hash=extrChr.getHash(majSize);
			cur=this->db.getValue(hash);
			if (cur==SVOCR_DB_NOT_FOUND)
				cur = requestUnknown(extrChr,hash);

			//check space
			//cout << "lastm=" << lastm << "  new=" << chr.getMStart() << endl;
			if (lastm!=-1 && extrChr.getXOffset() - lastm >SVOCR_OCR_SPACE_DETECTION)
				res+=' ';
			lastm = extrChr.getXEnd();

			//if maj we take the size
			if (isMaj(cur))
				majSize = extrChr.getHeight();
			//add the string
			res+=cur;

			//mask the done chars
			//c.drawBorderOnPicture();
			chr.whiteTheChar();
		}
		res+='\n';
		chr.reset();
		lastm = -1;
	}

	return res;
}

/*******************  FUNCTION  *********************/
std::string svOCR::requestUnknown(svOCRExtractedChar & extrChr,std::string & hash)
{
	std::string value;
	bool found =false;
	if (options->hasUseHeuristics())
	{
		svOCRHeuristic heur;
		svOCRHeuristicAnswer ans;
		heur.buildFromExtractedChar(extrChr,majSize);
		ans = db.askToGodOfChar(heur);
		cout << "Heuristic say " << ans.ans1 << " with dist " << ans.dist1 << endl;
		if (ans.hasSome && heur.isAccepted(ans))
		{
			cout << "It was accepted" << endl;
			value = ans.ans1;
//			if (majSize>0)
// 				this->db.addEntry(hash,value,false);
// 			found=true;
		}
	}
	if (found==false)
	{
		value = extrChr.askWhatItIs();
		if (majSize>0)
			this->db.addEntry(hash,majSize,value,true);
	}
	return value;
}

/*******************  FUNCTION  *********************/
bool svOCR::isMaj(std::string & value)
{
	return (value[0]>='A' && value[0]<='Z');
}

/*******************  FUNCTION  *********************/
bool svOCR::loadDb(std::string filename)
{
	return this->db.load(filename);
}

/*******************  FUNCTION  *********************/
bool svOCR::saveDb(std::string filename)
{
	return this->db.save(filename);
}

