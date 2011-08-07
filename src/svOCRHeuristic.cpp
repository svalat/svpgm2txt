/*****************************************************
             PROJECT  : svPgm2Txt
             VERSION  : 1.0
             DATE     : 08/2011
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "svOCRHeuristic.h"
#include <cmath>

/********************  MACRO  ***********************/
#define DUPL2(x) x,x
#define DUPL4(x) x,x,x,x
#define DUPL8(x) DUPL4(x),DUPL4(x)
#define NORME(x) sqrt((x)*(x))
#define SQR(x) ((x)*(x))
#define SET_DUPL2(x,val) (x)[0]=(x)[1]=val;
#define SET_DUPL4(x,val) (x)[0]=(x)[1]=(x)[2]=(x)[3]=val;
#define SET_DUPL8(x,val) SET_DUPL4((x),SET_DUPL4((x+4),val))

// float SVOCR_COEFS[SV_HEUR_NB_COEFS] =
// {
// 	0.0,
// 	0.0,
// 	0.0,
// 	QAUTRE(0.0),
// 	QAUTRE(0.0),
// 	QAUTRE(0.0),
// 	QAUTRE(0.0),
// 	DEUX(0.0),
// 	DEUX(0.0),
// 	QAUTRE(0.0),
// 	DEUX(0.0),
// 	DEUX(0.0),
// 	QAUTRE(0.0),
// 	0.0};

// float SVOCR_COEFS[SV_HEUR_NB_COEFS] =
// {
// 	3.8,
// 	9.1,
// 	2.6,
// 	QAUTRE(6.0),
// 	QAUTRE(6.0),
// 	QAUTRE(3.5),
// 	HUIT(2.0),
// 	HUIT(2.0),
// 	QAUTRE(3.0)};
 /*
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0};*/

/********************  GLOBALS  *********************/
float SVOCR_COEFS[SVOCR_HEUR_NB_COEFS] =
{.19003,1.09526,6741.97,9.1971,9.1971,9.1971,9.1971,0.714952,0.714952,0.714952,0.714952,3.88212,3.88212,3.88212,3.88212,9.31876,9.31876,9.31876,9.31876,9.36332,9.36332,5.63171,5.63171,8.86417,8.86417,8.86417,8.86417,6.34806,6.34806,4.36893,4.36893,8.81758,8.81758,8.81758,8.81758,9873.37,8};

/*******************  FUNCTION  *********************/
svOCRHeuristic::svOCRHeuristic()
{
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::buildFromExtractedChar( svExtractedChar & ch,int majSize)
{
	image img = ch.getImage();
	this->buildFromImage(img,majSize);
	calcVertPos(ch.getHash(majSize));
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::buildFromHash(std::string hash,int majSize)
{
	image img(1,1);
	svExtractedChar::buildImgFromHash(hash,img);
	this->buildFromImage(img,majSize);
	calcVertPos(hash);
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcVertPos(std::string hash)
{
	if (hash[0]=='t')
		vertpos = 1.0;
	else if (hash[0]=='m')
		vertpos = 0.5;
	else if (hash[0]=='b')
		vertpos = 0.0;
	else
		vertpos = -1.0;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::buildFromImage(image & img,int majSize)
{
	this->init();
	this->calcFillingLevel(img);
	this->calcCell(img);
	this->calcHPix(img);
	this->calcVPix(img);
	this->calcHSegm(img);
	this->calcVSegm(img);
	this->calcAngle(img);
	aspect = (float)img.lwidth/(float)img.lheight;
	ratioToMaj = (float)img.lheight/(float)majSize;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcAngle(image & img)
{
	angle[0]=(img.getColor(0,0)==0)?1.0:0.0;
	angle[1]=(img.getColor(img.lwidth-1,0)==0)?1.0:0.0;
	angle[2]=(img.getColor(0,img.lheight-1)==0)?1.0:0.0;
	angle[3]=(img.getColor(img.lwidth-1,img.lheight-1)==0)?1.0:0.0;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcHSegm(image & img)
{
	bool last;
	for (unsigned int y=0;y<img.lheight;y++)
	{
		last=(img.getColor(0,y)==0);
		for(unsigned int x=1;x<img.lwidth;x++)
		{
			if ((img.getColor(x,y)==0)!=last)
			{
				fit(y,img.lheight,4,hsegm);
				if (y==1)
					hsegm[4]++;
				if (y==img.lheight-1)
					hsegm[5]++;
				last=!last;
			}
		}
	}

	hsegm[0]/=(float)img.lheight/4.0;
	hsegm[1]/=(float)img.lheight/4.0;
	hsegm[2]/=(float)img.lheight/4.0;
	hsegm[3]/=(float)img.lheight/4.0;
// 	hsegm[4]/=(float)img.lheight/4.0;
// 	hsegm[5]/=(float)img.lheight/4.0;
// 	hsegm[6]/=(float)img.lheight/4.0;
// 	hsegm[7]/=(float)img.lheight/4.0;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcVSegm(image & img)
{
	bool last;
	for(unsigned int x=1;x<img.lwidth;x++)
	{
		last=(img.getColor(x,0)==0);
		for (unsigned int y=0;y<img.lheight;y++)
		{
			if ((img.getColor(x,y)==0)!=last)
			{
				if (x==1)
					vsegm[4]++;
				if (x==img.lheight-1)
					vsegm[5]++;
				fit(x,img.lwidth,4,vsegm);
				last=!last;
			}
		}
	}

	vsegm[0]/=(float)img.lwidth/4.0;
	vsegm[1]/=(float)img.lwidth/4.0;
	vsegm[2]/=(float)img.lwidth/4.0;
	vsegm[3]/=(float)img.lwidth/4.0;
// 	vsegm[4]/=(float)img.lwidth/4.0;
// 	vsegm[5]/=(float)img.lwidth/4.0;
// 	vsegm[6]/=(float)img.lwidth/4.0;
// 	vsegm[7]/=(float)img.lwidth/4.0;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::init()
{
	fillingLevel = 0.0;
	aspect = 0.0;
	vertpos = 0.0;
	for (int i=0;i<4;i++)
	{
		hpix[i]=0.0;
		vpix[i]=0.0;
		cell[i]=0.0;
		angle[i]=0.0;
	}
	for (int i=0;i<8;i++)
	{
		hsegm[i]=0.0;
		vsegm[i]=0.0;
	}
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::setRandomCoefs(float max)
{
	//for (int i=0;i<SV_HEUR_NB_COORD;i++)
	//	SVOCR_COEFS[i] = max * rand() / (RAND_MAX + 1.0);

	SVOCR_COEFS[0] = max * rand() / (RAND_MAX + 1.0);
	SVOCR_COEFS[1] = max * rand() / (RAND_MAX + 1.0);
	SVOCR_COEFS[2] = 10000.0*max * rand() / (RAND_MAX + 1.0);
	SET_DUPL4(SVOCR_COEFS+3,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL4(SVOCR_COEFS+7,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL4(SVOCR_COEFS+11,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL4(SVOCR_COEFS+15,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL2(SVOCR_COEFS+19,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL2(SVOCR_COEFS+21,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL4(SVOCR_COEFS+23,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL2(SVOCR_COEFS+27,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL2(SVOCR_COEFS+29,max * rand() / (RAND_MAX + 1.0));
	SET_DUPL4(SVOCR_COEFS+31,max * rand() / (RAND_MAX + 1.0));
	SVOCR_COEFS[SVOCR_HEUR_NB_COORD-1] = 10000.0*max * rand() / (RAND_MAX + 1.0);

// 	SVOCR_COEFS[3] = max * rand() / (RAND_MAX + 1.0);
// 	SVOCR_COEFS[4]=SVOCR_COEFS[5]=SVOCR_COEFS[6]=SVOCR_COEFS[3];

// 	SVOCR_COEFS[7] = max * rand() / (RAND_MAX + 1.0);
// 	SVOCR_COEFS[8]=SVOCR_COEFS[9]=SVOCR_COEFS[10]=SVOCR_COEFS[7];

// 	SVOCR_COEFS[11] = max * rand() / (RAND_MAX + 1.0);
// 	SVOCR_COEFS[12]=SVOCR_COEFS[13]=SVOCR_COEFS[14]=SVOCR_COEFS[11];

// 	SVOCR_COEFS[15] = max * rand() / (RAND_MAX + 1.0);
// 	SVOCR_COEFS[16]=SVOCR_COEFS[17]=SVOCR_COEFS[18]=SVOCR_COEFS[19]=SVOCR_COEFS[15];
// 	SVOCR_COEFS[20]=SVOCR_COEFS[21]=SVOCR_COEFS[22]=SVOCR_COEFS[15];
//
// 	SVOCR_COEFS[23] = max * rand() / (RAND_MAX + 1.0);
// 	SVOCR_COEFS[24]=SVOCR_COEFS[25]=SVOCR_COEFS[26]=SVOCR_COEFS[27]=SVOCR_COEFS[23];
// 	SVOCR_COEFS[28]=SVOCR_COEFS[29]=SVOCR_COEFS[30]=SVOCR_COEFS[23];
//
// 	SVOCR_COEFS[31] = max * rand() / (RAND_MAX + 1.0);
// 	SVOCR_COEFS[32]=SVOCR_COEFS[33]=SVOCR_COEFS[34]=SVOCR_COEFS[31];
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::setCoefs(std::string value)
{
	string tmp = "";
	int cnt=0;
	for (unsigned int i=0;i<value.size();i++)
	{
		if (value[i]==',')
		{
			SVOCR_COEFS[cnt] = atof(tmp.c_str());
			cout << "COEF[" << cnt << "] = " << SVOCR_COEFS[cnt] << endl;
			tmp="";
			cnt++;
		} else {
			tmp+=value[i];
		}
	}
	SVOCR_COEFS[cnt++] = atof(tmp.c_str());
	if (cnt!=SVOCR_HEUR_NB_COEFS)
	{
		cerr << "You need to provide "<< SVOCR_HEUR_NB_COEFS <<" float numbers as coefs.\n" << endl;
		cerr << "You only provided " << cnt << endl;
		exit(1);
	}
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::copyCoefs(float * dest)
{
	for (int i=0;i<SVOCR_HEUR_NB_COEFS;i++)
		dest[i]=SVOCR_COEFS[i];
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcFillingLevel(image & img)
{
	int filled = 0;
	for (unsigned int y=0;y<img.lheight;y++)
	{
		for(unsigned int x=0;x<img.lwidth;x++)
		{
			if (img.getColor(x,y)==0)
				filled++;
		}
	}

	this->fillingLevel = (float)filled/(float)(img.lwidth*img.lheight);
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcCell(image & img)
{
	float x1[2];
	//cal int parts
	for (unsigned int y=0;y<img.lheight;y++)
	{
		for(unsigned int x=0;x<img.lwidth;x++)
		{
			x1[0]=x1[1]=0.0;
			if (img.getColor(x,y)==0)
			{
				fit(x,img.lwidth,2,x1);
				fit(y,img.lheight,2,cell,x1[0]);
				fit(y,img.lheight,2,cell+2,x1[1]);
			}
		}
	}
	cell[0]/=(float)img.lwidth*(float)img.lheight/4.0;
	cell[1]/=(float)img.lwidth*(float)img.lheight/4.0;
	cell[2]/=(float)img.lwidth*(float)img.lheight/4.0;
	cell[3]/=(float)img.lwidth*(float)img.lheight/4.0;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::fit(int pos,int max,int nbSegm,float * res,float value)
{
	float dsplit = (float)max/(float)nbSegm;
	float dx;
	float split;
	for (int i = 1;i<nbSegm;i++)
	{
		split = (float)i*dsplit;
		if (pos==(int)split)
		{
			dx = split - (int)split;
			res[i-1]+=dx*value;
			res[i]+=value*(1.0-dx);
			return;
		} else if (pos < split) {
			res[i-1]+=value;
			return;
		}
	}
	res[nbSegm-1]+=value;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcHPix(image & img)
{
	for (unsigned int y=0;y<img.lheight;y++)
		for (unsigned int x=0;x<img.lwidth;x++)
			if (img.getColor(x,y)==0)
				fit(y,img.lheight,4,hpix);
	hpix[0]/=(float)img.lwidth*(float)img.lheight/4.0;
	hpix[1]/=(float)img.lwidth*(float)img.lheight/4.0;
	hpix[2]/=(float)img.lwidth*(float)img.lheight/4.0;
	hpix[3]/=(float)img.lwidth*(float)img.lheight/4.0;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::calcVPix(image & img)
{
	for (unsigned int y=0;y<img.lheight;y++)
		for (unsigned int x=0;x<img.lwidth;x++)
			if (img.getColor(x,y)==0)
				fit(x,img.lwidth,4,vpix);
	vpix[0]/=(float)img.lwidth*(float)img.lheight/4.0;
	vpix[1]/=(float)img.lwidth*(float)img.lheight/4.0;
	vpix[2]/=(float)img.lwidth*(float)img.lheight/4.0;
	vpix[3]/=(float)img.lwidth*(float)img.lheight/4.0;
}

/*******************  FUNCTION  *********************/
std::string svOCRHeuristic::getCDefinition(void)
{
	char buffer[4096];
	char * cur=buffer;
	cur+=sprintf(cur,"{%f, %f, %f, ",fillingLevel,aspect,vertpos);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",cell[0],cell[1],cell[2],cell[3]);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",hpix[0],hpix[1],hpix[2],hpix[3]);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",vpix[0],vpix[1],vpix[2],vpix[3]);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",hsegm[0],hsegm[1],hsegm[2],hsegm[3]);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",hsegm[4],hsegm[5],hsegm[6],hsegm[7]);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",vsegm[0],vsegm[1],vsegm[2],vsegm[3]);
	cur+=sprintf(cur,"%f, %f, %f, %f, ",vsegm[4],vsegm[5],vsegm[6],vsegm[7]);
	cur+=sprintf(cur,"%f, %f, %f, %f}",angle[0],angle[1],angle[2],angle[3]);
	return buffer;
}

/*******************  FUNCTION  *********************/
std::ostream & operator << (ostream & out, const svOCRHeuristic &heur)
{
	out << "#fillingLevel = " << heur.fillingLevel << endl;
	out << "#cells        = " << heur.cell[0] << " , " << heur.cell[1] << " , " << heur.cell[2] << " , " << heur.cell[3] << endl;
	out << "#HPix        = " << heur.hpix[0] << " , " << heur.hpix[1] << " , " << heur.hpix[2] << " , " << heur.hpix[3] << endl;
	out << "#VPix        = " << heur.vpix[0] << " , " << heur.vpix[1] << " , " << heur.vpix[2] << " , " << heur.vpix[3] << endl;
	out << "#HSegm       = " << heur.hsegm[0] << " , " << heur.hsegm[1] << " , " << heur.hsegm[2] << " , " << heur.hsegm[3] << endl;
	out << "#VSegm       = " << heur.vsegm[0] << " , " << heur.vsegm[1] << " , " << heur.vsegm[2] << " , " << heur.vsegm[3] << endl;
	out << "#aspect      = " << heur.aspect << endl;
	out << "#vetpos      = " << heur.vertpos << endl;
	return out;
}

/*******************  FUNCTION  *********************/
std::string svOCRHeuristic::getValue(void)
{
	return value;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::setValue(std::string value)
{
	this->value = value;
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::setCoord(float coord[15])
{
	fillingLevel = coord[0];
	aspect = coord[1];
	vertpos = coord[2];
	for (int i=0;i<4;i++)
	{
		cell[i] = coord[3+i];
		hpix[i] = coord[7+i];
		vpix[i] = coord[11+i];
		angle[i] = coord[31+i];
	}
	for (int i=0;i<8;i++)
	{
		hsegm[i] = coord[15+i];
		vsegm[i] = coord[23+i];
	}
	ratioToMaj = coord[SVOCR_HEUR_NB_COORD-1];
}

/*******************  FUNCTION  *********************/
svOCRHeuristicSimple svOCRHeuristic::getSimple()
{
	svOCRHeuristicSimple simple;
	simple.coord[0] = fillingLevel;
	simple.coord[1] = aspect;
	simple.coord[2] = vertpos;
	simple.coord[SVOCR_HEUR_NB_COORD-1] = ratioToMaj;
	for (int i=0;i<4;i++)
	{
		simple.coord[3+i] = cell[i];
		simple.coord[7+i] = hpix[i];
		simple.coord[11+i] = vpix[i];
		simple.coord[31+i] = angle[i];
	}
	for (int i=0;i<8;i++)
	{
		simple.coord[15+i] = hsegm[i];
		simple.coord[23+i] = vsegm[i];
	}
	simple.value = this->value;
	return simple;
}

/*******************  FUNCTION  *********************/
float svOCRHeuristic::getDistanceTo(const svOCRHeuristicSimple & simple,float * coordDist)
{
	float res = 0.0;
	float tmp;
	svOCRHeuristicSimple simple2 = this->getSimple();
	for (int i=0;i<SVOCR_HEUR_NB_COORD;i++)
	{
		tmp = SQR(SVOCR_COEFS[i]*(simple.coord[i] - simple2.coord[i]));
		if (coordDist!=NULL)
			coordDist[i]=tmp;
		res+= tmp;
	}
	return sqrt(res);
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::printCoordDists(const svOCRHeuristicSimple & simple)
{
	svOCRHeuristicSimple simple2 = this->getSimple();
	float diff;
	for (int i=0;i<SVOCR_HEUR_NB_COORD;i++)
	{
		diff = simple.coord[i]-simple2.coord[i];
		if (diff >= 0)
			cout << diff << " ";
		else
			cout << -diff << " ";
	}
}

/*******************  FUNCTION  *********************/
void svOCRHeuristic::setCoefs(float * value)
{
	for (int i=0;i<SVOCR_HEUR_NB_COEFS;i++)
		SVOCR_COEFS[i]=value[i];
}

/*******************  FUNCTION  *********************/
bool svOCRHeuristic::applyCut(float * coordDist)
{
	float max[SVOCR_HEUR_NB_COORD]={0.00401238,0.65352,0,21.1467,30.7743,12.6951,47.3046,0.127789,0.0887424,0.511156,0.511156,6.69816,3.1934,15.0709,15.0709,204.169,347.357,241.22,186.801,350.687,87.6718,0,0,139.686,427.789,314.294,377.01,362.681,161.191,0,0,77.7497,77.7497,77.7497,77.7497,243708};

	for (int i=0;i<3;i++)
		if (max[i]!=0.0 && coordDist[i] > max[i]*1.1)
			return false;
	return true;
}

/*******************  FUNCTION  *********************/
bool svOCRHeuristic::isAccepted(svOCRHeuristicAnswer & ans)
{
	if (ans.hasSome && ans.dist1<SVOCR_COEFS[SVOCR_HEUR_NB_COORD])
		return true;
	else
		return false;
}
