/**
 * 此文件存放方块的定义以及其初始化函数的定义
 *
 *
 */
#include<malloc.h>

//单个方块
struct block{
  int x,y;
};

//偏移向量，用首尾相接的链表来表示
struct offset{
  struct block allBlocks[4];
  struct offset* next;
};

//一组方块
//由一个主块和其他块的偏移量数组组成
//每次变换位置，只修改主块
//每一次旋转，只需让指针指向下一个偏移变量
//记得释放链表
struct tetromino{
  struct block main;
  struct offset* pOffset;
  char sign;
};

//新建一个偏移量变量
struct offset* newOffset(struct block* blocks){
  struct offset* os = (struct offset*)malloc(sizeof(struct offset));

  int i;
  for(i=0; i<4; i++){
    os->allBlocks[i] = blocks[i];
  }

  return os;
}

//新建偏移量链表
struct offset* newOffsetChain(struct block pb[][4], int num){
  if(pb == NULL || num <= 0 || num >4) return NULL;

  struct offset *head, *next, *before;
  head = before = newOffset(pb[0]);

  int i;
  for(i=1; i<num; i++){
    next = newOffset(pb[i]);
    before->next = next;
    before = next;
  }
  before->next = head;
  return head;
}

//释放链表
void freeOffsetChain(struct tetromino* t){
  struct offset *head, *next;
  head = next = t->pOffset;

  do{
    free(next);
    next = next->next;
  }while(next != head);
}

//初始化I形状的方块组
void initI(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 4;

  //初始化偏移量链表
  struct block blocks[2][4] = {
    {
      {0, 0},
      {-1,0},
      {-2, 0},
      {-3, 0}
    },
    {
      {0, 0},
      {0, -1},
      {0, 1},
      {0, 2}
    }
  };
  t->pOffset = newOffsetChain(blocks, 2);
  t->sign    = 'I';
}

//初始化L形状的方块组
void initL(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 4;

  //初始化偏移量链表
  struct block blocks[4][4] = {
    {
      {0, 0},
      {-1,0},
      {-2, 0},
      {0, 1}
    },
    {
      {0, 0},
      {1, 0},
      {0, 1},
      {0, 2}
    },
    {
      {0, 0},
      {0,-1},
      {1, 0},
      {2, 0}
    },
    {
      {0, 0},
      {-1, 0},
      {0, -1},
      {0, -2}
    }
  };
  t->pOffset = newOffsetChain(blocks, 4);
  t->sign    = 'L';
}

//初始化J形状的方块组
void initJ(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 5;

  //初始化偏移量链表
  struct block blocks[4][4] = {
    {
      {0, 0},
      {-1,0},
      {-2, 0},
      {0, -1}
    },
    {
      {0, 0},
      {-1, 0},
      {0, 1},
      {0, 2}
    },
    {
      {0, 0},
      {0,1},
      {1, 0},
      {2, 0}
    },
    {
      {0, 0},
      {1, 0},
      {0, -1},
      {0, -2}
    }
  };
  t->pOffset = newOffsetChain(blocks, 4);
  t->sign    = 'J';
}

//初始化O形状的方块组
void initO(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 4;

  //初始化偏移量链表
  struct block blocks[1][4] = {
    {
      {0, 0},
      {-1,0},
      {-1, 1},
      {0, 1}
    }
  };
  t->pOffset = newOffsetChain(blocks, 1);
  t->sign    = 'O';
}

//初始化T形状的方块组
void initT(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 4;

  //初始化偏移量链表
  struct block blocks[4][4] = {
    {
      {0, 0},
      {0,1},
      {-1, 0},
      {0, -1}
    },
    {
      {0, 0},
      {-1, 0},
      {0, 1},
      {1, 0}
    },
    {
      {0, 0},
      {0,1},
      {1, 0},
      {0, -1}
    },
    {
      {0, 0},
      {1, 0},
      {0, -1},
      {-1, 0}
    }
  };
  t->pOffset = newOffsetChain(blocks, 4);
  t->sign    = 'T';
}

//初始化Z形状的方块组
void initZ(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 4;

  //初始化偏移量链表
  struct block blocks[2][4] = {
    {
      {0, 0},
      {-1,-1},
      {-1, 0},
      {0, 1}
    },
    {
      {0, 0},
      {1, 0},
      {0, 1},
      {-1, 1}
    }
  };
  t->pOffset = newOffsetChain(blocks, 2);
  t->sign    = 'Z';
}

//初始化S形状的方块组
void initS(struct tetromino* t){
  //初始化主块的位置
  t->main.x = 0;
  t->main.y = 4;

  //初始化偏移量链表
  struct block blocks[2][4] = {
    {
      {0, 0},
      {-1,1},
      {-1, 0},
      {0, -1}
    },
    {
      {0, 0},
      {1, 0},
      {0, -1},
      {-1, -1}
    }
  };
  t->pOffset = newOffsetChain(blocks, 2);
  t->sign    = 'S';
}
