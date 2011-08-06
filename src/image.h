/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Gaspard Roland
                        Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef IMAGE_H
#define IMAGE_H

/********************  HEADERS  *********************/
#include "chaine.h"
#include "pgmHeader.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "svFont.h"

/**********************  USING  *********************/
using namespace std;

/*********************  TYPES  **********************/
typedef unsigned char COLOR;

/********************  ENUM  ************************/
enum rotateAngle {RIGHT_ROTATE=90,LEFT_ROTATE=-90,LEFT_ROTATE2=270,FLIP=180};

/*********************  CONSTS  *********************/
const COLOR defaultColor=255;
#define REAPEAT_CHAR(c) c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c
const char tradColor[]={REAPEAT_CHAR(' '),REAPEAT_CHAR('.'),REAPEAT_CHAR(','),REAPEAT_CHAR(';'),REAPEAT_CHAR('!'),REAPEAT_CHAR('1'),REAPEAT_CHAR('?'),REAPEAT_CHAR('='),REAPEAT_CHAR('O'),REAPEAT_CHAR('0'),REAPEAT_CHAR('%'),REAPEAT_CHAR('&'),REAPEAT_CHAR('8'),REAPEAT_CHAR('$'),REAPEAT_CHAR('#'),'#'};
// .,;!1?=O0%&8$#

/*********************  CLASS  **********************/
class image
{
	public:
		image(unsigned int width, unsigned int height);
		image(const image &image);
		image(const chaine & path);
		image(const char * path);
		~image(void);
		COLOR getColor(int x,int y)const;
		void setColor(int x, int y, COLOR value);
		void rotate(rotateAngle angle);
		void horizontalMirror(void);
		void verticalMirror(void);
		void line(int x1,int y1,int x2,int y2,COLOR color);
		void line2(int xi,int yi,int xf,int yf,COLOR color);
		void square(int x, int y , unsigned int width, unsigned int height, COLOR borderColor ,COLOR surfaceColor=defaultColor);
		void circle(int x, int y, int r,COLOR borderColor ,COLOR surfaceColor=defaultColor);
		void clear(COLOR color=defaultColor);
		void text(int x, int y, const char *s, COLOR color);
		void text(int x, int y, const chaine & s, COLOR color);
		void text(int x,int y,char c,COLOR color);
		void print(void)const;
		bool save(const char *path);
		bool save(const chaine &path);
		bool load(const char *path);
		bool load(const chaine & path);
		bool hlineIsEmpty(int y);
		bool vlineIsEmpty(int x);
	//private:
		COLOR * bitmap;
		unsigned int lwidth,lheight;
		void setSize(unsigned int width, unsigned int height);
		void copy(const image &image);
		inline int coord(int x,int y) const;
		int loadP2(FILE * fp);
};

#endif
