#include "game.h"

//����̨����ƶ�����
void gotoXY(int x, int y)
{
    //ע��x��y��˳��
    COORD pos = { y, x };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//����̨���ع����ʾ
void hideCursorDisplay()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = false;
    SetConsoleCursorInfo(hOut, &cci);
}

//�����ı���ɫ
void setColor(unsigned short ForeColor, unsigned short BackGroundColor)
{
    /*
    0 - BLACK ��             ��ɫ 0 - 7
    1 - BLUE ��              2 - GREEN ��
    3 - CYAN ��              4 - RED ��
    5 - MAGENTA ���         6 - BROWN ��
    7 - LIGHTGRAY ����       8 - DARKGRAY ��� ��ɫ8 - 15
    9 - LIGHTBLUE ����       10 - LIGHTGREEN ����
    11 - LIGHTCYAN ����      12 - LIGHTRED ����
    13 - LIGHTMAGENTA ����� 14 - YELLOW ��
    15 - WHITE ��
    */
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, ForeColor | BackGroundColor);
}


//�ӱ����ļ���ȡ����ʼ�� ��ͼ����
void initUIMapFromFile(char* pFilePath)
{
    //��.  ��ʾ��ǰĬ���ļ�·�� 
    //��.. ��ʾ��ǰ�ļ�·�� ��һ���ļ�·��
    FILE* pFile = fopen(pFilePath, "r");  //�ļ���·��, �򿪵��ļ�ģʽ read  write

    if (pFile == nullptr)
    {
        printf("�ļ���ȡʧ�ܣ�");
        return;
    }

    //��ȡ��Ϣ
    for (int i = 0; i < RowOfUIMap; ++i)
    {
        fscanf(pFile, "%d %d %d %d %d %d %d %d %d %d %d %d\n",
            &UIMap[i][0], &UIMap[i][1], &UIMap[i][2],
            &UIMap[i][3], &UIMap[i][4], &UIMap[i][5],
            &UIMap[i][6], &UIMap[i][7], &UIMap[i][8],
            (*(UIMap + i) + 9), (*(UIMap + i) + 10), (*(UIMap + i) + 11));
    }

    //�ر��ļ�
    fclose(pFile);
}

//��ϷUI����Ĵ�ӡ����
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
            case UI_FIXED_BLOCK: {setColor(2, 0);  cout << "��"; setColor(15, 0);  }  break;
            case UI_BLOCK:       {setColor(4, 0);  cout << "��";  setColor(15, 0); }  break;
            case UI_BORDER: {cout << "��"; }  break;
            default:break;
            }

            //����
            if (j == ColOfUIMap - 1)
            {
                cout << endl;
            }
        }
    }

}

//��������
void generateBlock(CBlock& block)
{
    //����block����Ϣ
    memset(block.shape, 0, 16 * sizeof(char));

    block.position = { 0 };  //���з����ʼλ��Ϊ0
    block.size = 3;          //�󲿷ַ����ʼ״̬��ʹ��3x3���ܱ�ʾ������Ĭ��size=3 

    //�������һ����������
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
        block.size = 2;  //����
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
        block.size = 4;   //����
        block.shape[1][0] = UI_BLOCK; block.shape[1][1] = UI_BLOCK;
        block.shape[1][2] = UI_BLOCK; block.shape[1][3] = UI_BLOCK;
        break;
    }

    default:
        break;
    }
}

//���Ʒ��鵽��Ϸ������߼�����
void drawBlock(const CBlock& block)
{
    for (int i = 0; i < block.size; ++i) //size ȷ����Ч����
    {
        for (int j = 0; j < block.size; ++j)
        {
            //�߽��жϣ����Ʒ����λ�ò����ǵ�ͼ�߽�
            if (UIMap[block.position.x + i][block.position.y + j] != UI_BORDER)
            {
                if (block.shape[i][j] == UI_BLOCK) //��Լʱ��
                {
                    UIMap[block.position.x + i][block.position.y + j] = block.shape[i][j];
                }
            }
        }
    }


}

//���������ϵķ���
void eraseBlock(const CBlock& block)
{
    for (int i = 0; i < block.size; ++i) //size ȷ����Ч����
    {
        for (int j = 0; j < block.size; ++j)
        {
            if (block.shape[i][j] == UI_BLOCK) //��Լʱ��
            {
                UIMap[block.position.x + i][block.position.y + j] = UI_BLANK;
            }
        }
    }

}

//�ƶ�����˹����
void moveBlock(CBlock& block, int dx ,int dy)
{
    //�ж��Ƿ����ƶ�
    bool bCanMove = canMove(block, dx, dy);

    //���ƶ�
    if (bCanMove)
    {
        eraseBlock(block);

        block.position.x += dx;
        block.position.y += dy;

        drawBlock(block);
    }

}

//�ж��Ƿ��ܹ��ƶ�
bool canMove(const CBlock& block, int dx, int dy) //isWin
{
    //�����ܹ��ƶ�,�õ����鼴��������λ��
    int xTmpPosInUIMapOfBlock = block.position.x + dx;       //��Ҫ1
    int yTmpPosInUIMapOfBlock = block.position.y + dy;

    // �ܷ��ƶ�����Ӧ�ܵ�������״����հ��л��е�Ӱ��
    for (int i = 0; i < block.size; i++)  //size ȷ����Ч����
    {
        for (int j = 0; j < block.size; j++)
        {
            // �ܷ��ƶ���ֻ�� �ܵ�������״���� �� ��Ч�ӿ� ��Ӱ��
            if (block.shape[i][j] == UI_BLOCK)  //
            {
                //ÿһ���ӿ� ��Ҫ���� ��ͼ�߼����� ��  ������
                int xTmpPosOfShapeInUIMap = xTmpPosInUIMapOfBlock + i;    //��Ҫ2
                int yTmpPosOfShapeInUIMap = yTmpPosInUIMapOfBlock + j;

                //1���ж�����ӿ� ���ڵ����� �Ƿ���б߽�
                if ((yTmpPosOfShapeInUIMap < 1) || (yTmpPosOfShapeInUIMap > ColOfUIMap - 2))
                {
                    return false;
                }

                //2���ж�����ӿ� ���ڵ�����  �Ƿ���б߽�
                if ((xTmpPosOfShapeInUIMap < 1) || (xTmpPosOfShapeInUIMap > RowOfUIMap - 2))
                {
                    return false;
                }

                //3���ж�����ӿ� ���ڵ����� �Ƿ�̶���
                if (UIMap[xTmpPosOfShapeInUIMap][yTmpPosOfShapeInUIMap] == UI_FIXED_BLOCK)
                {
                    return false;
                }
            }

        }
    }

    return true;
}

//����һ������ ���ݸ���ǰ���飬���Ҹ�����һ������
void transferNextBlockToCurBlockAndUpdateIt()
{
    //����һ�����ݸ���ǰ����
    g_curBlock = g_nextBlock;

    //���õ�ǰ����ĳ�ʼλ��
    g_curBlock.position.x = 1;
    g_curBlock.position.y = ColOfUIMap / 2 - 2;

    //������һ������
    generateBlock(g_nextBlock);
}

//��ǰ���鲻���ƶ��󣬼�¼Ϊ�̶����飬����UIMap��ֵ ��BLOCK ��ΪFIXED_BLOCK
void updateCurrentBlockStatusOfUIMap()
{
    for (int i = 0; i < g_curBlock.size; ++i) //size ȷ����Ч����
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

//��ӡNext����
void  printNextUIMap()

{    //��ӡ��ǰ�ĵ÷�
    gotoXY(2, (ColOfUIMap * 2 + 4));
    cout << "Score: " << g_score;

    //ÿ���ַ�������λ��,gotoʱ�� 2 * ColOfUIMap
    gotoXY(4, (ColOfUIMap * 2 + 4));
    cout << "Next:";

    //����g_nextBlock��ӡ��һ�������ͼ��
    //A����մ�ӡ����
    for (int i = 0; i < 4; ++i) //ע���� 4 
    {
        for (int j = 0; j < 4; ++j)
        {
            //gotoXY(5, (ColOfUIMap * 2 + 6);
            //ע��: �����һ��ʱ, ����x����Ҫ+1; ���ÿһ��ʱ,����y����Ҫ+2
            gotoXY(5 + i, (2 * ColOfUIMap + 6) + j * 2 );

            cout << "  " << endl;
        }
    }

    //B���ٽ� g_nextBlock ���Ƶ� NextUIMap����
    for (int i = 0; i < 4; ++i) //ע���� 4 
    {
        for (int j = 0; j < 4; ++j)
        {
            //gotoXY(5, (ColOfUIMap * 2 + 6);
            gotoXY(5 + i, (2 * ColOfUIMap + 6) + j * 2);

            if (g_nextBlock.shape[i][j] == UI_BLOCK)
            {
                cout << "��" << endl;
            }
            else
            {
                cout << "  " << endl;
            }
        }
    }

}

//���в��Ƿ�
void checkUIMapAndRemoveFullLine()
{
    //���µ��ϣ�����ÿһ��
    for (int i = RowOfUIMap - 1; i > 0; --i)
    {
        //�жϵ�ǰ���Ƿ�������
        //�ȼٶ���һ��������
        bool isFullLine = true;
        for (int j = 1; j < ColOfUIMap - 1;++j)
        {
            if (UIMap[i][j] != UI_FIXED_BLOCK)
            {
                isFullLine = false;
                break;
            }
        }

        //�����жϽ�����������еĲ���
        if (isFullLine)
        {
            //���ν� �������ϵ�ÿһ��  ����
            for (int m = i; m > 0;m--) //�ӵ�i�е���1��
            {
                for (int n = 1; n < ColOfUIMap - 1;n++)
                {
                    UIMap[m][n] = UIMap[m - 1][n];
                }
            }

            //ע������һ�������У���Ҫ��Ϊ��
            for (int n = 1; n < ColOfUIMap - 1;n++)
            {
                UIMap[1][n] = UI_BLANK;
            }

            //ע��:  ���������У������� 
            i++;

            //ͳ�Ʒ���
            g_score += 10;
        }
        //else
        //{
        //    //printf("���в�������\n");
        //}
    }
}

//�������ת
void rotateBlock(CBlock& block)
{
    //�жϷ����ܷ�ѡ��
    bool bCanRotate = canRotate(block);

    //����ܹ���ת
    if (bCanRotate)
    {
        //1������ԭ�з����ͼ��
        eraseBlock(block);

        //2�����·����ͼ��Ϊת�����
        rotateCells(block.shape, block.size);

        //3��������ת��ķ���
        drawBlock(block);
    }
}

//��ʱ����ת��״����ĺ���
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
    memcpy(shape, tmpShape, 4 * 4 *sizeof(char));  //ע��һ��Ҫ��4*4
}

//�����ܷ���ת
bool canRotate(const CBlock& block)
{
    //�Ե�ǰ�������״����1Ϊ�������õ���ת�����״����2
    //A��������һ����ʱ��״����
    char shapeAfterRotate[4][4] = { 0 };

    //B�����������״���飬��������ʱ���飬�õ���ת�����״����
    memcpy(shapeAfterRotate, block.shape, 4 * 4 * sizeof(char));
    rotateCells(shapeAfterRotate, block.size);

    //�����ܹ���ת 
    //�ø�����µ���״���� �Լ� ��ǰ�����λ�ã�ȥ�ж�UIMap��Ӧ��λ���� ��û�б߽� �Լ� �̶��� 

    for (int i = 0; i < block.size; ++i)  //size ȷ����Ч����
    {
        for (int j = 0; j < block.size; ++j)
        {
            // �ܷ��ƶ���ֻ�� �ܵ�������״���� �� ��Ч�ӿ� ��Ӱ��
            if (shapeAfterRotate[i][j] == UI_BLOCK)  //
            {
                //ÿһ���ӿ� ��Ҫ���� ��ͼ�߼����� ��  ������
                int xTmpPosOfShapeInUIMap = block.position.x + i;    //��Ҫ2
                int yTmpPosOfShapeInUIMap = block.position.y + j;

                //1���ж�����ӿ� ���ڵ����� �Ƿ���б߽�
                if ((yTmpPosOfShapeInUIMap < 1) || (yTmpPosOfShapeInUIMap > ColOfUIMap - 2))
                {
                    return false;
                }

                //2���ж�����ӿ� ���ڵ�����  �Ƿ���б߽�
                if ((xTmpPosOfShapeInUIMap < 1) || (xTmpPosOfShapeInUIMap > RowOfUIMap - 2))
                {
                    return false;
                }

                //3���ж�����ӿ� ���ڵ����� �Ƿ�̶���
                if (UIMap[xTmpPosOfShapeInUIMap][yTmpPosOfShapeInUIMap] == UI_FIXED_BLOCK)
                {
                    return false;
                }
            }

        }
    }

    return true;
}

//�ж���Ϸ�Ƿ����
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