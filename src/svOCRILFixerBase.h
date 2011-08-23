/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCRILFIXERBASE_H
#define SVOCRILFIXERBASE_H

/********************  HEADERS  *********************/
#include <list>
#include <string>

/*********************  CLASS  **********************/
class svOCRILFixerBase
{
	public:
		svOCRILFixerBase(void);
		virtual ~svOCRILFixerBase(void);
		virtual void registerPos(int pos);
		virtual std::string fixString(std::string value) const;
		virtual void clear(void);
		virtual bool forceAskingToUser(std::string value) const;
		virtual std::string hardFix(std::string value) const;
	protected:
		virtual void fixCharInString(std::string & value,int pos) const;
		std::list<int> registredPos;
};

#endif // SVOCRILFIXERBASE_H
