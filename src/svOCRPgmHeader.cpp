/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRPgmHeader.h"
#include <cstdlib>

/*******************  FUNCTION  *********************/
svOCRPgmHeader::svOCRPgmHeader(void)
{
	magNumber=BAD_FORMAT;
	width=0;
	height=0;
	greyMax=255;
}

/*******************  FUNCTION  *********************/
svOCRPgmHeader::svOCRPgmHeader(const svOCRPgmHeader & value)
{
	magNumber=value.magNumber;
	width=value.width;
	height=value.height;
	greyMax=value.greyMax;
	comment=value.comment;
}

/*******************  FUNCTION  *********************/
svOCRPgmHeader::~svOCRPgmHeader(void)
{
}

/*******************  FUNCTION  *********************/
bool svOCRPgmHeader::isWhitespace(char value)
{
	int i = 0;
	while (WHITESPACE[i] != '\0')
	{
		
		if (value == WHITESPACE[i])
			return true;
		i++;
	}
	return false;
}

/*******************  FUNCTION  *********************/
svOCRMagiqueNumber svOCRPgmHeader::checkMagiqueNumber(FILE *fp)
{
	char buffer[4];
	fread(buffer,sizeof(char), 3, fp);
	if (!(buffer[0]=='P' && (buffer[1]=='5' || buffer[1]=='2') && isWhitespace(buffer[2])))
		return BAD_FORMAT;
	if (buffer[1]=='2')
		return PLAINE_MODE;
	else
		return BINARY_MODE;
}

/*******************  FUNCTION  *********************/
bool svOCRPgmHeader::load(FILE *fp)
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
	std::string tmp;
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
						width=atoi(tmp.c_str());
						break;
					case 1:
						height=atoi(tmp.c_str());
						break;
					case 2:
						greyMax=atoi(tmp.c_str());
						break;
				}
				i++;
				}
		}
	fseek(fp,pos+3,SEEK_SET);
	return true;
}

/*******************  FUNCTION  *********************/
bool svOCRPgmHeader::save(FILE *fp)
{
	if (fp==NULL || magNumber==BAD_FORMAT)
		return false;
	//magic number
	fprintf(fp,"P%c\n",magNumber);
	fprintf(fp,"#%s\n",comment.c_str());
	fprintf(fp,"%d %d %d\n",width,height,greyMax);
	return true;
}

/*******************  FUNCTION  *********************/
std::string svOCRPgmHeader::getNextParam(char *s,int &pos)
{
	std::string result;
	if (s==NULL || pos<0)
		return result;
	while (isWhitespace(s[pos]))
		pos++;
	int debut=pos;
	while (!isWhitespace(s[pos]) || (s[pos]==' ' && s[debut]=='#'))
		pos++;
	s[pos]='\0';
	result = s+debut;
	pos++;
	return result;
}
