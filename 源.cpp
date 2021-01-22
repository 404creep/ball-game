#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include"EasyXPng.h"
#pragma once
#include <windows.h>
#pragma comment(lib,"Winmm.lib")

#define WIDTH 450 // ������
#define HEIGHT 700 // ����߶�
#define Radius 7 // С��뾶
#define friction 0.9// ����ϵ��

class MyTimer
{
private:
	LARGE_INTEGER m_clk;//����ʱ����Ϣ
	LONGLONG m_oldclk;//���濪ʼʱ�Ӻͽ���ʱ��
	int m_freq;//ʱ��Ƶ��

public:
	MyTimer();
	void Sleep(int ms);
	void Reset();
};

MyTimer::MyTimer()
{
	QueryPerformanceFrequency(&m_clk);
	m_freq = (int)m_clk.QuadPart / 1000;
	m_oldclk = 0;//��ü�������ʱ��Ƶ��
}

void MyTimer::Sleep(int ms)//��ʱ
{
	unsigned int c = ms * m_freq;
	if (m_oldclk == 0)
	{
		QueryPerformanceCounter(&m_clk);//��ʼ��ʱ
		m_oldclk = m_clk.QuadPart;//��ÿ�ʼʱ��
	}
	m_oldclk += c;
	QueryPerformanceCounter(&m_clk);
	if (m_clk.QuadPart > m_oldclk)
		m_oldclk = m_clk.QuadPart;
	else
		do
		{
			::Sleep(1);
			QueryPerformanceCounter(&m_clk);//�����ֹʱ��
		} while (m_clk.QuadPart < m_oldclk);
}

void MyTimer::Reset()//������ʱ��
{
	m_oldclk = 0;
}

struct Ball // ����С������Ľṹ��
{
	float Vx; // С��ˮƽ�����ٶ�
	float Vy; // С����ֱ�����ٶ�
	float x; // С��Բ��x����
	float y; // С��Բ��y����
	COLORREF  color; // С����ɫ
	float distance[2]; // ��¼�����������С��ľ��룬�Լ����С����±�
	float gravity; // �������ٶ�
};

struct smallBall // ���ڱ�ըС������Ľṹ��
{
	int smallballradius; // С��뾶
	float smallballVx; // С��ˮƽ�����ٶ�
	float smallballVy; // С����ֱ�����ٶ�
	float smallballx; // С��Բ��x����
	float smallbally; // С��Բ��y����
	COLORREF  color; // С����ɫ
	float smallballgravity; // �������ٶ�
};

struct Ball BALL[100]; // ÿ��С�������
struct smallBall Smallball[300]; //��ը���С��
int i, j;
int Access[10] = { 0,1,0 };//�ؿ��Ƿ����
int isReloadLevel = 1; // �Ƿ���ض�Ӧ�ؿ�
int Level = 1; // �ؿ���
int Numsmallball; // ��ը��С����
int Totalball; // ÿ��һ������С��
int Launchedball; // �Ѿ������С��
int Leftball; // ��������ʣ���С��
int Loseball = 0; // ��ʧ��С��
int Ballinbox; // ���е�С�����
int Passball; // ÿ����Ҫ���ٸ�С�����ͨ��
int width_launcher[5] = { 0,60,82,80,60 }; // ���������
int height_launcher[5] = { 0,80,64,74,80 }; // �������߶�
int Width_launcher; // ���������
int Height_launcher; // �������߶�
int x_launcher; // ���������Ͻ�x����
int y_launcher; // ���������Ͻ�y����
float x_wall[10]; // �������Ͻ�x����
float y_wall[10]; // �������Ͻ�y����
float Vx_wall[10]; // ����x�����˶��ٶ�
float Vy_wall[10];// ����y�����˶��ٶ�
float x_barrier; // �ϰ���x����
float y_barrier; // �ϰ���y����
float Vx_barrier; // �ϰ���x�����ٶ�
float Vy_barrier; // �ϰ���y�����ٶ�
float Vx_launcher; // ������x�����ٶ�

int gameStatus = 0; // ��Ϸ״̬��0Ϊ��Ϸ�����棬1Ϊ�ؿ�ѡ��2Ϊ������Ϸ״̬��3Ϊ��Ϸ��ͣ��4Ϊ������Ϸ

IMAGE im_launcher1, im_launcher2, im_launcher3, im_launcher4;  // ���巢����ͼ��	
IMAGE im_wall1, im_wall2, im_wall3, im_wall4,im_wall5; // ���嵲��ͼ��
IMAGE im_box; // �������ͼ��
IMAGE im_barrier; // �����ϰ���ͼ��
IMAGE im_continue, im_home1, im_home2, im_return, im_star, im_icon, im_firework; // ���尴ťͼ��
IMAGE im_bk; // ���屳��ͼ��
MOUSEMSG m; // ��꺯��

void startup();// ��ʼ����������
void Startmenu();//��Ϸ�����溯��
void Maininterface();// �ؿ�ѡ����溯��
void Pausemenu();// �˵����溯��
void readRecordFile();//��������
void writeRecordFile();//�浵����
void PlayMusicOnce(const TCHAR fileName[80]);//�������ֺ���
void Levelupdate(int n);//�ؿ���ʼ������
void get_distance(struct Ball BALL[]);//С�����뺯��
int isfinished();//�ж���Ϸ�Ƿ��������
void knock(struct Ball BALL[]);//С����ײ����
void show();//���ƺ���
void move();//С�򼰵�����˶�����
void updateWithoutInput();// �������޹صĸ��º���
void updateWithInput();// �������йصĸ��º���



void startup() // ��ʼ������
{
	initgraph(WIDTH, HEIGHT); // �¿�һ������
	setbkcolor(RGB(255, 255, 255)); // ����Ϊ��ɫ
	cleardevice(); //�Ա�����ɫ��ջ���
	mciSendString(_T("open backmusic.mp3 alias bkmusic"), NULL, 0, NULL); // �򿪱�������
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL); // ѭ������
	loadimage(&im_continue, _T("������ť.png")); // ���������ť
	loadimage(&im_home1, _T("��ҳ��ť1.png")); // ������ҳ��ť1
	loadimage(&im_home2, _T("��ҳ��ť2.png")); // ������ҳ��ť2
	loadimage(&im_return, _T("���¿�ʼ��ť.png")); // �������¿�ʼ��ť
	loadimage(&im_star, _T("����.png")); // ��������
	loadimage(&im_icon, _T("ͼ��.png")); // ����ͼ��
	loadimage(&im_box, _T("��.png")); // �����
	loadimage(&im_wall1, _T("����1.png")); // ���뵲��һ
	loadimage(&im_wall2, _T("����2.png")); // ���뵲���
	loadimage(&im_wall3, _T("����3.png")); // ���뵲����
	loadimage(&im_wall4, _T("����4.png")); // ���뵲����
	loadimage(&im_wall5, _T("����5.png")); // ���뵲����
	loadimage(&im_launcher1, _T("launcher1.png")); // ���뷢����һ
	loadimage(&im_launcher2, _T("launcher2.png")); // ���뷢������
	loadimage(&im_launcher3, _T("launcher3.png")); // ���뷢������
	loadimage(&im_launcher4, _T("launcher4.png")); // ���뷢������
	loadimage(&im_barrier, _T("barrier.png")); // �����ϰ���
	loadimage(&im_bk, _T("����.png")); // ���뱳��
	loadimage(&im_firework, _T("�̻�.png")); // ���뱳��
	BeginBatchDraw(); // ��ʼ��������
}

void Startmenu()//��Ϸ������
{
	putimage(0, 0, &im_bk);	// ��ʾ����
	setbkmode(TRANSPARENT); // ͸����ʾ
	settextcolor(HSVtoRGB(110, 0.8, 0.6)); // ������ɫ
	settextstyle(70, 0, _T("���Ĳ���")); // ��������
	outtextxy(WIDTH * 0.2, HEIGHT * 0.15, _T("С�����")); // ��ʾ����
	settextcolor(HSVtoRGB(330, 0.8, 0.6)); // ������ɫ
	settextstyle(30, 0, _T("����")); // ��������
	outtextxy(WIDTH * 0.3, HEIGHT * 0.28, _T("* ����Ϸ")); // ��ʾ����
	outtextxy(WIDTH * 0.3, HEIGHT * 0.35, _T("* ��ȡ��Ϸ�浵")); // ��ʾ����
	outtextxy(WIDTH * 0.3, HEIGHT * 0.42, _T("* ���浱ǰ����")); // ��ʾ����
	outtextxy(WIDTH * 0.3, HEIGHT * 0.49, _T("* �˳�")); // ��ʾ����

	settextcolor(BLUE); // ������ɫ
	settextstyle(20, 0, _T("����")); // ��������
	outtextxy(WIDTH * 0.2, HEIGHT * 0.6, _T("����������С��")); // ��ʾ����
	outtextxy(WIDTH * 0.2, HEIGHT * 0.65, _T("ESC����ͣ��Ϸ")); // ��ʾ����
	outtextxy(WIDTH * 0.2, HEIGHT * 0.7, _T("����С��ﵽҪ��ʱ��Ϸʤ��")); // ��ʾ����
	outtextxy(WIDTH * 0.2, HEIGHT * 0.75, _T("����С��ʧ�Ի�ø�������")); // ��ʾ����
	FlushBatchDraw(); // ������������
	Sleep(2); // �ȴ�2����

	if (MouseHit()) // ����������Ϣ
	{
		m = GetMouseMsg();  // ��������Ϣ
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.28 && m.y <= HEIGHT * 0.33 && m.x >= WIDTH * 0.3) // ����յ���ʼ����Ϸָ��
		{
			Access[1] = 1;
			for (i = 2; i <= 8; i++)
				Access[i] = 0;//�ؿ���Ϣ��ʼ��
			gameStatus = 1; // ����ؿ�ѡ�����
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.35 && m.y <= HEIGHT * 0.40 && m.x >= WIDTH * 0.3) // ����յ���ȡ�浵ָ��
		{
			readRecordFile(); // ��ȡ�浵
			gameStatus = 1; // ����ؿ�ѡ�����
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.42 && m.y <= HEIGHT * 0.47 && m.x >= WIDTH * 0.3) // ����յ�д��浵ָ��
			writeRecordFile(); // д��浵
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.49 && m.y <= HEIGHT * 0.54 && m.x >= WIDTH * 0.3) // ����յ��˳�ָ��
		{
			gameStatus = 2; // ������Ϸ״̬
			exit(0); // �˳�
		}
	}
}

void Maininterface() // �ؿ�ѡ����溯��
{
	cleardevice();//����
	for (i = 1; i <= 8; i++)//��ʾ��Ӧ�ؿ���Ϣ���Ƿ������
	{
		if (Access[i] == 1) // ����ùؿ��ɽ���
		{
			setlinecolor(RGB(0, 255, 0)); // ����������ɫ
			setfillcolor(RGB(0, 255, 0)); // ���÷�����ɫ
			fillroundrect(250 - 150 * (i % 2), 100 + 150 * ((i - 1) / 2), 250 - 150 * (i % 2) + 100, 100 + 150 * ((i - 1) / 2) + 100, 20, 20); // ���Ʒ���
		}
		if (Access[i] == 0) // ����ùؿ����ɽ���
		{
			setlinecolor(RGB(175, 175, 175)); // ����������ɫ
			setfillcolor(RGB(175, 175, 175)); // ���÷�����ɫ
			fillroundrect(250 - 150 * (i % 2), 100 + 150 * ((i - 1) / 2), 250 - 150 * (i % 2) + 100, 100 + 150 * ((i - 1) / 2) + 100, 20, 20); // ���Ʒ���
		}
	}
	putimage(370, 20, &im_home2);	// ��ʾ��Ϸ�����水ť
	setbkmode(TRANSPARENT); // ��������͸��    
	settextcolor(RGB(255, 0, 0));// �趨������ɫ
	settextstyle(28, 0, _T("����")); //  �趨���ִ�С����ʽ
	outtextxy(100, 130, _T("Level 1")); //  �����������
	outtextxy(250, 130, _T("Level 2")); //  �����������
	outtextxy(100, 280, _T("Level 3")); //  �����������
	outtextxy(250, 280, _T("Level 4")); //  �����������
	outtextxy(100, 430, _T("Level 5")); //  �����������
	outtextxy(250, 430, _T("Level 6")); //  �����������
	outtextxy(100, 580, _T("Level 7")); //  �����������
	outtextxy(250, 580, _T("Level 8")); //  �����������
	FlushBatchDraw();//��������
	if (MouseHit())   // ����������Ϣ
	{
		m = GetMouseMsg();  // ��������Ϣ
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 100 && m.y <= 200 && m.x >= 100 && m.x <= 200 && Access[1])
		{
			Level = 1;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 100 && m.y <= 200 && m.x >= 250 && m.x <= 350 && Access[2])
		{
			Level = 2;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 250 && m.y <= 350 && m.x >= 100 && m.x <= 200 && Access[3])
		{
			Level = 3;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 250 && m.y <= 350 && m.x >= 250 && m.x <= 350 && Access[4])
		{
			Level = 4;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 400 && m.y <= 500 && m.x >= 100 && m.x <= 200 && Access[5])
		{
			Level = 5;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 400 && m.y <= 500 && m.x >= 250 && m.x <= 350 && Access[6])
		{
			Level = 6;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 650 && m.x >= 100 && m.x <= 200 && Access[7])
		{
			Level = 7;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 650 && m.x >= 250 && m.x <= 350 && Access[8])
		{
			Level = 8;
			Levelupdate(Level); // �ؿ���Ϣ����
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 20 && m.y <= 63 && m.x >= 370 && m.x <= 420) // ����������Ϸ�����水ť
			gameStatus = 0; // ������Ϸ������
	}
}

void Pausemenu() // ��Ϸ��ͣ��˵�����
{
	putimage(0, 0, &im_bk);	// ��ʾ����
	setbkmode(TRANSPARENT); // ͸����ʾ
	settextcolor(HSVtoRGB(110, 0.8, 0.6)); // ������ɫ
	settextstyle(70, 0, _T("���Ĳ���")); // ��������
	outtextxy(WIDTH * 0.2, HEIGHT * 0.15, _T("С�����")); // ��ʾ����
	settextcolor(HSVtoRGB(330, 0.8, 0.6)); // ������ɫ
	settextstyle(30, 0, _T("����")); // ��������
	outtextxy(WIDTH * 0.3, HEIGHT * 0.28, _T("* ������Ϸ"));  // ��ʾ����
	outtextxy(WIDTH * 0.3, HEIGHT * 0.35, _T("* ���¿�ʼ")); // ��ʾ����
	outtextxy(WIDTH * 0.3, HEIGHT * 0.42, _T("* ����������")); // ��ʾ����
	outtextxy(WIDTH * 0.3, HEIGHT * 0.49, _T("* �˳�")); // ��ʾ����

	settextcolor(BLUE); // ������ɫ
	settextstyle(20, 0, _T("����")); // ��������
	outtextxy(WIDTH * 0.2, HEIGHT * 0.6, _T("����������С��")); // ��ʾ����
	outtextxy(WIDTH * 0.2, HEIGHT * 0.65, _T("ESC����ͣ��Ϸ")); // ��ʾ����
	outtextxy(WIDTH * 0.2, HEIGHT * 0.7, _T("����С��ﵽҪ��ʱ��Ϸʤ��")); // ��ʾ����
	outtextxy(WIDTH * 0.2, HEIGHT * 0.75, _T("����С��ʧ�Ի�ø�������")); // ��ʾ����
	FlushBatchDraw(); // ������������
	Sleep(2); // �ȴ�������

	if (MouseHit()) // ����������Ϣ
	{
		m = GetMouseMsg();  // ��������Ϣ
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.28 && m.y <= HEIGHT * 0.33 && m.x >= WIDTH * 0.3) // ����յ�����ָ��
			gameStatus = 4; // ����������Ϸ
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.35 && m.y <= HEIGHT * 0.40 && m.x >= WIDTH * 0.3) // ����յ����¿�ʼָ��
		{
			Levelupdate(Level);
			gameStatus = 4; // ����������Ϸ
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.42 && m.y <= HEIGHT * 0.47 && m.x >= WIDTH * 0.3) // ����յ�����������ָ��
			gameStatus = 0; // �����ʼ�˵�����
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.49 && m.y <= HEIGHT * 0.54 && m.x >= WIDTH * 0.3) // ����յ��˳�ָ��
		{
			gameStatus = 2; // ������Ϸ״̬
			exit(0); // �˳�
		}
	}
}

void readRecordFile()  //��ȡ��Ϸ�����ļ��浵
{
	FILE* fp;
	fp = fopen(".\\gameRecord.dat", "r");//��ȡ�ؿ�������Ϣ
	fscanf(fp, "%d %d %d %d %d %d %d %d", &Access[1], &Access[2], &Access[3], &Access[4], &Access[5], &Access[6], &Access[7], &Access[8]);
	fclose(fp);
}

void writeRecordFile()  // �洢��Ϸ�����ļ��浵
{
	FILE* fp;
	fp = fopen(".\\gameRecord.dat", "w");//д��ؿ�������Ϣ
	fprintf(fp, "%d %d %d %d %d %d %d %d", Access[1], Access[2], Access[3], Access[4], Access[5], Access[6], Access[7], Access[8]);
	fclose(fp);
}

void PlayMusicOnce(const TCHAR fileName[80]) // ����һ�����ֺ���
{
	TCHAR cmdString1[50];
	_stprintf(cmdString1, _T("open %s alias tmpmusic"), fileName); // ���������ַ���
	mciSendString(_T("close tmpmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
	mciSendString(cmdString1, NULL, 0, NULL); // ������
	mciSendString(_T("play tmpmusic"), NULL, 0, NULL); // ������һ��
}

void Levelupdate(int n) // ��ؿ��йصĳ�ʼ��
{
	if (n == 1)
	{
		Width_launcher = width_launcher[1]; // ����������
		Height_launcher = height_launcher[1];
		x_launcher = WIDTH / 2 - Width_launcher / 2; // ���������Ͻ�x����
		y_launcher = HEIGHT / 4 - Height_launcher; // ���������Ͻ�y����
		x_wall[0] = WIDTH / 4; // �������Ͻ�x����
		y_wall[0] = HEIGHT / 2;  // �������Ͻ�y����
		Vx_wall[0] = 1.5; // �����ٶ�Ϊ1.5
		Totalball = 30; // һ��30��С��
		Leftball = 30; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 20; // 20�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // �������ٶ�
			BALL[i].Vy = 2; // ��ֱ�����ٶ�
			BALL[i].Vx = 0; // ˮƽ�����ٶ�
			BALL[i].x = WIDTH / 2; // С����ʱx����
			BALL[i].y = y_launcher + Height_launcher; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 2)
	{
		Width_launcher = width_launcher[1]; // ����������
		Height_launcher = height_launcher[1]; // ����������
		x_launcher = WIDTH / 2 - Width_launcher / 2; // ���������Ͻ�x����
		y_launcher = HEIGHT / 4 - Height_launcher; // ���������Ͻ�y����
		Totalball = 35; // һ��35��С��
		Leftball = 35; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 25; // 25�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		x_wall[0] = 225 - 60; // ����һ���Ͻ�x����
		y_wall[0] = HEIGHT / 3; // ����һ���Ͻ�y����
		x_wall[1] = 225 - 40; // ��������Ͻ�x����
		y_wall[1] = HEIGHT / 2; // ��������Ͻ�y����
		Vx_wall[0] = -1.5; // �����ٶ�Ϊ-1.5
		Vx_wall[1] = 1.5; // �����ٶ�Ϊ1.5
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // �������ٶ�
			BALL[i].Vy = 2; // ��ֱ�����ٶ�
			BALL[i].Vx = 0; // ˮƽ�����ٶ�
			BALL[i].x = WIDTH / 2; // С����ʱx����
			BALL[i].y = y_launcher + Height_launcher; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 3)
	{
		Width_launcher = width_launcher[1]; // ����������
		Height_launcher = height_launcher[1]; // ����������
		x_launcher = WIDTH / 2 - Width_launcher / 2; // ���������Ͻ�x����
		y_launcher = HEIGHT / 4 - Height_launcher; // ���������Ͻ�y����
		Totalball = 30; // һ��30��С��
		Leftball = 30; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 25; // 25�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		x_wall[0] = WIDTH / 2 - 50; // ����һ���Ͻ�x����
		y_wall[0] = HEIGHT / 2 - 75; // ����һ���Ͻ�y����
		Vy_wall[0] = 1; // ����һ�ٶ�Ϊ1
		x_wall[1] = 40; // ��������Ͻ�x����
		y_wall[1] = HEIGHT / 2; // ��������Ͻ�y����
		x_wall[2] = 282; // ���������Ͻ�x����
		y_wall[2] = HEIGHT / 2; // ���������Ͻ�y����
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // �������ٶ�
			BALL[i].Vy = 2; // ��ֱ�����ٶ�
			BALL[i].Vx = 0; // ˮƽ�����ٶ�
			BALL[i].x = WIDTH / 2; // С����ʱx����
			BALL[i].y = y_launcher + Height_launcher; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 4)
	{
		Width_launcher = width_launcher[2]; // ����������
		Height_launcher = height_launcher[2]; // ����������
		x_launcher = WIDTH / 2 - Width_launcher / 2; // ���������Ͻ�x����
		y_launcher = HEIGHT / 4 - Height_launcher; // ���������Ͻ�y����
		Totalball = 30; // һ��30��С��
		Leftball = 30; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 20; // 20�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		x_wall[0] = 100; // ����һ���Ͻ�x����
		y_wall[0] = HEIGHT / 2; // ����һ���Ͻ�y����
		Vx_wall[0] = 1.5; // ����һ�ٶ�Ϊ1.5
		x_wall[1] = 40; // ��������Ͻ�x����
		y_wall[1] = 50; // ��������Ͻ�y����
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.03; // �������ٶ�
			BALL[i].Vy = 0; // ��ֱ�����ٶ�
			BALL[i].Vx = -3; // ˮƽ�����ٶ�
			BALL[i].x = x_launcher; // С����ʱx����
			BALL[i].y = y_launcher + Height_launcher / 2; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 5)
	{
		Width_launcher = width_launcher[3]; // ����������
		Height_launcher = height_launcher[3]; // ����������
		x_launcher = WIDTH / 4; // ���������Ͻ�x����
		y_launcher = HEIGHT / 7 - 50; // ���������Ͻ�y����
		Totalball = 35; // һ��35��С��
		Leftball = 35; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 25; // 25�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		x_wall[0] = 250; // ����һ���Ͻ�x����
		y_wall[0] = 130; // ����һ���Ͻ�y����
		Vx_wall[1] = 1; // ����һ�ٶ�Ϊ1
		x_wall[1] = 150;  // ��������Ͻ�x����
		y_wall[1] = 150; // ��������Ͻ�y����
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.05; // �������ٶ�
			BALL[i].Vy = 3; // ��ֱ�����ٶ�
			BALL[i].Vx = 3; // ˮƽ�����ٶ�
			BALL[i].x = x_launcher + Width_launcher; // С����ʱx����
			BALL[i].y = y_launcher + Height_launcher; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 6)
	{
		Width_launcher = width_launcher[1]; // ����������
		Height_launcher = height_launcher[1]; // ����������
		x_launcher = WIDTH / 2 - Width_launcher / 2; // ���������Ͻ�x����
		y_launcher = HEIGHT / 4 - Height_launcher; // ���������Ͻ�y����
		x_barrier = 195; // �ϰ���x����
		y_barrier = 200; // �ϰ���y����
		Vx_barrier = 0; // �ϰ���x�����ٶ�Ϊ0
		Vy_barrier = 3; // �ϰ���y�����ٶ�Ϊ2
		Totalball = 30; // һ��30��С��
		Leftball = 30; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 20; // 20�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		Numsmallball = 0; // ��ը��С��
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // �������ٶ�
			BALL[i].Vy = 2; // ��ֱ�����ٶ�
			BALL[i].Vx = 0; // ˮƽ�����ٶ�
			BALL[i].x = WIDTH / 2; // С����ʱx����
			BALL[i].y = y_launcher + Height_launcher; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 7)
	{
		Width_launcher = width_launcher[4]; // ����������
		Height_launcher = height_launcher[4]; // ����������
		x_launcher = WIDTH / 9; // ���������Ͻ�x����
		y_launcher = HEIGHT / 4 - Height_launcher; // ���������Ͻ�y����
		x_barrier = 150; // �ϰ���x����
		y_barrier = 300; // �ϰ���y����
		Vx_barrier = 2; // �ϰ���x�����ٶ�Ϊ0
		Totalball = 40; // һ��40��С��
		Leftball = 40; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 24; // 24�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		Numsmallball = 0; // ��ը��С��
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // �������ٶ�
			BALL[i].Vy = -2; // ��ֱ�����ٶ�
			BALL[i].Vx = 1.5; // ˮƽ�����ٶ�
			BALL[i].x = WIDTH / 9 + 50; // С����ʱx����
			BALL[i].y = y_launcher; // С����ʱy����
			float h = rand() % 360; // С�������ɫ
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 8)
	{
		Width_launcher = width_launcher[1]; // ����������
		Height_launcher = height_launcher[1]; // ����������
		x_launcher = 100; // ���������Ͻ�x����
		y_launcher = HEIGHT / 7 - 50; // ���������Ͻ�y����
		Totalball = 36; // һ��36��С��
		Leftball = 36; // ʣ��С�����
		Ballinbox = 0; // ���е���
		Passball = 25; // 25�������
		Launchedball = 0; // ��ʼ0���Ѿ������С��
		Vx_launcher = 2; // �������ٶ�
	}
}

void get_distance(struct Ball BALL[]) // ��������С������֮��ľ���ƽ��
{
	for (i = 1; i <= Launchedball; i++) // ����С��
	{
		for (j = 1; j <= Launchedball; j++) // ����С��
		{

			if (i != j) // �Լ����Լ�����Ҫ��
			{
				int dist2;
				dist2 = (BALL[i].x - BALL[j].x) * (BALL[i].x - BALL[j].x)
					+ (BALL[i].y - BALL[j].y) * (BALL[i].y - BALL[j].y);//�������
				if (dist2 < BALL[i].distance[0]) // �������С�ڵ�ǰ��С����
				{
					BALL[i].distance[0] = dist2; // ��¼��С����
					BALL[i].distance[1] = j; // ��¼С���±�
				}
			}
		}
	}
}

int isfinished() // �ж�С���Ƿ�ȫ��������
{
	int count = 0; // ����
	if (Ballinbox == Passball) // ������е�С�������ڹ��ص�С����
		return 1;//��Ϸ����
	else
	{
		if (Launchedball < Totalball) // �Ѿ������С����С��С������
			return 0;//��Ϸ����
		else
		{
			for (i = 1; i <= Launchedball; i++)
			{
				// ����ڿ��л��������
				if ((BALL[i].x >= WIDTH / 2 - 92 && BALL[i].x <= WIDTH / 2 + 92 && BALL[i].y >= HEIGHT - 250 && BALL[i].Vy <= 2) || BALL[i].y - Radius >= HEIGHT)
					count++;
			}
			if (count == Totalball) // �����������С������
				return 1;//��Ϸ����
			else
				return 0;//��Ϸ����
		}
	}
}

void knock(struct Ball BALL[]) // �ж���֮�䡢��Ϳ��Ƿ���ײ
{
	for (i = 1; i <= Launchedball; i++) // С��֮�����ײ
	{
		if (BALL[i].distance[0] <= 4 * Radius * Radius) // ��С����С�������뾶������ײ
		{
			j = BALL[i].distance[1]; // jΪС���±�
			if (BALL[i].y - Radius >= HEIGHT); // �䵽��Ļ�������ײ���ᷢ��Ч��
			else if (BALL[i].x >= WIDTH / 2 - 92 && BALL[i].x <= WIDTH / 2 + 92 && BALL[i].y >= HEIGHT - 250) // �ڿ�����ײ
			{
				if (fabs(BALL[i].Vx) > 1 || fabs(BALL[j].Vx) > 1 || fabs(BALL[i].Vy) > 1 || fabs(BALL[j].Vy) > 1) // ���ĳһ��������ٶȵľ���ֵ����1
				{
					BALL[i].Vx = 0.5; // x�����ٶ�
					BALL[j].Vx = -0.5;
					if (BALL[i].x <= BALL[j].x) // ���С��i��С��j���
					{
						BALL[i].Vx = -BALL[i].Vx; // x�����ٶ��෴
						BALL[j].Vx = -BALL[j].Vx;
					}
					BALL[i].Vy = 0.5; //С��y�����ٶ�
					BALL[j].Vy = -0.5;
					if (BALL[i].y <= BALL[j].y) // ���С��i��С��j�·�
					{
						BALL[i].Vy = -BALL[i].Vy; // y�����ٶ��෴
						BALL[j].Vy = -BALL[j].Vy;
					}
				}
				else
				{
					while ((BALL[i].x - BALL[j].x) * (BALL[i].x - BALL[j].x) + (BALL[i].y - BALL[j].y) * (BALL[i].y - BALL[j].y) < 4 * Radius * Radius + 8)
					{
						if (BALL[i].y < BALL[j].y) // ���С��i��j�·�
						{
							BALL[i].y -= 0.1; // С��i�½�һ��
						}
						if (BALL[i].y > BALL[j].y)
						{
							BALL[j].y -= 0.1;
						}
						if (BALL[i].x < BALL[j].x)
						{
							BALL[i].x -= 0.05;
							BALL[j].x += 0.05;
						}
						if (BALL[i].x > BALL[j].x)
						{
							BALL[j].x -= 0.05;
							BALL[i].x += 0.05;
						}
					}
					BALL[i].Vx = BALL[i].Vx * friction; // С���ٶ���Ħ����Ӱ�����С
					BALL[j].Vx = BALL[j].Vx * friction;
					BALL[i].Vy = BALL[i].Vy * friction;
					BALL[j].Vy = BALL[j].Vy * friction;
				}
			}
			else // �ڿ�����ײ
			{
				BALL[i].Vx = 0.2 + rand() % 5 * 1.0 / 5; // С����ײʱx�����ٶ����
				BALL[j].Vx = -0.2 - rand() % 5 * 1.0 / 5;
				if (BALL[i].x <= BALL[j].x)
				{
					BALL[i].Vx = -BALL[i].Vx;
					BALL[j].Vx = -BALL[j].Vx;
				}
				BALL[i].Vy = 0.2 + rand() % 5 * 1.0 / 5; // С����ʱy�����ٶȸı�
				BALL[j].Vy = -0.2 - rand() % 5 * 1.0 / 5;
				if (BALL[i].y <= BALL[j].y)
				{
					BALL[i].Vy = -BALL[i].Vy;
					BALL[j].Vy = -BALL[j].Vy;
				}
			}
		}
		BALL[j].distance[0] = 99999999; // ���⽻�������ٶȣ�������С�����¸�ֵ
		BALL[j].distance[1] = -1;
		BALL[i].distance[0] = 99999999;
		BALL[i].distance[1] = -1;
	}
	//�ж��Ƿ������ײ
	for (i = 1; i <= Launchedball; i++) // ����С��
	{
		if (BALL[i].y + Radius >= HEIGHT - 250 && BALL[i].x <= WIDTH / 2 - 60 && BALL[i].x >= WIDTH / 2 - 92) // ����Ϳ�����ϲ���ײ
		{
			BALL[i].y = HEIGHT - 257;
			BALL[i].Vy = -2; // ����
		}
		if (BALL[i].y + Radius >= HEIGHT - 250 && BALL[i].x >= WIDTH / 2 + 60 && BALL[i].x <= WIDTH / 2 + 92) // ����Ϳ�����ϲ���ײ
		{
			BALL[i].y = HEIGHT - 257;
			BALL[i].Vy = -2; // ����
		}
		if (BALL[i].y - Radius >= HEIGHT - 250 && BALL[i].x - Radius >= WIDTH / 2 - 92 && BALL[i].x + Radius <= WIDTH / 2 + 92)
		{
			if (BALL[i].x - Radius <= WIDTH / 2 - 57) // ���������߽���ײ
			{
				BALL[i].x = WIDTH / 2 - 57 + Radius;
				BALL[i].Vx = -BALL[i].Vx * friction; // x�����ٶȸı�
			}
			if (BALL[i].x + Radius >= WIDTH / 2 + 57) // ��������ұ߽���ײ
			{
				BALL[i].x = WIDTH / 2 + 57 - Radius;
				BALL[i].Vx = -BALL[i].Vx * friction; // x�����ٶȸı�
			}

			if (BALL[i].y + Radius >= HEIGHT - 184) // ���С�����ĵײ���ײ
			{
				BALL[i].y = HEIGHT - 190;
				BALL[i].Vy = 0; // y�����ٶ�Ϊ0
			}
		}
	}
}

void show()
{
	while (gameStatus == 0)
		Startmenu(); // �������˵�����
	while (gameStatus == 1)
		Maininterface(); // ����ؿ�ѡ�����
	while (gameStatus == 3)
		Pausemenu(); // ��Ϸ��ͣ��˵����棬��ESC���������ý���
	if (isReloadLevel == 1) // ���¼��عؿ�
	{
		Levelupdate(Level); // ���¹ؿ�����
		isReloadLevel = 0;
	}
	//������Ϸ
	cleardevice(); // ����
	putimage(WIDTH / 2 - 92, HEIGHT - 250, &im_box); // ��ʾ��
	if (Level == 1) // ��һ��
	{
		putimage(x_wall[0], y_wall[0], &im_wall1); // ��ʾ����һ
		putimage(x_launcher, y_launcher, &im_launcher1); // ��ʾ������
	}

	if (Level == 2) // �ڶ���
	{
		putimage(x_wall[0], y_wall[0], &im_wall1); // ��ʾ����һ
		putimage(x_wall[1], y_wall[1], &im_wall1); // ��ʾ�����
		putimage(x_launcher, y_launcher, &im_launcher1); // ��ʾ������
	}
	if (Level == 3) // ������
	{
		putimage(x_wall[1], y_wall[1], &im_wall2); // ��ʾ�����
		putimage(x_wall[2], y_wall[2], &im_wall3); // ��ʾ������		
		putimage(x_wall[0], y_wall[0], &im_wall1); // ��ʾ����һ
		putimage(x_launcher, y_launcher, &im_launcher1); // ��ʾ������
	}
	if (Level == 4) // ���Ĺ�
	{
		putimage(x_wall[0], y_wall[0], &im_wall1); // ��ʾ����һ
		putimage(x_wall[1], y_wall[1], &im_wall4); // ��ʾ������
		putimage(x_launcher, y_launcher, &im_launcher2); // ��ʾ������
	}
	if (Level == 5) // �����
	{
		putimage(x_wall[0], y_wall[0], &im_wall5); // ��ʾ������
		putimage(x_wall[1], y_wall[1], &im_wall4); // ��ʾ������
		putimage(x_launcher, y_launcher, &im_launcher3); // ��ʾ������
	}
	if (Level == 6) // ������
	{
		putimage(x_launcher, y_launcher, &im_launcher1); // ��ʾ������
		putimage(x_barrier, y_barrier, &im_barrier); // ��ʾ�ϰ���
		for (i = 1; i <= Numsmallball; i++)
		{
			setfillcolor(Smallball[i].color); // С����ɫ
			setlinecolor(Smallball[i].color); // С����ɫ
			fillcircle(Smallball[i].smallballx, Smallball[i].smallbally, Smallball[i].smallballradius); // ����С��
		}
	}
	if (Level == 7) // ���߹�
	{
		putimage(x_launcher, y_launcher, &im_launcher4); // ��ʾ������
		putimage(x_barrier, y_barrier, &im_barrier); // ��ʾ�ϰ���
		for (i = 1; i <= Numsmallball; i++)
		{
			setfillcolor(Smallball[i].color); // С����ɫ
			setlinecolor(Smallball[i].color); // С����ɫ
			fillcircle(Smallball[i].smallballx, Smallball[i].smallbally, Smallball[i].smallballradius); // ����С��
		}
	}
	if (Level == 8) // �ڰ˹�
	{
		putimage(x_launcher, y_launcher, &im_launcher1); // ��ʾ������
	}
	// ��ʾ���ǵڼ���
	TCHAR s[20]; // �ַ���
	setbkmode(TRANSPARENT); // ����͸����ʾ
	_stprintf(s, _T("�� %d ��"), Level); // ���������ַ���
	settextcolor(RGB(0, 50, 200));  // ����������ɫ
	settextstyle(30, 0, _T("����")); // �������ִ�С������
	outtextxy(WIDTH * 0.38, 20, s); // �������

	//��ʾ�������е�С�����
	TCHAR t[20]; // �ַ���
	_stprintf(t, _T("%d / %d"), Leftball, Totalball); // ���������ַ���
	settextcolor(RGB(200, 50, 0));  // ����������ɫ
	settextstyle(30, 0, _T("����")); // �������ִ�С������
	outtextxy(340, 80, t); // �������

	//��ʾ���е�С�����
	TCHAR r[20]; // �ַ���
	_stprintf(r, _T("%d / %d"), Ballinbox, Passball); // ���������ַ���
	settextcolor(RGB(200, 50, 0));  // ����������ɫ
	settextstyle(30, 0, _T("����")); // �������ִ�С������
	outtextxy(WIDTH / 2 - 40, HEIGHT - 150, r); // �������

	for (i = 1; i <= Launchedball; i++)
	{
		setfillcolor(BALL[i].color); // С����ɫ
		setlinecolor(BALL[i].color); // С����ɫ
		fillcircle(BALL[i].x, BALL[i].y, Radius); // ����С��
	}
	if (isfinished())
	{
		if (Ballinbox == Passball && Level < 8) // ������е�С����
		{
			cleardevice(); // ����
			setbkmode(TRANSPARENT); // ��������͸��    
			settextcolor(RGB(0, 0, 0)); // �趨������ɫ
			settextstyle(70, 0, _T("MV Boli")); // �趨���ִ�С����ʽ
			outtextxy(150, 50, _T("LEVEL")); // �����������
			settextcolor(BLUE); // �趨������ɫ
			settextstyle(110, 0, _T("Arial Black")); // �趨���ִ�С����ʽ
			outtextxy(60, 100, _T("CLEAR!")); // �����������
			putimage(WIDTH / 2 - 120, 450, &im_continue); // ��ʾ������ť
			putimage(20, 550, &im_home1); // ��ʾ���عؿ�ѡ��ť
			putimage(WIDTH / 2 + 130, 550, &im_return); // ��ʾ���¿�ʼ��ť
			TCHAR x[100]; // �ַ���
			_stprintf(x, _T("l o s e   b a l l s : %d"), Loseball); // ���������ַ���
			settextcolor(HSVtoRGB(280, 0.4, 0.8)); // �趨������ɫ
			settextstyle(50, 0, _T("Comic Sans MS")); // �趨���ִ�С����ʽ
			outtextxy(70, 370, x); // �����������
			Access[Level + 1] = 1; // �ùؿ����Խ���
			FlushBatchDraw(); // ������������
			if (Loseball <= 10)
			{
				Sleep(500);
				putimage(80, 280, &im_star); // ��ʾ����
				FlushBatchDraw(); // ������������
			}
			if (Loseball <= 5)
			{
				Sleep(500);
				putimage(180, 210, &im_star); // ��ʾ����
				FlushBatchDraw(); // ������������
			}
			if (Loseball <= 2)
			{
				Sleep(500);
				putimage(280, 280, &im_star); // ��ʾ����
				FlushBatchDraw(); // ������������
			}
			while (isReloadLevel == 0)
			{
				if (MouseHit())   // ����������Ϣ
				{
					m = GetMouseMsg();  // ��������Ϣ
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 450 && m.y <= 450 + 74 && m.x >= WIDTH / 2 - 120 && m.x <= WIDTH / 2 + 120) // ������������������Ұ����˼�����ť
					{
						Level++; // �ؿ���һ
						isReloadLevel = 1;
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 61 && m.x >= WIDTH / 2 + 130 && m.x <= WIDTH / 2 + 200) // ������������������Ұ��������¿�ʼ��ť
					{
						isReloadLevel = 1; // ���¿�ʼ
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 62 && m.x >= 20 && m.x <= 20 + 70) // ������������������Ұ����˷��عؿ�ѡ��ť
					{
						isReloadLevel = 1;
						gameStatus = 1; // ����ؿ�ѡ�����
					}
				}
			}
		}
		if (Level == 8 && isReloadLevel == 0 && Ballinbox == Passball) // ����ڰ˹�ͨ��
		{
			cleardevice(); // ����
			putimage(WIDTH / 2, 30, &im_firework); // ��ʾ�̻�
			setbkmode(TRANSPARENT); // ��������͸��    
			settextcolor(RED); // �趨������ɫ
			settextstyle(110, 0, _T("������κ")); // �趨���ִ�С����ʽ
			outtextxy(10, 150, _T("��ϲͨ��")); // �����������
			putimage(20, 550, &im_home1); // ��ʾ���عؿ�ѡ��ť
			putimage(WIDTH / 2 + 130, 550, &im_return); // ��ʾ���¿�ʼ��ť
			TCHAR x[100]; // �ַ���
			_stprintf(x, _T("l o s e   b a l l s : %d"), Loseball); // ���������ַ���
			settextcolor(HSVtoRGB(280, 0.4, 0.8)); // �趨������ɫ
			settextstyle(50, 0, _T("Comic Sans MS")); // �趨���ִ�С����ʽ
			outtextxy(70, 470, x); // �����������
			FlushBatchDraw(); // ������������
			if (Loseball <= 10)
			{
				Sleep(500);
				putimage(80, 350, &im_star); // ��ʾ����
				FlushBatchDraw(); // ������������
			}
			if (Loseball <= 5)
			{
				Sleep(500);
				putimage(180, 280, &im_star); // ��ʾ����
				FlushBatchDraw(); // ������������
			}
			if (Loseball <= 2)
			{
				Sleep(500);
				putimage(280, 350, &im_star); // ��ʾ����
				FlushBatchDraw(); // ������������
			}
			while (isReloadLevel == 0)
			{
				if (MouseHit())   // ����������Ϣ
				{
					m = GetMouseMsg();  // ��������Ϣ
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 61 && m.x >= WIDTH / 2 + 130 && m.x <= WIDTH / 2 + 200) // ������������������Ұ��������¿�ʼ��ť
					{
						isReloadLevel = 1; // ���¿�ʼ
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 62 && m.x >= 20 && m.x <= 20 + 70) // ������������������Ұ����˷��عؿ�ѡ��ť
					{
						isReloadLevel = 1;
						gameStatus = 1; // ����ؿ�ѡ�����
					}
				}
			}
		}
		if (Ballinbox < Passball) // ��Ϸʧ��
		{
			cleardevice(); // ����
			setbkmode(TRANSPARENT); // ��������͸��    
			settextcolor(RGB(0, 0, 0)); // �趨������ɫ
			settextstyle(70, 0, _T("MV Boli")); // �趨���ִ�С����ʽ
			outtextxy(130, 100, _T("GAME")); // �����������
			settextcolor(RED); // �趨������ɫ
			settextstyle(110, 0, _T("Arial Black")); // �趨���ִ�С����ʽ
			outtextxy(40, 150, _T("FAILED...")); // �����������
			putimage(WIDTH / 2 + 130, 550, &im_return); // ��ʾ���¿�ʼ��ť
			putimage(20, 550, &im_home1); // ��ʾ������ҳ��ť
			putimage(WIDTH / 2 - 100, 280, &im_icon); // ��ʾͼ�갴ť
			FlushBatchDraw(); // ������������
			while (isReloadLevel == 0)
			{
				if (MouseHit())   // ����������Ϣ
				{
					m = GetMouseMsg();  // ��������Ϣ
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 61 && m.x >= WIDTH / 2 + 130 && m.x <= WIDTH / 2 + 200) // ������������������Ұ��������¿�ʼ��ť
					{
						isReloadLevel = 1; // ���¿�ʼ
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 62 && m.x >= 20 && m.x <= 20 + 70) // ������������������Ұ����˹ؿ�ѡ��ť
					{
						isReloadLevel = 1;
						gameStatus = 1; // ����ؿ�ѡ�����
					}
				}
			}
		}
	}
	FlushBatchDraw(); // ������������
}

void move() // С�򼰵�����˶�
{
	for (i = 1; i <= Launchedball; i++) // С���˶�
	{
		BALL[i].Vy = BALL[i].Vy + BALL[i].gravity; // С��y�����ٶȱ仯
		if (BALL[i].Vy > 8) // С��y�����ٶ�����Ϊ8
			BALL[i].Vy = 8;
		BALL[i].y = BALL[i].y + BALL[i].Vy; // С��y����仯
		BALL[i].x = BALL[i].x + BALL[i].Vx; // С��x����仯
	}
	if (Level == 1)
	{
		x_wall[0] = x_wall[0] + Vx_wall[0]; // �����˶�
		if (x_wall[0] >= 300) // ���������˶�
			Vx_wall[0] = -Vx_wall[0];
		if (x_wall[0] <= 50)
			Vx_wall[0] = -Vx_wall[0];
		for (i = 1; i <= Launchedball; i++) // С���������巴��
		{
			if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] && BALL[i].x <= x_wall[0] + 100) // ���С����������
			{
				BALL[i].Vy = -BALL[i].Vy * 0.85;
				BALL[i].y = y_wall[0] - Radius;
			}
		}
	}
	if (Level == 2)
	{
		for (i = 0; i < 2; i++)
			x_wall[i] = x_wall[i] + Vx_wall[i]; // �����˶�
		if (x_wall[0] > 225 - 40 || x_wall[0] < 50) // ���������˶�
			Vx_wall[0] = -Vx_wall[0];
		if (x_wall[1] > 300 || x_wall[1] < 225 - 60)
			Vx_wall[1] = -Vx_wall[1];
		for (i = 1; i <= Launchedball; i++)
		{
			for (j = 0; j < 2; j++)
			{
				//���С����������
				if (BALL[i].y + Radius >= y_wall[j] && BALL[i].y - Radius <= y_wall[j] + 20 && BALL[i].x >= x_wall[j] && BALL[i].x <= x_wall[j] + 98)
				{
					BALL[i].Vy = -BALL[i].Vy * 0.45; // �ٶȸı�
					BALL[i].y = y_wall[j] - Radius;
				}
				//���С�����������ұ�
				if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] + 98 && BALL[i].x <= x_wall[0] + 100)
				{
					BALL[i].Vy = -BALL[i].Vy * 0.45; // �ٶȸı�
					BALL[i].Vx = 2;
				}
			}
		}
	}
	if (Level == 3)
	{
		y_wall[0] = y_wall[0] + Vy_wall[0]; // �����˶�
		if (y_wall[0] >= HEIGHT / 2 - 40) // ���������˶�
			Vy_wall[0] = -Vy_wall[0];
		if (y_wall[0] <= HEIGHT / 2 - 110)
			Vy_wall[0] = -Vy_wall[0];
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] && BALL[i].x <= x_wall[0] + 100) // ���С�������ƶ��еĵ��巴��
			{
				if (BALL[i].Vx == 0) // ���ˮƽ����û���ٶ�
					BALL[i].Vx = -1 + rand() % 20 * 1.0 / 10; // �������ٶ�
				if (BALL[i].Vy > 1.5)
				{
					BALL[i].y = y_wall[0] - Radius;
					BALL[i].Vy = -BALL[i].Vy * 0.75;
				}
				else if (BALL[i].Vy >= 0) // y�����ٶ���0��1.5֮��
				{
					BALL[i].Vy = 1;
					BALL[i].y = y_wall[0] - Radius;
				}
				else if (BALL[i].Vy <= -1) 
				{
					BALL[i].y = y_wall[0] + Radius + 20;
					BALL[i].Vy = -BALL[i].Vy * 0.75;
				}
				else
				{
					BALL[i].y = BALL[i].y - 15 * BALL[i].Vy;
					if (BALL[i].y + Radius >= y_wall[0])
						BALL[i].y = y_wall[0] - Radius;
					else
					{
						BALL[i].y = y_wall[0] + Radius + 20;
						BALL[i].Vy = -BALL[i].Vy * 0.75;
					}
				}
			}
			//���С������б���壬��С�򷴵�
			if (fabs(BALL[i].x * 3 - BALL[i].y * 7 + 2316) - 20 <= Radius * sqrt(58) && BALL[i].x >= 40 && BALL[i].x <= 170) // ���С���������б����
			{
				BALL[i].Vy = -1 - BALL[i].Vy * 0.4;
				BALL[i].Vx = rand() % 10 * 1.0 / 10 + 0.5;
			}
			if (fabs(3 * BALL[i].x + 7 * BALL[i].y - 3644) <= Radius * sqrt(58) && BALL[i].x >= 282 && BALL[i].x <= 410) // ���С��������б����
			{
				BALL[i].Vy = -1 - BALL[i].Vy * 0.4;
				BALL[i].Vx = -rand() % 10 * 1.0 / 10 - 0.5;
			}
		}
	}
	if (Level == 4)
	{
		x_wall[0] = x_wall[0] + Vx_wall[0]; // �����˶�
		if (x_wall[0] >= 250) // ���������˶�
			Vx_wall[0] = -Vx_wall[0];
		if (x_wall[0] <= 90)
			Vx_wall[0] = -Vx_wall[0];
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].x - Radius <= 60 && BALL[i].y <= 315) // ���С����������
			{
				BALL[i].Vx = 2; // С���ٶ�
				BALL[i].Vy = 2;
				BALL[i].gravity = 0.1;
			}
			if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] && BALL[i].x <= x_wall[0] + 100) // ���С����������
			{
				BALL[i].Vy = -BALL[i].Vy * 0.85; // С���ٶȸı�
				BALL[i].y = y_wall[0] - Radius;
			}
		}
	}
	if (Level == 5)
	{
		x_wall[1] = x_wall[1] + Vx_wall[1]; // �����˶�
		if (x_wall[1] <= 50 || x_wall[1] >= 200) // ���������˶�
			Vx_wall[1] = -Vx_wall[1];
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].x + Radius >= x_wall[0] && BALL[i].y >= y_wall[0] && BALL[i].y <= y_wall[0] + 100) // ���С����������
			{
				BALL[i].Vx = -BALL[i].Vx; // С���ٶȸı�
				BALL[i].x = x_wall[0] - Radius;
			}
			if (BALL[i].x - Radius <= x_wall[1] + 23 && BALL[i].x - Radius >= x_wall[1] && BALL[i].y >= y_wall[1] && BALL[i].y <= y_wall[1] + 265) // ���С����������
			{
				BALL[i].Vx = -BALL[i].Vx; // С���ٶȸı�
				if (BALL[i].Vy < 0)
				{
					BALL[i].Vy = -BALL[i].Vy;
					BALL[i].Vx = -2;
				}
				BALL[i].x = x_wall[1] + 23 + Radius;
			}
		}
	}
	if (Level == 6)
	{
		x_barrier = x_barrier + Vx_barrier; // �ϰ����˶�
		y_barrier = y_barrier + Vy_barrier; // �ϰ����˶�
		if (y_barrier > 350) // �ϰ��ﵽ��ײ�
		{
			y_barrier = 350;
			Vx_barrier = 3; // �ٶȸı�
			Vy_barrier = 0;
		}
		if (x_barrier > 300) // �ϰ��ﵽ���Ҳ�
		{
			x_barrier = 300;
			Vx_barrier = 0; // �ٶȸı�
			Vy_barrier = -3;
		}
		if (y_barrier < 200) // �ϰ��ﵽ�ﶥ��
		{
			y_barrier = 200;
			Vx_barrier = -3; // �ٶȸı�
			Vy_barrier = 0;
		}
		if (x_barrier < 195) // �ϰ��ﵽ�����
		{
			x_barrier = 195;
			Vx_barrier = 0; // �ٶȸı�
			Vy_barrier = 3;
		}
		for (i = 1; i <= Numsmallball; i++)//���ѵ�С���˶�
		{
			Smallball[i].smallballVy = Smallball[i].smallballVy + Smallball[i].smallballgravity; // С��y�����ٶȱ仯
			if (Smallball[i].smallballVy > 8) // С��y�����ٶ�����Ϊ8
				Smallball[i].smallballVy = 8;
			Smallball[i].smallbally = Smallball[i].smallbally + Smallball[i].smallballVy; // С��y����仯
			Smallball[i].smallballx = Smallball[i].smallballx + Smallball[i].smallballVx; // С��x����仯
		}
		int w;
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].y + Radius >= y_barrier && BALL[i].y - Radius <= y_barrier + 60 && BALL[i].x + Radius >= x_barrier && BALL[i].x - Radius <= x_barrier + 64) // ���С�������ϰ���
			{
				w = rand() % 5 + 5; // С��������
				Numsmallball = Numsmallball + w; // �������ѵ�С��
				for (j = Numsmallball - w + 1; j <= Numsmallball; j++)
				{
					Smallball[j].smallballVx = rand() % 5 - 2; // С��ˮƽ�����ٶ�
					Smallball[j].smallballVy = rand() % 5 - 2; // С����ֱ�����ٶ�
					Smallball[j].smallballx = BALL[i].x; // С��Բ��x����
					Smallball[j].smallbally = BALL[i].y; // С��Բ��y����
					Smallball[j].smallballgravity = 0.1; // ����
					Smallball[j].smallballradius = rand() % 4 + 2; // ����뾶
					float h = rand() % 360; // С�������ɫ
					Smallball[j].color = HSVtoRGB(h, 0.6, 0.8);
				}
				BALL[i].y = 800;//ʹС���������
				BALL[i].x = 0;
				BALL[i].Vx = BALL[i].Vy = BALL[i].gravity = 0;
			}
		}
	}
	if (Level == 7)
	{
		x_barrier = x_barrier + Vx_barrier; // �ϰ����˶�
		if (x_barrier > 300) // �ϰ��ﵽ���Ҳ�
		{
			Vx_barrier = -Vx_barrier; // �ٶȸı�
		}
		if (x_barrier < 100) // �ϰ��ﵽ�����
		{
			Vx_barrier = -Vx_barrier; // �ٶȸı�
		}
		for (i = 1; i <= Numsmallball; i++)
		{
			Smallball[i].smallballVy = Smallball[i].smallballVy + Smallball[i].smallballgravity; // С��y�����ٶȱ仯
			if (Smallball[i].smallballVy > 8) // С��y�����ٶ�����Ϊ8
				Smallball[i].smallballVy = 8;
			Smallball[i].smallbally = Smallball[i].smallbally + Smallball[i].smallballVy; // С��y����仯
			Smallball[i].smallballx = Smallball[i].smallballx + Smallball[i].smallballVx; // С��x����仯
		}
		int w;
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].y + Radius >= y_barrier && BALL[i].y - Radius <= y_barrier + 60 && BALL[i].x + Radius >= x_barrier && BALL[i].x - Radius <= x_barrier + 64) // ���С�������ϰ���
			{
				w = rand() % 5 + 5; // С��������
				Numsmallball = Numsmallball + w; // �������ѵ�С��
				for (j = Numsmallball - w + 1; j <= Numsmallball; j++)
				{
					Smallball[j].smallballVx = rand() % 5 - 2; // С��ˮƽ�����ٶ�
					Smallball[j].smallballVy = rand() % 5 - 2; // С����ֱ�����ٶ�
					Smallball[j].smallballx = BALL[i].x; // С��Բ��x����
					Smallball[j].smallbally = BALL[i].y; // С��Բ��y����
					Smallball[j].smallballgravity = 0.1; // ����
					Smallball[j].smallballradius = rand() % 4 + 2; // ����뾶
					float h = rand() % 360; // С�������ɫ
					Smallball[j].color = HSVtoRGB(h, 0.6, 0.8);
				}
				BALL[i].y = 800;//ʹС���������
				BALL[i].x = 0;
				BALL[i].Vx = BALL[i].Vy = BALL[i].gravity = 0;
			}
		}
	}
	if (Level == 8)
	{
		x_launcher = x_launcher + Vx_launcher; // �������˶�
		if (x_launcher >= 300) // �����������˶�
			Vx_launcher = -Vx_launcher;
		if (x_launcher <= 100)
			Vx_launcher = -Vx_launcher;
	}
	get_distance(BALL);//С�����ײ
	knock(BALL);
}

void updateWithoutInput() // �������޹صĸ��º���
{
	move();
	MyTimer:Sleep(8);
	int t = 0, q = 0;
	for (i = 1; i <= Launchedball; i++) // ����С��
	{
		if (BALL[i].y - Radius >= HEIGHT - 250 && BALL[i].x - Radius >= WIDTH / 2 - 92 && BALL[i].x + Radius <= WIDTH / 2 + 92) // С���ڿ���
			t++;
		if (BALL[i].y >= HEIGHT || BALL[i].y <= 0 || BALL[i].x >= WIDTH || BALL[i].x <= 0) // С���ڻ�����
			q++;
		if (BALL[i].y - Radius >= HEIGHT)//С���������
		{
			BALL[i].y = 800;
			BALL[i].x = 0;
			BALL[i].Vx = BALL[i].Vy = BALL[i].gravity = 0;
		}
	}
	Ballinbox = t; // �������С�����
	Loseball = q; // ������ʧС�����
}

void updateWithInput()  // �������йصĸ��º���
{
	char input;
	if (kbhit())  // �ж��Ƿ�������
	{
		input = getch();  // �����û��Ĳ�ͬ�������жϣ���������س�
		if (input == 27) // ����ESC��
		{
			gameStatus = 3; // ��ͣ��Ϸ
		}
	}
	if (MouseHit())// ����������Ϣ
	{
		m = GetMouseMsg(); // ��ȡ�����Ϣ
		if (gameStatus == 4) // �������Ϸ��
		{
			if (isfinished()) // �ж���Ϸ�Ƿ����
				return; // �����������������
			else if (Leftball > 0 && m.uMsg == WM_LBUTTONDOWN) // ���ʣ��С�����0�Ұ���Ϊ������
			{
				Leftball--; // ʣ���С�����
				Launchedball++; // �Ѿ������С������
				if (Level == 8)
				{
					BALL[Launchedball].gravity = 0.05; // �������ٶ�
					BALL[Launchedball].Vy = 2; // ��ֱ�����ٶ�
					BALL[Launchedball].Vx = Vx_launcher / 4; // ˮƽ�����ٶ�
					BALL[Launchedball].x = x_launcher + Width_launcher / 2; // С����ʱx����
					BALL[Launchedball].y = y_launcher + Height_launcher; // С����ʱy����
					float h = rand() % 360; // С�������ɫ
					BALL[Launchedball].color = HSVtoRGB(h, 0.6, 0.8);
					BALL[Launchedball].distance[0] = 99999999;
					BALL[Launchedball].distance[1] = -1;
				}
				PlayMusicOnce(_T("lmusic.mp3")); // ����һ�η���С�����Ч
			}
		}
	}
}

int main() // ������
{
	startup();  // ��ʼ����������ִ��һ��	
	while (1) // һֱѭ��
	{
		show();  // ���л���
		updateWithoutInput(); // �������޹صĸ��� 
		updateWithInput();  // �������йصĸ���
	}
	return 0;
}