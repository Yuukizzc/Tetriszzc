#include "game.h"

//控制台光标移动函数
void gotoXY(int x, int y)
{
    //注意x和y的顺序
    COORD pos = { y, x };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//控制台隐藏光标显示
void hideCursorDisplay()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hOut, &cci);
}

//设置文本颜色
void setColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
    /*
    0 - BLACK 黑             深色 0 - 7
    1 - BLUE 兰              2 - GREEN 绿
    3 - CYAN 青              4 - RED 红
    5 - MAGENTA 洋红         6 - BROWN 棕
    7 - LIGHTGRAY 淡灰       8 - DARKGRAY 深灰 淡色8 - 15
    9 - LIGHTBLUE 淡兰       10 - LIGHTGREEN 淡绿
    11 - LIGHTCYAN 淡青      12 - LIGHTRED 淡红
    13 - LIGHTMAGENTA 淡洋红 14 - YELLOW 黄
    15 - WHITE 白
    */
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, ForeColor | BackGroundColor);
}


//从本地文件读取，初始化 地图数组
void initUIMapFromFile(char* pFilePath)
{
    //用.  表示当前默认文件路径 
    //用.. 表示当前文件路径 上一级文件路径
    FILE* pFile = fopen(pFilePath, "r");  //文件的路径, 打开的文件模式 read  write

    if (pFile == nullptr)
    {
        printf("文件读取失败！");
        return;
    }

    //读取信息
    for (int i = 0; i < RowOfUIMap; ++i)
    {
        fscanf(pFile, "%d %d %d %d %d %d %d %d %d %d %d %d\n",
            &UIMap[i][0], &UIMap[i][1], &UIMap[i][2],
            &UIMap[i][3], &UIMap[i][4], &UIMap[i][5],
            &UIMap[i][6], &UIMap[i][7], &UIMap[i][8],
            (*(UIMap + i) + 9), (*(UIMap + i) + 10), (*(UIMap + i) + 11));
    }

    //关闭文件
    fclose(pFile);
}

//游戏UI界面的打印函数
void printUIMap()
{
    gotoXY(0, 0);
    for (int i = 0; i < RowOfUIMap; ++i)
    {
        for (int j = 0; j < ColOfUIMap; ++j)
        {
            switch (UIMap[i][j])
            {
            case UI_BLANK:  {cout << "  "; } break;
            case UI_FIXED_BLOCK: {setColor(2, 0);  cout << "■"; setColor(15, 0);  }  break;
            case UI_BLOCK:       {setColor(4, 0);  cout << "■";  setColor(15, 0); }  break;
            case UI_BORDER: {cout << "□"; }  break;
            default:break;
            }

            //换行
            if (j == ColOfUIMap - 1)
            {
                cout << endl;
            }
        }
    }

}

//产生方块
void generateBlock(CBlock& block)
{
    //重置block的信息
    memset(block.shape, 0, 16 * sizeof(char));

    block.position = { 0 };  //所有方块初始位置为0
    block.size = 3;          //大部分方块初始状态，使用3x3就能表示，设置默认size=3 

    //随机产生一个方块类型
    int blockType = (rand()*rand() + rand()) % ShapeType; //0~6

    switch (blockType)
    {
    case 0:
    {  /*
       0 0 0 0
       2 2 2 0
       0 2 0 0
       0 0 0 0
       */
        block.name = 'T';
        block.shape[1][0] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        block.shape[1][2] = UI_BLOCK; block.shape[2][1] = UI_BLOCK;
        break;
    }
    case 1:
    {  /*
       0 0 0 0
       2 2 2 0
       0 0 2 0
       0 0 0 0
       */
        block.name = 'L';
        block.shape[1][0] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        block.shape[1][2] = UI_BLOCK; block.shape[2][2] = UI_BLOCK;
        break;
    }
    case 2:
    {   /*
        0 0 2 0
        2 2 2 0
        0 0 0 0
        0 0 0 0
        */
        block.name = 'J';
        block.shape[1][0] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        block.shape[1][2] = UI_BLOCK; block.shape[0][2] = UI_BLOCK;
        break;
    }
    case 3:
    {  /*
       2 0 0 0
       2 2 0 0
       0 2 0 0
       0 0 0 0
       */
        block.name = 'N';
        block.shape[0][0] = UI_BLOCK; block.shape[1][0] = UI_BLOCK;
        block.shape[1][1] = UI_BLOCK; block.shape[2][1] = UI_BLOCK;
        break;
    }
    case 4:
    {  /*
       0 2 0 0
       2 2 0 0
       2 0 0 0
       0 0 0 0
       */
        block.name = 'Z';
        block.shape[1][0] = UI_BLOCK; block.shape[2][0] = UI_BLOCK;
        block.shape[0][1] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        break;
    }
    case 5:
    {  /*
       2 2 0 0
       2 2 0 0
       0 0 0 0
       0 0 0 0
       */
        block.name = 'O';
        block.size = 2;  //更新
        block.shape[0][0] = UI_BLOCK; block.shape[0][1] = UI_BLOCK;
        block.shape[1][0] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        break;
    }
    case 6:
    {  /*
       0 0 0 0
       2 2 2 2
       0 0 0 0
       0 0 0 0
       */
        block.name = 'I';
        block.size = 4;   //更新
        block.shape[1][0] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        block.shape[1][2] = UI_BLOCK; block.shape[1][3] = UI_BLOCK;
        break;
    }

    default:
        break;
    }
}

//绘制方块到游戏界面的逻辑数组
void drawBlock(const CBlock& block)
{
    for (int i = 0; i < block.size; ++i) //size 确认有效区域
    {
        for (int j = 0; j < block.size; ++j)
        {
            //边界判断，绘制方块的位置不能是地图边界
            if (UIMap[block.position.x + i][block.position.y + j] != UI_BORDER)
            {
                if (block.shape[i][j] == UI_BLOCK) //节约时间
                {
                    UIMap[block.position.x + i][block.position.y + j] = block.shape[i][j];
                }
            }
        }
    }


}

//擦除界面上的方块
void eraseBlock(const CBlock& block)
{
    for (int i = 0; i < block.size; ++i) //size 确认有效区域
    {
        for (int j = 0; j < block.size; ++j)
        {
            if (block.shape[i][j] == UI_BLOCK) //节约时间
            {
                UIMap[block.position.x + i][block.position.y + j] = UI_BLANK;
            }
        }
    }

}

//移动俄罗斯方块
void moveBlock(CBlock& block, int dx ,int dy)
{
    //判断是否能移动
    bool bCanMove = canMove(block, dx, dy);

    //能移动
    if (bCanMove)
    {
        eraseBlock(block);

        block.position.x += dx;
        block.position.y += dy;

        drawBlock(block);
    }

}

//判断是否能够移动
bool canMove(const CBlock& block, int dx, int dy) //isWin
{
    //假设能够移动,得到方块即将移至的位置
    int xTmpPosInUIMapOfBlock = block.position.x + dx;       //重要1
    int yTmpPosInUIMapOfBlock = block.position.y + dy;

    // 能否移动，不应受到方块形状数组空白行或列的影响
    for (int i = 0; i < block.size; i++)  //size 确认有效区域
    {
        for (int j = 0; j < block.size; j++)
        {
            // 能否移动，只需 受到方块形状数组 中 有效子块 的影响
            if (block.shape[i][j] == UI_BLOCK)  //
            {
                //每一个子块 将要绘制 地图逻辑数组 上  的坐标
                int xTmpPosOfShapeInUIMap = xTmpPosInUIMapOfBlock + i;    //重要2
                int yTmpPosOfShapeInUIMap = yTmpPosInUIMapOfBlock + j;

                //1、判断这个子块 所在的坐标 是否出列边界
                if ((yTmpPosOfShapeInUIMap < 1) || (yTmpPosOfShapeInUIMap > ColOfUIMap - 2))
                {
                    return false;
                }

                //2、判断这个子块 所在的坐标  是否出行边界
                if ((xTmpPosOfShapeInUIMap < 1) || (xTmpPosOfShapeInUIMap > RowOfUIMap - 2))
                {
                    return false;
                }

                //3、判断这个子块 所在的坐标 是否固定块
                if (UIMap[xTmpPosOfShapeInUIMap][yTmpPosOfShapeInUIMap] == UI_FIXED_BLOCK)
                {
                    return false;
                }
            }

        }
    }

    return true;
}

//将下一个方块 传递给当前方块，并且更新下一个方块
void transferNextBlockToCurBlockAndUpdateIt()
{
    //将下一个传递给当前方块
    g_curBlock = g_nextBlock;

    //设置当前方块的初始位置
    g_curBlock.position.x = 1;
    g_curBlock.position.y = ColOfUIMap / 2 - 2;

    //更新下一个方框
    generateBlock(g_nextBlock);
}

//当前方块不能移动后，记录为固定方块，更新UIMap的值 由BLOCK 变为FIXED_BLOCK
void updateCurrentBlockStatusOfUIMap()
{
    for (int i = 0; i < g_curBlock.size; ++i) //size 确认有效区域
    {
        for (int j = 0; j < g_curBlock.size; ++j)
        {
            if (g_curBlock.shape[i][j] == UI_BLOCK)
            {
                UIMap[g_curBlock.position.x + i][g_curBlock.position.y + j] = UI_FIXED_BLOCK;
            }
        }
    }

}

//打印Next界面
void  printNextUIMap()

{    //打印当前的得分
    gotoXY(2, (ColOfUIMap * 2 + 4));
    cout << "Score: " << g_score;

    //每个字符是两个位宽,goto时是 2 * ColOfUIMap
    gotoXY(4, (ColOfUIMap * 2 + 4));
    cout << "Next:";

    //根据g_nextBlock打印下一个方块的图案
    //A、清空打印区域
    for (int i = 0; i < 4; ++i) //注意下 4 
    {
        for (int j = 0; j < 4; ++j)
        {
            //gotoXY(5, (ColOfUIMap * 2 + 6);
            //注意: 清空下一行时, 光标的x坐标要+1; 清空每一列时,光标的y坐标要+2
            gotoXY(5 + i, (2 * ColOfUIMap + 6) + j * 2 );

            cout << "  " << endl;
        }
    }

    //B、再将 g_nextBlock 绘制到 NextUIMap区域
    for (int i = 0; i < 4; ++i) //注意下 4 
    {
        for (int j = 0; j < 4; ++j)
        {
            //gotoXY(5, (ColOfUIMap * 2 + 6);
            gotoXY(5 + i, (2 * ColOfUIMap + 6) + j * 2);

            if (g_nextBlock.shape[i][j] == UI_BLOCK)
            {
                cout << "■" << endl;
            }
            else
            {
                cout << "  " << endl;
            }
        }
    }

}

//消行并记分
void checkUIMapAndRemoveFullLine()
{
    //从下到上，遍历每一行
    for (int i = RowOfUIMap - 1; i > 0; --i)
    {
        //判断当前行是否是满行
        //先假定这一行是满行
        bool isFullLine = true;
        for (int j = 1; j < ColOfUIMap - 1;++j)
        {
            if (UIMap[i][j] != UI_FIXED_BLOCK)
            {
                isFullLine = false;
                break;
            }
        }

        //根据判断结果，做出消行的操作
        if (isFullLine)
        {
            //依次将 此行以上的每一行  下落
            for (int m = i; m > 0;m--) //从第i行到第1行
            {
                for (int n = 1; n < ColOfUIMap - 1;n++)
                {
                    UIMap[m][n] = UIMap[m - 1][n];
                }
            }

            //注意最后的一行是重行，需要清为空
            for (int n = 1; n < ColOfUIMap - 1;n++)
            {
                UIMap[1][n] = UI_BLANK;
            }

            //注意:  避免掉落的行，被跳过 
            i++;

            //统计分数
            g_score += 10;
        }
        //else
        //{
        //    //printf("此行不是满行\n");
        //}
    }
}

//方块的旋转
void rotateBlock(CBlock& block)
{
    //判断方块能否选择
    bool bCanRotate = canRotate(block);

    //如果能够旋转
    if (bCanRotate)
    {
        //1、擦除原有方块的图形
        eraseBlock(block);

        //2、更新方块的图形为转换后的
        rotateCells(block.shape, block.size);

        //3、绘制旋转后的方块
        drawBlock(block);
    }
}

//逆时针旋转形状数组的函数
void rotateCells(char shape[][4], int size)
{
    char tmpShape[4][4] = { 0 };

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            tmpShape[size - 1 - j][i] = shape[i][j];
        }
    }
    memcpy(shape, tmpShape, 4 * 4 *sizeof(char));  //注意一定要是4*4
}

//方块能否旋转
bool canRotate(const CBlock& block)
{
    //以当前方块的形状数组1为基础，得到旋转后的形状数组2
    //A、定义了一个临时形状数组
    char shapeAfterRotate[4][4] = { 0 };

    //B、将方块的形状数组，拷贝到临时数组，得到旋转后的形状数组
    memcpy(shapeAfterRotate, block.shape, 4 * 4 * sizeof(char));
    rotateCells(shapeAfterRotate, block.size);

    //假设能够旋转 
    //拿个这个新的形状数组 以及 当前方块的位置，去判断UIMap对应的位置上 有没有边界 以及 固定块 

    for (int i = 0; i < block.size; ++i)  //size 确认有效区域
    {
        for (int j = 0; j < block.size; ++j)
        {
            // 能否移动，只需 受到方块形状数组 中 有效子块 的影响
            if (shapeAfterRotate[i][j] == UI_BLOCK)  //
            {
                //每一个子块 将要绘制 地图逻辑数组 上  的坐标
                int xTmpPosOfShapeInUIMap = block.position.x + i;    //重要2
                int yTmpPosOfShapeInUIMap = block.position.y + j;

                //1、判断这个子块 所在的坐标 是否出列边界
                if ((yTmpPosOfShapeInUIMap < 1) || (yTmpPosOfShapeInUIMap > ColOfUIMap - 2))
                {
                    return false;
                }

                //2、判断这个子块 所在的坐标  是否出行边界
                if ((xTmpPosOfShapeInUIMap < 1) || (xTmpPosOfShapeInUIMap > RowOfUIMap - 2))
                {
                    return false;
                }

                //3、判断这个子块 所在的坐标 是否固定块
                if (UIMap[xTmpPosOfShapeInUIMap][yTmpPosOfShapeInUIMap] == UI_FIXED_BLOCK)
                {
                    return false;
                }
            }

        }
    }

    return true;
}

//判断游戏是否结束
bool isEnd()
{
    for (int i = 0; i < ColOfUIMap - 1;i++)
    {
        if (UIMap[1][i] == UI_FIXED_BLOCK)
        {
            return true;
        }
    }

    return false;
}