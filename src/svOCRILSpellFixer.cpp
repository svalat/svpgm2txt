/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRILSpellFixer.h"
#include <aspell.h>
#include <iostream>
#include <cstdlib>

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
svOCRILSpellFixer::svOCRILSpellFixer(std::string lang)
{
	spell_config = new_aspell_config();
	aspell_config_replace(spell_config, "lang", lang.c_str()); 
	
	AspellCanHaveError * possible_err = new_aspell_speller(spell_config);

	if (aspell_error_number(possible_err) != 0)
	{
		cerr << aspell_error_message(possible_err) << endl;
		spell_checker = NULL;
		abort();
	} else {
		spell_checker = to_aspell_speller(possible_err); 
	}
}

/*******************  FUNCTION  *********************/
svOCRILSpellFixer::~svOCRILSpellFixer(void )
{
	delete_aspell_speller(spell_checker); 
}

/*******************  FUNCTION  *********************/
void svOCRILSpellFixer::clear ( void )
{
	registredPos.clear();
}

/*******************  FUNCTION  *********************/
string svOCRILSpellFixer::fixString ( string value ) const
{
	for (list<int>::const_iterator it = registredPos.begin() ; it != registredPos.end() ; ++it)
		fixCharInString(value,*it);
	return value;
}

/*******************  FUNCTION  *********************/
void svOCRILSpellFixer::registerPos ( int pos )
{
	registredPos.push_back(pos);
}

/*******************  FUNCTION  *********************/
bool svOCRILSpellFixer::isSeparator ( char value ) const
{
	const char separators[] = "\n:;,()[]=+*{} .!?-";
	for (unsigned int i = 0 ; i <= sizeof(separators) ; ++i)
		if (separators[i] == value)
			return true;
	return false;
}

/*******************  FUNCTION  *********************/
void svOCRILSpellFixer::fixCharInString(string& value, int pos) const
{
	svOCRILSpellFixerWordVariant word = getWord(value,pos);
	int variant_l = aspell_speller_check(spell_checker, word.variant_l.c_str(), word.variant_l.size());
	int variant_I = aspell_speller_check(spell_checker, word.variant_I.c_str(), word.variant_I.size());
	
	cerr << "default is : " << value[pos]  << endl;
	if (variant_l == 0 && variant_I != 0)
		value[pos] = 'I';
	else if (variant_l != 0 && variant_I == 0)
		value[pos] = 'l';
	cerr << "final is : " << value[pos]  << endl;
}

/*******************  FUNCTION  *********************/
svOCRILSpellFixerWordVariant svOCRILSpellFixer::getWord(string value, int pos) const
{
	svOCRILSpellFixerWordVariant res;
	int i = pos;
	
	//search starting pos
	while (pos > 0 && !isSeparator(value[i]))
		--i;
	
	//return to the first letter
	if (pos != 0)
		++i;
	
	//move until end of word
	while (pos < (int)value.size() && !isSeparator(value[i]))
	{
		if (i == pos)
		{
			res.variant_I += 'I';
			res.variant_l += 'l';
		} else {
			res.variant_I += value[i];
			res.variant_l += value[i];
		}
		++i;
	}
	
	return res;
}
