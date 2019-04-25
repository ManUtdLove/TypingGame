#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

int letterCount = 100;  //������ĸ��������
int beginFlag = 1;  //��Ϸ��ʱ������0-��ʾ����
int downCount;  //������ĸ����ͳ��
int dropLetter; //�����ĸ����ͳ��
int errorCount; //��������ͳ��
int speed = 500;    //�����ٶ�
int bulletSpeed = 30;   //�ӵ��ٶ�
CRITICAL_SECTION cs;    //�߳���

typedef struct tag_letter
{
    int x, y;
    char ch;
    int life;
}Letter;
Letter *letters;

int *vis;
int *visBullet;
int *bulletflag;

void initLetters()
{
    downCount = 0;
    dropLetter = 0;
    errorCount = 0;
    vis = (int *)malloc(sizeof(int) * letterCount);
    visBullet = (int *)malloc(sizeof(int) * letterCount);
    bulletflag = (int *)malloc(sizeof(int) * letterCount);
    memset(vis, 0, sizeof(int) * letterCount);
    memset(visBullet, 0, sizeof(int) * letterCount);
    letters = (struct tag_letter *)malloc(sizeof(struct tag_letter) * letterCount);
    srand((unsigned)time(NULL));
    for(int i = 0; i < letterCount; i++)
    {
        bulletflag[i] = i;
        letters[i].ch = rand() % 26 + 'A';
        letters[i].x = rand() % 80;
        letters[i].y = - i * 2;
        letters[i].life = 1;
    }
}



/***************���ܺ���***************/
//��λ����Ļ��ĳ��λ��
void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(handle, coord);
}

//������ĸ
void drawLetter(char ch, int x, int y)
{
    gotoxy(x, y);
    printf("%c", ch);
}

void setTitle(void *p)
{
    while(1)
    {
        char str[30] = "";
        sprintf(str, "title ����ĸ:%d ������ĸ:%d ����:%d", letterCount, downCount, dropLetter);
        system(str);
    }
}
DWORD WINAPI runBullet(void *p)
{
    int letterId = *(int *) p;
    if(!visBullet[letterId])
    {
        visBullet[letterId] = 1;
    }
    int y = 25;
    int x = letters[letterId].x;
    while(y > letters[letterId].y)
    {
        EnterCriticalSection(&cs);
        drawLetter(' ', x, y);
        drawLetter(12, x, y - 1);
        LeaveCriticalSection(&cs);
        Sleep(bulletSpeed);
        y--;
    }
    EnterCriticalSection(&cs);
    drawLetter(' ', x, y);
    letters[letterId].life = 0;
    drawLetter(' ', letters[letterId].x, letters[letterId].y);
    LeaveCriticalSection(&cs);
}
//letterMoving:��������ĸ�½�
void letterMoving()
{
    int cnt = 0;
    for(int i = 0; i < letterCount; i++)
    {
        if(letters[i].life == 1)
        {
            cnt++;
        }
        if(letters[i].y < 25 && letters[i].y >= 0 && letters[i].life == 1)
        {
            if(!vis[i])
            {
                downCount++;
                vis[i] = 1;
            }
            EnterCriticalSection(&cs);
            drawLetter(' ', letters[i].x, letters[i].y);
            drawLetter(letters[i].ch, letters[i].x, letters[i].y + 1);
            LeaveCriticalSection(&cs);
        }
        else if(letters[i].y > 25)
        {
             letters[i].life = 0;
        }
        else if(letters[i].y == 25)
        {
            EnterCriticalSection(&cs);
            drawLetter(' ', letters[i].x, letters[i].y);
            LeaveCriticalSection(&cs);
        }

        letters[i].y++;
    }
    if(cnt == 0)
    {
        beginFlag = 0;
    }
}

DWORD WINAPI runLetter(void *p)
{
    while(letters[letterCount - 1].y < 25)
    {
        letterMoving();
        Sleep(speed);
    }
    beginFlag = 0;
}

//���ؿ���̨���
void hideCursor()
{
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO CursorInfo;
        GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
        CursorInfo.bVisible = 0; //���ؿ���̨���
        SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
}

void gameBegin()
{
    printf("-------------------------------------\n");
    printf("    һ���򵥵Ĵ�����Ϸ����ESC�˳�\n");
    printf("       ������ǰ�л���Ӣ�����뷨\n");
    printf("-------------------------------------\n");
    printf("��������Ҫ���ֵ�����:");
    while(scanf("%d", &letterCount) != 1)
    {
        system("cls");
        scanf("%*s");
        printf("�������ֵ�����⣬���������֣�\n");
        printf("��������Ҫ���ֵ�����:");
    }
    int type;
    int flag = 1;
    while(flag)
    {
        printf("1-��\n2-�е�\n3-����\n4-ħ��\n");
        printf("��ѡ����Ҫ���ֵĵȼ�:");
        while(scanf("%d", &type) != 1)
        {
            system("cls");
            scanf("%*s");
            printf("�������ֵ�����⣬���������֣�\n");
            printf("1-��\n2-�е�\n3-����\n4-ħ��\n");
            printf("��ѡ����Ҫ���ֵĵȼ�:");
        }
        switch(type)
        {
        case 1:
            speed = 500;
            flag = 0;
            break;
        case 2:
            speed = 350;
            flag = 0;
            break;
        case 3:
            speed = 250;
            flag = 0;
            break;
        case 4:
            speed = 100;
            flag = 0;
            break;
        default:
            printf("��ѡ���ֵ����������ѡ��\n");
        }
    }

    printf("��ĸ��׼������,");
    system("pause");
    system("cls");
}

void gameOver()
{
    gotoxy(30, 12);
    printf("����ĸ:%d�� ������:%d�� ���󰴼�:%d��", letterCount, dropLetter, errorCount);
}

int main()
{
    gameBegin();
    hideCursor();
    InitializeCriticalSection(&cs);
    initLetters();
    CreateThread(NULL, 0, runLetter, NULL, 0, NULL);
    _beginthread(setTitle, 0, NULL);
    while(beginFlag)
    {
        if(_kbhit())
        {
            char ch = _getch();
            ch = toupper(ch);
            if(ch == 27)
            {
                break;
            }
            int flag = 1;
            for(int i = 0; i < letterCount; i++)
            {
                if(letters[i].y < 25 && letters[i].y >= 0 && letters[i].life == 1 && letters[i].ch == ch && visBullet[i] == 0)
                {
                    flag = 0;
                    dropLetter++;
                    CreateThread(NULL, 0, runBullet, &bulletflag[i], 0, NULL);
                    break;
                }
            }
            if(flag) errorCount++;
            //printf("%c", ch);
        }
    }
    gameOver();
    InitializeCriticalSection(&cs);
    system("\n\n\npause");
    return 0;
}
