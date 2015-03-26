#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<time.h>
#include<stdlib.h>

#define INDENT 1
#define CONVEX 0
#define WIDTH  15

#define ENTER     7181
#define BACKSPACE 3592
#define INSERT    20992
#define DELETE    21248

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

#define DimensionsToOne 1
#define DimensionsToTwo 2

char sds[10];

int ExMaxX,ExMaxY,ExRow,ExCol;
int LayoutSum ;

int *MinesPosition = NULL;
int *MinesAroundNumber = NULL;


void InitSystem()
{   char f[100];
	int graphdriver,graphmode,i;
	detectgraph(&graphdriver,&graphmode);
	initgraph(&graphdriver,&graphmode,"C:\\TC20\\BGI");
	cleardevice();
	char s[100];
	setbkcolor(BLACK);
	ExMaxX = getmaxx();
	ExMaxY = getmaxy();
	ExRow = 10;
	ExCol = 10;
	LayoutSum = ExCol * ExRow;
	MinesPosition = (int *)malloc( LayoutSum * sizeof(int) );

	if(MinesPosition == NULL)
	{
		exit(1);
		outtextxy(500,730,"The memory wasn't allocation!");
	}
	for(i = 0; i < LayoutSum; ++i)
		MinesPosition[i] = 0;

	MinesAroundNumber = (int*)malloc(LayoutSum * sizeof(int)) ;
	if( MinesAroundNumber == NULL)
	{
    	exit(1);
		outtextxy(500,730,"The memory wasn't allocation!");
	}

}

void BeginPoint(int *x0,int *y0)
{
	*x0 =300 ; /*(ExMaxX - ExCol * (WIDTH + 1) + 1) / 2;*/
	*y0 =300 ; /*(ExMaxY - ExRow * (WIDTH + 1) + 1) / 2;    */
}



void DrawBlock(int x1,int y1,int x2,int y2,int type)
{
	setcolor(WHITE);
	rectangle(x1,y1,x2,y2);
	setfillstyle(SOLID_FILL,LIGHTGRAY);
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
	line(x1 - 1,y2 - 1,x2 - 1,y2 - 1);  /* draw bottom and interior line of rectangle */
	line(x2 - 1,y1 + 1,x2 - 1,y2 - 1);  /* draw right and interior line of rectangle */
}


void DrawNotClientArea()
{
	int x0,y0;
	int Length = ExCol * ( WIDTH + 1) + 1; /* the length of client area */
	int Width = ExRow * ( WIDTH + 1) + 1; /* the width of client area */
	BeginPoint(&x0,&y0);
	DrawBlock(x0 - 4,y0 - 32 ,x0 + Length + 2,y0 +  Width + 2, CONVEX);
	DrawBlock(x0 - 2,y0 - 30 ,x0 + Length ,y0 - 2, CONVEX);
	DrawBlock(x0 + 10, y0 - 28,x0 + 50, y0 -  4, INDENT);
	DrawBlock(x0 + Length - 50,y0 - 28, x0 + Length - 10,y0 - 4,INDENT);
}



void DrawClientArea()
{
	int i,j,x,y,x0,y0;

	BeginPoint(&x0,&y0);

	y = y0;

	for( i =  0; i < ExRow; ++i)
	{
		x = x0;
		for( j = 0;j < ExCol; ++j)
		{
			DrawBlock(x,y,x+WIDTH,y+WIDTH,CONVEX);
			x += WIDTH + 1;
		}
		y += WIDTH + 1;
	}
}





void InitMoveCursor()
{
	int  Key = 0,i = ExRow / 2, j = ExCol / 2,x0,y0,x,y;
    setcolor(BLUE);
	setlinestyle(DASHED_LINE,0,THICK_WIDTH);
	BeginPoint(&x0,&y0);
	while(Key != ESC )
	{
	 x = x0 + (j - 1 ) * (WIDTH + 1);
	 y = y0 + (i - 1 ) * (WIDTH + 1);
     setwritemode(1);
	 rectangle(x + 2, y + 2,x + WIDTH -2 ,y + WIDTH -2);
	 Key = bioskey(0);
     rectangle(x + 2, y + 2,x + WIDTH -2 ,y + WIDTH -2);
	 setwritemode(0);
	 switch( Key )
	 {
		case DOWN  : if (i != ExRow ) ++i;break;
		case UP    : if (i != 1 )     --i;break;
		case LEFT  : if (j != 1 )     --j;break;
		case RIGHT : if (j != ExCol ) ++j;break;
	 }
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
			MinesPosition[RandomNumber] = -1;
			++CountMines;
		}

	}
}


void DrawSingleMine(int i,int j)
{
  int x,y,x0,y0,Radius = 3.5,Width = 2;
  BeginPoint(&x0,&y0);
  x = x0 + (j - 1) * (WIDTH + 1) ;
  y = y0 + (i - 1) * (WIDTH + 1) ;
  setwritemode(0);
  DrawBlock(x,y,x + WIDTH,y + WIDTH,INDENT);
  setcolor(BLACK);
  circle(x + WIDTH / 2,y + WIDTH / 2 + 2,Radius);
  setfillstyle(SOLID_FILL,BLACK);
  floodfill(x + WIDTH / 2,y + WIDTH / 2 + 2,BLACK);
  rectangle(x + WIDTH / 2 - Width / 2,y + 4,x + WIDTH / 2 + Width / 2,y + 6);
  floodfill(x + WIDTH / 2,y + 5,BLACK);
  setcolor(MAGENTA);
  line(x + WIDTH / 2,y + 4,x + WIDTH / 2 + 1.5,y + 2);
}

void ShowMines()
{
   int k,i,j;
   for(k = 0; k < LayoutSum; ++k)
   {
		if(MinesPosition[k] == -1)
		{
			DimensionConversion(&k,&i,&j,DimensionsToTwo);
			DrawSingleMine(i,j);
		}
	}
}


void CalAroundMines()
{

  int k,Number;
  int Left = -1,Right = 1,Up = - ExCol,Down = ExCol;
  int LeftUp = -1 - ExCol,RightUp = 1 - ExCol;
  int LeftDown = ExCol - 1,RightDown = ExCol + 1;

  for(k = 0; k < LayoutSum; ++k)
  {

	Number = 0;

	if(k % ExCol != 0 )
	{
	if( -1 == MinesPosition[k + Left] )
			++Number;
	if((k + LeftUp) >= 0 && -1 == MinesPosition[k + LeftUp])
			++Number;
	if((k + LeftDown) < LayoutSum && -1 == MinesPosition[k + LeftDown])
			++Number;
	}
	if(k % ExCol != ExCol - 1)
	{
	if( -1 == MinesPosition[k + Right])
			++Number;
	if((k + RightUp) >= 0 && -1 == MinesPosition[k + RightUp])
			++Number;
	if((k + RightDown) < LayoutSum && -1 == MinesPosition[k + RightDown])
			++Number;
	}
	if(k / ExCol != 0 && -1 == MinesPosition[k + Up])
		++Number;
	if(k / ExCol != ExRow - 1 && -1 == MinesPosition[k + Down])
		++Number;
	MinesAroundNumber[k] = Number;
	}
}




void CloseSystem()
{
	free(MinesPosition);
	free(MinesAroundNumber);
	closegraph();
}

void main()
{  int i,j;
	InitSystem();
	DrawNotClientArea();
	DrawClientArea();

	RandomizeMinesPosition();
	ShowMines();
	CalAroundMines();
	for(i = 0,j = 1; i< LayoutSum; ++i,++j)
	{
		printf("%2d-%d  ",MinesPosition[i],MinesAroundNumber[i]);
		if (j % 10 == 0)
		printf("\n");
	}

	InitMoveCursor();

	/*setcolor(WHITE);
	setwritemode(0);
	outtextxy(500,730,"The memory wasn't allocation!");
	*/
	CloseSystem();
}


