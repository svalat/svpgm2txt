/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.1
             DATE     : 08/2011
             AUTHOR   : Gaspard Roland
                        Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRImage.h"
#include <cstdlib>

/*********************  CONSTS  *********************/
#define SVOCR_REAPEAT_CHAR(c) c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c,c
const char colorConversionTable[]={SVOCR_REAPEAT_CHAR(' '),SVOCR_REAPEAT_CHAR('.'),SVOCR_REAPEAT_CHAR(','),SVOCR_REAPEAT_CHAR(';'),SVOCR_REAPEAT_CHAR('!'),SVOCR_REAPEAT_CHAR('1'),SVOCR_REAPEAT_CHAR('?'),SVOCR_REAPEAT_CHAR('='),SVOCR_REAPEAT_CHAR('O'),SVOCR_REAPEAT_CHAR('0'),SVOCR_REAPEAT_CHAR('%'),SVOCR_REAPEAT_CHAR('&'),SVOCR_REAPEAT_CHAR('8'),SVOCR_REAPEAT_CHAR('$'),SVOCR_REAPEAT_CHAR('#'),'#'};
// .,;!1?=O0%&8$#

/*******************  FUNCTION  *********************/
svOCRImage::svOCRImage(unsigned int width,unsigned int height)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->setSize(width,height);
	this->clear();
}

/*******************  FUNCTION  *********************/
svOCRImage::svOCRImage(const svOCRImage &image)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->copy(image);
}

/*******************  FUNCTION  *********************/
svOCRImage::svOCRImage(const std::string & path)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->load(path);
}

/*******************  FUNCTION  *********************/
svOCRImage::svOCRImage(const char * path)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->load(path);
}

/*******************  FUNCTION  *********************/
svOCRImage::~svOCRImage(void)
{
	if (this->bitmap!=NULL)
		delete[] bitmap;
}

/*******************  FUNCTION  *********************/
inline int svOCRImage::coord(int x,int y) const
{
	return y*this->lwidth+x;
}

/*******************  FUNCTION  *********************/
void svOCRImage::setSize(unsigned int width, unsigned int height)
{
	if (this->lwidth == width && this->lheight==height)
		return;
	if (this->bitmap!=NULL)
		delete[] this->bitmap;
	if (width==0 || height==0)
		bitmap=NULL;
	else
		bitmap=new SVOCR_COLOR[height*width];
	this->lwidth=width;
	this->lheight=height;
}

/*******************  FUNCTION  *********************/
void svOCRImage::copy(const svOCRImage &image)
{
	if (image.bitmap==NULL)
		return;
	this->setSize(image.lwidth,image.lheight);
	unsigned int i=0;
	while (i<lheight*lwidth)
		{
		this->bitmap[i]=image.bitmap[i];
		i++;
		}
}

/*******************  FUNCTION  *********************/
SVOCR_COLOR svOCRImage::getColor(int x,int y)const
{
	if (x>(int)lwidth || x<0 || y>(int)lheight || y<0)
		return SVOCR_DEFAULT_COLOR;
	return this->bitmap[coord(x,y)];
}

/*******************  FUNCTION  *********************/
void svOCRImage::setColor(int x, int y, SVOCR_COLOR value)
{
	if (x>(int)lwidth || x<0 || y>(int)lheight || y<0)
		return;
	this->bitmap[coord(x,y)]=value;
}

/*******************  FUNCTION  *********************/
unsigned int svOCRImage::getWidth() const
{
	return this->lwidth;
}

/*******************  FUNCTION  *********************/
unsigned int svOCRImage::getHeight() const
{
	return this->lheight;
}

/*******************  FUNCTION  *********************/
void svOCRImage::line(int xi,int yi,int xf,int yf,SVOCR_COLOR color)
{
	if(color==SVOCR_DEFAULT_COLOR || bitmap==NULL)
		return;
	int dx,dy,cumul,x,y ;
	x = xi ;
	y = yi ;
	dx = xf - xi ;
	dy = yf - yi ;
	bitmap[coord(x,y)]=color;
	cumul = dx / 2 ;
	for ( x = xi+1 ; x <= xf ; x++ )
		{
  		cumul += dy ;
  		if (cumul >= dx)
  			{
    			cumul -= dx ;
    			y += 1 ;
    			}
  		bitmap[coord(x,y)]=color;
  		}
}

/*******************  FUNCTION  *********************/
void svOCRImage::clear(SVOCR_COLOR color)
{
	for (unsigned int y=0;y<lheight;y++)
		for (unsigned int x=0;x<lwidth;x++)
			{
			this->bitmap[coord(x,y)]=color;
			}
}

/*******************  FUNCTION  *********************/
void svOCRImage::print()const
{
	char buffer[lwidth+4];
	for (unsigned int x=0;x<lwidth+2;x++) cout << '-';cout << endl;
	for (unsigned int y=0;y<lheight;y++)
		{
		buffer[0]='|';
		for (unsigned int x=0;x<lwidth;x++)
			buffer[x+1]=colorConversionTable[this->bitmap[coord(x,y)]];
		buffer[lwidth+1]='|';
		buffer[lwidth+2]='\n';
		buffer[lwidth+3]='\0';
		cout << buffer;
		}
	for (unsigned int x=0;x<lwidth+2;x++) cout << '-';cout << endl;
}

/*******************  FUNCTION  *********************/
bool svOCRImage::save(const char *path)
{
	if (path==NULL|| this->bitmap==NULL)
		return false;
	FILE *fp=fopen(path,"w");
	if (fp==NULL)
		return false;
	svOCRPgmHeader head;
	head.magNumber=BINARY_MODE;
	head.width=lwidth;
	head.height=lheight;
	head.greyMax=255;
	head.comment="coucou c'est nous!!";
	head.save(fp);
	fwrite(this->bitmap,sizeof(SVOCR_COLOR),lwidth*lheight,fp);
	fclose(fp);
	return true;
}

/*******************  FUNCTION  *********************/
bool svOCRImage::save(const std::string &path)
{
	return save(path.c_str());
}

/*******************  FUNCTION  *********************/
bool svOCRImage::load(const char *path)
{
	if (path==NULL)
		return false;
	FILE *fp=fopen(path,"r");
	if (fp==NULL)
		return false;
	svOCRPgmHeader head;
	if (!head.load(fp))
		return false;
	if (head.greyMax>(1<<8*sizeof(SVOCR_COLOR)))
		return false;
	size_t size;
	this->setSize(head.width,head.height);
	if (head.magNumber==BINARY_MODE)
		size=fread(this->bitmap,sizeof(SVOCR_COLOR),lheight*lwidth,fp);
	else
		size=loadP2(fp);
	if (size!=lheight*lwidth)
		return false;
	fclose(fp);
	return true;
}

/*******************  FUNCTION  *********************/
bool svOCRImage::load(const std::string & path)
{
	return load(path.c_str());
}

/*******************  FUNCTION  *********************/
int svOCRImage::loadP2(FILE *fp)
{
	if (fp==NULL)
		return 0;
	char buffer[71];
	buffer[0]='\0';
	std::string tmp;
	int pos=0;
	unsigned int i=0;
	while (i<lheight*lwidth)
		{
			if (buffer[pos]=='\0')
				{
				if(feof(fp))
					return i;
				fgets(buffer, 71,fp);
				pos=0;
				}
			tmp=svOCRPgmHeader::getNextParam(buffer,pos);
			if (tmp[0]!='#')
				{
				bitmap[i]=(SVOCR_COLOR)atoi(tmp.c_str());
				i++;
				}
		}
	return i;
}

/*******************  FUNCTION  *********************/
bool svOCRImage::vlineIsEmpty(int x)
{
	if (x > (int)this->lwidth )
		return false;
	for (int i=0;i<(int)this->lheight;i++)
		if (this->getColor(x,i)<127)
			return false;
	return true;
}


/*******************  FUNCTION  *********************/
bool svOCRImage::hlineIsEmpty(int y)
{
	if (y >(int) this->lheight )
		return false;
	for (int i=0;i<(int)this->lwidth;i++)
		if (this->getColor(i,y)<127)
			return false;
	return true;
}
