/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRILFixerBase.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
svOCRILFixerBase::svOCRILFixerBase()
{

}

/*******************  FUNCTION  *********************/
svOCRILFixerBase::~svOCRILFixerBase()
{

}

/*******************  FUNCTION  *********************/
void svOCRILFixerBase::clear(void )
{
	registredPos.clear();
}

/*******************  FUNCTION  *********************/
std::string svOCRILFixerBase::fixString(std::string value) const
{
	for (list<int>::const_iterator it = registredPos.begin() ; it != registredPos.end() ; ++it)
		fixCharInString(value,*it);
	return value;
}

/*******************  FUNCTION  *********************/
void svOCRILFixerBase::fixCharInString(std::string& value, int pos) const
{

}

/*******************  FUNCTION  *********************/
void svOCRILFixerBase::registerPos(int pos)
{
	registredPos.push_back(pos);
}

/*******************  FUNCTION  *********************/
bool svOCRILFixerBase::forceAskingToUser(string value) const
{
	return false;
}

/*******************  FUNCTION  *********************/
string svOCRILFixerBase::hardFix(string value) const
{
	return value;
}
