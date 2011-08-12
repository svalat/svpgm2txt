/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCRILUPPERCASEFIXER_H
#define SVOCRILUPPERCASEFIXER_H

/********************  HEADERS  *********************/
#include <list>
#include <string>

/*********************  CLASS  **********************/
class svOCRILUpperCaseFixer
{
	public:
		void registerPos(int pos);
		std::string fixString(std::string value) const;
		void clear(void);
	protected:
		void fixCharInString(std::string & value,int pos) const;
		bool isAnUpperWord(const std::string & value,int pos) const;
		bool isASmipleI(const std::string & value, int pos) const;
		bool isAFirstWordInSentance(const std::string & value,int pos) const;
		bool isFollowedByUpperCase(char value) const;
		bool isSeparator(char value) const;
		bool isUpperCase(char value) const;
		std::list<int> registredPos;
};

#endif // SVOCRILUPPERCASEFIXER_H
