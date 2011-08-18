/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCRILSPELLFIXER_H
#define SVOCRILSPELLFIXER_H

/********************  HEADERS  *********************/
#include <list>
#include <string>

/*********************  STRUCT  *********************/
struct svOCRILSpellFixerWordVariant
{
	std::string variant_I;
	std::string variant_l;
};

/*********************  CLASS  **********************/
class svOCRILSpellFixer
{
	public:
		svOCRILSpellFixer(std::string lang);
		~svOCRILSpellFixer(void);
		void registerPos(int pos);
		std::string fixString(std::string value) const;
		void clear(void);
	protected:
		void fixCharInString(std::string & value,int pos) const;
		bool isSeparator(char value) const;
		svOCRILSpellFixerWordVariant getWord(std::string value,int pos) const;
		std::list<int> registredPos;
	private:
		struct AspellConfig * spell_config;
		struct AspellSpeller * spell_checker;
};

#endif // SVOCRILSPELLFIXER_H
