/*****************************************************
             PROJECT  : svpgm2txt
             VERSION  : 1.2
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRLine.h"
#include "svOCRGlobalConfig.h"

/*******************  FUNCTION  *********************/
svOCRLine::svOCRLine()
{
	this->init();
}

/*******************  FUNCTION  *********************/
void svOCRLine::init()
{
	this->img = NULL;
	this->ystart = 0;
	this->yend = 0;
	this->isOk = false;
}

/*******************  FUNCTION  *********************/
bool svOCRLine::buildLine(svOCRImage & img,unsigned int startPoint)
{
	this->img = & img;
	this->isOk=false;
	//search to top point
	while (img.hlineIsEmpty(startPoint))
	{
		if (startPoint >= this->img->getHeight())
			return false;
		startPoint++;
	}
	this->ystart = startPoint-2;

	//search bottom point
	while (!img.hlineIsEmpty(startPoint) || startPoint-this->ystart<SVOCR_MIN_LINE_HEIGHT)
	{
		if (startPoint >= this->img->getHeight())
			return false;
		startPoint++;
	}
	this->yend = startPoint +1;

	this->isOk=true;
	return true;
}

/*******************  FUNCTION  *********************/
int svOCRLine::getHeight(void) const
{
	if (isOk)
		return this->yend-this->ystart;
	else
		return 0;
}

/*******************  FUNCTION  *********************/
void svOCRLine::drawBorderOnPicture(void)
{
	if (this->isOk == false || this->img == NULL)
		return;
	this->img->line(0,ystart,img->getWidth(),ystart,SVOCR_DRAW_BORDER_COLOR);
	this->img->line(0,yend,img->getWidth(),yend,SVOCR_DRAW_BORDER_COLOR);
}

/*******************  FUNCTION  *********************/
int svOCRLine::getStart(void) const
{
	return this->ystart;
}

/*******************  FUNCTION  *********************/
int svOCRLine::getEnd(void) const
{
	return this->yend;
}

/*******************  FUNCTION  *********************/
svOCRImage * svOCRLine::getImage(void)
{
	return this->img;
}

/*******************  FUNCTION  *********************/
bool svOCRLine::getIsOk(void) const
{
	return this->isOk;
}
