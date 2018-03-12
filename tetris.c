/**
 * 此文件为主程序文件
 *
 *
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "tetromino.c"
#include "draw.c"

//定义地图行列数
#define ROW 20
#define COL 10

//定义地图type
enum mapType{
  NONE,
  BLOCK
};

//定义俄罗斯方块的类型
enum tetrominoType{
  TI,
  TL,
  TJ,
  TO,
  TT,
  TZ,
  TS
};

//全局变量
enum mapType map[ROW][COL]; //地图
int score = 0; //分数
int isOver = 0; //是否游戏结束
int speed = 3; //速度，1-3
bool isHolding = false; //是否处于按住键盘的状态
struct tetromino nowTetromino, nextTetromino;

//函数
void initMap();
void initInfo();
void initScene();
void genTetromino(struct tetromino* t);
struct block getBlock(struct tetromino* t, int num);
void flushTetromino(struct tetromino* before, struct tetromino* after);
void falling();
bool checkOver();
bool checkClear(int row);
int checkMove(struct tetromino* after, bool isFall);
void clear();
void keepOnSence(struct tetromino* t);
void flushInfo();
void showScoreInfo();
void showNextInfo(struct tetromino* after);

int main(){
  HideCursor();
  initScene();

  _getch();
  while(!isOver)
    falling();

  _getch();
  return 0;
}

//初始化地图
void initMap(){
  int i,j;
  for(i=0; i<ROW; i++)
    for(j=0; j<COL; j++)
      map[i][j] = NONE;
}

//初始化提示信息和地图边界
void initInfo(){
  drawCol(30, 0, 20, '|');
  drawRow(20, 0, 30, '=');
	SetPos(35,13);
	printf("Use a, d to move, space to rotate");
  SetPos(35, 14);
  printf("s to move down, and space to control.");
	SetPos(35,17);
	printf("Any key to start!");
}

//初始化变量和场景
void initScene(){
  initMap();
  initInfo();

  genTetromino(&nowTetromino);
  flushTetromino(NULL, &nowTetromino);

  genTetromino(&nextTetromino);
  flushInfo();

  score = 0;
  isOver = 0;
}

//生成一块俄罗斯方块
void genTetromino(struct tetromino* t){
  int type;
  srand(time(0) + type);
  type = rand()%7;

  switch(type){
  case TI:
    initI(t);
    break;
  case TL:
    initL(t);
    break;
  case TJ:
    initJ(t);
    break;
  case TO:
    initO(t);
    break;
  case TT:
    initT(t);
    break;
  case TZ:
    initZ(t);
    break;
  case TS:
    initS(t);
    break;
  }
}

//获得真实坐标
//参数num为此组方块的第num位
struct block getBlock(struct tetromino* t, int num){
  struct block b = {
    t->main.x + t->pOffset->allBlocks[num].x,
    t->main.y + t->pOffset->allBlocks[num].y,
  };

  return b;
}

//刷新俄罗斯方块
void flushTetromino(struct tetromino* before, struct tetromino* after){
  int i;

  if(before != NULL){
    for(i=0; i<4; i++){
      struct block beforeBlock = getBlock(before, i);
      if(beforeBlock.x < 0) continue;

      SetPos(beforeBlock.y * 3, beforeBlock.x);
      puts("   ");
    }
  }

  for(i=0; i<4; i++){
    struct block afterBlock = getBlock(after, i);
    if(afterBlock.x < 0) continue;

    SetPos(afterBlock.y * 3, afterBlock.x);
    printf("[%c]", after->sign);
  }
}

//下落过程
void falling(){
  int curSpeed = 40 - speed*10;
  struct tetromino nextStep = nowTetromino;
  char ch;

  while(curSpeed--){
    if(!_kbhit())
      isHolding = false;
    else if(!isHolding){
      ch = _getch();
      isHolding = true;

      switch(ch){
      case 'a':
      case 'A':
        {
          nextStep.main.y--;
          break;
        }
      case 'd':
      case 'D':
        {
          nextStep.main.y++;
          break;
        }
      case 's':
      case 'S':
        {
          do{
            nextStep.main.x++;
          }
          while(checkMove(&nextStep, true) == 0);
          nextStep.main.x--;
          break;
        }
      case ' ':
        {
          nextStep.pOffset = nextStep.pOffset->next;
          break;
        }
      }

      if(checkMove(&nextStep, false) == 0){
        flushTetromino(&nowTetromino, &nextStep);
        nowTetromino = nextStep;
      }
    }

    while(_kbhit())
       ch = _getch();

    Sleep(20);
  }

  //判定能否下落
  nextStep = nowTetromino;
  nextStep.main.x++;
  int result = checkMove(&nextStep, true);
  if(result == 0){
    flushTetromino(&nowTetromino, &nextStep);
    nowTetromino = nextStep;
  }
  else if(result == 1){
    nextStep = nowTetromino;
  }
  else if(result == 2){
    nextStep = nowTetromino;
    keepOnSence(&nowTetromino);
    //在此判定是否可以消除
    clear();
    if(checkOver()){
      isOver = 1;
      freeOffsetChain(&nowTetromino);
      freeOffsetChain(&nextTetromino);
      return;
    }
    freeOffsetChain(&nowTetromino);
    nowTetromino = nextTetromino;
    genTetromino(&nextTetromino);
    flushTetromino(NULL, &nowTetromino);
    flushInfo();
  }
}

//检查目标位置是否可以移动
//返回0代表可以移动
//返回1代表被边框或者固定住的方块阻挡
//返回2代表下落失败
int checkMove(struct tetromino* nextStep, bool isFall){
  int i;
  struct block b;

  for(i=0; i<4; i++){
    b=getBlock(nextStep, i);
    if(b.x < 0) continue;
    if(b.x >= ROW) return 2;
    if(b.y < 0 || b.y >= COL) return 1;
    if(map[b.x][b.y] != NONE){
      return (isFall ? 2 : 1);
    }
  }
  return 0;
}

//将方块固定到场景上面
void keepOnSence(struct tetromino* t){
  int i;
  struct block b;
  for(i=0; i<4; i++){
    b = getBlock(t, i);
    if(b.x < 0) continue;
    map[b.x][b.y] = BLOCK;

    SetPos(b.y * 3, b.x);
    puts("[*]");
  }
}

//检查某一行能否消除
bool checkClear(int row){
  int i;
  for(i=0; i<COL; i++){
    if(map[row][i] == NONE)
      return false;
  }
  return true;
}

void clear(){
  int i,j,k;
  for(i=ROW-1; i>=0; ){
    if(checkClear(i)){
      //加分
      score++;
      //清空这一行
      for(j=0; j<COL; j++){
        map[i][j] = NONE;
      }
      //让上面的落下来
      for(j=i-1; j>=0; j--){
        for(k=0; k<COL; k++){
          if(map[j][k] != NONE){
            map[j+1][k] = BLOCK;
            map[j][k] = NONE;
          }
        }
      }
    }
    else i--;
  }
  //重绘场景
  for(i=0; i<ROW; i++){
    for(j=0; j<COL; j++){
      SetPos(j*3, i);
      puts((map[i][j] == NONE)?"   ":"[*]");
    }
  }
}

//判断是否已经落到顶层
bool checkOver(){
  int i;
  for(i=0; i<COL; i++){
    if(map[0][i] == BLOCK){
      return true;
    }
  }
  return false;
}

void flushInfo(){
  int i;
  for(i=3; i<11; i++){
    SetPos(35, i);
    printf("                    ");
  }

  showNextInfo(&nextTetromino);
  showScoreInfo();
}

void showScoreInfo(){
  SetPos(35, 10);
  printf("Now score: %d", score);
}

void showNextInfo(struct tetromino* after){
  SetPos(35, 3);
  printf("Next will be :");

  struct block b1,b2 = {8, 13};
  b1 = after->main;
  after->main = b2;
  flushTetromino(NULL, after);
  after->main = b1;
}
