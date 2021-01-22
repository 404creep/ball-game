#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include"EasyXPng.h"
#pragma once
#include <windows.h>
#pragma comment(lib,"Winmm.lib")

#define WIDTH 450 // 画面宽度
#define HEIGHT 700 // 画面高度
#define Radius 7 // 小球半径
#define friction 0.9// 阻尼系数

class MyTimer
{
private:
	LARGE_INTEGER m_clk;//保存时钟信息
	LONGLONG m_oldclk;//保存开始时钟和结束时钟
	int m_freq;//时钟频率

public:
	MyTimer();
	void Sleep(int ms);
	void Reset();
};

MyTimer::MyTimer()
{
	QueryPerformanceFrequency(&m_clk);
	m_freq = (int)m_clk.QuadPart / 1000;
	m_oldclk = 0;//获得计数器的时钟频率
}

void MyTimer::Sleep(int ms)//延时
{
	unsigned int c = ms * m_freq;
	if (m_oldclk == 0)
	{
		QueryPerformanceCounter(&m_clk);//开始计时
		m_oldclk = m_clk.QuadPart;//获得开始时钟
	}
	m_oldclk += c;
	QueryPerformanceCounter(&m_clk);
	if (m_clk.QuadPart > m_oldclk)
		m_oldclk = m_clk.QuadPart;
	else
		do
		{
			::Sleep(1);
			QueryPerformanceCounter(&m_clk);//获得终止时钟
		} while (m_clk.QuadPart < m_oldclk);
}

void MyTimer::Reset()//重置延时器
{
	m_oldclk = 0;
}

struct Ball // 关于小球参数的结构体
{
	float Vx; // 小球水平方向速度
	float Vy; // 小球竖直方向速度
	float x; // 小球圆心x坐标
	float y; // 小球圆心y坐标
	COLORREF  color; // 小球颜色
	float distance[2]; // 记录距离它最近的小球的距离，以及这个小球的下标
	float gravity; // 重力加速度
};

struct smallBall // 关于爆炸小球参数的结构体
{
	int smallballradius; // 小球半径
	float smallballVx; // 小球水平方向速度
	float smallballVy; // 小球竖直方向速度
	float smallballx; // 小球圆心x坐标
	float smallbally; // 小球圆心y坐标
	COLORREF  color; // 小球颜色
	float smallballgravity; // 重力加速度
};

struct Ball BALL[100]; // 每个小球个参数
struct smallBall Smallball[300]; //爆炸后的小球
int i, j;
int Access[10] = { 0,1,0 };//关卡是否解锁
int isReloadLevel = 1; // 是否加载对应关卡
int Level = 1; // 关卡数
int Numsmallball; // 爆炸后小球数
int Totalball; // 每局一共多少小球
int Launchedball; // 已经发射的小球
int Leftball; // 发射器中剩余的小球
int Loseball = 0; // 丢失的小球
int Ballinbox; // 框中的小球个数
int Passball; // 每局需要多少个小球可以通关
int width_launcher[5] = { 0,60,82,80,60 }; // 发射器宽度
int height_launcher[5] = { 0,80,64,74,80 }; // 发射器高度
int Width_launcher; // 发射器宽度
int Height_launcher; // 发射器高度
int x_launcher; // 发射器左上角x坐标
int y_launcher; // 发射器左上角y坐标
float x_wall[10]; // 挡板左上角x坐标
float y_wall[10]; // 挡板左上角y坐标
float Vx_wall[10]; // 挡板x方向运动速度
float Vy_wall[10];// 挡板y方向运动速度
float x_barrier; // 障碍物x坐标
float y_barrier; // 障碍物y坐标
float Vx_barrier; // 障碍物x方向速度
float Vy_barrier; // 障碍物y方向速度
float Vx_launcher; // 发射器x方向速度

int gameStatus = 0; // 游戏状态，0为游戏主界面，1为关卡选择，2为结束游戏状态，3为游戏暂停，4为正常游戏

IMAGE im_launcher1, im_launcher2, im_launcher3, im_launcher4;  // 定义发射器图像	
IMAGE im_wall1, im_wall2, im_wall3, im_wall4,im_wall5; // 定义挡板图像
IMAGE im_box; // 定义框子图像
IMAGE im_barrier; // 定义障碍物图像
IMAGE im_continue, im_home1, im_home2, im_return, im_star, im_icon, im_firework; // 定义按钮图像
IMAGE im_bk; // 定义背景图像
MOUSEMSG m; // 鼠标函数

void startup();// 初始化函数函数
void Startmenu();//游戏主界面函数
void Maininterface();// 关卡选择界面函数
void Pausemenu();// 菜单界面函数
void readRecordFile();//读档函数
void writeRecordFile();//存档函数
void PlayMusicOnce(const TCHAR fileName[80]);//播放音乐函数
void Levelupdate(int n);//关卡初始化函数
void get_distance(struct Ball BALL[]);//小球间距离函数
int isfinished();//判断游戏是否结束函数
void knock(struct Ball BALL[]);//小球碰撞函数
void show();//绘制函数
void move();//小球及挡板的运动函数
void updateWithoutInput();// 和输入无关的更新函数
void updateWithInput();// 和输入有关的更新函数



void startup() // 初始化函数
{
	initgraph(WIDTH, HEIGHT); // 新开一个画面
	setbkcolor(RGB(255, 255, 255)); // 背景为白色
	cleardevice(); //以背景颜色清空画布
	mciSendString(_T("open backmusic.mp3 alias bkmusic"), NULL, 0, NULL); // 打开背景音乐
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL); // 循环播放
	loadimage(&im_continue, _T("继续按钮.png")); // 导入继续按钮
	loadimage(&im_home1, _T("主页按钮1.png")); // 导入主页按钮1
	loadimage(&im_home2, _T("主页按钮2.png")); // 导入主页按钮2
	loadimage(&im_return, _T("重新开始按钮.png")); // 导入重新开始按钮
	loadimage(&im_star, _T("星星.png")); // 导入星星
	loadimage(&im_icon, _T("图标.png")); // 导入图标
	loadimage(&im_box, _T("框.png")); // 导入框
	loadimage(&im_wall1, _T("挡板1.png")); // 导入挡板一
	loadimage(&im_wall2, _T("挡板2.png")); // 导入挡板二
	loadimage(&im_wall3, _T("挡板3.png")); // 导入挡板三
	loadimage(&im_wall4, _T("挡板4.png")); // 导入挡板四
	loadimage(&im_wall5, _T("挡板5.png")); // 导入挡板五
	loadimage(&im_launcher1, _T("launcher1.png")); // 导入发射器一
	loadimage(&im_launcher2, _T("launcher2.png")); // 导入发射器二
	loadimage(&im_launcher3, _T("launcher3.png")); // 导入发射器三
	loadimage(&im_launcher4, _T("launcher4.png")); // 导入发射器四
	loadimage(&im_barrier, _T("barrier.png")); // 导入障碍物
	loadimage(&im_bk, _T("背景.png")); // 导入背景
	loadimage(&im_firework, _T("烟花.png")); // 导入背景
	BeginBatchDraw(); // 开始批量绘制
}

void Startmenu()//游戏主界面
{
	putimage(0, 0, &im_bk);	// 显示背景
	setbkmode(TRANSPARENT); // 透明显示
	settextcolor(HSVtoRGB(110, 0.8, 0.6)); // 设置颜色
	settextstyle(70, 0, _T("华文彩云")); // 设置字体
	outtextxy(WIDTH * 0.2, HEIGHT * 0.15, _T("小球进框")); // 显示文字
	settextcolor(HSVtoRGB(330, 0.8, 0.6)); // 设置颜色
	settextstyle(30, 0, _T("黑体")); // 设置字体
	outtextxy(WIDTH * 0.3, HEIGHT * 0.28, _T("* 新游戏")); // 显示文字
	outtextxy(WIDTH * 0.3, HEIGHT * 0.35, _T("* 读取游戏存档")); // 显示文字
	outtextxy(WIDTH * 0.3, HEIGHT * 0.42, _T("* 保存当前进度")); // 显示文字
	outtextxy(WIDTH * 0.3, HEIGHT * 0.49, _T("* 退出")); // 显示文字

	settextcolor(BLUE); // 设置颜色
	settextstyle(20, 0, _T("黑体")); // 设置字体
	outtextxy(WIDTH * 0.2, HEIGHT * 0.6, _T("鼠标左键发射小球")); // 显示文字
	outtextxy(WIDTH * 0.2, HEIGHT * 0.65, _T("ESC键暂停游戏")); // 显示文字
	outtextxy(WIDTH * 0.2, HEIGHT * 0.7, _T("框内小球达到要求时游戏胜利")); // 显示文字
	outtextxy(WIDTH * 0.2, HEIGHT * 0.75, _T("减少小球丢失以获得更多星星")); // 显示文字
	FlushBatchDraw(); // 结束批量绘制
	Sleep(2); // 等待2毫秒

	if (MouseHit()) // 如果有鼠标消息
	{
		m = GetMouseMsg();  // 获得鼠标消息
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.28 && m.y <= HEIGHT * 0.33 && m.x >= WIDTH * 0.3) // 如果收到开始新游戏指令
		{
			Access[1] = 1;
			for (i = 2; i <= 8; i++)
				Access[i] = 0;//关卡信息初始化
			gameStatus = 1; // 进入关卡选择界面
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.35 && m.y <= HEIGHT * 0.40 && m.x >= WIDTH * 0.3) // 如果收到读取存档指令
		{
			readRecordFile(); // 读取存档
			gameStatus = 1; // 进入关卡选择界面
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.42 && m.y <= HEIGHT * 0.47 && m.x >= WIDTH * 0.3) // 如果收到写入存档指令
			writeRecordFile(); // 写入存档
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.49 && m.y <= HEIGHT * 0.54 && m.x >= WIDTH * 0.3) // 如果收到退出指令
		{
			gameStatus = 2; // 结束游戏状态
			exit(0); // 退出
		}
	}
}

void Maininterface() // 关卡选择界面函数
{
	cleardevice();//清屏
	for (i = 1; i <= 8; i++)//显示对应关卡信息（是否解锁）
	{
		if (Access[i] == 1) // 如果该关卡可进入
		{
			setlinecolor(RGB(0, 255, 0)); // 设置线条颜色
			setfillcolor(RGB(0, 255, 0)); // 设置方块颜色
			fillroundrect(250 - 150 * (i % 2), 100 + 150 * ((i - 1) / 2), 250 - 150 * (i % 2) + 100, 100 + 150 * ((i - 1) / 2) + 100, 20, 20); // 绘制方块
		}
		if (Access[i] == 0) // 如果该关卡不可进入
		{
			setlinecolor(RGB(175, 175, 175)); // 设置线条颜色
			setfillcolor(RGB(175, 175, 175)); // 设置方块颜色
			fillroundrect(250 - 150 * (i % 2), 100 + 150 * ((i - 1) / 2), 250 - 150 * (i % 2) + 100, 100 + 150 * ((i - 1) / 2) + 100, 20, 20); // 绘制方块
		}
	}
	putimage(370, 20, &im_home2);	// 显示游戏主界面按钮
	setbkmode(TRANSPARENT); // 文字字体透明    
	settextcolor(RGB(255, 0, 0));// 设定文字颜色
	settextstyle(28, 0, _T("宋体")); //  设定文字大小、样式
	outtextxy(100, 130, _T("Level 1")); //  输出文字内容
	outtextxy(250, 130, _T("Level 2")); //  输出文字内容
	outtextxy(100, 280, _T("Level 3")); //  输出文字内容
	outtextxy(250, 280, _T("Level 4")); //  输出文字内容
	outtextxy(100, 430, _T("Level 5")); //  输出文字内容
	outtextxy(250, 430, _T("Level 6")); //  输出文字内容
	outtextxy(100, 580, _T("Level 7")); //  输出文字内容
	outtextxy(250, 580, _T("Level 8")); //  输出文字内容
	FlushBatchDraw();//批量绘制
	if (MouseHit())   // 如果有鼠标消息
	{
		m = GetMouseMsg();  // 获得鼠标消息
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 100 && m.y <= 200 && m.x >= 100 && m.x <= 200 && Access[1])
		{
			Level = 1;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 100 && m.y <= 200 && m.x >= 250 && m.x <= 350 && Access[2])
		{
			Level = 2;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 250 && m.y <= 350 && m.x >= 100 && m.x <= 200 && Access[3])
		{
			Level = 3;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 250 && m.y <= 350 && m.x >= 250 && m.x <= 350 && Access[4])
		{
			Level = 4;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 400 && m.y <= 500 && m.x >= 100 && m.x <= 200 && Access[5])
		{
			Level = 5;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 400 && m.y <= 500 && m.x >= 250 && m.x <= 350 && Access[6])
		{
			Level = 6;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 650 && m.x >= 100 && m.x <= 200 && Access[7])
		{
			Level = 7;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 650 && m.x >= 250 && m.x <= 350 && Access[8])
		{
			Level = 8;
			Levelupdate(Level); // 关卡信息更新
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= 20 && m.y <= 63 && m.x >= 370 && m.x <= 420) // 如果鼠标点击游戏主界面按钮
			gameStatus = 0; // 进入游戏主界面
	}
}

void Pausemenu() // 游戏暂停后菜单界面
{
	putimage(0, 0, &im_bk);	// 显示背景
	setbkmode(TRANSPARENT); // 透明显示
	settextcolor(HSVtoRGB(110, 0.8, 0.6)); // 设置颜色
	settextstyle(70, 0, _T("华文彩云")); // 设置字体
	outtextxy(WIDTH * 0.2, HEIGHT * 0.15, _T("小球进框")); // 显示文字
	settextcolor(HSVtoRGB(330, 0.8, 0.6)); // 设置颜色
	settextstyle(30, 0, _T("黑体")); // 设置字体
	outtextxy(WIDTH * 0.3, HEIGHT * 0.28, _T("* 继续游戏"));  // 显示文字
	outtextxy(WIDTH * 0.3, HEIGHT * 0.35, _T("* 重新开始")); // 显示文字
	outtextxy(WIDTH * 0.3, HEIGHT * 0.42, _T("* 返回主界面")); // 显示文字
	outtextxy(WIDTH * 0.3, HEIGHT * 0.49, _T("* 退出")); // 显示文字

	settextcolor(BLUE); // 设置颜色
	settextstyle(20, 0, _T("黑体")); // 设置字体
	outtextxy(WIDTH * 0.2, HEIGHT * 0.6, _T("鼠标左键发射小球")); // 显示文字
	outtextxy(WIDTH * 0.2, HEIGHT * 0.65, _T("ESC键暂停游戏")); // 显示文字
	outtextxy(WIDTH * 0.2, HEIGHT * 0.7, _T("框内小球达到要求时游戏胜利")); // 显示文字
	outtextxy(WIDTH * 0.2, HEIGHT * 0.75, _T("减少小球丢失以获得更多星星")); // 显示文字
	FlushBatchDraw(); // 结束批量绘制
	Sleep(2); // 等待两毫秒

	if (MouseHit()) // 如果有鼠标消息
	{
		m = GetMouseMsg();  // 获得鼠标消息
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.28 && m.y <= HEIGHT * 0.33 && m.x >= WIDTH * 0.3) // 如果收到继续指令
			gameStatus = 4; // 进入正常游戏
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.35 && m.y <= HEIGHT * 0.40 && m.x >= WIDTH * 0.3) // 如果收到重新开始指令
		{
			Levelupdate(Level);
			gameStatus = 4; // 进入正常游戏
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.42 && m.y <= HEIGHT * 0.47 && m.x >= WIDTH * 0.3) // 如果收到返回主界面指令
			gameStatus = 0; // 进入初始菜单界面
		if (m.uMsg == WM_LBUTTONDOWN && m.y >= HEIGHT * 0.49 && m.y <= HEIGHT * 0.54 && m.x >= WIDTH * 0.3) // 如果收到退出指令
		{
			gameStatus = 2; // 结束游戏状态
			exit(0); // 退出
		}
	}
}

void readRecordFile()  //读取游戏数据文件存档
{
	FILE* fp;
	fp = fopen(".\\gameRecord.dat", "r");//读取关卡解锁信息
	fscanf(fp, "%d %d %d %d %d %d %d %d", &Access[1], &Access[2], &Access[3], &Access[4], &Access[5], &Access[6], &Access[7], &Access[8]);
	fclose(fp);
}

void writeRecordFile()  // 存储游戏数据文件存档
{
	FILE* fp;
	fp = fopen(".\\gameRecord.dat", "w");//写入关卡解锁信息
	fprintf(fp, "%d %d %d %d %d %d %d %d", Access[1], Access[2], Access[3], Access[4], Access[5], Access[6], Access[7], Access[8]);
	fclose(fp);
}

void PlayMusicOnce(const TCHAR fileName[80]) // 播放一次音乐函数
{
	TCHAR cmdString1[50];
	_stprintf(cmdString1, _T("open %s alias tmpmusic"), fileName); // 生成命令字符串
	mciSendString(_T("close tmpmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
	mciSendString(cmdString1, NULL, 0, NULL); // 打开音乐
	mciSendString(_T("play tmpmusic"), NULL, 0, NULL); // 仅播放一次
}

void Levelupdate(int n) // 与关卡有关的初始化
{
	if (n == 1)
	{
		Width_launcher = width_launcher[1]; // 发射器参数
		Height_launcher = height_launcher[1];
		x_launcher = WIDTH / 2 - Width_launcher / 2; // 发射器左上角x坐标
		y_launcher = HEIGHT / 4 - Height_launcher; // 发射器左上角y坐标
		x_wall[0] = WIDTH / 4; // 挡板左上角x坐标
		y_wall[0] = HEIGHT / 2;  // 挡板左上角y坐标
		Vx_wall[0] = 1.5; // 挡板速度为1.5
		Totalball = 30; // 一共30个小球
		Leftball = 30; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 20; // 20个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // 重力加速度
			BALL[i].Vy = 2; // 竖直方向速度
			BALL[i].Vx = 0; // 水平方向速度
			BALL[i].x = WIDTH / 2; // 小球发射时x坐标
			BALL[i].y = y_launcher + Height_launcher; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 2)
	{
		Width_launcher = width_launcher[1]; // 发射器参数
		Height_launcher = height_launcher[1]; // 发射器参数
		x_launcher = WIDTH / 2 - Width_launcher / 2; // 发射器左上角x坐标
		y_launcher = HEIGHT / 4 - Height_launcher; // 发射器左上角y坐标
		Totalball = 35; // 一共35个小球
		Leftball = 35; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 25; // 25个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		x_wall[0] = 225 - 60; // 挡板一左上角x坐标
		y_wall[0] = HEIGHT / 3; // 挡板一左上角y坐标
		x_wall[1] = 225 - 40; // 挡板二左上角x坐标
		y_wall[1] = HEIGHT / 2; // 挡板二左上角y坐标
		Vx_wall[0] = -1.5; // 挡板速度为-1.5
		Vx_wall[1] = 1.5; // 挡板速度为1.5
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // 重力加速度
			BALL[i].Vy = 2; // 竖直方向速度
			BALL[i].Vx = 0; // 水平方向速度
			BALL[i].x = WIDTH / 2; // 小球发射时x坐标
			BALL[i].y = y_launcher + Height_launcher; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 3)
	{
		Width_launcher = width_launcher[1]; // 发射器参数
		Height_launcher = height_launcher[1]; // 发射器参数
		x_launcher = WIDTH / 2 - Width_launcher / 2; // 发射器左上角x坐标
		y_launcher = HEIGHT / 4 - Height_launcher; // 发射器左上角y坐标
		Totalball = 30; // 一共30个小球
		Leftball = 30; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 25; // 25个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		x_wall[0] = WIDTH / 2 - 50; // 挡板一左上角x坐标
		y_wall[0] = HEIGHT / 2 - 75; // 挡板一左上角y坐标
		Vy_wall[0] = 1; // 挡板一速度为1
		x_wall[1] = 40; // 挡板二左上角x坐标
		y_wall[1] = HEIGHT / 2; // 挡板二左上角y坐标
		x_wall[2] = 282; // 挡板三左上角x坐标
		y_wall[2] = HEIGHT / 2; // 挡板三左上角y坐标
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // 重力加速度
			BALL[i].Vy = 2; // 竖直方向速度
			BALL[i].Vx = 0; // 水平方向速度
			BALL[i].x = WIDTH / 2; // 小球发射时x坐标
			BALL[i].y = y_launcher + Height_launcher; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 4)
	{
		Width_launcher = width_launcher[2]; // 发射器参数
		Height_launcher = height_launcher[2]; // 发射器参数
		x_launcher = WIDTH / 2 - Width_launcher / 2; // 发射器左上角x坐标
		y_launcher = HEIGHT / 4 - Height_launcher; // 发射器左上角y坐标
		Totalball = 30; // 一共30个小球
		Leftball = 30; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 20; // 20个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		x_wall[0] = 100; // 挡板一左上角x坐标
		y_wall[0] = HEIGHT / 2; // 挡板一左上角y坐标
		Vx_wall[0] = 1.5; // 挡板一速度为1.5
		x_wall[1] = 40; // 挡板二左上角x坐标
		y_wall[1] = 50; // 挡板二左上角y坐标
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.03; // 重力加速度
			BALL[i].Vy = 0; // 竖直方向速度
			BALL[i].Vx = -3; // 水平方向速度
			BALL[i].x = x_launcher; // 小球发射时x坐标
			BALL[i].y = y_launcher + Height_launcher / 2; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 5)
	{
		Width_launcher = width_launcher[3]; // 发射器参数
		Height_launcher = height_launcher[3]; // 发射器参数
		x_launcher = WIDTH / 4; // 发射器左上角x坐标
		y_launcher = HEIGHT / 7 - 50; // 发射器左上角y坐标
		Totalball = 35; // 一共35个小球
		Leftball = 35; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 25; // 25个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		x_wall[0] = 250; // 挡板一左上角x坐标
		y_wall[0] = 130; // 挡板一左上角y坐标
		Vx_wall[1] = 1; // 挡板一速度为1
		x_wall[1] = 150;  // 挡板二左上角x坐标
		y_wall[1] = 150; // 挡板二左上角y坐标
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.05; // 重力加速度
			BALL[i].Vy = 3; // 竖直方向速度
			BALL[i].Vx = 3; // 水平方向速度
			BALL[i].x = x_launcher + Width_launcher; // 小球发射时x坐标
			BALL[i].y = y_launcher + Height_launcher; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 6)
	{
		Width_launcher = width_launcher[1]; // 发射器参数
		Height_launcher = height_launcher[1]; // 发射器参数
		x_launcher = WIDTH / 2 - Width_launcher / 2; // 发射器左上角x坐标
		y_launcher = HEIGHT / 4 - Height_launcher; // 发射器左上角y坐标
		x_barrier = 195; // 障碍物x坐标
		y_barrier = 200; // 障碍物y坐标
		Vx_barrier = 0; // 障碍物x方向速度为0
		Vy_barrier = 3; // 障碍物y方向速度为2
		Totalball = 30; // 一共30个小球
		Leftball = 30; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 20; // 20个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		Numsmallball = 0; // 爆炸的小球
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // 重力加速度
			BALL[i].Vy = 2; // 竖直方向速度
			BALL[i].Vx = 0; // 水平方向速度
			BALL[i].x = WIDTH / 2; // 小球发射时x坐标
			BALL[i].y = y_launcher + Height_launcher; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 7)
	{
		Width_launcher = width_launcher[4]; // 发射器参数
		Height_launcher = height_launcher[4]; // 发射器参数
		x_launcher = WIDTH / 9; // 发射器左上角x坐标
		y_launcher = HEIGHT / 4 - Height_launcher; // 发射器左上角y坐标
		x_barrier = 150; // 障碍物x坐标
		y_barrier = 300; // 障碍物y坐标
		Vx_barrier = 2; // 障碍物x方向速度为0
		Totalball = 40; // 一共40个小球
		Leftball = 40; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 24; // 24个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		Numsmallball = 0; // 爆炸的小球
		for (i = 1; i <= Totalball; i++)
		{
			BALL[i].gravity = 0.1; // 重力加速度
			BALL[i].Vy = -2; // 竖直方向速度
			BALL[i].Vx = 1.5; // 水平方向速度
			BALL[i].x = WIDTH / 9 + 50; // 小球发射时x坐标
			BALL[i].y = y_launcher; // 小球发射时y坐标
			float h = rand() % 360; // 小球随机颜色
			BALL[i].color = HSVtoRGB(h, 0.6, 0.8);
			BALL[i].distance[0] = 99999999;
			BALL[i].distance[1] = -1;
		}
	}
	if (n == 8)
	{
		Width_launcher = width_launcher[1]; // 发射器参数
		Height_launcher = height_launcher[1]; // 发射器参数
		x_launcher = 100; // 发射器左上角x坐标
		y_launcher = HEIGHT / 7 - 50; // 发射器左上角y坐标
		Totalball = 36; // 一共36个小球
		Leftball = 36; // 剩余小球个数
		Ballinbox = 0; // 框中的球
		Passball = 25; // 25个球过关
		Launchedball = 0; // 开始0个已经发射的小球
		Vx_launcher = 2; // 发射器速度
	}
}

void get_distance(struct Ball BALL[]) // 计算所有小球两两之间的距离平方
{
	for (i = 1; i <= Launchedball; i++) // 遍历小球
	{
		for (j = 1; j <= Launchedball; j++) // 遍历小球
		{

			if (i != j) // 自己和自己不需要比
			{
				int dist2;
				dist2 = (BALL[i].x - BALL[j].x) * (BALL[i].x - BALL[j].x)
					+ (BALL[i].y - BALL[j].y) * (BALL[i].y - BALL[j].y);//计算距离
				if (dist2 < BALL[i].distance[0]) // 如果距离小于当前最小距离
				{
					BALL[i].distance[0] = dist2; // 记录最小距离
					BALL[i].distance[1] = j; // 记录小球下标
				}
			}
		}
	}
}

int isfinished() // 判断小球是否全部下落完
{
	int count = 0; // 个数
	if (Ballinbox == Passball) // 如果框中的小球数等于过关的小球数
		return 1;//游戏结束
	else
	{
		if (Launchedball < Totalball) // 已经发射的小球数小于小球总数
			return 0;//游戏继续
		else
		{
			for (i = 1; i <= Launchedball; i++)
			{
				// 如果在框中或掉出画面
				if ((BALL[i].x >= WIDTH / 2 - 92 && BALL[i].x <= WIDTH / 2 + 92 && BALL[i].y >= HEIGHT - 250 && BALL[i].Vy <= 2) || BALL[i].y - Radius >= HEIGHT)
					count++;
			}
			if (count == Totalball) // 如果个数等于小球总数
				return 1;//游戏结束
			else
				return 0;//游戏继续
		}
	}
}

void knock(struct Ball BALL[]) // 判断球之间、球和框是否碰撞
{
	for (i = 1; i <= Launchedball; i++) // 小球之间的碰撞
	{
		if (BALL[i].distance[0] <= 4 * Radius * Radius) // 最小距离小于两个半径发生碰撞
		{
			j = BALL[i].distance[1]; // j为小球下标
			if (BALL[i].y - Radius >= HEIGHT); // 落到屏幕外的球碰撞不会发生效果
			else if (BALL[i].x >= WIDTH / 2 - 92 && BALL[i].x <= WIDTH / 2 + 92 && BALL[i].y >= HEIGHT - 250) // 在框内碰撞
			{
				if (fabs(BALL[i].Vx) > 1 || fabs(BALL[j].Vx) > 1 || fabs(BALL[i].Vy) > 1 || fabs(BALL[j].Vy) > 1) // 如果某一个方向的速度的绝对值大于1
				{
					BALL[i].Vx = 0.5; // x方向速度
					BALL[j].Vx = -0.5;
					if (BALL[i].x <= BALL[j].x) // 如果小球i在小球j左侧
					{
						BALL[i].Vx = -BALL[i].Vx; // x方向速度相反
						BALL[j].Vx = -BALL[j].Vx;
					}
					BALL[i].Vy = 0.5; //小球y方向速度
					BALL[j].Vy = -0.5;
					if (BALL[i].y <= BALL[j].y) // 如果小球i在小球j下方
					{
						BALL[i].Vy = -BALL[i].Vy; // y方向速度相反
						BALL[j].Vy = -BALL[j].Vy;
					}
				}
				else
				{
					while ((BALL[i].x - BALL[j].x) * (BALL[i].x - BALL[j].x) + (BALL[i].y - BALL[j].y) * (BALL[i].y - BALL[j].y) < 4 * Radius * Radius + 8)
					{
						if (BALL[i].y < BALL[j].y) // 如果小球i在j下方
						{
							BALL[i].y -= 0.1; // 小球i下降一点
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
					BALL[i].Vx = BALL[i].Vx * friction; // 小球速度受摩擦力影响而减小
					BALL[j].Vx = BALL[j].Vx * friction;
					BALL[i].Vy = BALL[i].Vy * friction;
					BALL[j].Vy = BALL[j].Vy * friction;
				}
			}
			else // 在框外碰撞
			{
				BALL[i].Vx = 0.2 + rand() % 5 * 1.0 / 5; // 小球碰撞时x方向速度随机
				BALL[j].Vx = -0.2 - rand() % 5 * 1.0 / 5;
				if (BALL[i].x <= BALL[j].x)
				{
					BALL[i].Vx = -BALL[i].Vx;
					BALL[j].Vx = -BALL[j].Vx;
				}
				BALL[i].Vy = 0.2 + rand() % 5 * 1.0 / 5; // 小球碰时y方向速度改变
				BALL[j].Vy = -0.2 - rand() % 5 * 1.0 / 5;
				if (BALL[i].y <= BALL[j].y)
				{
					BALL[i].Vy = -BALL[i].Vy;
					BALL[j].Vy = -BALL[j].Vy;
				}
			}
		}
		BALL[j].distance[0] = 99999999; // 避免交换两次速度，将两个小球重新赋值
		BALL[j].distance[1] = -1;
		BALL[i].distance[0] = 99999999;
		BALL[i].distance[1] = -1;
	}
	//判断是否与框相撞
	for (i = 1; i <= Launchedball; i++) // 遍历小球
	{
		if (BALL[i].y + Radius >= HEIGHT - 250 && BALL[i].x <= WIDTH / 2 - 60 && BALL[i].x >= WIDTH / 2 - 92) // 如果和框的左上侧碰撞
		{
			BALL[i].y = HEIGHT - 257;
			BALL[i].Vy = -2; // 反弹
		}
		if (BALL[i].y + Radius >= HEIGHT - 250 && BALL[i].x >= WIDTH / 2 + 60 && BALL[i].x <= WIDTH / 2 + 92) // 如果和框的右上侧碰撞
		{
			BALL[i].y = HEIGHT - 257;
			BALL[i].Vy = -2; // 反弹
		}
		if (BALL[i].y - Radius >= HEIGHT - 250 && BALL[i].x - Radius >= WIDTH / 2 - 92 && BALL[i].x + Radius <= WIDTH / 2 + 92)
		{
			if (BALL[i].x - Radius <= WIDTH / 2 - 57) // 如果与框的左边界碰撞
			{
				BALL[i].x = WIDTH / 2 - 57 + Radius;
				BALL[i].Vx = -BALL[i].Vx * friction; // x方向速度改变
			}
			if (BALL[i].x + Radius >= WIDTH / 2 + 57) // 如果与框的右边界碰撞
			{
				BALL[i].x = WIDTH / 2 + 57 - Radius;
				BALL[i].Vx = -BALL[i].Vx * friction; // x方向速度改变
			}

			if (BALL[i].y + Radius >= HEIGHT - 184) // 如果小球与框的底部碰撞
			{
				BALL[i].y = HEIGHT - 190;
				BALL[i].Vy = 0; // y方向速度为0
			}
		}
	}
}

void show()
{
	while (gameStatus == 0)
		Startmenu(); // 进入主菜单界面
	while (gameStatus == 1)
		Maininterface(); // 进入关卡选择界面
	while (gameStatus == 3)
		Pausemenu(); // 游戏暂停后菜单界面，按ESC键后启动该界面
	if (isReloadLevel == 1) // 重新加载关卡
	{
		Levelupdate(Level); // 更新关卡数据
		isReloadLevel = 0;
	}
	//进入游戏
	cleardevice(); // 清屏
	putimage(WIDTH / 2 - 92, HEIGHT - 250, &im_box); // 显示框
	if (Level == 1) // 第一关
	{
		putimage(x_wall[0], y_wall[0], &im_wall1); // 显示挡板一
		putimage(x_launcher, y_launcher, &im_launcher1); // 显示发射器
	}

	if (Level == 2) // 第二关
	{
		putimage(x_wall[0], y_wall[0], &im_wall1); // 显示挡板一
		putimage(x_wall[1], y_wall[1], &im_wall1); // 显示挡板二
		putimage(x_launcher, y_launcher, &im_launcher1); // 显示发射器
	}
	if (Level == 3) // 第三关
	{
		putimage(x_wall[1], y_wall[1], &im_wall2); // 显示挡板二
		putimage(x_wall[2], y_wall[2], &im_wall3); // 显示挡板三		
		putimage(x_wall[0], y_wall[0], &im_wall1); // 显示挡板一
		putimage(x_launcher, y_launcher, &im_launcher1); // 显示发射器
	}
	if (Level == 4) // 第四关
	{
		putimage(x_wall[0], y_wall[0], &im_wall1); // 显示挡板一
		putimage(x_wall[1], y_wall[1], &im_wall4); // 显示挡板四
		putimage(x_launcher, y_launcher, &im_launcher2); // 显示发射器
	}
	if (Level == 5) // 第五关
	{
		putimage(x_wall[0], y_wall[0], &im_wall5); // 显示挡板五
		putimage(x_wall[1], y_wall[1], &im_wall4); // 显示挡板四
		putimage(x_launcher, y_launcher, &im_launcher3); // 显示发射器
	}
	if (Level == 6) // 第六关
	{
		putimage(x_launcher, y_launcher, &im_launcher1); // 显示发射器
		putimage(x_barrier, y_barrier, &im_barrier); // 显示障碍物
		for (i = 1; i <= Numsmallball; i++)
		{
			setfillcolor(Smallball[i].color); // 小球颜色
			setlinecolor(Smallball[i].color); // 小球颜色
			fillcircle(Smallball[i].smallballx, Smallball[i].smallbally, Smallball[i].smallballradius); // 绘制小球
		}
	}
	if (Level == 7) // 第七关
	{
		putimage(x_launcher, y_launcher, &im_launcher4); // 显示发射器
		putimage(x_barrier, y_barrier, &im_barrier); // 显示障碍物
		for (i = 1; i <= Numsmallball; i++)
		{
			setfillcolor(Smallball[i].color); // 小球颜色
			setlinecolor(Smallball[i].color); // 小球颜色
			fillcircle(Smallball[i].smallballx, Smallball[i].smallbally, Smallball[i].smallballradius); // 绘制小球
		}
	}
	if (Level == 8) // 第八关
	{
		putimage(x_launcher, y_launcher, &im_launcher1); // 显示发射器
	}
	// 显示这是第几关
	TCHAR s[20]; // 字符串
	setbkmode(TRANSPARENT); // 文字透明显示
	_stprintf(s, _T("第 %d 关"), Level); // 生成文字字符串
	settextcolor(RGB(0, 50, 200));  // 设置文字颜色
	settextstyle(30, 0, _T("黑体")); // 设置文字大小、字体
	outtextxy(WIDTH * 0.38, 20, s); // 输出文字

	//显示发射器中的小球参数
	TCHAR t[20]; // 字符串
	_stprintf(t, _T("%d / %d"), Leftball, Totalball); // 生成文字字符串
	settextcolor(RGB(200, 50, 0));  // 设置文字颜色
	settextstyle(30, 0, _T("黑体")); // 设置文字大小、字体
	outtextxy(340, 80, t); // 输出文字

	//显示框中的小球参数
	TCHAR r[20]; // 字符串
	_stprintf(r, _T("%d / %d"), Ballinbox, Passball); // 生成文字字符串
	settextcolor(RGB(200, 50, 0));  // 设置文字颜色
	settextstyle(30, 0, _T("黑体")); // 设置文字大小、字体
	outtextxy(WIDTH / 2 - 40, HEIGHT - 150, r); // 输出文字

	for (i = 1; i <= Launchedball; i++)
	{
		setfillcolor(BALL[i].color); // 小球颜色
		setlinecolor(BALL[i].color); // 小球颜色
		fillcircle(BALL[i].x, BALL[i].y, Radius); // 绘制小球
	}
	if (isfinished())
	{
		if (Ballinbox == Passball && Level < 8) // 如果框中的小球达标
		{
			cleardevice(); // 清屏
			setbkmode(TRANSPARENT); // 文字字体透明    
			settextcolor(RGB(0, 0, 0)); // 设定文字颜色
			settextstyle(70, 0, _T("MV Boli")); // 设定文字大小、样式
			outtextxy(150, 50, _T("LEVEL")); // 输出文字内容
			settextcolor(BLUE); // 设定文字颜色
			settextstyle(110, 0, _T("Arial Black")); // 设定文字大小、样式
			outtextxy(60, 100, _T("CLEAR!")); // 输出文字内容
			putimage(WIDTH / 2 - 120, 450, &im_continue); // 显示继续按钮
			putimage(20, 550, &im_home1); // 显示返回关卡选择按钮
			putimage(WIDTH / 2 + 130, 550, &im_return); // 显示重新开始按钮
			TCHAR x[100]; // 字符串
			_stprintf(x, _T("l o s e   b a l l s : %d"), Loseball); // 生成文字字符串
			settextcolor(HSVtoRGB(280, 0.4, 0.8)); // 设定文字颜色
			settextstyle(50, 0, _T("Comic Sans MS")); // 设定文字大小、样式
			outtextxy(70, 370, x); // 输出文字内容
			Access[Level + 1] = 1; // 该关卡可以进入
			FlushBatchDraw(); // 结束批量绘制
			if (Loseball <= 10)
			{
				Sleep(500);
				putimage(80, 280, &im_star); // 显示星星
				FlushBatchDraw(); // 结束批量绘制
			}
			if (Loseball <= 5)
			{
				Sleep(500);
				putimage(180, 210, &im_star); // 显示星星
				FlushBatchDraw(); // 结束批量绘制
			}
			if (Loseball <= 2)
			{
				Sleep(500);
				putimage(280, 280, &im_star); // 显示星星
				FlushBatchDraw(); // 结束批量绘制
			}
			while (isReloadLevel == 0)
			{
				if (MouseHit())   // 如果有鼠标消息
				{
					m = GetMouseMsg();  // 获得鼠标消息
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 450 && m.y <= 450 + 74 && m.x >= WIDTH / 2 - 120 && m.x <= WIDTH / 2 + 120) // 如果按下鼠标左键，并且按到了继续按钮
					{
						Level++; // 关卡加一
						isReloadLevel = 1;
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 61 && m.x >= WIDTH / 2 + 130 && m.x <= WIDTH / 2 + 200) // 如果按下鼠标左键，并且按到了重新开始按钮
					{
						isReloadLevel = 1; // 重新开始
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 62 && m.x >= 20 && m.x <= 20 + 70) // 如果按下鼠标左键，并且按到了返回关卡选择按钮
					{
						isReloadLevel = 1;
						gameStatus = 1; // 进入关卡选择界面
					}
				}
			}
		}
		if (Level == 8 && isReloadLevel == 0 && Ballinbox == Passball) // 如果第八关通过
		{
			cleardevice(); // 清屏
			putimage(WIDTH / 2, 30, &im_firework); // 显示烟花
			setbkmode(TRANSPARENT); // 文字字体透明    
			settextcolor(RED); // 设定文字颜色
			settextstyle(110, 0, _T("华文新魏")); // 设定文字大小、样式
			outtextxy(10, 150, _T("恭喜通关")); // 输出文字内容
			putimage(20, 550, &im_home1); // 显示返回关卡选择按钮
			putimage(WIDTH / 2 + 130, 550, &im_return); // 显示重新开始按钮
			TCHAR x[100]; // 字符串
			_stprintf(x, _T("l o s e   b a l l s : %d"), Loseball); // 生成文字字符串
			settextcolor(HSVtoRGB(280, 0.4, 0.8)); // 设定文字颜色
			settextstyle(50, 0, _T("Comic Sans MS")); // 设定文字大小、样式
			outtextxy(70, 470, x); // 输出文字内容
			FlushBatchDraw(); // 结束批量绘制
			if (Loseball <= 10)
			{
				Sleep(500);
				putimage(80, 350, &im_star); // 显示星星
				FlushBatchDraw(); // 结束批量绘制
			}
			if (Loseball <= 5)
			{
				Sleep(500);
				putimage(180, 280, &im_star); // 显示星星
				FlushBatchDraw(); // 结束批量绘制
			}
			if (Loseball <= 2)
			{
				Sleep(500);
				putimage(280, 350, &im_star); // 显示星星
				FlushBatchDraw(); // 结束批量绘制
			}
			while (isReloadLevel == 0)
			{
				if (MouseHit())   // 如果有鼠标消息
				{
					m = GetMouseMsg();  // 获得鼠标消息
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 61 && m.x >= WIDTH / 2 + 130 && m.x <= WIDTH / 2 + 200) // 如果按下鼠标左键，并且按到了重新开始按钮
					{
						isReloadLevel = 1; // 重新开始
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 62 && m.x >= 20 && m.x <= 20 + 70) // 如果按下鼠标左键，并且按到了返回关卡选择按钮
					{
						isReloadLevel = 1;
						gameStatus = 1; // 进入关卡选择界面
					}
				}
			}
		}
		if (Ballinbox < Passball) // 游戏失败
		{
			cleardevice(); // 清屏
			setbkmode(TRANSPARENT); // 文字字体透明    
			settextcolor(RGB(0, 0, 0)); // 设定文字颜色
			settextstyle(70, 0, _T("MV Boli")); // 设定文字大小、样式
			outtextxy(130, 100, _T("GAME")); // 输出文字内容
			settextcolor(RED); // 设定文字颜色
			settextstyle(110, 0, _T("Arial Black")); // 设定文字大小、样式
			outtextxy(40, 150, _T("FAILED...")); // 输出文字内容
			putimage(WIDTH / 2 + 130, 550, &im_return); // 显示重新开始按钮
			putimage(20, 550, &im_home1); // 显示返回主页按钮
			putimage(WIDTH / 2 - 100, 280, &im_icon); // 显示图标按钮
			FlushBatchDraw(); // 结束批量绘制
			while (isReloadLevel == 0)
			{
				if (MouseHit())   // 如果有鼠标消息
				{
					m = GetMouseMsg();  // 获得鼠标消息
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 61 && m.x >= WIDTH / 2 + 130 && m.x <= WIDTH / 2 + 200) // 如果按下鼠标左键，并且按到了重新开始按钮
					{
						isReloadLevel = 1; // 重新开始
					}
					if (m.uMsg == WM_LBUTTONDOWN && m.y >= 550 && m.y <= 550 + 62 && m.x >= 20 && m.x <= 20 + 70) // 如果按下鼠标左键，并且按到了关卡选择按钮
					{
						isReloadLevel = 1;
						gameStatus = 1; // 进入关卡选择界面
					}
				}
			}
		}
	}
	FlushBatchDraw(); // 结束批量绘制
}

void move() // 小球及挡板的运动
{
	for (i = 1; i <= Launchedball; i++) // 小球运动
	{
		BALL[i].Vy = BALL[i].Vy + BALL[i].gravity; // 小球y方向速度变化
		if (BALL[i].Vy > 8) // 小球y方向速度上限为8
			BALL[i].Vy = 8;
		BALL[i].y = BALL[i].y + BALL[i].Vy; // 小球y坐标变化
		BALL[i].x = BALL[i].x + BALL[i].Vx; // 小球x坐标变化
	}
	if (Level == 1)
	{
		x_wall[0] = x_wall[0] + Vx_wall[0]; // 挡板运动
		if (x_wall[0] >= 300) // 挡板往返运动
			Vx_wall[0] = -Vx_wall[0];
		if (x_wall[0] <= 50)
			Vx_wall[0] = -Vx_wall[0];
		for (i = 1; i <= Launchedball; i++) // 小球碰到挡板反弹
		{
			if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] && BALL[i].x <= x_wall[0] + 100) // 如果小球碰到挡板
			{
				BALL[i].Vy = -BALL[i].Vy * 0.85;
				BALL[i].y = y_wall[0] - Radius;
			}
		}
	}
	if (Level == 2)
	{
		for (i = 0; i < 2; i++)
			x_wall[i] = x_wall[i] + Vx_wall[i]; // 挡板运动
		if (x_wall[0] > 225 - 40 || x_wall[0] < 50) // 挡板往返运动
			Vx_wall[0] = -Vx_wall[0];
		if (x_wall[1] > 300 || x_wall[1] < 225 - 60)
			Vx_wall[1] = -Vx_wall[1];
		for (i = 1; i <= Launchedball; i++)
		{
			for (j = 0; j < 2; j++)
			{
				//如果小球碰到挡板
				if (BALL[i].y + Radius >= y_wall[j] && BALL[i].y - Radius <= y_wall[j] + 20 && BALL[i].x >= x_wall[j] && BALL[i].x <= x_wall[j] + 98)
				{
					BALL[i].Vy = -BALL[i].Vy * 0.45; // 速度改变
					BALL[i].y = y_wall[j] - Radius;
				}
				//如果小球碰到挡板右边
				if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] + 98 && BALL[i].x <= x_wall[0] + 100)
				{
					BALL[i].Vy = -BALL[i].Vy * 0.45; // 速度改变
					BALL[i].Vx = 2;
				}
			}
		}
	}
	if (Level == 3)
	{
		y_wall[0] = y_wall[0] + Vy_wall[0]; // 挡板运动
		if (y_wall[0] >= HEIGHT / 2 - 40) // 挡板往返运动
			Vy_wall[0] = -Vy_wall[0];
		if (y_wall[0] <= HEIGHT / 2 - 110)
			Vy_wall[0] = -Vy_wall[0];
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] && BALL[i].x <= x_wall[0] + 100) // 如果小球碰到移动中的挡板反弹
			{
				if (BALL[i].Vx == 0) // 如果水平方向没有速度
					BALL[i].Vx = -1 + rand() % 20 * 1.0 / 10; // 获得随机速度
				if (BALL[i].Vy > 1.5)
				{
					BALL[i].y = y_wall[0] - Radius;
					BALL[i].Vy = -BALL[i].Vy * 0.75;
				}
				else if (BALL[i].Vy >= 0) // y方向速度在0到1.5之间
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
			//如果小球碰到斜挡板，则小球反弹
			if (fabs(BALL[i].x * 3 - BALL[i].y * 7 + 2316) - 20 <= Radius * sqrt(58) && BALL[i].x >= 40 && BALL[i].x <= 170) // 如果小球碰到左边斜挡板
			{
				BALL[i].Vy = -1 - BALL[i].Vy * 0.4;
				BALL[i].Vx = rand() % 10 * 1.0 / 10 + 0.5;
			}
			if (fabs(3 * BALL[i].x + 7 * BALL[i].y - 3644) <= Radius * sqrt(58) && BALL[i].x >= 282 && BALL[i].x <= 410) // 如果小球碰到右斜挡板
			{
				BALL[i].Vy = -1 - BALL[i].Vy * 0.4;
				BALL[i].Vx = -rand() % 10 * 1.0 / 10 - 0.5;
			}
		}
	}
	if (Level == 4)
	{
		x_wall[0] = x_wall[0] + Vx_wall[0]; // 挡板运动
		if (x_wall[0] >= 250) // 挡板往返运动
			Vx_wall[0] = -Vx_wall[0];
		if (x_wall[0] <= 90)
			Vx_wall[0] = -Vx_wall[0];
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].x - Radius <= 60 && BALL[i].y <= 315) // 如果小球碰到挡板
			{
				BALL[i].Vx = 2; // 小球速度
				BALL[i].Vy = 2;
				BALL[i].gravity = 0.1;
			}
			if (BALL[i].y + Radius >= y_wall[0] && BALL[i].y - Radius <= y_wall[0] + 20 && BALL[i].x >= x_wall[0] && BALL[i].x <= x_wall[0] + 100) // 如果小球碰到挡板
			{
				BALL[i].Vy = -BALL[i].Vy * 0.85; // 小球速度改变
				BALL[i].y = y_wall[0] - Radius;
			}
		}
	}
	if (Level == 5)
	{
		x_wall[1] = x_wall[1] + Vx_wall[1]; // 挡板运动
		if (x_wall[1] <= 50 || x_wall[1] >= 200) // 挡板往返运动
			Vx_wall[1] = -Vx_wall[1];
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].x + Radius >= x_wall[0] && BALL[i].y >= y_wall[0] && BALL[i].y <= y_wall[0] + 100) // 如果小球碰到挡板
			{
				BALL[i].Vx = -BALL[i].Vx; // 小球速度改变
				BALL[i].x = x_wall[0] - Radius;
			}
			if (BALL[i].x - Radius <= x_wall[1] + 23 && BALL[i].x - Radius >= x_wall[1] && BALL[i].y >= y_wall[1] && BALL[i].y <= y_wall[1] + 265) // 如果小球碰到挡板
			{
				BALL[i].Vx = -BALL[i].Vx; // 小球速度改变
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
		x_barrier = x_barrier + Vx_barrier; // 障碍物运动
		y_barrier = y_barrier + Vy_barrier; // 障碍物运动
		if (y_barrier > 350) // 障碍物到达底部
		{
			y_barrier = 350;
			Vx_barrier = 3; // 速度改变
			Vy_barrier = 0;
		}
		if (x_barrier > 300) // 障碍物到达右侧
		{
			x_barrier = 300;
			Vx_barrier = 0; // 速度改变
			Vy_barrier = -3;
		}
		if (y_barrier < 200) // 障碍物到达顶部
		{
			y_barrier = 200;
			Vx_barrier = -3; // 速度改变
			Vy_barrier = 0;
		}
		if (x_barrier < 195) // 障碍物到达左侧
		{
			x_barrier = 195;
			Vx_barrier = 0; // 速度改变
			Vy_barrier = 3;
		}
		for (i = 1; i <= Numsmallball; i++)//碎裂的小球运动
		{
			Smallball[i].smallballVy = Smallball[i].smallballVy + Smallball[i].smallballgravity; // 小球y方向速度变化
			if (Smallball[i].smallballVy > 8) // 小球y方向速度上限为8
				Smallball[i].smallballVy = 8;
			Smallball[i].smallbally = Smallball[i].smallbally + Smallball[i].smallballVy; // 小球y坐标变化
			Smallball[i].smallballx = Smallball[i].smallballx + Smallball[i].smallballVx; // 小球x坐标变化
		}
		int w;
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].y + Radius >= y_barrier && BALL[i].y - Radius <= y_barrier + 60 && BALL[i].x + Radius >= x_barrier && BALL[i].x - Radius <= x_barrier + 64) // 如果小球碰到障碍物
			{
				w = rand() % 5 + 5; // 小球个数随机
				Numsmallball = Numsmallball + w; // 产生碎裂的小球
				for (j = Numsmallball - w + 1; j <= Numsmallball; j++)
				{
					Smallball[j].smallballVx = rand() % 5 - 2; // 小球水平方向速度
					Smallball[j].smallballVy = rand() % 5 - 2; // 小球竖直方向速度
					Smallball[j].smallballx = BALL[i].x; // 小球圆心x坐标
					Smallball[j].smallbally = BALL[i].y; // 小球圆心y坐标
					Smallball[j].smallballgravity = 0.1; // 重力
					Smallball[j].smallballradius = rand() % 4 + 2; // 随机半径
					float h = rand() % 360; // 小球随机颜色
					Smallball[j].color = HSVtoRGB(h, 0.6, 0.8);
				}
				BALL[i].y = 800;//使小球掉出框外
				BALL[i].x = 0;
				BALL[i].Vx = BALL[i].Vy = BALL[i].gravity = 0;
			}
		}
	}
	if (Level == 7)
	{
		x_barrier = x_barrier + Vx_barrier; // 障碍物运动
		if (x_barrier > 300) // 障碍物到达右侧
		{
			Vx_barrier = -Vx_barrier; // 速度改变
		}
		if (x_barrier < 100) // 障碍物到达左侧
		{
			Vx_barrier = -Vx_barrier; // 速度改变
		}
		for (i = 1; i <= Numsmallball; i++)
		{
			Smallball[i].smallballVy = Smallball[i].smallballVy + Smallball[i].smallballgravity; // 小球y方向速度变化
			if (Smallball[i].smallballVy > 8) // 小球y方向速度上限为8
				Smallball[i].smallballVy = 8;
			Smallball[i].smallbally = Smallball[i].smallbally + Smallball[i].smallballVy; // 小球y坐标变化
			Smallball[i].smallballx = Smallball[i].smallballx + Smallball[i].smallballVx; // 小球x坐标变化
		}
		int w;
		for (i = 1; i <= Launchedball; i++)
		{
			if (BALL[i].y + Radius >= y_barrier && BALL[i].y - Radius <= y_barrier + 60 && BALL[i].x + Radius >= x_barrier && BALL[i].x - Radius <= x_barrier + 64) // 如果小球碰到障碍物
			{
				w = rand() % 5 + 5; // 小球个数随机
				Numsmallball = Numsmallball + w; // 产生碎裂的小球
				for (j = Numsmallball - w + 1; j <= Numsmallball; j++)
				{
					Smallball[j].smallballVx = rand() % 5 - 2; // 小球水平方向速度
					Smallball[j].smallballVy = rand() % 5 - 2; // 小球竖直方向速度
					Smallball[j].smallballx = BALL[i].x; // 小球圆心x坐标
					Smallball[j].smallbally = BALL[i].y; // 小球圆心y坐标
					Smallball[j].smallballgravity = 0.1; // 重力
					Smallball[j].smallballradius = rand() % 4 + 2; // 随机半径
					float h = rand() % 360; // 小球随机颜色
					Smallball[j].color = HSVtoRGB(h, 0.6, 0.8);
				}
				BALL[i].y = 800;//使小球掉出框外
				BALL[i].x = 0;
				BALL[i].Vx = BALL[i].Vy = BALL[i].gravity = 0;
			}
		}
	}
	if (Level == 8)
	{
		x_launcher = x_launcher + Vx_launcher; // 发射器运动
		if (x_launcher >= 300) // 发射器往返运动
			Vx_launcher = -Vx_launcher;
		if (x_launcher <= 100)
			Vx_launcher = -Vx_launcher;
	}
	get_distance(BALL);//小球的碰撞
	knock(BALL);
}

void updateWithoutInput() // 与输入无关的更新函数
{
	move();
	MyTimer:Sleep(8);
	int t = 0, q = 0;
	for (i = 1; i <= Launchedball; i++) // 遍历小球
	{
		if (BALL[i].y - Radius >= HEIGHT - 250 && BALL[i].x - Radius >= WIDTH / 2 - 92 && BALL[i].x + Radius <= WIDTH / 2 + 92) // 小球在框中
			t++;
		if (BALL[i].y >= HEIGHT || BALL[i].y <= 0 || BALL[i].x >= WIDTH || BALL[i].x <= 0) // 小球在画面外
			q++;
		if (BALL[i].y - Radius >= HEIGHT)//小球掉出框外
		{
			BALL[i].y = 800;
			BALL[i].x = 0;
			BALL[i].Vx = BALL[i].Vy = BALL[i].gravity = 0;
		}
	}
	Ballinbox = t; // 计算框中小球个数
	Loseball = q; // 计算损失小球个数
}

void updateWithInput()  // 和输入有关的更新函数
{
	char input;
	if (kbhit())  // 判断是否有输入
	{
		input = getch();  // 根据用户的不同输入来判断，不必输入回车
		if (input == 27) // 输入ESC键
		{
			gameStatus = 3; // 暂停游戏
		}
	}
	if (MouseHit())// 如果有鼠标信息
	{
		m = GetMouseMsg(); // 获取鼠标信息
		if (gameStatus == 4) // 如果在游戏中
		{
			if (isfinished()) // 判断游戏是否结束
				return; // 如果结束，则函数返回
			else if (Leftball > 0 && m.uMsg == WM_LBUTTONDOWN) // 如果剩余小球大于0且按键为鼠标左键
			{
				Leftball--; // 剩余的小球减少
				Launchedball++; // 已经发射的小球增加
				if (Level == 8)
				{
					BALL[Launchedball].gravity = 0.05; // 重力加速度
					BALL[Launchedball].Vy = 2; // 竖直方向速度
					BALL[Launchedball].Vx = Vx_launcher / 4; // 水平方向速度
					BALL[Launchedball].x = x_launcher + Width_launcher / 2; // 小球发射时x坐标
					BALL[Launchedball].y = y_launcher + Height_launcher; // 小球发射时y坐标
					float h = rand() % 360; // 小球随机颜色
					BALL[Launchedball].color = HSVtoRGB(h, 0.6, 0.8);
					BALL[Launchedball].distance[0] = 99999999;
					BALL[Launchedball].distance[1] = -1;
				}
				PlayMusicOnce(_T("lmusic.mp3")); // 播放一次发射小球的音效
			}
		}
	}
}

int main() // 主函数
{
	startup();  // 初始化函数，仅执行一次	
	while (1) // 一直循环
	{
		show();  // 进行绘制
		updateWithoutInput(); // 和输入无关的更新 
		updateWithInput();  // 和输入有关的更新
	}
	return 0;
}