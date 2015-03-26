#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<bios.h>
#include<math.h>

#define  ESC   283
#define  ENTER 7181
#define  TAB   3849

#define  UP    18432
#define  DOWN  20480
#define  LEFT  19200
#define  RIGHT 19712

#define  SPACE 14624

#define  LEFTWALL   0
#define  RIGHTWALL  13
#define  BOTTOMWALL 20

#define  BACKCOLOR     LIGHTGREEN
#define  ERAZECOLOR    BACKCOLOR

enum DIFFICULTY
{
	  First  = 12,
	  Second =  8,
	  Three  =  4,
	  Four   =  2
}Difficulty;



enum OPTION
{
	Start,
	Level,
	Rank,
	Operation,
	Restart,
	Pause
};

enum BOOL
{
	FALSE,
	TRUE,
	WALL,
	OVER
};


unsigned long ExTetris[25]=
{
	0x06C0,0x0462,0x0360,0x4620,
	0x08E0,0x0226,0x0710,0x6440,
	0x0C60,0x0264,0x0630,0x2640,
	0x0E40,0x0464,0x0270,0x2620,
	0x4444,0x00F0,0x2222,0x0F00,
	0x0E80,0x0446,0x0170,0x6220,
	0x0660
};

int LayoutTet[21][14];
int LineDistance,Distance,OptionLines,FontHeight,FontSize,GridWid;
int ShowScoreX0,ShowScoreY1,ShowLevelX0,ShowLevelY1,ShowNextShapeX0,ShowNextShapeY0;
int DynGridBaseX,DynGridBaseY; /* x and y are of  dynamic grid original point */
int InRectLen = 0,InRectWid = 0, Score = 0;
enum BOOL LevelFirst = TRUE;
enum BOOL ScoreFirst = TRUE;
/*int Score = 0,Level = 1;*/


void InitGraph()
{
   int graphdriver = DETECT,graphmode;
   initgraph(&graphdriver,&graphmode,"C:\\TC20\\BGI");
   Difficulty      = First;
}

void SetOutsideWall()
{
	int i,j;
	for(i = 0; i < 21; ++i)
	{
		LayoutTet[i][LEFTWALL]   = WALL;
		LayoutTet[i][RIGHTWALL]  = WALL;
	}
	for(i = 1; i < 13; ++i)
		LayoutTet[BOTTOMWALL][i] = WALL;
	for(i = 1; i < 13; ++i)
		for(j = 0; j < 20; ++j)
			 LayoutTet[j][i] = 0;
}

void DrawBlock(int i,int j,int Color)
{
	 int x = 0, y = 0;
	 x += (j - 1) * (GridWid + 1) + DynGridBaseX;
	 y += i * (GridWid + 1) + DynGridBaseY;
	 setwritemode(0);
	 setcolor(Color);
	 rectangle(x,y, x + GridWid - 1, y + GridWid - 1);
	 setfillstyle(SOLID_FILL,Color);
	 floodfill(x + GridWid / 2, y + GridWid / 2,Color);
}

void DrawCurrentOption(int x0,int y0,int x1,enum OPTION Option)
{
	int TextMaxLen,CurOptRectX0,CurOptRectY0,CurOptRectX1,CurOptRectY1;
	FontSize      = 8;
	TextMaxLen    = textwidth("Operation");
	CurOptRectX0  = (x0 + x1) / 2 - TextMaxLen / 2 - NORM_WIDTH;
	CurOptRectX1  = (x0 + x1) / 2 + TextMaxLen / 2 + NORM_WIDTH;
	CurOptRectY0  =  y0 + Distance - 4 + Option * (LineDistance + FontSize) - NORM_WIDTH;
	CurOptRectY1  =  CurOptRectY0  + FontSize + 1;
	setwritemode(1);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	setcolor(LIGHTGREEN);
	rectangle(CurOptRectX0,CurOptRectY0 ,CurOptRectX1,CurOptRectY1);
}

void ShowLevel(int OriginX1,int OriginY0)
{
	char *OutText = NULL;
	int Distance = 5;
	if(LevelFirst == TRUE)
	{
		setcolor(WHITE);
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy(OriginX1 + Distance ,OriginY0 + Distance ,"Level:");
		ShowLevelX0  = OriginX1 + Distance * 6;
		ShowLevelY1  = OriginY0 + Distance * 6;
		LevelFirst = FALSE;
	}
	setcolor(YELLOW);
	rectangle(ShowLevelX0 - 20,ShowLevelY1 - 10,ShowLevelX0 + 20,ShowLevelY1 + 8);
	setfillstyle(SOLID_FILL,BACKCOLOR);
	floodfill(ShowLevelX0,ShowLevelY1,YELLOW);
	setcolor(WHITE);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	sprintf(OutText,"%d",Level);
	outtextxy(ShowLevelX0,ShowLevelY1,OutText);
}

void ShowScore(int OriginX1,int OriginY0)
{
	char * OutText = NULL;
	int Distance = 5;
    settextjustify(LEFT_TEXT,CENTER_TEXT);
	if(ScoreFirst == TRUE)
	{   setcolor(WHITE);
        settextjustify(LEFT_TEXT,CENTER_TEXT);
		outtextxy(OriginX1 + Distance,OriginY0 + Distance * 16,"Score:");
		ShowScoreX0  = OriginX1 + Distance * 6;
		ShowScoreY1  = OriginY0 + Distance * 21;
		ScoreFirst   = FALSE;
	}
	setcolor(YELLOW);
	rectangle(ShowScoreX0 - 20,ShowScoreY1 - 10,ShowScoreX0 + 20,ShowScoreY1 + 8);
	setfillstyle(SOLID_FILL,BACKCOLOR);
	floodfill(ShowScoreX0,ShowScoreY1,YELLOW);
	setcolor(WHITE);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	sprintf(OutText,"%d",Score);
	outtextxy(ShowScoreX0,ShowScoreY1,OutText);
}

void ShowNextText(int OriginX1,int OriginY0)
{

	int Distance = 5;
	setcolor(WHITE);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	outtextxy(OriginX1 + Distance,OriginY0 + Distance * 31,"Next:");
	ShowNextShapeX0 = OriginX1 + Distance * 3 + 1;
	ShowNextShapeY0 = OriginY0 + Distance * 36;
}

void DrawGameLayout()
{
	int OutRectLen,OutRectWid,GridColNum,GridRowNum;
	int OriginX0,OriginY0,OriginX1,OriginY1,Distance;

	OutRectLen    = 450;
	GridWid       = 15;
	OutRectWid    = OutRectLen * 0.7;
	GridColNum    = 12;
	GridRowNum    = 20;
	InRectLen     = GridRowNum * (GridWid + 1) - 1;
	InRectWid     = GridColNum * (GridWid + 1) - 1;
	OriginX0      = (getmaxx() - OutRectWid) / 2;
	OriginY0      = (getmaxy() - OutRectLen) / 2;
	OriginX1      = OriginX0 + OutRectWid;
	OriginY1      = OriginY0 + OutRectLen;

	setwritemode(0);
	setcolor(LIGHTGREEN);
	rectangle(OriginX0,OriginY0,OriginX1,OriginY1);
    setfillstyle(SOLID_FILL,LIGHTGREEN);
	floodfill((OriginX0 + OriginX1) / 2,(OriginY1 + OriginY0) / 2,LIGHTGREEN);

	Distance      = (OutRectWid - InRectWid) / 2;
	OriginX0     += Distance / 2;
	OriginX1      = OriginX0 + InRectWid;
	OriginY0     += Distance / 2;
	OriginY1      = OriginY0 + InRectLen;
	DynGridBaseX  = OriginX0;
	DynGridBaseY  = OriginY0;
	setcolor(YELLOW);
	rectangle(OriginX0 - 1,OriginY0 - 1,OriginX1 + 1,OriginY1 + 1);

	ShowLevel(OriginX1,OriginY0);
	ShowScore(OriginX1,OriginY0);
	ShowNextText(OriginX1,OriginY0);
	/*******************Draw operation instruction ******************/
}


enum BOOL JudgeDrawOk_Test(unsigned int Shape,int NextI,int NextJ)
{
	unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
	int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
	int i = 0,j = 0,DrawI,DrawJ;
	enum BOOL DrawFlag = TRUE;

	while(i < 4)
	{
		 Number[i]  = Shape & HexBit[i];
		 Number[i]  = Number[i] >> (4 * i);
		 ++i;
	}
	for(i = 0,DrawI = NextI; i < 4 ;++i,--DrawI)
	{
		 if(Number[i] == 0)
			continue;
		 if(DrawFlag != TRUE)
			break;
		 for(j = 0,DrawJ = NextJ;j < 4;++j,++DrawJ)
		 {
			if( (Number[i] & BinBit[j]) != 0 && LayoutTet[DrawI][DrawJ] == WALL)
			{
					DrawFlag = WALL;
					break;
			}

		 }
	}
	return DrawFlag;
}
enum BOOL JudgeDrawOk(unsigned int Shape,int NextI,int NextJ)
	{
		unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
		int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
		int i = 0,j = 0,DrawI,DrawJ;
		enum BOOL DrawFlag = TRUE;

		while(i < 4)
		{
			 Number[i]  = Shape & HexBit[i];
			 Number[i]  = Number[i] >> (4 * i);
			 ++i;
		}
		i = 0;
			DrawI = NextI;

		while(i < 4)
		{
			 if(Number[i] == 0)
			 {
				--DrawI;
				++i;
				continue;
			 }
			 if(DrawFlag != TRUE)
				break;
			 j = 0;
			 DrawJ = NextJ;
			 while(j < 4)
			 {
				if( (Number[i] & BinBit[j]) != 0)
				{
					if(LayoutTet[DrawI][DrawJ] == WALL)
					{
						DrawFlag = WALL;
						break;
					}
				}
				++j;
				++DrawJ;
			 }
			 ++i;
			 --DrawI;
		}
		return DrawFlag;
	}

enum BOOL MarkWall(unsigned int Shape,int StopI,int StopJ)
{
    unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
	int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
	int i = 0,j = 0,MarkI,MarkJ;
	enum BOOL MarkFlag = TRUE;
	while(i < 4)
	{
		 Number[i]  = Shape & HexBit[i];
		 Number[i]  = Number[i] >> (4 * i);
		 ++i;
	}
	i = 0;
	MarkI = StopI;
	for(i = 0,MarkI = StopI; i < 4; ++i,--MarkI)
	{
		if(MarkI < 0)
		 {
			MarkFlag = OVER;
			break;
		 }
		for(j = 0,MarkJ = StopJ; j < 4; ++j,MarkJ++)
			if( (Number[i] & BinBit[j]) != 0)
				LayoutTet[MarkI][MarkJ] = WALL;

	}
	return MarkFlag;
}




void DrawShape(unsigned int Shape,int MoveI,int MoveJ,int Color)
{
	unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
	int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
	int i = 0,j = 0,DrawI,DrawJ;

	while(i < 4)
	{
		 Number[i]  = Shape & HexBit[i];
		 Number[i]  = Number[i] >> (4 * i);
		 i++;
	}
	for(i = 0,DrawI = MoveI; i < 4; ++i,--DrawI)
	{
		 if(DrawI < 0)
			break;
		 for(j = 0,DrawJ = MoveJ; j < 4;++j,DrawJ++)
		 {
			if( (Number[i] & BinBit[j]) != 0 )
				DrawBlock(DrawI,DrawJ,Color);

		 }
	}
}


int ChangeShape(int Shape)
{
	int Change = 0;
	if(Shape != 24)
		if( ++Shape % 4 == 0)
			Change = -3;
		else
			Change = 1;
	return Change;
}





unsigned int GetCutImageInfo(int StopI)
{
	int Col,Row;
	unsigned int CutInfo = 0;
	enum BOOL CutFlag;
	for(Row = 0; Row < 4; ++Row,--StopI)
	{
    	if(StopI == 20)
				continue;
        CutFlag = TRUE;					 /* 1 is first column of game zone */
		for(Col = 1; Col < 13; ++Col)    /* 20 is tail column of game zone */
			if(LayoutTet[StopI][Col] != WALL)
			{
				CutFlag = FALSE;
				break;
			}
		if(CutFlag == TRUE)
			CutInfo += (unsigned int)pow(2,Row);
	}
	return CutInfo;
}


void CalcScore(unsigned int CutInfo)
{
	int i = 0;
	if(CutInfo == 0)
		return;
	if(CutInfo % 3 == 0  && CutInfo != 9)
		if(CutInfo % 5 == 0)
				Score += 55;             	  /** lasting 4 lines **/
		else
				Score += 25;                  /** lasting 2 lines **/
	else if(CutInfo % 7 == 0)
				Score += 40;                  /** lasting 3 lines **/
	while(i < 4)
		if( (CutInfo & (unsigned int)pow(2,i++)) != 0)   /** only one line **/
				Score += 10;
	ShowScore(0,0);
}

void CutLine(unsigned int CutInfo, int CutJ)
{
	 int i,Row,Col;
	 int GameZoneLeft = DynGridBaseX;
	 int GameZoneTop = DynGridBaseY;
	 int GameZoneRight = DynGridBaseX + (GridWid + 1) * 12 - 1;
	 unsigned  ImageSize;
	 void *ImageAddr = NULL;
	 for(i = 0; i < 4; ++i,--CutJ)
		if( (CutInfo & (unsigned int)pow(2,i)) != 0)
		{
			ImageSize = imagesize(GameZoneLeft,GameZoneTop,GameZoneRight,GameZoneTop + (GridWid + 1) * CutJ - 2 ) ;
			ImageAddr = malloc(ImageSize);
			getimage(GameZoneLeft,GameZoneTop,GameZoneRight,GameZoneTop + (GridWid + 1) * CutJ - 2,ImageAddr);
			putimage(GameZoneLeft,GameZoneTop + (GridWid + 1),ImageAddr,COPY_PUT);
			free(ImageAddr);
			for(Row = CutJ; Row >= 1; --Row)
				for(Col = 1; Col < 13; ++Col)
						LayoutTet[Row][Col] = LayoutTet[Row - 1][Col] ;
			++CutJ;
		}
}


enum BOOL SolveDown(clock_t *Start,int RandomShape,int *MoveI,int MoveJ,int Color)
{
	enum BOOL DownFlag = FALSE;
	enum BOOL MarkFlag = FALSE;
	DownFlag = JudgeDrawOk(ExTetris[RandomShape],(*MoveI) + 1,MoveJ);

		if( DownFlag == TRUE )
		{
			if( (clock() - (*Start)) / Difficulty > 0)
			{
				*Start = clock();
				DrawShape(ExTetris[RandomShape],(*MoveI),MoveJ,BACKCOLOR);
				DrawShape(ExTetris[RandomShape],++(*MoveI),MoveJ,Color);
			}
		}
		else
			MarkFlag = MarkWall(ExTetris[RandomShape],(*MoveI),MoveJ);
	return MarkFlag;
}

void SolveMorph(int *MorphIncrement,int *RandomShape,int MoveI,int MoveJ,int Color)
{
	enum BOOL MorphFlag = FALSE;
	if(*MorphIncrement != 0)
	{
		MorphFlag = JudgeDrawOk(ExTetris[*RandomShape + *MorphIncrement],MoveI,MoveJ);
		if(MorphFlag == TRUE)
		{
			DrawShape(ExTetris[*RandomShape],MoveI,MoveJ,BACKCOLOR);
			*RandomShape += *MorphIncrement;
			DrawShape(ExTetris[*RandomShape],MoveI,MoveJ,Color);
			*MorphIncrement = 0;
		}
	}
}

void SolveHoriChange(int *HoriIncrement,int RandomShape,int MoveI,int *MoveJ,int Color)
{
	enum BOOL HorizontalFlag = FALSE;
	if(*HoriIncrement != 0 )
	{
		HorizontalFlag = JudgeDrawOk(ExTetris[RandomShape],MoveI,*MoveJ + *HoriIncrement);
		if(HorizontalFlag != WALL)
		{
			DrawShape(ExTetris[RandomShape],MoveI,*MoveJ,BACKCOLOR);
			(*MoveJ) += (*HoriIncrement);
			DrawShape(ExTetris[RandomShape],MoveI,*MoveJ,Color);
			*HoriIncrement = 0;
		}
	}
}

void SolveQuickDown(int *Accelerate,int RandomShape,int *MoveI,int MoveJ,int Color)
{
	enum BOOL DownFlag = FALSE;
	if(*Accelerate != 0)
	{
		while(*Accelerate)
		{
			DownFlag = JudgeDrawOk(ExTetris[RandomShape],*MoveI + 1,MoveJ);
			if(DownFlag == WALL)
			{
				*Accelerate = 0;
				/*   it's repeated job ***/
		/*		MarkWall(ExTetris[RandomShape],*MoveI,MoveJ); */
				break;
			}
			DrawShape(ExTetris[RandomShape],*MoveI,MoveJ,BACKCOLOR);
			DrawShape(ExTetris[RandomShape],++(*MoveI),MoveJ,Color);
			--(*Accelerate);
		}
	}

}

void SolveKey(int Key,int* RandomShape,int *MoveI,int *MoveJ,int Color)
{
	int HoriIncrement  = 0,MorphIncrement = 0,Accelerate = 0;
	switch(Key)
	{
		case LEFT      : HoriIncrement  = -1;
						 SolveHoriChange(&HoriIncrement,*RandomShape,*MoveI,MoveJ,Color);
						 break;

		case RIGHT     : HoriIncrement  =  1;
						 SolveHoriChange(&HoriIncrement,*RandomShape,*MoveI,MoveJ,Color);
						 break;

		case DOWN      : Accelerate     =  4;
						 SolveQuickDown(&Accelerate,*RandomShape,MoveI,*MoveJ,Color);
						 break;

		case UP        : MorphIncrement = ChangeShape(*RandomShape);
						 SolveMorph(&MorphIncrement,RandomShape,*MoveI,*MoveJ,Color);
						 break;
	}
}


void DrawNextShape(int NextShape,int NextColor)
{
	int NextShapeI =  14,NextShapeJ = 14;
    /* ShowNextShape */
	DrawShape(NextShape,NextShapeI,NextShapeJ,NextColor);
}

void CreateNextShape(int * NextShape,int * NextColor,int ShapeSum)
{
		*NextShape = rand() % ShapeSum;
		*NextColor = (*NextShape) % 16;
		if(*NextColor == BACKCOLOR)
			*NextColor -= 1;
		DrawNextShape(ExTetris[*NextShape],*NextColor);
}


void ShowGameOver()
{
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(LIGHTBLUE);
	outtextxy(getmaxx()/2,getmaxy()/2,"Game over!");
}

void Gaming(int *StartKey)
{
	int GameKey,ShapeSum,MoveI,MoveJ,RandomShape,Color,NextShape,NextColor;
	unsigned int CutInfo;
	enum BOOL StopDownFlag;
	clock_t Start;
	int t;

	GameKey  = 0;
	MoveI    = 0;
	MoveJ    = 5;
	ShapeSum = 25;
	srand(time(NULL));
	RandomShape = rand() % ShapeSum;
    Color = RandomShape % 16;
	if(Color == BACKCOLOR)
		 Color -= 1;
	DrawShape(ExTetris[RandomShape],MoveI,MoveJ,Color);
	CreateNextShape(&NextShape,&NextColor,ShapeSum);

	Start = clock();
	while(GameKey != ESC)
	{
			StopDownFlag = SolveDown(&Start,RandomShape,&MoveI,MoveJ,Color);
			if(StopDownFlag == TRUE)
			{
				CutInfo = GetCutImageInfo(MoveI);
				if(CutInfo != 0)
				{
					CalcScore(CutInfo);
					CutLine(CutInfo,MoveI);
				}
                MoveI = 0; MoveJ = 5;
				DrawNextShape(ExTetris[NextShape],ERAZECOLOR);
				RandomShape = NextShape;
				Color = NextColor;
				DrawShape(ExTetris[RandomShape],MoveI,MoveJ,Color);
                CreateNextShape(&NextShape,&NextColor,ShapeSum);
			}else if(StopDownFlag == OVER)
			{
					GameKey = ESC;
					ShowGameOver();
					delay(500);
					cleardevice();
			}
			GameKey = bioskey(1);
			for(t = 1000; GameKey == 0  && GameKey != ESC && t > 0; --t)
						 GameKey = bioskey(1);
			if(GameKey == 0)
				continue;
			GameKey = bioskey(0);
			if(StopDownFlag == FALSE)
                    SolveKey(GameKey,&RandomShape,&MoveI,&MoveJ,Color);
	}
}


void StartMenuExecution(enum OPTION Option,int *StartKey)
{
	 switch(Option)
	 {
		case Start     :  DrawGameLayout();
						  Gaming(StartKey);     break;
		case Level     :  /**LevelMenu()**/  	break;
		case Rank      :  /**ReadRankingFile*/ 	break;
		case Operation :  /**ShowOperation*/    break;
	 }
}

void StartMenu(int x0,int y0,int x1)
{
	int StartMenuKey = 0;
	enum OPTION Option  = Start;
	while(StartMenuKey != ESC)
	{
		StartMenuKey = bioskey(1);
		if(StartMenuKey == 0) continue;
		StartMenuKey = bioskey(0);
		switch(StartMenuKey)
		{
			case UP     : if(Option != Start)
						  {
							DrawCurrentOption(x0,y0,x1,Option);
							--Option;
							DrawCurrentOption(x0,y0,x1,Option);
						  }
						  break;
			case DOWN   : if(Option != Operation)
						  {
							DrawCurrentOption(x0,y0,x1,Option);
							++Option;
							DrawCurrentOption(x0,y0,x1,Option);
						  }
						  break;
			case ENTER  : StartMenuExecution(Option,&StartMenuKey);
						  break;
		}
	}
}



void DrawStartMenu()
{
	int StartMenuLength,StartMenuWidth,i,x0,y0,x1,y1;
	char OptionText[4][10] = {"Start","Level","Rank","Operation"};
	FontSize        = 1;
	OptionLines     = 4;
	StartMenuLength = 200;
	StartMenuWidth  = StartMenuLength * 0.618;
	x0 = (getmaxx() - StartMenuLength) / 2;
	y0 = (getmaxy() - StartMenuWidth ) / 2;
	LineDistance    = 8 * FontSize;
	FontHeight      = 8 * FontSize;
	Distance=(StartMenuWidth - OptionLines * (LineDistance + FontHeight) + FontHeight) / 2;
	x1 = x0 + StartMenuLength;
	y1 = y0 + StartMenuWidth;
	setwritemode(0);
	setcolor(CYAN);
	rectangle(x0,y0,x1,y1);
	setfillstyle(SOLID_FILL,CYAN);
	floodfill(getmaxx() / 2,getmaxy() / 2,CYAN);
	setcolor(LIGHTRED);
	settextstyle(DEFAULT_FONT,0,FontSize);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	for(i = 0;i < 4; ++i)
		outtextxy((x0 + x1) / 2,y0 + Distance + ((LineDistance + FontHeight) * i),OptionText[i]);
	DrawCurrentOption(x0,y0,x1,Start);
	StartMenu(x0,y0,x1);
}

void CloseSystem()
{   getch();
	closegraph();
}

void main()
{
   InitGraph();
   SetOutsideWall();
   DrawStartMenu();
   CloseSystem();
}

	/************************************* Test code ****************
	setwritemode(0);
	setfillstyle(SOLID_FILL,BLACK);
	floodfill(2,2,LIGHTGREEN);

	for(i = 0; i < 21; i++)
	{
		for(j = 0; j < 14; ++j)
		{


			setwritemode(0);
			sprintf(OutText,"%2d",LayoutTet[i][j]);

			setcolor(RED);
			rectangle((j + 1)  * 10 - 4,(i + 1) * 12 - 6,(j + 2) * 10 - 4,(i + 2) * 12 - 6);
			setfillstyle(SOLID_FILL,RED);
			floodfill((j + 1)  * 10 - 2,(i + 1) * 12 - 4,RED);
			setcolor(WHITE);
			outtextxy(j  * 10,(i + 1) * 12,OutText);
		}
	}
	********************************************************************/


	/*******************************Test code ***********************
	enum BOOL JudgeDrawOk(unsigned int Shape,int NextI,int NextJ)
	{
		unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
		int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
		int i = 0,j = 0;
		int ShapeInfo[4][4];
		enum BOOL DrawFlag = TRUE;

		while(i < 4)
		{
			 Number[i]  = Shape & HexBit[i];
			 Number[i]  = Number[i] >> (4 * i);
			 ++i;
		}

		for(i = 0; i < 4; ++i)
		{
			for(j = 0; j < 4; ++j)
			{
				ShapeInfo[i][j] = (Number[i] & BinBit[j]) ;
			}
		}

		for(i = 0; i < 4; ++i)
		{
			for(j = 0; j < 4; ++j)
			{
				if((NextI - i) < 0 )
					break;
				if(ShapeInfo[i][j] != 0 && LayoutTet[NextI - i][NextJ + j] == WALL)
				{
					DrawFlag = WALL;
					break;
				}
			}
		}
		return DrawFlag;
	}
	*/
	/************************  Test code  *********************************
	 enum BOOL MarkWall(unsigned int Shape,int StopI,int StopJ)
	{
		unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
		int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
		int i = 0,j = 0,MarkI,MarkJ;
		enum BOOL MarkFlag = TRUE;

		while(i < 4)
		{
			 Number[i]  = Shape & HexBit[i];
			 Number[i]  = Number[i] >> (4 * i);
			 ++i;
		}
		i = 0;
		MarkI = StopI;
		while(i < 4)
		{
			 if(MarkI < 0)
			 {
				MarkFlag = OVER;
				break;
			 }
			 j = 0;
			 MarkJ = StopJ;
			 while(j < 4)
			 {
				if( (Number[i] & BinBit[j++]) != 0)
						LayoutTet[MarkI][MarkJ] = WALL;
				++MarkJ;
			 }
				 --MarkI;
			 ++i;
		}
		return MarkFlag;
	}

	**************************************************************************/
	/*********************************Test  code *****************************
	enum BOOL JudgeDrawOk(unsigned int Shape,int NextI,int NextJ)
	{
		unsigned long HexBit[4] = {0x000F,0x00F0,0x0F00,0xF000};
		int BinBit[4] = {8,4,2,1},Number[4] = {0,0,0,0};
		int i = 0,j = 0,DrawI,DrawJ;
		enum BOOL DrawFlag = TRUE;

		while(i < 4)
		{
			 Number[i]  = Shape & HexBit[i];
			 Number[i]  = Number[i] >> (4 * i);
			 ++i;
		}
		i = 0;
			DrawI = NextI;

		while(i < 4)
		{
			 if(Number[i] == 0)
			 {
				--DrawI;
				++i;
				continue;
			 }
			 if(DrawFlag != TRUE)
				break;
			 j = 0;
			 DrawJ = NextJ;
			 while(j < 4)
			 {
				if( (Number[i] & BinBit[j]) != 0)
				{
					if(LayoutTet[DrawI][DrawJ] == WALL)
					{
						DrawFlag = WALL;
						break;
					}
				}
				++j;
				++DrawJ;
			 }
			 ++i;
			 --DrawI;
		}
		return DrawFlag;
	}
	************************************************************************/

	/*************************** Test   Code *****************************
	IdentifyKey(GameKey,&HoriIncrement,RandomShape,&MorphIncrement);
	SolveMorph(&MorphIncrement,&RandomShape,MoveI,MoveJ,Color);
	SolveHoriChange(&HoriIncrement,RandomShape,MoveI,&MoveJ,Color);
	SolveQuickDown(&Accelerate,RandomShape,&MoveI,MoveJ,Color);
	**********************************************************************/
