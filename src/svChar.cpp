/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svChar.h"
#include <iostream>
#include "svGlobalConfig.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
svChar::svChar()
{
	this->init();
}

/*******************  FUNCTION  *********************/
bool svChar::buildChar(svLine & line,int start)
{
	if (line.getIsOk()==false)
		return false;
	this->line = &line;
	this->img = line.getImage();
	if (this->img == NULL)
		return false;

	this->isOk = false;

	int min = 0;
	int anglemin = 0;
	int res;
	for (int i=1;i<SV_CHAR_ANGLE_MAX;i++)
	{
		res = this->buildChar(start,line.getHeight()/i);
		if (min==0 || (res!=0 && res<min))
		{
			anglemin = line.getHeight()/i;
			min=res;
		}
	}
	//cout << "min=" << res << endl;

	if (min!=0 && buildChar(start,anglemin)!=0)
	{
		this->isOk =true;
		return true;
	} else {
		return false;
	}
}

/*******************  FUNCTION  *********************/
void svChar::drawBorderOnPicture(void)
{
	if (isOk==false)
		return;
	int cnt=0;
	int delta=0;
	for (int y=line->getEnd();y>=line->getStart();y--)
	{
		if (cnt++==angle)
		{
			delta++;
			cnt=0;
		}
		this->img->setColor(start+delta,y,SV_DRAW_BORDER_COLOR);
		this->img->setColor(end+delta,y,SV_DRAW_BORDER_COLOR);
	}
}

/*******************  FUNCTION  *********************/
void svChar::reset(void)
{
	this->init();
}

/*******************  FUNCTION  *********************/
void svChar::whiteTheChar(COLOR color)
{
	if (isOk==false)
		return;
	int cnt=0;
	int delta=0;
	for (int y=line->getEnd();y>=line->getStart();y--)
	{
		if (cnt++==angle)
		{
			delta++;
			cnt=0;
		}
		for (int x=start;x<end;x++)
			if (img->getColor(x+delta,y)<SV_WHILE_CHAR_LIMIT)
				img->setColor(x+delta,y,color);
	}
}

/*******************  FUNCTION  *********************/
void svChar::init()
{
	angle = 0;
	img = NULL;
	line = NULL;
	start = 0;
	end = 0;
	isOk = false;
}

/*******************  FUNCTION  *********************/
int svChar::buildChar(int start,int angle)
{
	if (this->line == NULL || this->img == NULL)
		return 0;

	start-=this->line->getHeight()/(angle+1);
	//find starting point
	while (isEmptyLine(start,angle))
	{
		start++;
		if (start >= (int)img->lwidth)
			return 0;
	}
	this->start = start;

	//find starting point
	while (!isEmptyLine(start,angle) || start-this->start<SV_MIN_CHAR_WIDTH)
	{
		start++;
		if (start >= (int)img->lwidth)
			return 0;
	}
	this->end = start;
	this->angle = angle;

	return end-this->start;
}

/*******************  FUNCTION  *********************/
bool svChar::isEmptyLine(int pos,int angle)
{
	if (img==NULL || line ==NULL)
		return false;
	int cnt=0;
	int delta=0;
	for (int y=line->getEnd();y>=line->getStart();y--)
	{
		if (cnt++==angle)
		{
			delta++;
			cnt=0;
		}
		if (img->getColor(pos+delta,y)<SV_EMPTY_LINE_COLOR_LIMIT)
			return false;
	}

	return true;
}

/*******************  FUNCTION  *********************/
int svChar::getStart(void) const
{
	return this->start;
}

/*******************  FUNCTION  *********************/
int svChar::getEnd(void) const
{
	return this->end;
}

/*******************  FUNCTION  *********************/
image * svChar::getImage(void)
{
	return this->img;
}

/*******************  FUNCTION  *********************/
bool svChar::getIsOk(void) const
{
	return this->isOk;
}

/*******************  FUNCTION  *********************/
int svChar::getWidth(void) const
{
	return end-start+line->getHeight()/(angle);
}

/*******************  FUNCTION  *********************/
int svChar::getHeight(void) const
{
	if (line==NULL)
		return 0;
	else
		return this->line->getHeight();
}

/*******************  FUNCTION  *********************/
svLine * svChar::getLine(void)
{
	return this->line;
}

/*******************  FUNCTION  *********************/
int svChar::getMStart(void)
{
	return this->start+line->getHeight()/(angle+1)/2;
}
