#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<time.h>
#include<stdlib.h>

#define INDENT    1
#define CONVEX    0
#define WIDTH     15

#define ENTER     7181
#define BACKSPACE 3592
#define RESTART   BACKSPACE
#define INSERT    20992
#define DELETE    21248

#define F1  	  15104
#define F2  	  15360
#define F3  	  15616
#define F4  	  15872
#define F5  	  16128
#define F6  	  16384
#define F7  	  16640
#define F8  	  16896
#define F9  	  17152
#define F10 	  17408

#define HOME      18716
#define END       20224

#define UP        18432
#define DOWN      20480
#define LEFT      19200
#define RIGHT     19712
#define PAGEUP    18688
#define PAGEDOWN  20736

#define TAB       3849
#define ESC       283

#define SPACE     14624
#define MINE      -1

#define DimensionsToOne 1
#define DimensionsToTwo 2

#define NORMAL        0
#define DIGGED        1
#define MARKEDWRONG  -100
#define MARKEDRIGHT   100

#define SMILE         1
#define CRY           0

#define MARK          1
#define CANCLE        1
#define COVER         0

#define FIRST         255
#define START         0

#define SENIOR        0
#define MIDDLE        1
#define HIGH          2

#define STORE         1
#define SHOW          0

int ExMaxX,ExMaxY,ExRow,ExCol,ExX0,ExY0,ExI0,ExJ0; /*the extend variables*/
int LayoutSum,*MinesPosition = NULL;
int MineDiggedNumber;
int MarkedRightNumber;

enum BOOL
{
	FALSE,
	TRUE
};

void InitSetDifficulty(int Difficulty);
void InitSystem();
void InitCursor();
void InitDrawOriginalPoint();
void CalAroundMines();
void CalOrdinate(int i,int j,int *x,int *y);
void DrawBlock(int x1,int y1,int x2,int y2,int type);
void DrawFace(int type);
void DrawTip(int NotClientLength,int NotClientWidth,int ClientWidth);
void DrawMenu();
void DrawNotClientArea();
void DrawFlag(int i,int j,int type);
void DrawClientArea();
void DrawDiggedBackground(int i,int j);
void DrawWrongMark(int i,int j);
void DrawSingleMine(int i,int j,int type);
void DrawCursor(int i,int j);
void ShowMarkNumber(int Number);
void ShowTime(int T);
void ShowAllMines();
void ShowGameOver();
void ShowDigit(int i,int j,int Number);
void ShowSuccess();
int IsSuccess();
void IsMine(int i,int j);
void IsGameOver(int i,int j,int* GameOver,int *Key,int *Control);
void DimensionConversion(int *k,int *i,int *j,int Type);
void RandomizeMinesPosition();
void AdjacenceIsZero(int k);
void CheckMines(int i ,int j,int *GameOver,int *SFlag);
void MarkOrCancleMine(int i,int j);
void StoreShowImage(int *Type);
void MoveCursor(int *Control);


void InitSetDifficulty(int Difficulty)
{
	if(Difficulty == SENIOR)
	{
		ExRow = 9;
		ExCol = 9;
	}else if(Difficulty == MIDDLE)
	{
		ExRow = 9;
		ExCol = 15;
	}else
	{
		ExRow = 15;
		ExCol = 15;
	}
}

void InitSystem()
{
	int graphdriver,graphmode,i;
	detectgraph(&graphdriver,&graphmode);
	initgraph(&graphdriver,&graphmode,"C:\\TC20\\BGI");
	cleardevice();
	setbkcolor(BLACK);
	ExMaxX = getmaxx();
	ExMaxY = getmaxy();
	MineDiggedNumber = 0;
	MarkedRightNumber= 0;
	LayoutSum = ExCol * ExRow;
	MinesPosition = (int *)malloc( LayoutSum * sizeof(int) );

	if(MinesPosition == NULL)
	{
		exit(1);
		outtextxy(0,0,"The memory wasn't allocation!");
	}
	for(i = 0; i < LayoutSum; ++i)
		MinesPosition[i] = 0;

}

void InitDrawOriginalPoint()
{
	ExX0 = (ExMaxX - ExCol * (WIDTH + 1) + 1) / 2;
	ExY0 = (ExMaxY - ExRow * (WIDTH + 1) + 1) / 2;
}

void CalOrdinate(int i,int j,int *x,int *y)
{
	*x = ExX0 + (j - 1) * (WIDTH + 1);
	*y = ExY0 + (i - 1) * (WIDTH + 1);
}

void DrawBlock(int x1,int y1,int x2,int y2,int type)
{
	setwritemode(0);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	rectangle(x1,y1,x2,y2);

	floodfill((x1+x2)/2,(y1+y2)/2,WHITE);

	if(type == CONVEX)
		setcolor(WHITE);
	else
		setcolor(DARKGRAY);
	line(x1,y1,x2,y1);          /* redraw top line of rectangle */
	line(x1,y1,x1,y2);          /* redraw left line of rectangle */
	line(x1 + 1,y1 + 1,x2 - 1,y1 + 1);  /* draw head and interior line of rectangle */
	line(x1 + 1,y1 + 1,x1 + 1,y2 - 1);  /* draw left and interior line of rectangle */

	if(type == CONVEX)
		setcolor(DARKGRAY);
	else
		setcolor(WHITE);
	line(x1,y2,x2,y2);          /* redraw bottom line of rectangle */
	line(x2,y1,x2,y2);          /* redraw right line of rectangle */
	line(x1 + 1,y2 - 1,x2 - 1,y2 - 1);  /* draw bottom and interior line of rectangle */
	line(x2 - 1,y1 + 1,x2 - 1,y2 - 1);  /* draw right and interior line of rectangle */
}
void DrawFace(int type)
{
	int ClientLength,NotClientWidth;
	int x,y,FaceCircleRadius ;

	ClientLength = ExCol * ( WIDTH + 1) + 1; /* the length of client area */
	NotClientWidth  = 32;
	FaceCircleRadius  =  (NotClientWidth - 12) / 2;
    x = ExX0 + ClientLength / 2;
	y = ExY0 - NotClientWidth / 2;
	setwritemode(0);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
    setcolor(YELLOW);
	if(type)
	{
			/********erase previuos****/
			arc(x,y + FaceCircleRadius + 2,50,128,FaceCircleRadius - 2);
			setcolor(BLACK);
			/********draw smile********/
			arc(x,y,222,322,FaceCircleRadius - 2);

	}
	else
	{
            /********erase previuos****/
			arc(x,y,222,322,FaceCircleRadius - 2);
			setcolor(BLACK);
			/********draw cry**********/
			arc(x,y + FaceCircleRadius + 2,50,128,FaceCircleRadius - 2);
	}

}

void DrawTip(int NotClientLength,int NotClientWidth,int ClientWidth)
{
	int TipZoneX0,TipZoneY0,TipZoneX1,TipZoneY1,TipZoneWidth;
	char  *Tip = "Menu : F10" ;
	TipZoneWidth = 15;
	TipZoneX0 = ExX0 - 4;
	TipZoneY0 = ExY0 - TipZoneWidth - NotClientWidth;
	TipZoneX1 = ExX0 + NotClientLength;
	TipZoneY1 = ExY0 ;

	setwritemode(0);
	setcolor(LIGHTBLUE);
	rectangle(TipZoneX0,TipZoneY0,TipZoneX1,TipZoneY1);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	floodfill(TipZoneX0 + NotClientLength / 2,TipZoneY0 + TipZoneWidth / 2,LIGHTBLUE);
	setlinestyle(SOLID_LINE,0,THICK_WIDTH);
	rectangle(TipZoneX0 - 2,TipZoneY0,TipZoneX1 + 1 ,ExY0 + ClientWidth + 3);
    setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	setcolor(WHITE);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	outtextxy(ExX0,TipZoneY0 + TipZoneWidth / 2,Tip);

}

void DrawMenu()
{
	int NotClientWidth = 32;
	int TextWidth = 80 /*textwidth("Exit  :ESC")*/;
	setcolor(LIGHTBLUE);
	setwritemode(0);
	rectangle(ExX0 - 4,ExY0 - NotClientWidth,ExX0 + TextWidth + 1,ExY0 + 5 * 7);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	floodfill(ExX0 + 4 + 10 + 10,ExY0 - NotClientWidth / 2,LIGHTBLUE);
	setwritemode(0);
	setcolor(WHITE);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	outtextxy(ExX0,ExY0 - 24,"Exit  :ESC");
	outtextxy(ExX0,ExY0 - 12,"ReNew :BS ");
	outtextxy(ExX0,ExY0 - 0 ,"Senior:F1 ");
	outtextxy(ExX0,ExY0 + 12,"Middle:F2 ");
	outtextxy(ExX0,ExY0 + 24,"High  :F3 ");

}

void DrawNotClientArea()
{
	int ClientLength ,ClientWidth,NotClientLength,NotClientWidth;
	int x,y,TimeTableWidth = 50 ,HeadRadius;

	ClientLength = ExCol * ( WIDTH + 1) + 1; /* the length of client area */
	ClientWidth  = ExRow * ( WIDTH + 1) + 1; /* the width of client area */
	NotClientLength = ClientLength + 2;
	NotClientWidth  = 32;
	DrawTip(NotClientLength,NotClientWidth,ClientWidth);
	x = ExX0 + ClientLength / 2;
	y = ExY0 - NotClientWidth / 2;
	HeadRadius = (NotClientWidth - 12) / 2;
	DrawBlock(ExX0 - 4,ExY0 - NotClientWidth,ExX0 + NotClientLength,ExY0 + ClientWidth + 2, CONVEX);
	DrawBlock(ExX0 - 2,ExY0 - NotClientWidth + 2,ExX0 + ClientLength ,ExY0 - 2, CONVEX);
	DrawBlock(ExX0 + 10,ExY0 - NotClientWidth  + 4,ExX0 + 10 + TimeTableWidth, ExY0 -  4, INDENT);
	DrawBlock(ExX0 + ClientLength - TimeTableWidth - 10,ExY0 - 28, ExX0 + ClientLength - 10,ExY0 - 4,INDENT);

	setcolor(YELLOW);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	circle(x,y,HeadRadius);
	setfillstyle(SOLID_FILL,YELLOW);
	floodfill(x,y,YELLOW);

	setcolor(BLACK);
	setlinestyle(SOLID_LINE,0,THICK_WIDTH);
	circle(x - HeadRadius / 2,y - HeadRadius / 3, 1);
	setlinestyle(SOLID_LINE,0,THICK_WIDTH);
	circle(x + HeadRadius / 2,y - HeadRadius / 3, 1);

	DrawFace(SMILE);

}

void DrawFlag(int i,int j,int type)
{
	int FlagWidth = 5,FlagLength = 11,x,y;
	CalOrdinate(i,j,&x,&y);
	x += (WIDTH - FlagWidth) / 2;
	y += (WIDTH - FlagLength) / 2;
	setwritemode(type);
	setcolor(CYAN);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	line(x + 1,y,x + 1,y + FlagLength);
	line(x + 1,y,x + FlagWidth - 1,y + FlagLength / 2);
	line(x + 1,y + FlagLength * 2 /3 ,x + FlagWidth,y + FlagLength * 2 / 3);
}

void ShowMarkNumber(int Number)
{
	int NotClientWidth;
	int TimeTableWidth = 50;
	char * OutText = NULL;

	NotClientWidth  = 32;

	DrawBlock(ExX0 + 10,ExY0 - NotClientWidth  + 4,ExX0 + 10 + TimeTableWidth, ExY0 -  4, INDENT);
	setwritemode(0);
	if(Number < 10)
		sprintf(OutText,"00%d",Number);
	else if(Number < 100)
		sprintf(OutText,"0%2d",Number);
	else
		sprintf(OutText,"%3d",Number);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(RED);
	outtextxy(ExX0 +  10 + TimeTableWidth / 2 ,ExY0 - NotClientWidth / 2  ,OutText);


}

void ShowTime(int T)
{
	int ClientLength,NotClientLength,NotClientWidth;
	int x,y;
	int TimeTableWidth = 50;
	char *Time = NULL;
	ClientLength = ExCol * ( WIDTH + 1) + 1;
	NotClientWidth  = 32;
	if(T < 10)
		sprintf(Time,"00%d",T);
	else if(T < 100)
		sprintf(Time,"0%2d",T);
	else
		sprintf(Time,"%3d",T);
	DrawBlock(ExX0 + ClientLength - TimeTableWidth  - 10 ,ExY0 - 28, ExX0 + ClientLength - 10,ExY0 - 4,INDENT);
	setwritemode(0);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(RED);
	outtextxy(ExX0 +  ClientLength - TimeTableWidth / 2 - 10 ,ExY0 - NotClientWidth / 2  ,Time);
}



void DrawClientArea()
{
	int i,j,x,y;

	y = ExY0;

	for( i =  0; i < ExRow; ++i)
	{
		x = ExX0;
		for( j = 0;j < ExCol; ++j)
		{
			DrawBlock(x,y,x+WIDTH,y+WIDTH,CONVEX);
			x += WIDTH + 1;
		}
		y += WIDTH + 1;
	}
}

void DimensionConversion(int *k,int *i,int *j,int Type)
{
	if(Type == DimensionsToOne)
		*k = (*i - 1) * ExRow + (*j) - 1;
	else
	{
		*i = (*k) / ExCol + 1;
		*j = (*k) % ExCol + 1;
	}
}



void RandomizeMinesPosition()   /* named LayoutMines is available */
{
	int SumMines  = LayoutSum * 3 / 10,CountMines = 0,RandomNumber;
	randomize();
	while(CountMines < SumMines)
	{
		RandomNumber = random(LayoutSum);
		if( 0 == MinesPosition[RandomNumber])
		{
			MinesPosition[RandomNumber] = MINE;
			++CountMines;
		}
	}
}

void DrawDiggedBackground(int i,int j)
{
	int x,y;

	CalOrdinate(i,j,&x,&y);
	setwritemode(0);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	setcolor(RED);         /* mark the diged mine by magenta rectangle */
	rectangle(x+1,y+1,x + WIDTH-1,y + WIDTH-1);
	setfillstyle(SOLID_FILL,RED);
	floodfill(x + WIDTH/2,y + WIDTH/2,RED);
}

void DrawWrongMark(int i,int j)
{
	int x,y;

	CalOrdinate(i,j,&x,&y);
	setlinestyle(SOLID_LINE,0,NORM_WIDTH);
	setcolor(RED);
	line(x + 3,y + 3, x + WIDTH - 3,y + WIDTH - 3);
	line(x + 3,y + WIDTH - 3,x + WIDTH - 3,y + 3);
}

void DrawSingleMine(int i,int j,int type)
{
  int x,y,Radius = 3.5,Width = 2;

  CalOrdinate(i,j,&x,&y);
  setwritemode(0);
  switch(type)
  {
	case NORMAL :      DrawBlock(x,y,x + WIDTH,y + WIDTH,INDENT);break;
	case DIGGED :      DrawDiggedBackground(i,j);break;
	case MARKEDWRONG : DrawBlock(x,y,x + WIDTH,y + WIDTH,INDENT);
					   DrawWrongMark(i,j);break;
  }
  setcolor(BLACK);
  circle(x + WIDTH / 2,y + WIDTH / 2 + 2,Radius);
  setfillstyle(SOLID_FILL,BLACK);
  floodfill(x + WIDTH / 2,y + WIDTH / 2 + 2,BLACK);
  rectangle(x + WIDTH / 2 - Width / 2,y + 4,x + WIDTH / 2 + Width / 2,y + 6);
  floodfill(x + WIDTH / 2,y + 5,BLACK);
  setcolor(MAGENTA);
  line(x + WIDTH / 2,y + 4,x + WIDTH / 2 + 1.5,y + 2);
}



void ShowAllMines()
{
   int k,i,j;

   for(k = 0; k < LayoutSum; ++k)
   {
   		DimensionConversion(&k,&i,&j,DimensionsToTwo);
		if(MinesPosition[k] == MINE)
			DrawSingleMine(i,j,NORMAL);
		else if(MinesPosition[k] > MARKEDRIGHT)
			DrawSingleMine(i,j,MARKEDWRONG);
	}
}

#define Left  -1
#define Right  1
#define Up  - ExCol
#define Down  ExCol
#define LeftUp  -1 - ExCol
#define RightUp  1 - ExCol
#define LeftDown  ExCol - 1
#define RightDown  ExCol + 1

void CalAroundMines()
{

  int k,Number;

  for(k = 0; k < LayoutSum; ++k)
  {
	Number = 0;

	if(MinesPosition[k] == MINE)
		continue;

	if(k % ExCol != 0 )
	{
		if( MINE == MinesPosition[k + Left] )
			++Number;
		if((k + LeftUp) >= 0 && MINE == MinesPosition[k + LeftUp])
			++Number;
		if((k + LeftDown) < LayoutSum && MINE == MinesPosition[k + LeftDown])
			++Number;
	}
	if(k % ExCol != ExCol - 1)
	{
		if( MINE == MinesPosition[k + Right])
			++Number;
		if((k + RightUp) >= 0 && MINE == MinesPosition[k + RightUp])
			++Number;
		if((k + RightDown) < LayoutSum && MINE == MinesPosition[k + RightDown])
			++Number;
	}
	if(k / ExCol != 0 && MINE == MinesPosition[k + Up])
			++Number;
	if(k / ExCol != ExRow - 1 && MINE == MinesPosition[k + Down])
			++Number;

	MinesPosition[k] = Number;
  }
}


void ShowGameOver()
{
	int x , y;
	char Out[] = "Game Over!";

	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	x = getmaxx() / 2;
	y = getmaxy() / 2;
	outtextxy(x,y,Out);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,0);/***unsave the font setting ***/
}


void IsMine(int i,int j)
{

	ShowAllMines();
	DrawSingleMine(i,j,DIGGED);

	ShowGameOver();
    DrawFace(CRY);
}

void ShowDigit(int i,int j,int Number)
{
  int x,y;

  char *OutText = NULL;
  CalOrdinate(i,j,&x,&y);
  setwritemode(0);
  DrawBlock(x,y,x + WIDTH,y + WIDTH,INDENT);
  switch(Number)
  {
	case 1 : setcolor(GREEN); 	   break;
	case 2 : setcolor(YELLOW);	   break;
	case 3 : setcolor(RED);        break;
	case 4 : setcolor(BLUE);       break;
	case 5 : setcolor(CYAN);       break;
	case 6 : setcolor(WHITE);      break;
	case 7 : setcolor(BROWN);      break;
	case 8 : setcolor(LIGHTGREEN); break;
  }
  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
  setfillstyle(SOLID_FILL,BLACK);
  sprintf(OutText,"%d",Number);
  settextjustify(CENTER_TEXT,CENTER_TEXT);/**** set perform  center****/
  x += WIDTH / 2;
  y += WIDTH / 2;
  outtextxy(x ,y ,OutText);
}



void ShowSuccess()
{
    int x , y;
	char Out[] = " Success !";
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	x = getmaxx() / 2;
	y = getmaxy() / 2;
	outtextxy(x,y,Out);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,0);/***unsave the font setting ***/

}

int IsSuccess()
{
	int MinesNumber =  LayoutSum * 3 / 10;
	if(MarkedRightNumber == MinesNumber  && LayoutSum - MineDiggedNumber == MinesNumber)
	{
	   ShowSuccess();
	   return TRUE;
	}else
	   return FALSE;
}

void AdjacenceIsZero(int k)
{

	if(MinesPosition[k] < 0 || MinesPosition[k]>9)   /*** beyond scope *****/
		return;

	if(MinesPosition[k] > 0 && MinesPosition[k] < 9 )
	{
		int i,j;
		DimensionConversion(&k,&i,&j,DimensionsToTwo);
		ShowDigit(i,j,MinesPosition[k]);
		MinesPosition[k] += 20;
		++MineDiggedNumber;
	}
	if(MinesPosition[k] == 0)
	{
	  int i,j,x,y;
	  DimensionConversion(&k,&i,&j,DimensionsToTwo);
	  CalOrdinate(i,j,&x,&y);
	  DrawBlock(x,y,x + WIDTH,y + WIDTH,INDENT);
	  MinesPosition[k] += 20;
      ++MineDiggedNumber;
	  if(k % ExCol != 0 )
	  {
			AdjacenceIsZero(k + Left);
			if(k + LeftUp >= 0)
				AdjacenceIsZero(k + LeftUp);
			if(k + LeftDown < LayoutSum )
				AdjacenceIsZero(k + LeftDown);
	  }
	  if(k % ExCol != ExCol - 1)
	  {
			AdjacenceIsZero(k + Right);
			if(k + RightUp >= 0 )
				AdjacenceIsZero(k + RightUp);
			if(k + RightDown < LayoutSum )
				AdjacenceIsZero(k + RightDown);
	  }
	  if(k / ExCol != 0 )
			AdjacenceIsZero(k + Up);
	  if(k / ExCol != ExRow - 1 )
			AdjacenceIsZero(k + Down);

	}


}


void CheckMines(int i ,int j,int *GameOver,int *SFlag)
{
	int k;

	DimensionConversion(&k,&i,&j,DimensionsToOne);
	if(MinesPosition[k] == MINE)
	{
		IsMine(i,j);
        *GameOver = 1;
	}
	if(MinesPosition[k] < 9 && MinesPosition[k] >0)
	{
		ShowDigit(i,j,MinesPosition[k]);
		MinesPosition[k] += 20;
		++MineDiggedNumber;
		*SFlag = 1;
	}

	if( 0 == MinesPosition[k])
		AdjacenceIsZero(k);

}

void MarkOrCancleMine(int i,int j)
{
	int k;
	static int Number = 0;

	DimensionConversion(&k,&i,&j,DimensionsToOne);
	if(MinesPosition[k] == MINE)
	{
		MinesPosition[k] = MARKEDRIGHT;
		++MarkedRightNumber;
		DrawFlag(i,j,MARK);
		++Number;
		ShowMarkNumber(Number);
	}
	else if(MinesPosition[k] > -1 && MinesPosition[k] < 9)
	{
		MinesPosition[k] += MARKEDRIGHT + 1;/*****MARKED WRONG*******/
		DrawFlag(i,j,MARK);
		++Number;
        ShowMarkNumber(Number);
	}
	else if(MinesPosition[k] >= 100)
	{
		MinesPosition[k] -= (MARKEDRIGHT + 1);/*****MARKED CANCLE******/
		DrawFlag(i,j,CANCLE);
		--Number;
		if(Number >= 0)
			ShowMarkNumber(Number);
	}
}


void DrawCursor(int i,int j)
{
	int x,y;

	CalOrdinate(i,j,&x,&y);
    setwritemode(1);
	setcolor(BLUE);
	rectangle(x + 2, y + 2,x + WIDTH - 3 ,y + WIDTH - 3);
}

void IsGameOver(int i,int j,int* GameOver,int *Key,int *Control)
{
	 if((*Key) == ESC )
	 {
		*Control = ESC;

	 }
	 if(*GameOver != 1)
			DrawCursor(i,j);
	 if(*Key == RESTART || *Key == F1 || *Key ==F2 || *Key == F3 )
	 {
				*Control = *Key;
				(*Key) = ESC;
	 }

}


void StoreShowImage(int *Type)
{
	int ImageX0,ImageY0,ImageX1,ImageY1,NotClientWidth = 32,TipZoneWidth = 15;
	unsigned int ImageSize = 0;
	static unsigned int *ImageAddr = NULL;

	CalOrdinate(ExRow,ExCol,&ImageX1,&ImageY1);
	ImageX1 += 2;
	ImageY1 += 2;
	ImageX0 = ExX0 - 4;
	ImageY0 = ExY0 - NotClientWidth - TipZoneWidth;

	if(*Type == STORE)
	{
		ImageSize = imagesize(0,0,getmaxx(),getmaxy());
		ImageAddr = (unsigned int *)malloc(ImageSize * sizeof(unsigned int));
		if(ImageAddr == NULL)
			printf("Memory allocation failed!\n");
		if(ImageAddr != NULL)
			getimage(ImageX0,ImageY0,ImageX1,ImageY1,ImageAddr);
		--(*Type);
        DrawMenu();
	}else
	{
		putimage(ImageX0,ImageY0,ImageAddr,COPY_PUT);
		++(*Type);
		free(ImageAddr);
	}

}

void InitCursor()
{
    if(ExRow % 2 == 1 && ExCol % 2 == 1)
	{
			ExI0 = ExRow / 2 + 1;
			ExJ0 = ExCol / 2 + 1;
	}else
	{
			ExI0 = 1;
			ExJ0 = 1;
	}
}


void MoveCursor(int *Control)
{
	int MenuFlag = 1,i = ExI0,j = ExJ0;
	int T = 0,StartFlag = 0,GameOver = 0, Key = 0;
	time_t Time;

	Time = time(NULL);
	ShowTime(T);  		/* display the second is 0 */
	ShowMarkNumber(0);  /* display the number ofmarked mines is 0 */

	DrawCursor(i,j);
	while(Key != ESC )
	{
		if(IsSuccess())
			GameOver = TRUE;

		if(Time != time(NULL) && StartFlag != 0 && GameOver == 0)
		{
			Time = time(NULL);
			++T;
			ShowTime(T);
		}
		Key = bioskey(1);
		if(Key == 0 )
			continue;
		Key = bioskey(0);


		IsGameOver(i,j,&GameOver,&Key,Control);
		if((Key == ESC && *Control != ESC) || GameOver == TRUE)
			continue;

		setwritemode(0);
	 switch(Key)
	 {
		case DOWN      : if (i != ExRow ) ++i;                    break;
		case UP        : if (i != 1 )     --i;                    break;
		case LEFT      : if (j != 1 )     --j;                    break;
		case RIGHT     : if (j != ExCol ) ++j;                    break;
		case SPACE     : CheckMines(i,j,&GameOver,&StartFlag);    break;
		case ENTER     : MarkOrCancleMine(i,j);    				  break;
		case ESC       : *Control = ESC;                          break;
		case F10       : StoreShowImage(&MenuFlag);               break;
	 }
	 if(GameOver != TRUE)
		DrawCursor(i,j);

   }
}



void ProgramTurnTo(int *Key,int *Difficulty)
{
		if(*Key == F1)
			*Difficulty = SENIOR;
		else if(*Key == F2)
			*Difficulty = MIDDLE;
		else if(*Key == F3)
			*Difficulty = HIGH;

}

void CloseSystem()
{
	free(MinesPosition);
	closegraph();
}


void main()
{
	int Control = 0,Difficulty  = SENIOR;

	do
	{
        int ProgramExecutionTime   = START;
		InitSetDifficulty(Difficulty); /* set default difficulty is senior */
		InitSystem();
		InitDrawOriginalPoint();
		DrawNotClientArea();
		DrawClientArea();

		RandomizeMinesPosition();
		CalAroundMines();

		Control = bioskey(1);
		if(Control == 0 && ProgramExecutionTime != START)    continue;
		if(ProgramExecutionTime != START)
			Control = bioskey(0) ;
		++ProgramExecutionTime;
		InitCursor();
		MoveCursor(&Control);
		if(Control != ESC )
			ProgramTurnTo(&Control,&Difficulty);
		CloseSystem();

	}while(Control != ESC );

}


