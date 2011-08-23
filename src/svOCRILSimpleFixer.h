/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCRSIMPLEFIXER_H
#define SVOCRSIMPLEFIXER_H

/********************  HEADERS  *********************/
#include "svOCRILFixerBase.h"

/*********************  TYPES  **********************/
typedef svOCRILFixerBase svOCRILNullFixer;

/*********************  CLASS  **********************/
class svOCRILForceValueFixer : public svOCRILFixerBase
{
	public:
		svOCRILForceValueFixer(std::string forcedValue);
		std::string hardFix(std::string value) const;
	protected:
		std::string forcedValue;
};

/*********************  CLASS  **********************/
class svOCRILAskFixer : public svOCRILFixerBase
{
	public:
		bool forceAskingToUser(std::string value) const;
};

#endif // SVOCRSIMPLEFIXER_H
