/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "pgmHeader.h"

/*******************  FUNCTION  *********************/
pgmHeader::pgmHeader(void)
{
	magNumber=BAD_FORMAT;
	width=0;
	height=0;
	greyMax=255;
}

/*******************  FUNCTION  *********************/
pgmHeader::pgmHeader(const pgmHeader & value)
{
	magNumber=value.magNumber;
	width=value.width;
	height=value.height;
	greyMax=value.greyMax;
	comment=value.comment;
}

/*******************  FUNCTION  *********************/
pgmHeader::~pgmHeader(void)
{
}

/*******************  FUNCTION  *********************/
magiqueNumber pgmHeader::checkMagiqueNumber(FILE *fp)
{
	char buffer[4];
	fread(buffer,sizeof(char), 3, fp);
	if (!(buffer[0]=='P' && (buffer[1]=='5' || buffer[1]=='2') && IS_WHITESPACE(buffer[2])))
		return BAD_FORMAT;
	if (buffer[1]=='2')
		return PLAINE_MODE;
	else
		return BINARY_MODE;
}

/*******************  FUNCTION  *********************/
bool pgmHeader::load(FILE *fp)
{
	if (fp==NULL)
		return false;
	magNumber=checkMagiqueNumber(fp);
	if (magNumber==BAD_FORMAT)
		return false;
	char buffer[PGM_BUFFER_SIZE];
	int size=fread(buffer,sizeof(char),PGM_BUFFER_SIZE,fp);
	buffer[size]='\0';
	int pos=0;
	chaine tmp;
	int i=0;
	while (i<3)
		{
			tmp=getNextParam(buffer,pos);
			if (tmp[0]=='#')
				comment=tmp;
			else 
				{
				switch (i)
				{
					case 0:
						width=tmp.toInt();
						break;
					case 1:
						height=tmp.toInt();
						break;
					case 2:
						greyMax=tmp.toInt();
						break;
				}
				i++;
				}
		}
	fseek(fp,pos+3,SEEK_SET);
	return true;
}
/**
  * [#] [c] [o] [m] [m] [e] [n] [t]
  * [\n] [\n]          //space
  * [2] [0] [0]   //largeur
  * [ ]		  //space
  * [2] [0] [0]   //hauteur
  * [ ]		  //space
  * [2] [5] [5]	  //maximum grey value
  * [\n]	  //space
  * []....        //data
 **/

/*******************  FUNCTION  *********************/
bool pgmHeader::save(FILE *fp)
{
	if (fp==NULL || magNumber==BAD_FORMAT)
		return false;
	//magic number
	fprintf(fp,"P%c\n",magNumber);
	fprintf(fp,"#%s\n",comment.getstr());
	fprintf(fp,"%d %d %d\n",width,height,greyMax);
	return true;
}

/*******************  FUNCTION  *********************/
chaine pgmHeader::getNextParam(char *s,int &pos)
{
	chaine result;
	if (s==NULL || pos<0)
		return result;
	while (IS_WHITESPACE(s[pos]))
		pos++;
	int debut=pos;
	while (!IS_WHITESPACE(s[pos]) || (s[pos]==' ' && s[debut]=='#'))
		pos++;
	s[pos]='\0';
	result.change(s+debut);
	pos++;
	return result;
}

//   *           |
//  [ ] [#] [c] [ ] [d] [\n] ...[\0] 
