#include "utility.h"
#include "game.h"

/*
�׶���Ŀ_Tetris����˼��
һ��������Ϸ��ͼ -- ��ͼ��Ϣö�� // UIMap���� // printUIMap���� //��������1
�������ַ���  ----- ���󷽿� CBlock
���������ƶ�һ�� -- canMove��eraseBlock��drawBlock
*/

/*
�׶���Ŀ_�����л���Next���� --->    ��Ҫ��� ---> ��ϸ��� ---> �����д
----------------------------------�����л�------------------------------------
1���½���ʾ ��ǰ�� �� ��һ���� ��CBlock�Ľṹ�����
2����ǰ���� �� ��һ������ ֮��Ĺ�ϵ
    A���Ȳ��� ��һ������1
    B��Ȼ�� ��һ������1 ���� ��ǰ���飬���� ��һ������2

    C����ǰ���飺 1���ܵ�������Ӧ  2���Զ�����

    D���� ��ǰ���� �����ƶ���Ҳ�����̶���������
       //����Ϸ�еķ���תΪ�̶�����;
       �� ��һ������2 ��ֵ����ǰ����;
       ���� ��һ������3��

    E���ظ� C��D
----------------------------------��ӡNext����------------------------------------
ע�⣺λ���2
*/

/*
�׶���Ŀ_���мǷֺͷ�����ת
--------------------------���кͼǷ�---------------------------------------------
A������ -- checkUIMapAndRemoveFullLine
    A1�����е�ʱ����ѡ�� -- ��ǰ���鲻���ƶ�
    A2�����еķ���
    //���µ��� ���α���ÿһ�У��жϵ�ǰ���Ƿ�������
      (�ȼٶ������У�Ȼ���ڱ������У��ж� ÿһ���Ƿ��ǹ̶���)

    //��ǰ��������, ���ν� �������ϵ�ÿһ��  ����

    //ע�⣺ ���������У������� 

B���Ƿ֣����������¼������һ�У���10��

--------------------------������ת---------------------------------------------
A��ȷ����ת���ĺ���ת����
B�����ڷ������״shape������˵��ȷ����ת�ķ��� --- ת�ã��½�ͬ�ȴ�С�����飩
C����ת��ʱ��  --- ��"W"��ת

D����ת��ʵʩ
    D1���ж��Ƿ��ܹ���ת -- canRotate
        D11���ȼ����ܹ���ת���õ���ת�����״���� -- rotateCells
        D12���ø������״���� �Լ� ��ǰ�����λ�ã�ȥ�ж�UIMap��Ӧ��û�б߽� �Լ� �̶��� -- rotateBlock
        D13�����ؽ��

    D2����ǰ���� �ܹ� ��ת -- rotateBlock
        D21������ UIMap�ϵ� ��ǰ����
        D22��ʹ��rotateCells���� ��ǰ�������״���� Ϊ��ת���״̬
        D23�����»��� UIMap�ϵĵ�ǰ����
*/

//������Ϸ�����Ӧ���߼�����
int UIMap[RowOfUIMap][ColOfUIMap] = {0};

//��ǰ�������һ������ 
CBlock g_curBlock = {0}; //current
CBlock g_nextBlock = {0};

//ͳ�Ƶ�ǰ�ķ���
int g_score = 0;

int main()
{
    //
    srand(time(0));
    hideCursorDisplay();

    //-----��Ϸ-------
    initUIMapFromFile(".//Resource//UIMap.txt");

    //��������һ������
    generateBlock(g_nextBlock);

    //����һ������ ���ݸ���ǰ���飬���Ҹ�����һ������
    transferNextBlockToCurBlockAndUpdateIt();

    //���߼��ϵ� ��ǰ���� ���Ƶ�UIMap 
    drawBlock(g_curBlock);
    printUIMap();
    printNextUIMap();

    //��Ϸ��ѭ��
    while (true)
    {
        //1����ӡ��Ϸ����
        printUIMap();

        //�ж���Ϸ�Ƿ����
        if (isEnd())
        {
            break;
        }
  
        //2.1���������û�����룬��ǰ����Ӧ��ÿ��һ��ʱ������ -- 500ms
        //GetTickCount���أ�retrieve���Ӳ���ϵͳ������������elapsed���ĺ����������ķ���ֵ��DWORD��
        DWORD static dwPreTime = GetTickCount();  //previoius ��һ��ʱ��
        DWORD dwCurTime = GetTickCount();         //��ǰʱ��

        if (dwCurTime - dwPreTime >= 500)  //500����
        {
            //��ǰ�������µ���һ��
            if (canMove(g_curBlock, 1, 0))
            {
                moveBlock(g_curBlock, 1, 0);  //����
            }
            else
            {
                //�� ��ǰ���� �����ƶ�ʱ
                //A����UIMap�е�ǰ����� ��Ϣ������Ϊ�̶�����
                updateCurrentBlockStatusOfUIMap();
                
                //B������Ƿ������в�����
                checkUIMapAndRemoveFullLine();

                //C������һ������ ���ݸ���ǰ���飬���Ҹ�����һ������
                transferNextBlockToCurBlockAndUpdateIt();

                //D��������һ������Ľ���
                printNextUIMap();
            }

            dwPreTime = dwCurTime;
        }


        //2.2�����ݼ����Ƿ������룬���ƶ� "��ǰ����"
        while (_kbhit())
        {
            char keyCode = _getch();
            switch (keyCode)
            {
            case 'a':
            case 'A':
            {
                //����
                moveBlock(g_curBlock, 0, -1);
                break;
            }
            case 'd':
            case 'D':
            {
                //����
                moveBlock(g_curBlock, 0, 1);
                break;
            }
            case 's':
            case 'S':
            {
                //����
                moveBlock(g_curBlock, 1, 0);
                break;
            }
            case 'w':
            case 'W':
            {
                //����
                rotateBlock(g_curBlock);
                break;
            }
            case ' ':
            {
                //��������
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
    cout << "����Ϸ������" << endl;

    return 0;
}

