#pragma once

#include "utility.h"
//------------ �궨��----------------------------
//��ͼ�Ŀ�͸�
#define RowOfUIMap 22
#define ColOfUIMap 12      //column user interface

//����˹�������������
#define ShapeType  7

//------------ �������͵Ķ��� -------------------
enum CUIMap_Info
{
    //ö��ֵ
    UI_BLANK,
    UI_BORDER,
    UI_BLOCK,
    UI_FIXED_BLOCK,
    UI_NONE
};

//�������ַ���---- - ���󷽿� CBlock
//�������˹����Ľṹ�� -- ��������һ������˹���飬���ͼֽ
struct CPosition
{
    int x;
    int y;
};

struct CBlock   //Class ���� Block 
{
    char name;           //���� 7
    char shape[4][4];    //��״         
    CPosition position;  //λ��  //int xPos; int yPos;   
    int size;            //��С
};


//------------- ����---------------------------
extern int UIMap[RowOfUIMap][ColOfUIMap];
extern CBlock g_curBlock; //current
extern CBlock g_nextBlock;
extern int g_score;

//����̨����ƶ�����
void gotoXY(int x, int y);

//����̨���ع����ʾ
void hideCursorDisplay();

//�����ı���ɫ
void setColor(unsigned short ForeColor, unsigned short BackGroundColor = 0);

//�ӱ����ļ���ȡ����ʼ�� ��ͼ����
void initUIMapFromFile(char* pFilePath);

//��ϷUI����Ĵ�ӡ����
void printUIMap();

//��������
void generateBlock(CBlock& block);

//���Ʒ��鵽��Ϸ������߼�����
void drawBlock(const CBlock& block);

//���������ϵķ���
void eraseBlock(const CBlock& block);

//�ƶ�����˹����
void moveBlock(CBlock& block, int dx, int dy);

//�ж��Ƿ��ܹ��ƶ�
bool canMove(const CBlock& block, int dx, int dy);

//����һ������ ���ݸ���ǰ���飬���Ҹ�����һ������
void transferNextBlockToCurBlockAndUpdateIt();

//��ǰ���鲻���ƶ��󣬼�¼Ϊ�̶����飬����UIMap��ֵ ��BLOCK ��ΪFIXED_BLOCK
void updateCurrentBlockStatusOfUIMap();

//��ӡNext����
void  printNextUIMap();

//���в��Ƿ�
void checkUIMapAndRemoveFullLine();

//����ı���
void rotateBlock(CBlock& block);

//��ʱ����ת��״����ĺ���
void rotateCells(char shape[][4], int size);

//�����ܷ���ת
bool canRotate(const CBlock& block);

//�ж���Ϸ�Ƿ����
bool isEnd();