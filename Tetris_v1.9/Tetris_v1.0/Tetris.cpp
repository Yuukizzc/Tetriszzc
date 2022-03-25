#include "utility.h"
#include "game.h"

/*
阶段项目_Tetris基本思想
一、构建游戏地图 -- 地图信息枚举 // UIMap数组 // printUIMap函数 //不闪方案1
二、出现方块  ----- 抽象方块 CBlock
三、方块移动一步 -- canMove、eraseBlock、drawBlock
*/

/*
阶段项目_方块切换和Next界面 --->    概要设计 ---> 详细设计 ---> 代码编写
----------------------------------方块切换------------------------------------
1、新建表示 当前块 和 下一个块 的CBlock的结构体变量
2、当前方块 和 下一个方块 之间的关系
    A、先产生 下一个方块1
    B、然后将 下一个方块1 赋给 当前方块，更新 下一个方块2

    C、当前方块： 1、受到操作响应  2、自动下落

    D、当 当前方块 不能移动（也即：固定下来）：
       //将游戏中的方块转为固定方块;
       把 下一个方块2 赋值给当前方块;
       更新 下一个方块3。

    E、重复 C～D
----------------------------------打印Next界面------------------------------------
注意：位宽×2
*/

/*
阶段项目_消行记分和方块旋转
--------------------------消行和记分---------------------------------------------
A、消行 -- checkUIMapAndRemoveFullLine
    A1、消行的时机的选择 -- 当前方块不能移动
    A2、消行的方案
    //从下到上 依次遍历每一行，判断当前行是否是满行
      (先假定是满行，然后在遍历此行，判断 每一列是否是固定块)

    //当前行是满行, 依次将 此行以上的每一行  下落

    //注意： 避免掉落的行，被跳过 

B、记分：定义变量记录，消除一行，加10分

--------------------------方块旋转---------------------------------------------
A、确定旋转中心和旋转方向
B、对于方块的形状shape数组来说，确认旋转的方案 --- 转置（新建同等大小的数组）
C、旋转的时机  --- 按"W"旋转

D、旋转的实施
    D1、判断是否能够旋转 -- canRotate
        D11、先假设能够旋转，得到旋转后的形状数组 -- rotateCells
        D12、拿个这个形状数组 以及 当前方块的位置，去判断UIMap对应有没有边界 以及 固定块 -- rotateBlock
        D13、返回结果

    D2、当前方块 能够 旋转 -- rotateBlock
        D21、擦除 UIMap上的 当前方块
        D22、使用rotateCells更新 当前方块的形状数组 为旋转后的状态
        D23、重新绘制 UIMap上的当前方块
*/

//定义游戏界面对应的逻辑数组
int UIMap[RowOfUIMap][ColOfUIMap] = {0};

//当前方块和下一个方块 
CBlock g_curBlock = {0}; //current
CBlock g_nextBlock = {0};

//统计当前的分数
int g_score = 0;

int main()
{
    //
    srand(time(0));
    hideCursorDisplay();

    //-----游戏-------
    initUIMapFromFile(".//Resource//UIMap.txt");

    //先生成下一个方块
    generateBlock(g_nextBlock);

    //将下一个方块 传递给当前方块，并且更新下一个方块
    transferNextBlockToCurBlockAndUpdateIt();

    //将逻辑上的 当前方块 绘制到UIMap 
    drawBlock(g_curBlock);
    printUIMap();
    printNextUIMap();

    //游戏主循环
    while (true)
    {
        //1、打印游戏画面
        printUIMap();

        //判断游戏是否结束
        if (isEnd())
        {
            break;
        }
  
        //2.1、如果键盘没有输入，当前方块应该每隔一段时间下落 -- 500ms
        //GetTickCount返回（retrieve）从操作系统启动所经过（elapsed）的毫秒数，它的返回值是DWORD。
        DWORD static dwPreTime = GetTickCount();  //previoius 上一次时间
        DWORD dwCurTime = GetTickCount();         //当前时间

        if (dwCurTime - dwPreTime >= 500)  //500毫秒
        {
            //当前方块往下掉落一次
            if (canMove(g_curBlock, 1, 0))
            {
                moveBlock(g_curBlock, 1, 0);  //下移
            }
            else
            {
                //当 当前方块 不能移动时
                //A、将UIMap中当前方块的 信息，更新为固定方块
                updateCurrentBlockStatusOfUIMap();
                
                //B、检查是否有满行并消除
                checkUIMapAndRemoveFullLine();

                //C、将下一个方块 传递给当前方块，并且更新下一个方块
                transferNextBlockToCurBlockAndUpdateIt();

                //D、更新下一个方块的界面
                printNextUIMap();
            }

            dwPreTime = dwCurTime;
        }


        //2.2、根据键盘是否有输入，来移动 "当前方块"
        while (_kbhit())
        {
            char keyCode = _getch();
            switch (keyCode)
            {
            case 'a':
            case 'A':
            {
                //左移
                moveBlock(g_curBlock, 0, -1);
                break;
            }
            case 'd':
            case 'D':
            {
                //右移
                moveBlock(g_curBlock, 0, 1);
                break;
            }
            case 's':
            case 'S':
            {
                //下移
                moveBlock(g_curBlock, 1, 0);
                break;
            }
            case 'w':
            case 'W':
            {
                //变形
                rotateBlock(g_curBlock);
                break;
            }
            case ' ':
            {
                //快速下落
                while (canMove(g_curBlock, 1, 0))
                {
                    moveBlock(g_curBlock, 1, 0);
                }
                break;
            }
            default:
                break;
            }

        }
    }

    gotoXY(7, 8);
    cout << "【游戏结束】" << endl;

    return 0;
}

