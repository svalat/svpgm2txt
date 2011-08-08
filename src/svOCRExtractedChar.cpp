/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 2009
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "svOCRExtractedChar.h"
#include "svOCRGlobalConfig.h"

/*********************  CONSTS  *********************/
static const char SVOCR_SUMCHARCONVERT[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ-abcdefghijklmnopqrstuvwxyz+0123456789";

/*******************  FUNCTION  *********************/
svOCRExtractedChar::svOCRExtractedChar(void)
	:img(1,1)
{
	this->init();
}

/*******************  FUNCTION  *********************/
svOCRImage svOCRExtractedChar::getImage()
{
	return this->img;
}

/*******************  FUNCTION  *********************/
int svOCRExtractedChar::getHeight()
{
	return img.getHeight();
}

/*******************  FUNCTION  *********************/
void svOCRExtractedChar::buildExtractedChar(svOCRChar & source)
{
	static int cnt=0;
	if (source.getIsOk()==false)
		return;

	this->source = &source;

	this->img.setSize(source.getWidth(),source.getHeight());
	this->img.clear(SVOCR_CLEAR_WITH_COLOR);
	for (int y=0;y<source.getHeight();y++)
		for (int x=0;x<source.getWidth();x++)
		{
			if (source.getImage()->getColor(x+source.getStart(),y+source.getLine()->getStart())==(unsigned char)1)
			{
				this->img.setColor(x,y,0);
			}
		}
	cnt++;
	char tmp[1024];
	sprintf(tmp,"out/img%04d.pgm",cnt);
	img.save(tmp);
	xoffset = source.getStart();
	yoffset = source.getLine()->getStart();
}

/*******************  FUNCTION  *********************/
unsigned char svOCRExtractedChar::traduct(char value)
{
	for (unsigned char i=0;i<sizeof(SVOCR_SUMCHARCONVERT);i++)
		if (value==SVOCR_SUMCHARCONVERT[i])
			return i;
	return -1;
}

/*******************  FUNCTION  *********************/
void svOCRExtractedChar::buildImgFromHash(std::string hash,svOCRImage & img)
{
	//char hpos = hash[0];
	std::string wstr = hash.substr(1,4);
	std::string hstr = hash.substr(5,4);
	int w = atoi(wstr.c_str());
	int h = atoi(hstr.c_str());
	img.setSize(w,h);
	img.clear(255);
	int pos = 9;
	unsigned char masq = 1;
	char tmp = hash[pos++];
	unsigned char tmp2 = traduct(tmp);
	unsigned char tmp3 = 0;
	for (int y=0;y<h;y++)
	{
		for (int x = 0; x<w; x++)
		{
			if ((tmp2 & masq)==0)
			{
				img.setColor(x,y,0);
				tmp3|=(0xFF & masq);
			}
			masq <<=1;
			if (masq == 1<<6)
			{
				tmp = hash[pos++];
				tmp2 = traduct(tmp);
				tmp3=0;
				masq = 1;
			}
		}
	}
}

/*******************  FUNCTION  *********************/
void svOCRExtractedChar::applyCrop(void)
{
	int top=0;
	int bottom=img.getHeight()-1;
	int left=0;
	int right=img.getWidth()-1;
	static int cnt=0;
	svOCRImage copy(img);
	//find up part
	while (img.hlineIsEmpty(top))
		top++;
	//find down
	while (img.hlineIsEmpty(bottom))
		bottom--;
	//find left
	while (img.vlineIsEmpty(left))
		left++;
	//find right
	while (img.vlineIsEmpty(right))
		right--;

	right++;
	bottom++;

	if (top > (int)img.getHeight()/2)
		this->hpos = 'b';
	else if (bottom < (int)img.getHeight()/2)
		this->hpos = 't';
	else
		this->hpos = 'm';

	//cout << "w=" << img.getWidth() << " h=" << img.getHeight() << " x1=" << left << " x2=" << right << " y1=" << top << " y2=" << bottom << endl;
	img.setSize(right-left,bottom-top);
	for (int y=0;y<(int)img.getHeight();y++)
		for (int x=0;x<(int)img.getWidth();x++)
			img.setColor(x,y,copy.getColor(x+left,y+top));
	cnt++;
	char tmp[1024];
	sprintf(tmp,"out/%c/img%04d-croped.pgm",hpos,cnt);
	img.save(tmp);
	xoffset+=left;
	yoffset+=top;
}

/*******************  FUNCTION  *********************/
int svOCRExtractedChar::getXOffset()
{
	return xoffset;
}

/*******************  FUNCTION  *********************/
int svOCRExtractedChar::getXEnd()
{
	return xoffset+img.getWidth();
}

/*******************  FUNCTION  *********************/
std::string svOCRExtractedChar::getHash(int majSize)
{
	char buffer[2048];
	char * cur =buffer;
	*(cur++)=hpos;
	cur+=sprintf(cur,"%04d%04d",img.getWidth(),img.getHeight());
	int pos=0;
	unsigned char tmp=0;
	for (int y=0;y<(int)img.getHeight();y++)
	{
		for (int x=0;x<(int)img.getWidth();x++)
		{
			tmp |= (img.getColor(x,y)>127)<<(pos++);
			if (pos==6)
			{
				*(cur++)=SVOCR_SUMCHARCONVERT[tmp];
				pos=0;
				tmp=0;
			}
		}
	}
	*(cur++)=SVOCR_SUMCHARCONVERT[tmp];
	//cur+=sprintf(cur,"%04d",majSize);
	*(cur)='\0';
	return buffer;
}

/*******************  FUNCTION  *********************/
std::string svOCRExtractedChar::askWhatItIs(void)
{
	int startx=this->source->getStart()-40;
	int endx=this->source->getStart()+img.getWidth()+40;
	SVOCR_COLOR color;
	if (startx<0)
	{
		endx-=startx;
		startx=0;
	}
	if (endx>(int)source->getImage()->getWidth())
	{
		startx-=endx - source->getImage()->getWidth() ;
		endx = source->getImage()->getWidth();
	}
	if (startx<0)
		startx=0;

	//cout << "startx=" << startx << " endx=" << endx << endl;

	for (int y=source->getLine()->getStart();y<source->getLine()->getEnd();y++)
	{
		for (int x=startx;x<endx;x++)
		{
			color = source->getImage()->getColor(x,y);
			if (color==255)
				printf(".");
			else if (x >= xoffset && x < xoffset + (int)img.getWidth() && img.getColor(x-xoffset,y-yoffset)==0 && color<127)
				printf("\e[47m#\e[0m");
			else
				printf("+");
		}
		printf("\n");
	}
	std::string res;
	cout << "Plase enter the corresponding char : ";
	cin >> res;
	return res;
}

/*******************  FUNCTION  *********************/
void svOCRExtractedChar::init()
{
	this->source = NULL;
	this->hpos = 0;
	this->isOk = false;
	this->xoffset = 0;
	this->yoffset = 0;
}
