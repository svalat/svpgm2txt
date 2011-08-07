/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svExtractedChar.h"
#include <cstdio>
#include <cstring>
#include "svOCRGlobalConfig.h"

/*********************  CONSTS  *********************/
const char SUMCHARCONVERT[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ-abcdefghijklmnopqrstuvwxyz+0123456789";

/*******************  FUNCTION  *********************/
svExtractedChar::svExtractedChar(void)
	:img(1,1)
{
	this->init();
}

/*******************  FUNCTION  *********************/
image svExtractedChar::getImage()
{
	return this->img;
}

/*******************  FUNCTION  *********************/
int svExtractedChar::getHeight()
{
	return img.lheight;
}

/*******************  FUNCTION  *********************/
void svExtractedChar::buildExtractedChar(svChar & source)
{
	static int cnt=0;
	if (source.getIsOk()==false)
		return;

	this->source = &source;

	this->img.setSize(source.getWidth(),source.getHeight());
	this->img.clear(SV_CLEAR_WITH_COLOR);
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
unsigned char svExtractedChar::traduct(char value)
{
	for (unsigned char i=0;i<sizeof(SUMCHARCONVERT);i++)
		if (value==SUMCHARCONVERT[i])
			return i;
	return -1;
}

/*******************  FUNCTION  *********************/
void svExtractedChar::buildImgFromHash(std::string hash,image & img)
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
void svExtractedChar::applyCrop(void)
{
	int top=0;
	int bottom=img.lheight-1;
	int left=0;
	int right=img.lwidth-1;
	static int cnt=0;
	image copy(img);
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

	if (top>img.lheight/2)
		this->hpos = 'b';
	else if (bottom < img.lheight/2)
		this->hpos = 't';
	else
		this->hpos = 'm';

	//cout << "w=" << img.lwidth << " h=" << img.lheight << " x1=" << left << " x2=" << right << " y1=" << top << " y2=" << bottom << endl;
	img.setSize(right-left,bottom-top);
	for (int y=0;y<img.lheight;y++)
		for (int x=0;x<img.lwidth;x++)
			img.setColor(x,y,copy.getColor(x+left,y+top));
	cnt++;
	char tmp[1024];
	sprintf(tmp,"out/%c/img%04d-croped.pgm",hpos,cnt);
	img.save(tmp);
	xoffset+=left;
	yoffset+=top;
}

/*******************  FUNCTION  *********************/
int svExtractedChar::getXOffset()
{
	return xoffset;
}

/*******************  FUNCTION  *********************/
int svExtractedChar::getXEnd()
{
	return xoffset+img.lwidth;
}

/*******************  FUNCTION  *********************/
std::string svExtractedChar::getHash(int majSize)
{
	char buffer[2048];
	char * cur =buffer;
	*(cur++)=hpos;
	cur+=sprintf(cur,"%04d%04d",img.lwidth,img.lheight);
	int pos=0;
	unsigned char tmp=0;
	for (int y=0;y<img.lheight;y++)
	{
		for (int x=0;x<img.lwidth;x++)
		{
			tmp |= (img.getColor(x,y)>127)<<(pos++);
			if (pos==6)
			{
				*(cur++)=SUMCHARCONVERT[tmp];
				pos=0;
				tmp=0;
			}
		}
	}
	*(cur++)=SUMCHARCONVERT[tmp];
	cur+=sprintf(cur,"%04d",majSize);
	*(cur)='\0';
	return buffer;
}

/*******************  FUNCTION  *********************/
std::string svExtractedChar::askWhatItIs(void)
{
	int startx=this->source->getStart()-40;
	int endx=this->source->getStart()+img.lwidth+40;
	COLOR color;
	if (startx<0)
	{
		endx-=startx;
		startx=0;
	}
	if (endx>source->getImage()->lwidth)
	{
		startx-=endx - source->getImage()->lwidth ;
		endx = source->getImage()->lwidth;
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
			else if (x >= xoffset && x < xoffset + img.lwidth && img.getColor(x-xoffset,y-yoffset)==0 && color<127)
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
void svExtractedChar::init()
{
	this->source = NULL;
	this->hpos = 0;
	this->isOk = false;
	this->xoffset = 0;
	this->yoffset = 0;
}
