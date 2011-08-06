/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CHAINE_H
#define CHAINE_H

/********************  HEADERS  *********************/
#include <stdlib.h>
#include <iostream>

/**********************  USING  *********************/
using namespace std;

/********************  MACRO  ***********************/
#define BLOC_SIZE 8 
#define LISTETRIM " \t"
#define MY_ASSERT(test,msg,douceur) if (test) {cerr << "[ASSERTION]" << msg << endl;exit(1);}

/*******************  FUNCTION  *********************/
int minimum(int a,int b);
int maximum(int a,int b);

/*********************  CLASS  **********************/
/**
 * Prend en charge une gestion simplifié des chaines
 * de caractère sous forme d'un objet C++ pour masquer
 * les problème de gestion de mémoire associé
 * au travail de chaine.
 * @author GASPARD Roland
 * @author VALAT Sebastien
 * @version >=0.0.1 
 * @brief Chaine de caracteres
 * @todo A faire (ajouter ...)
 * @deprecated A supprimer pour version >= 0.0.2
 * @bug BUG
**/
class chaine
{
	public:
		chaine(void);//constructeur
		chaine(const chaine & value);//constructeur de RECOPIE
		chaine(const char *value);
		~chaine(void);//destructeur
		int taille(void) const;//taille de la chaine
		const char *getstr(void) const;//retourne un ptr sur la chaine
		void add(const char *s);//concat deux chaines
		void add(const chaine & s);//concat deux chaines
		char getc(int pos) const;//retourne le caractere a la position pos
		void maj(void);//met en maj
		void min(void);//met en min
		void trim(void);//
		void ltrim(void);//
		void rtrim(void);//
		chaine getsubstr(int pos,int l) const;//
		int find(const char *s, bool maj=true) const;//
		int find(const chaine & s, bool maj=true) const;//
		void change(const char *s);
		void change(const chaine & s);
		static bool compare(const char *s1, const char *s2);
		bool inferieur(const char *s1, const char *s2, bool ouegal=false)const;
		chaine &operator=(const chaine &value);
		bool operator==(const chaine &value1)const;
		bool operator!=(const chaine &value1)const;
		bool operator<(const chaine &value1)const;
		bool operator>(const chaine &value1)const;
		bool operator<=(const chaine &value1)const;
		bool operator>=(const chaine &value1)const;
		chaine &operator+=(const chaine &value1);
		char &operator[](int rang);
		char operator[](int rang)const;
		static int ctaille(const char *s);
		static bool contain(char c,const char *s);
		static bool comp(char c1,char c2, bool maj);
		int toInt(void);
		// = == != >< <= >= + +=  -> * new delete & | && || . ++ --
	private:
		/**pointeur sur la chaine C**/
		char *str;
		/**longueur de la chaine**/
		int len;
		int allocsize;
		int strcopy(const char *src,char *dest,int l=-1);
		void reall(unsigned int t);
		static char min(char c);
		
	//fonction amies
	friend chaine operator+(const chaine &value1, const chaine &value2);
	friend ostream & operator<<(ostream &out, const chaine &value2);
};

#endif
