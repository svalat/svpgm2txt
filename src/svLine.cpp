/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svLine.h"
#include "svGlobalConfig.h"

/*******************  FUNCTION  *********************/
svLine::svLine()
{
	this->init();
}

/*******************  FUNCTION  *********************/
void svLine::init()
{
	this->img = NULL;
	this->ystart = 0;
	this->yend = 0;
	this->isOk = false;
}

/*******************  FUNCTION  *********************/
bool svLine::buildLine(image & img,unsigned int startPoint)
{
	this->img = & img;
	this->isOk=false;
	//search to top point
	while (img.hlineIsEmpty(startPoint))
	{
		if (startPoint >= this->img->lheight)
			return false;
		startPoint++;
	}
	this->ystart = startPoint-2;

	//search bottom point
	while (!img.hlineIsEmpty(startPoint) || startPoint-this->ystart<SV_MIN_LINE_HEIGHT)
	{
		if (startPoint >= this->img->lheight)
			return false;
		startPoint++;
	}
	this->yend = startPoint +1;

	this->isOk=true;
	return true;
}

/*******************  FUNCTION  *********************/
int svLine::getHeight(void) const
{
	if (isOk)
		return this->yend-this->ystart;
	else
		return 0;
}

/*******************  FUNCTION  *********************/
void svLine::drawBorderOnPicture(void)
{
	if (this->isOk == false || this->img == NULL)
		return;
	this->img->line2(0,ystart,img->lwidth,ystart,SV_DRAW_BORDER_COLOR);
	this->img->line2(0,yend,img->lwidth,yend,SV_DRAW_BORDER_COLOR);
}

/*******************  FUNCTION  *********************/
int svLine::getStart(void) const
{
	return this->ystart;
}

/*******************  FUNCTION  *********************/
int svLine::getEnd(void) const
{
	return this->yend;
}

/*******************  FUNCTION  *********************/
image * svLine::getImage(void)
{
	return this->img;
}

/*******************  FUNCTION  *********************/
bool svLine::getIsOk(void) const
{
	return this->isOk;
}
