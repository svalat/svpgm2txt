/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "chaine.h"

/********************  GLOBALS  *********************/
static char caractere_null;

/*******************  FUNCTION  *********************/
chaine::chaine(void)
{
	str=NULL;
	len=0;
	allocsize=0;
	this->change("");
}

/*******************  FUNCTION  *********************/
chaine::chaine(const chaine & value)
{
	len=0;
	allocsize=0;
	str=NULL;
	this->change(value);
}

/*******************  FUNCTION  *********************/
chaine::chaine(const char *value)
{
	len=0;
	allocsize=0;
	str=NULL;
	this->change(value);
}

/*******************  FUNCTION  *********************/
chaine::~chaine(void)
{
	if (this->str!=NULL)
		free(str);
}

/*******************  FUNCTION  *********************/
int minimum(int a,int b)
{
	if (a<b)
		return a;
	else
		return b;
}

/*******************  FUNCTION  *********************/
int maximum(int a,int b)
{
	if (a>b)
		return a;
	else
		return b;
}

/*******************  FUNCTION  *********************/
int chaine::taille(void) const
{
	return this->len;
}

const char *chaine::getstr(void) const
{
	return this->str;
}

/*******************  FUNCTION  *********************/
void chaine::add(const char *s)
{
	reall(this->len+ctaille(s));
	len+=strcopy(s,this->str+len);
}

void chaine::add(const chaine & s)
{
	reall(this->len+s.len);
	strcopy(s.str,this->str+len);
	this->len+=s.len;
}

/*******************  FUNCTION  *********************/
char chaine::getc(int pos) const
{
	//erreurs
	if (pos <0 || pos >= this->len)
		return '\0';
	//normal
	return this->str[pos];
}

/*******************  FUNCTION  *********************/
void chaine::maj(void)
{
	int i=0;
	while (this->str[i]!='\0')
		{
		if (str[i]>=97 && str[i]<=122)
			this->str[i]-=32;
		i++;
		}
}

/*******************  FUNCTION  *********************/
void chaine::min(void)
{
	int i=0;
	while (this->str[i]!='\0')
		{
		if (str[i]>=65 && str[i]<=90)
			this->str[i]+=32;
		i++;
		}
}

/*******************  FUNCTION  *********************/
char chaine::min(char c)
{
	if (c>=65 && c<=90)
		return c+32;
	else
		return c;
}

/*******************  FUNCTION  *********************/
bool chaine::comp(char c1,char c2, bool maj)
{
	if (maj)
		return (c1==c2);
	else
		return (min(c1)==min(c2));
}

/*******************  FUNCTION  *********************/
void chaine::ltrim(void)
{
	int i=0;
	while(this->str[i]!='\0' && this->contain(this->str[i],LISTETRIM))
		i++;
	strcopy(str+i,str);
	this->len-=i;
	reall(this->len);
}

/*******************  FUNCTION  *********************/
void chaine::rtrim(void)
{
	int i=len-1;
	while (i>0 && this->contain(this->str[i],LISTETRIM))
		i--;
	str[i+1]='\0';
	this->len=i+1;
	reall(this->len);
}

/*******************  FUNCTION  *********************/
void chaine::trim(void)
{
	this->ltrim();
	this->rtrim();
}

/*******************  FUNCTION  *********************/
bool chaine::contain(char c,const char *s)
{
	while (*s!='\0')
		{
		if (*s==c)
			return true;
		s++;
		}
	return false;
}

/*******************  FUNCTION  *********************/
/**
  * Retourne d'une chaine une sous chaine.
  * @param pos position de la sous chaine a retourner.
  * @param l longueur de la sous chaine a retourner. 
  *          Si l est negative on retourne alors la sous 
  *          chaine de longueur l qui precede pos;
  * @return Retourne une sous chaine de longueur l et en cas 
  *         d'erreur une chaine nulle.
**/
chaine chaine::getsubstr(int pos,int l) const
{
	chaine tmp;
	//erreur
	if (pos<0 || pos>=len || l==0)
		return tmp;
	 //cas l negatif (on ramene à l positif)
	if (l<0)
		{
		l=minimum(-l,pos);
		pos=maximum(0,pos-l);	
		}
	//on traite
	int tailletmp=minimum(l,len-pos+1);
	tmp.reall(tailletmp);
	tmp.len=tmp.strcopy(this->str+pos,tmp.str,tailletmp);
	return tmp;
}

/*******************  FUNCTION  *********************/
int chaine::find(const char *s, bool maj) const
{
	int i=0,j=0;
	while (this->str[i]!='\0')
		{
		j=0;
		while (comp(this->str[i+j],s[j],maj))
			{
			j++;
			if (s[j]=='\0')
				return i;
			}
		i++;
		}
	return -1;
}

/*******************  FUNCTION  *********************/
int chaine::find(const chaine & s, bool maj) const
{
	return find(s.str,maj);
}

/*******************  FUNCTION  *********************/
int chaine::toInt(void)
{
	return atoi(this->str);
}

/*******************  FUNCTION  *********************/
void chaine::change(const char *s)
{
	reall(ctaille(s));
	len=strcopy(s,this->str);
}

/*******************  FUNCTION  *********************/
void chaine::change(const chaine &s)
{
	reall(s.len);
	len=strcopy(s.str,this->str);
}

/*******************  FUNCTION  *********************/
int chaine::ctaille(const char *s)
{
	if (s==NULL)
		return 0;
	int i=0;
		while (s[i]!='\0')
			i++;
	return i;
}

/*******************  FUNCTION  *********************/
int chaine::strcopy(const char *src,char *dest,int l)
{
	//erreur
	MY_ASSERT(dest==NULL,"strcopy=>dest=NULL",return -1);
	if (src==NULL)
		return 0;
	int i=0;
	while(src[i]!='\0' && (l<0 || i<l))
	{
		dest[i]=src[i];
		i++;
	}
	dest[i]='\0';
	return i;
}

/*******************  FUNCTION  *********************/
void chaine::reall(unsigned int t)
{
	int needbloc=((t+1)/BLOC_SIZE)+1;
	if (this->allocsize!=needbloc)
		{
		if (str==NULL)
		{
			str=(char *)malloc(needbloc*BLOC_SIZE*sizeof(char));
			*str='\0';
		} else {
			str=(char *)realloc(str,needbloc*BLOC_SIZE*sizeof(char));
		}
		
		/*
		tmp=(char *)calloc(needbloc*BLOC_SIZE,sizeof(char));
		strcopy(this->str,tmp);
		if (this->str!=NULL)
			free(str);
		this->str=tmp;*/
		this->allocsize=needbloc;
		}
}

/*******************  FUNCTION  *********************/
bool chaine::compare(const char *s1, const char *s2)
{	
	int i=0;
	while(s1[i]!='\0' && s1[i]==s2[i])
		i++;
	return s2[i]==s1[i];
}

/*******************  FUNCTION  *********************/
bool chaine::inferieur(const char *s1, const char *s2, bool ouegal)const
{
	int i=0;
	while(s1[i]==s2[i] && s1[i]!='\0')
		i++;
	return s1[i] < s2[i] || (s1[i]==s2[i] && ouegal);
}

/*******************  FUNCTION  *********************/
chaine &chaine::operator=(const chaine &value)
{
	this->change(value);
	return *this;
}

/*******************  FUNCTION  *********************/
bool chaine::operator==(const chaine &value1)const
{
	return compare(this->str,value1.str);
}

/*******************  FUNCTION  *********************/
bool chaine::operator!=(const chaine &value1)const
{
	return !compare(this->str,value1.str);
}

/*******************  FUNCTION  *********************/
bool chaine::operator<(const chaine &value1)const
{
	return inferieur(this->str,value1.str);
}

/*******************  FUNCTION  *********************/
bool chaine::operator<=(const chaine &value1)const
{
	return inferieur(this->str,value1.str,true);
}

/*******************  FUNCTION  *********************/
bool chaine::operator>(const chaine &value1)const
{
	return !inferieur(this->str,value1.str,true);
}

/*******************  FUNCTION  *********************/
bool chaine::operator>=(const chaine &value1)const
{
	return !inferieur(this->str,value1.str);
}

/*******************  FUNCTION  *********************/
chaine &chaine::operator+=(const chaine &value1)
{
	this->add(value1);
	return *this;
}

/*******************  FUNCTION  *********************/
char &chaine::operator[](int rang)
{
	
	if (rang<0 || rang>this->len)
		{
		caractere_null='\0';
		return caractere_null;
		}
	else
		return this->str[rang];
}

/*******************  FUNCTION  *********************/
char chaine::operator[](int rang)const
{
	return this->getc(rang);
}

/*******************  FUNCTION  *********************/
chaine operator+(const chaine &value1, const chaine &value2)
{
	chaine value;//ok
	value.add(value1);
	value.add(value2);
	return value;
}

/*******************  FUNCTION  *********************/
ostream & operator<<(ostream &out, const chaine &value2)
{
	out << value2.str;
	return out;
}
