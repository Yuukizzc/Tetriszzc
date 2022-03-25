#pragma once

#include "utility.h"
//------------ 宏定义----------------------------
//地图的宽和高
#define RowOfUIMap 22
#define ColOfUIMap 12      //column user interface

//俄罗斯方块的种类类型
#define ShapeType  7

//------------ 数据类型的定义 -------------------
enum CUIMap_Info
{
    //枚举值
    UI_BLANK,
    UI_BORDER,
    UI_BLOCK,
    UI_FIXED_BLOCK,
    UI_NONE
};

//二、出现方块---- - 抽象方块 CBlock
//定义俄罗斯方块的结构体 -- 便于描述一个俄罗斯方块，设计图纸
struct CPosition
{
    int x;
    int y;
};

struct CBlock   //Class 类型 Block 
{
    char name;           //类型 7
    char shape[4][4];    //形状         
    CPosition position;  //位置  //int xPos; int yPos;   
    int size;            //大小
};


//------------- 声明---------------------------
extern int UIMap[RowOfUIMap][ColOfUIMap];
extern CBlock g_curBlock; //current
extern CBlock g_nextBlock;
extern int g_score;

//控制台光标移动函数
void gotoXY(int x, int y);

//控制台隐藏光标显示
void hideCursorDisplay();

//设置文本颜色
void setColor(unsigned short ForeColor, unsigned short BackGroundColor = 0);

//从本地文件读取，初始化 地图数组
void initUIMapFromFile(char* pFilePath);

//游戏UI界面的打印函数
void printUIMap();

//产生方块
void generateBlock(CBlock& block);

//绘制方块到游戏界面的逻辑数组
void drawBlock(const CBlock& block);

//擦除界面上的方块
void eraseBlock(const CBlock& block);

//移动俄罗斯方块
void moveBlock(CBlock& block, int dx, int dy);

//判断是否能够移动
bool canMove(const CBlock& block, int dx, int dy);

//将下一个方块 传递给当前方块，并且更新下一个方块
void transferNextBlockToCurBlockAndUpdateIt();

//当前方块不能移动后，记录为固定方块，更新UIMap的值 由BLOCK 变为FIXED_BLOCK
void updateCurrentBlockStatusOfUIMap();

//打印Next界面
void  printNextUIMap();

//消行并记分
void checkUIMapAndRemoveFullLine();

//方块的变形
void rotateBlock(CBlock& block);

//逆时针旋转形状数组的函数
void rotateCells(char shape[][4], int size);

//方块能否旋转
bool canRotate(const CBlock& block);

//判断游戏是否结束
bool isEnd();