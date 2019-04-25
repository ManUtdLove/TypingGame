/*************************************
    ��Ϸ����:   ������Ϸ
    ���̣߳���Ĺ����лῨ��
    �����Ѿ���һ����������Ϸ�ˣ��ʺϳ�ѧ������
*************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#define LETTER_COUNT 30 //�ܹ����ٸ���������
#define SPEED 500       //����������ٶ�
#define BULLET_SPEED 30 //�ӵ�������ٶ�

/***************��ʼ��***************/
//�����ĸ���ӵ�
typedef struct tag_letter
{
    //λ��
    int x, y;
    //��ĸ
    char ch;
    //�Ƿ���ʾ 0-����ʾ��1-��ʾ
    int isDisplay;
    //���� 0-������1-����
    int life;
}Letter;

Letter letters[LETTER_COUNT];
Letter bullet;
//��Ϸ��ĸ��ʼ��
void initLetters()
{
    srand((unsigned)time(NULL));
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        letters[i].ch = rand() % 26 + 'A';
        letters[i].x = rand() % 80;
        letters[i].y = - i * 2;
        letters[i].life = 1;
    }
}
//�ӵ���ʼ��
void initBullet(int x)
{
    bullet.ch = 12;
    bullet.x = x;
    bullet.y = 25;
    bullet.life = 1;
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

//�����ж�������ĸ�Ƿ�Ӧ����ʾ
void judgeLetterDisplay()
{
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        if(letters[i].life == 0)
        {
            letters[i].isDisplay = 0;
        }
        else
        {
            if(letters[i].y < 0 || letters[i].y > 24)
            {
                letters[i].isDisplay = 0;
            }
            else
            {
                letters[i].isDisplay = 1;
            }
        }
    }
}

//letterMoving:��������ĸ�½�
void letterMoving()
{
    judgeLetterDisplay();
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        if(letters[i].y < 25)
        {
            if(letters[i].isDisplay == 1)
            {
                drawLetter(' ', letters[i].x, letters[i].y);
                drawLetter(letters[i].ch, letters[i].x, letters[i].y + 1);
            }
            else
            {
                drawLetter(' ', letters[i].x, letters[i].y);
            }
            letters[i].y++;
        }
        else
        {
            drawLetter(' ', letters[i].x, letters[i].y);
        }
    }
}
//�ӵ�����
void bulletMoving()
{
    if(bullet.life == 1)
    {
        drawLetter(' ', bullet.x, bullet.y);
        drawLetter(bullet.ch, bullet.x, bullet.y - 1);
    }
    bullet.y--;
}
//�����
int calcResult()
{
    int counter = 0;
    for(int i = 0; i < LETTER_COUNT; i++)
    {
        if(letters[i].life == 0)
        {
            counter++;
        }
    }
    return counter;
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

//��ʾ��Ϣ
void menu()
{
    printf("һ������Ĵ�����Ϸ\n\n�����뷨����Ӣ��״̬�Ϳ��Կ�ʼ����\n\n");
    printf("%d����ĸ������Ϯ������\n\n", LETTER_COUNT);
    system("pause");
    system("cls");
}


int main()
{
    hideCursor();
    menu();
    initLetters();
    while(letters[LETTER_COUNT - 1].y < 25)
    {
        bullet.life = 1;
        if(_kbhit())//��ȡ���̰���
        {
            char ch = _getch();
            ch = toupper(ch);
            for(int k = 0; k < LETTER_COUNT; k++)//����ÿ���ӵ���������û�ж�Ӧ��������ĸ
            {
                if(letters[k].isDisplay == 1 && letters[k].ch == ch)
                {
                    initBullet(letters[k].x);
                    while(bullet.y > 0)
                    {
                        bulletMoving();
                        if(bullet.y == letters[k].y)
                        {
                            letters[k].life = 0;
                            bullet.life = 0;
                            drawLetter(' ', bullet.x, bullet.y);
                        }
                        Sleep(BULLET_SPEED);
                    }
                    break;
                }
            }
        }
        Sleep(SPEED);
        letterMoving();
    }
    /*******************��Ϸ����******************/
    gotoxy(30, 12);
    printf("�ܹ���%d����ĸ��������%d��",LETTER_COUNT, calcResult());
    getchar();
    return 0;
}
