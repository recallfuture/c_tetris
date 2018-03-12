/**
 * 本文件存放光标相关的函数定义
 *
 */

void SetPos(COORD a)// 移动光标（隐）
{
    HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, a);
}
 
void SetPos(int i, int j)// 移动光标
{
    COORD pos={i, j};
    SetPos(pos);
}
 
void HideCursor()//隐藏光标
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
 
//把第y行，[x1, x2) 之间的坐标填充为 ch
void drawRow(int y, int x1, int x2, char ch)
{
	int i;
    SetPos(x1,y);
    for(i = 0; i <= (x2-x1); i++)
        printf("%c",ch);
}
 
//在a, b 纵坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawRow(COORD a, COORD b, char ch)
{
    if(a.Y == b.Y)
        drawRow(a.Y, a.X, b.X, ch);
    else
    {
        SetPos(0, 25);
        printf("error code 01：无法填充行，因为两个坐标的纵坐标(x)不相等");
        system("pause");
    }
}
 
//把第x列，[y1, y2] 之间的坐标填充为 ch
void drawCol(int x, int y1, int y2, char ch)
{
    int y=y1;
    while(y!=y2+1)
    {
        SetPos(x, y);
        printf("%c",ch);
        y++;
    }
}
 
//在a, b 横坐标相同的前提下，把坐标 [a, b] 之间填充为 ch
void drawCol(COORD a, COORD b, char ch)
{
    if(a.X == b.X)
        drawCol(a.X, a.Y, b.Y, ch);
    else
    {
        SetPos(0, 25);
        printf("error code 02：无法填充列，因为两个坐标的横坐标(y)不相等");
        system("pause");
    }
}

//左上角坐标、右下角坐标、用row填充行、用col填充列
void drawFrame(COORD a, COORD  b, char ch)
{
    drawRow(a.Y, a.X, b.X, ch);
    drawRow(b.Y, a.X, b.X, ch);
    drawCol(a.X, a.Y+1, b.Y-1, ch);
    drawCol(b.X, a.Y+1, b.Y-1, ch);
}
 
void drawFrame(int x1, int y1, int x2, int y2, char ch)
{
    COORD a={x1, y1};
    COORD b={x2, y2};
    drawFrame(a, b, ch);
}
