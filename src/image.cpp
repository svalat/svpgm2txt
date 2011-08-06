/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Gaspard Roland
                        Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "image.h"

/*******************  FUNCTION  *********************/
image::image(unsigned int width,unsigned int height)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->setSize(width,height);
	this->clear();
}

/*******************  FUNCTION  *********************/
image::image(const image &image)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->copy(image);
}

/*******************  FUNCTION  *********************/
image::image(const chaine & path)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->load(path);
}

/*******************  FUNCTION  *********************/
image::image(const char * path)
{
	this->bitmap=NULL;
	this->lwidth=0;
	this->lheight=0;
	this->load(path);
}

/*******************  FUNCTION  *********************/
image::~image(void)
{
	if (this->bitmap!=NULL)
		delete[] bitmap;
}

/*******************  FUNCTION  *********************/
inline int image::coord(int x,int y) const
{
	return y*this->lwidth+x;
}

/*******************  FUNCTION  *********************/
void image::setSize(unsigned int width, unsigned int height)
{
	if (this->lwidth == width && this->lheight==height)
		return;
	if (this->bitmap!=NULL)
		delete[] this->bitmap;
	if (width==0 || height==0)
		bitmap=NULL;
	else
		bitmap=new COLOR[height*width];
	this->lwidth=width;
	this->lheight=height;
}

/*******************  FUNCTION  *********************/
void image::copy(const image &image)
{
	if (image.bitmap==NULL)
		return;
	this->setSize(image.lwidth,image.lheight);
	int i=0;
	while (i<lheight*lwidth)
		{
		this->bitmap[i]=image.bitmap[i];
		i++;
		}
}

/*******************  FUNCTION  *********************/
COLOR image::getColor(int x,int y)const
{
	if (x>lwidth || x<0 || y>lheight || y<0)
		return defaultColor;
	return this->bitmap[coord(x,y)];
}

/*******************  FUNCTION  *********************/
void image::setColor(int x, int y, COLOR value)
{
	if (x>lwidth || x<0 || y>lheight || y<0)
		return;
	this->bitmap[coord(x,y)]=value;
}

/*******************  FUNCTION  *********************/
void image::rotate(rotateAngle angle)
{
/*********************
*  NE JAMAIS JAMAIS JAMAIS JAMAIS JAMAIS
*  FAIRE CA
*********************/
	if (bitmap==NULL)
		return;
	COLOR *src=this->bitmap;
	int width=lwidth-1;
	int height=lheight-1;
	int finI=lheight;
	int finJ=lwidth;
	if (lheight!=lwidth && angle!=FLIP)
		{
		this->bitmap=NULL;
		this->setSize(lheight,lwidth);
		cout << "reize -> " << (long int) src << "," << (long int) bitmap << endl;
		}
	else
		{
			finJ=lheight/2;
			finI=(lwidth+1)/2;
		}
	COLOR tmp;
	if (angle==FLIP)
		{
		for (int j=0;j<lheight/2;j++)
			for(int i=0;i<lwidth;i++)
				{
				tmp=src[coord(i,j)];
				bitmap[coord(i,j)]=src[coord(width-i,height-j)];
				bitmap[coord(width-i,height-j)]=tmp;
				}
		} else {
			for (int j=0;j<finJ;j++)
				for(int i=0;i<finI;i++)
					{
					if (angle==RIGHT_ROTATE)
						{
						tmp=src[coord(i,j)];
						bitmap[coord(i,j)]                =  src[j+(height-i)*lheight];
						if (lheight==lwidth)
							{
							bitmap[coord(j,height-i)]         =  src[coord(height-i,height-j)];
							bitmap[coord(height-i,height-j)]  =  src[coord(height-j,i)];
							bitmap[coord(height-j,i)]         =  tmp;
							}
						}
					else
						{
						tmp=src[coord(i,j)];
						bitmap[coord(i,j)]               =  src[width-j+i*lheight];
						if (lheight==lwidth)
							{
							bitmap[coord(width-j,i)]         =  src[coord(width-i,width-j)];
							bitmap[coord(width-i,width-j)]   =  src[coord(j,width-i)];
							bitmap[coord(j,width-i)]         =  tmp;
							}
						}
					}
		if (lheight!=lwidth)
			delete[] src;
	}
}

/*******************  FUNCTION  *********************/
void image::horizontalMirror(void)
{
	for (int y=0;y<lheight;y++)
		for(int x=0;x<lwidth/2;x++)
			{
			int tmp=this->bitmap[coord(x,y)];
			this->bitmap[coord(x,y)]=this->bitmap[coord(lwidth-x-1,y)];
			this->bitmap[coord(lwidth-x-1,y)]=tmp;
			}
}

/*******************  FUNCTION  *********************/
void image::verticalMirror(void)
{
	for (int y=0;y<lwidth;y++)
		for(int x=0;x<lheight/2;x++)
			{
			int tmp=this->bitmap[coord(y,x)];
			this->bitmap[coord(y,x)]=this->bitmap[coord(y,lheight-x-1)];
			this->bitmap[coord(y,lheight-x-1)]=tmp;
			}
}

/*******************  FUNCTION  *********************/
void image::line(int x1,int y1,int x2,int y2,COLOR color)
{
	if (color==defaultColor)
		return;
	int sens=1;
	if (x1==x2)
		{
		if (y1>y2)
			sens=-1;
		for (int i=y1;i!=y2;i+=sens)
			{
			if (i>0 || i<lheight || x1>0 || x1<lwidth)
				this->bitmap[coord(x1,i)]=color;
			}
		}
	else if (y1==y2)
		{
		if (x1>x2)
			sens=-1;
		for (int i=x1;i!=x2;i+=sens)
			{
			if (i>0 || i<lwidth || y1>0 || y1<lheight)
				this->bitmap[coord(i,y1)]=color;
			}
		}
	else if (y2-y1<x2-x1)
		{

			float a=(float)(y2-y1)/(float)(x2-x1);
			cout << a << endl;
			int b=y1;
			if (x1>x2)
				sens=-1;
			for (int i=x1;i!=x2+sens;i+=sens)
				{
				int y=(int)(a*(float)i)+b;
				if (i>0 || i<lwidth || y>0 || y<lheight)
					this->bitmap[coord(i,y)]=color;
				}
		}
	else
		{
			float a=(float)(x2-x1)/(float)(y2-y1);
			cout << a << endl;
			int b=x1;
			if (y1>y2)
				sens=-1;
			for (int i=y1;i!=y2+sens;i+=sens)
				{
				int x=(int)(a*(float)i)+b;
				if (i>0 || i<lheight || x>0 || x<lwidth)
					this->bitmap[coord(x,i)]=color;
				}
		}
}

/*******************  FUNCTION  *********************/
void image::line2(int xi,int yi,int xf,int yf,COLOR color)
{
	if(color==defaultColor || bitmap==NULL)
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
void image::square(int x, int y ,unsigned int width,unsigned int height, COLOR borderColor ,COLOR surfaceColor)
{
	if (borderColor==defaultColor && surfaceColor==defaultColor)
		return;
	if (borderColor==defaultColor)
		borderColor=surfaceColor;
	for (int py=maximum(y,0);py<minimum(lheight,y+height);py++)
		for (int px=maximum(x,0);px<minimum(lwidth,x+width);px++)
			{
			if (py==y || py==y+height-1 || px==x || px==x+width-1)
				this->bitmap[coord(px,py)]=borderColor;
			else
				if (surfaceColor!=defaultColor)
					this->bitmap[coord(px,py)]=surfaceColor;
			}
}

/*******************  FUNCTION  *********************/
void image::circle(int x, int y, int r,COLOR borderColor ,COLOR surfaceColor)
{

	if (r<0 || (borderColor==0 && surfaceColor==0))
		return;
	if (borderColor==defaultColor)
		borderColor=surfaceColor;
	if (r==0)
		this->setColor(x,y,borderColor);
	for (int i=-r;i<=r;i++)
		{
		float cosA=cosf(asinf(((float)i/(float)r)));
		int rfoiscosA=(int)(r*cosA);
		int e1=x-rfoiscosA;
		int e2=x+rfoiscosA;

		for (int j=e1;j<=e2;j++)
			if (j>0 && j<lwidth && i+y>0 && i+y<lheight)
				this->bitmap[coord(j,i+y)]=(j==e1 || e2==j)?borderColor:surfaceColor;
		}
}

/*******************  FUNCTION  *********************/
void image::clear(COLOR color)
{
	for (int y=0;y<lheight;y++)
		for (int x=0;x<lwidth;x++)
			{
			this->bitmap[coord(x,y)]=color;
			}
}

/*******************  FUNCTION  *********************/
void image::text(int x, int y, const char *s, COLOR color)
{
	if (s==NULL || color==defaultColor)
		return;
	int i=0,px=x;
	while(s[i]!='\0')
		{
		if (s[i]=='\n')
			{
			y+=SVFONT_CHAR_HEIGHT+1;
			px=x;
			}
		else
			{
			text(px,y,s[i],color);
			px+=SVFONT_CHAR_WIDTH;
			}
		i++;
		}
}

/*******************  FUNCTION  *********************/
void image::text(int x,int y,char c,COLOR color)
{
	if (color==defaultColor)
		return;
	int offset=c*SVFONT_CHAR_WIDTH*SVFONT_CHAR_HEIGHT;

	for (int py=maximum(0,-y);py<minimum(SVFONT_CHAR_HEIGHT,lheight-y);py++)
		for (int px=maximum(0,-x);px<minimum(SVFONT_CHAR_WIDTH,lwidth-x);px++)
			{
			if(SVFONT[offset+px*SVFONT_CHAR_WIDTH+SVFONT_CHAR_HEIGHT-py-1]==255)
				this->bitmap[coord(px+x,py+y)]=color;
			}
}

/*******************  FUNCTION  *********************/
void image::text(int x, int y, const chaine & s, COLOR color)
{

	this->text(x,y,s.getstr(),color);
}

/*******************  FUNCTION  *********************/
void image::print()const
{
	char buffer[lwidth+4];
	for (int x=0;x<lwidth+2;x++) cout << '-';cout << endl;
	for (int y=0;y<lheight;y++)
		{
		buffer[0]='|';
		for (int x=0;x<lwidth;x++)
			buffer[x+1]=tradColor[this->bitmap[coord(x,y)]];
		buffer[lwidth+1]='|';
		buffer[lwidth+2]='\n';
		buffer[lwidth+3]='\0';
		cout << buffer;
		}
	for (int x=0;x<lwidth+2;x++) cout << '-';cout << endl;
}

/*******************  FUNCTION  *********************/
bool image::save(const char *path)
{
	if (path==NULL|| this->bitmap==NULL)
		return false;
	FILE *fp=fopen(path,"w");
	if (fp==NULL)
		return false;
	pgmHeader head;
	head.magNumber=BINARY_MODE;
	head.width=lwidth;
	head.height=lheight;
	head.greyMax=255;
	head.comment="coucou c'est nous!!";
	head.save(fp);
	fwrite(this->bitmap,sizeof(COLOR),lwidth*lheight,fp);
	fclose(fp);
	return true;
}

/*******************  FUNCTION  *********************/
bool image::save(const chaine &path)
{
	save(path.getstr());
}

/*******************  FUNCTION  *********************/
bool image::load(const char *path)
{
	if (path==NULL)
		return false;
	FILE *fp=fopen(path,"r");
	if (fp==NULL)
		return false;
	pgmHeader head;
	if (!head.load(fp))
		return false;
	if (head.greyMax>(1<<8*sizeof(COLOR)))
		return false;
	int size;
	this->setSize(head.width,head.height);
	if (head.magNumber==BINARY_MODE)
		size=fread(this->bitmap,sizeof(COLOR),lheight*lwidth,fp);
	else
		size=loadP2(fp);
	if (size!=lheight*lwidth)
		return false;
	fclose(fp);
	return true;
}

/*******************  FUNCTION  *********************/
bool image::load(const chaine & path)
{
	return load(path.getstr());
}

/*******************  FUNCTION  *********************/
int image::loadP2(FILE *fp)
{
	if (fp==NULL)
		return 0;
	char buffer[71];
	buffer[0]='\0';
	chaine tmp;
	int pos=0;
	int i=0;
	while (i<lheight*lwidth)
		{
			if (buffer[pos]=='\0')
				{
				if(feof(fp))
					return i;
				fgets(buffer, 71,fp);
				pos=0;
				}
			tmp=pgmHeader::getNextParam(buffer,pos);
			if (tmp[0]!='#')
				{
				bitmap[i]=(COLOR)tmp.toInt();
				i++;
				}
		}
	return i;
}

/*******************  FUNCTION  *********************/
bool image::vlineIsEmpty(int x)
{
	if (x > this->lwidth )
		return false;
	for (int i=0;i<this->lheight;i++)
		if (this->getColor(x,i)<127)
			return false;
	return true;
}


/*******************  FUNCTION  *********************/
bool image::hlineIsEmpty(int y)
{
	if (y > this->lheight )
		return false;
	for (int i=0;i<this->lwidth;i++)
		if (this->getColor(i,y)<127)
			return false;
	return true;
}
