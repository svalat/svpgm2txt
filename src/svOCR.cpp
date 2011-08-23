/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCR.h"
#include "svOCRImage.h"
#include "svOCRGlobalConfig.h"
#include "svOCRILUpperCaseFixer.h"
#include "svOCRILSpellFixer.h"
#include "svOCRILSimpleFixer.h"
#include <cstdlib>

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
svOCR::svOCR(const svOCROptions & options)
{
	this->options = & options;
	this->majSize = 0;
	this->buildDefaultFixer();
	this->fixer = this->defaultFixer;
}

/*******************  FUNCTION  *********************/
svOCR::~svOCR(void )
{
	if (defaultFixer != NULL)
		delete defaultFixer;
}

/*******************  FUNCTION  *********************/
void svOCR::buildDefaultFixer(void )
{
	switch(options->getILFix())
	{
		case SVOCR_IL_FIX_NONE:
			defaultFixer = new svOCRILNullFixer();
			break;
		case SVOCR_IL_FIX_ALLWAYS_ASK:
			defaultFixer = new svOCRILAskFixer();
			break;
		case SVOCR_IL_FIX_ASPELL:
			defaultFixer = new svOCRILSpellFixer(options->getSpellLang());
			break;
		case SVOCR_IL_FIX_FORCE_I:
			defaultFixer = new svOCRILForceValueFixer("I");
			break;
		case SVOCR_IL_FIX_FORCE_L:
			defaultFixer = new svOCRILForceValueFixer("l");
			break;
		case SVOCR_IL_FIX_UPPER_CASE:
			defaultFixer = new svOCRILUpperCaseFixer();
			break;
		default:
			cerr << "Unsupported I/l fixer " << options->getILFix() << endl;
			abort();
			break;
	};
}

/*******************  FUNCTION  *********************/
void svOCR::setFixer(svOCRILFixerBase* fixer)
{
	this->fixer = fixer;
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
			hash = extrChr.getHash(majSize);
			cur = this->db.getValue(hash);
			
			//special fix, ask for each I/l
			if (fixer->forceAskingToUser(cur))
				cur = requestUnknown(extrChr,hash);
			
			//if not found
			if (cur == SVOCR_DB_NOT_FOUND)
				cur = requestUnknown(extrChr,hash);
			
			//special fix, force I/l value
			cur = fixer->hardFix(cur);

			//check space
			//cout << "lastm=" << lastm << "  new=" << chr.getMStart() << endl;
			if (lastm!=-1 && extrChr.getXOffset() - lastm > this->options->getWhitespaceWidth())
				res += ' ';
			lastm = extrChr.getXEnd();

			//if maj we take the size
			if (isMaj(cur))
				majSize = extrChr.getHeight();
			//add the string
			if (cur != SVOCR_IGNORE_STRING)
			{
				if (cur == "I" || cur == "l")
					fixer->registerPos(res.size());
				res+=cur;
			}

			//mask the done chars
			//c.drawBorderOnPicture();
			chr.whiteTheChar();
		}
		res+='\n';
		chr.reset();
		lastm = -1;
	}
	
	//final fixes
	res = fixer->fixString(res);
	
	//final clear of fixer
	fixer->clear();

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

