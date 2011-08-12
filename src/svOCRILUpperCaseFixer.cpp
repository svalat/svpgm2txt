/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRILUpperCaseFixer.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
void svOCRILUpperCaseFixer::clear ( void )
{
	registredPos.clear();
}

/*******************  FUNCTION  *********************/
string svOCRILUpperCaseFixer::fixString ( string value ) const
{
	for (list<int>::const_iterator it = registredPos.begin() ; it != registredPos.end() ; ++it)
		fixCharInString(value,*it);
	return value;
}

/*******************  FUNCTION  *********************/
void svOCRILUpperCaseFixer::fixCharInString ( string& value, int pos ) const
{
	//errors
	if (pos < 0 || pos > (int)value.size())
		return;
	
	//check if simple '... I ...'
	if (isASmipleI(value,pos))
		value[pos] = 'I';
	else if (isAnUpperWord(value,pos))
		value[pos] = 'I';
	else if (isAFirstWordInSentance(value,pos))
		value[pos] = 'I';
	else
		value[pos] = 'l';
}

/*******************  FUNCTION  *********************/
void svOCRILUpperCaseFixer::registerPos ( int pos )
{
	registredPos.push_back(pos);
}

/*******************  FUNCTION  *********************/
bool svOCRILUpperCaseFixer::isAFirstWordInSentance ( const std::string& value, int pos ) const
{
	//find start pos of word
	int i = 0;
	while (i > 0 && !isSeparator(value[i]))
		--i;
	
	//find previous word or a end sentance separator
	while(i > 0 && isSeparator(value[i]))
	{
		if (isFollowedByUpperCase(value[i]))
			return true;
		--i;
	}
	
	return (i==0);
}

/*******************  FUNCTION  *********************/
bool svOCRILUpperCaseFixer::isAnUpperWord ( const std::string& value, int pos ) const
{
	unsigned int i = pos;
	//search starting pos
	while (pos > 0 && !isSeparator(value[i]))
		--i;
	
	//move until end of word an check if get only uppercase (return false if found a lower case char).
	while (pos < (int)value.size() && !isSeparator(value[i]))
	{
		if (isUpperCase(value[i]) == false)
			return false;
		++i;
	}
	
	return true;
}

/*******************  FUNCTION  *********************/
bool svOCRILUpperCaseFixer::isASmipleI ( const std::string& value, int pos ) const
{
	return (isSeparator(value[pos-1]) && isSeparator(value[pos+1]));
}

/*******************  FUNCTION  *********************/
bool svOCRILUpperCaseFixer::isFollowedByUpperCase ( char value ) const
{
	const char separators[] = ".!?";
	for (unsigned int i = 0 ; i < sizeof(separators) ; ++i)
		if (separators[i] == value)
			return true;
	return false;
}

/*******************  FUNCTION  *********************/
bool svOCRILUpperCaseFixer::isSeparator ( char value ) const
{
	const char separators[] = "\n:;,()[]=+*{} .!?";
	for (unsigned int i = 0 ; i <= sizeof(separators) ; ++i)
		if (separators[i] == value)
			return true;
	return false;
}

/*******************  FUNCTION  *********************/
bool svOCRILUpperCaseFixer::isUpperCase ( char value ) const
{
	return (value >= 'A' && value <= 'Z');
}
