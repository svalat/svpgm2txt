/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Gaspard Roland
                        Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef SVOCR_IMAGE_H
#define SVOCR_IMAGE_H

/********************  HEADERS  *********************/
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include "svOCRPgmHeader.h"

/**********************  USING  *********************/
using namespace std;

/*********************  TYPES  **********************/
typedef unsigned char SVOCR_COLOR;

/********************  MACRO  ***********************/
#define SVOCR_DEFAULT_COLOR 255

/*********************  CLASS  **********************/
class svOCRImage
{
	public:
		svOCRImage(unsigned int width, unsigned int height);
		svOCRImage(const svOCRImage &image);
		svOCRImage(const std::string & path);
		svOCRImage(const char * path);
		~svOCRImage(void);
		SVOCR_COLOR getColor(int x,int y)const;
		void setColor(int x, int y, SVOCR_COLOR value);
		void line(int xi,int yi,int xf,int yf,SVOCR_COLOR color);
		void clear(SVOCR_COLOR color=SVOCR_DEFAULT_COLOR);
		void print(void)const;
		bool save(const char *path);
		bool save(const std::string &path);
		bool load(const char *path);
		bool load(const std::string & path);
		bool hlineIsEmpty(int y);
		bool vlineIsEmpty(int x);
		void setSize(unsigned int width, unsigned int height);
		void copy(const svOCRImage &image);
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		
	protected:
		SVOCR_COLOR * bitmap;
		unsigned int lwidth,lheight;
		inline int coord(int x,int y) const;
		int loadP2(FILE * fp);
};

#endif
