/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRILSimpleFixer.h"

/*******************  FUNCTION  *********************/
bool svOCRILAskFixer::forceAskingToUser(std::string value) const
{
	return (value == "I" || value == "l");
}

/*******************  FUNCTION  *********************/
svOCRILForceValueFixer::svOCRILForceValueFixer(std::string forcedValue)
{
	this->forcedValue = forcedValue;
}

/*******************  FUNCTION  *********************/
std::string svOCRILForceValueFixer::hardFix(std::string value) const
{
	if (value == "I" || value == "l")
		return forcedValue;
	else
		return value;
}
