//作者：https://www.luogu.com.cn/user/223412
//改：https://www.luogu.com.cn/user/377751
/*
源自generals.io
大洋彼岸的服务器体验极差！

使用说明
整合了零人、单人、双人版

单人版&&通用规则
图片：https://www.picb.cc/image/tAJbks和https://www.picb.cc/image/tAJ7Nj
先输入0/1/2，代表游戏人数
然后要输入一个随机数种子（更新：长度可以任意），相同种子（相同编译器？）生成相同地图
地图：X为障碍，黑色背景的数为原有城市，红色为玩家的，蓝色为电脑的，城市和普通地面颜色不同
数字表示：数大于99时只显示最高位和一个字母 （e代表100，k代表1000，w代表10000），数大于99999就显示"ju" （巨）
在屏幕左下角可以看到光标所指的数的精确值（hp）
光标是一个小于号"<"，用方向键控制光标
按下w/s/a/d时，光标的左边那个数（前提是自己的）会随光标移动
按下z，再按w/s/a/d，光标的左边那个数（前提是自己的）的一半会随光标移动
无法移动的情况：遇到障碍/下一个位置上的数不是自己的，且>=自己的数-1
时间以回合（count）为单位，在屏幕左下角
移动光标不算回合，移动数字算，按到空格键视作放弃一回合
城市每回合+2，其余被占领的地面每10回合+1
屏幕左下角的"army"指一方数字和，"land"指一方占领的格子数

零人版
两个一样的bfs算法对打
只要观战就可以啦
可以按方向键移动光标，但不能移动任何数
按下空格键进入下一回合
如果想快点看到结果，按f键，之后无法移动光标

双人版
红方键盘：(z/x+) w/s/a/d，默认只移动光标，按z移一半数，按x移全部
蓝方键盘：(n/m+) 方向键，默认只移动光标，按n移一半数，按m移全部
任何一方按空格可以放弃一回合
游戏时间会很漫长
地图有概率出现“封城”，城市也可能分布不公平，换一个种子重启（继续碰运气）即可
当一方操作结束，光标会移到地图的左上角或右下角，（希望）能方便操作

结束条件：占领所有主塔（最开始的塔再【1】【1】或【n】【n]）
*/
#include<iostream>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<deque>
#include<vector>
#include<queue>
#include<string>
#include<cstring>
#include<map>
#include<stack>
#include<set>
#include<windows.h>
#include<conio.h>
#include<ctime>
using namespace std;

const int cA = 5; //red城市
const int cB = 3; //blue城市
const int ca = 4; //red地面
const int cb = 1; //blue地面
const int cC = 0; //black原有城市
const int msize = 25; //地图大小
const int mount = 16; //障碍数量
const int grey = 36; //城市数量
const int pmain = 1; //城市hp每回合增加量
const int lcnt = 25; //地面hp增加1所需回合

char cMap[msize][msize];//大写字母表示城市，小写表示地面，Aa是红，Bb是蓝，C是白，X是障碍
int hp[msize][msize];
int nCount, sx, sy; //回合累加，和光标的坐标
bool bVis[msize][msize];//bfs
int dir[4][2] = {1, 0, -1, 0, 0, 1, 0, -1}; //bfs

struct dRet { //决策信息：位置为(x,y)的数移到(x+dx,y+dy)
	int x;
	int y;
	int dx;
	int dy;
};

struct node { //bfs
	int x;
	int y;
	int step;
};

void vInit();//生成地图
void vMove(int dx, int dy); //移动光标
//以下5个函数，保证A=='A'||A=='B'
void vMoveNum(int dx, int dy, char A);
void vMoveNum(int x, int y, int dx, int dy, char A);
void vDiv(int dx, int dy, char A);
void vDecide(char A);
dRet bfs(int x, int y, char A);
void vPlus();//每回合数值增加，计算army和land
void vChange(int x, int y); //刷新屏幕上指定坐标的数
void gotoxy(int x, int y); //移动输出的位置
void color(int t, int b); //设置输出颜色，t为文字色，b为背景色
void vMain0();
void vMain1();
void vMain2();
void vEnd();//判断游戏结束

int main() {
	while (1) {
		system("cls");
		int p;
		cout << "player:" << endl;
		cin >> p;
		system("cls");
		if (p == 0) vMain0();
		if (p == 1) vMain1();
		if (p == 2) vMain2();
	}
	return 0;
}

void vMain0() {
	int in;
	bool cur = true;
	vInit();
	for (nCount = 0;; nCount++) {
		gotoxy(msize, 0);
		color(15, 0);
		cout << "count=" << nCount << endl;
		if (cur) {
			do {
				in = getch();
				if (in == 224) {
					in = getch();
					if (in == 72) vMove(-1, 0);
					if (in == 80) vMove(1, 0);
					if (in == 75) vMove(0, -1);
					if (in == 77) vMove(0, 1);
				}
			} while (in != ' ' && in != 'f');
			if (in == 'f') cur = false;
		}
//        Sleep(20);
		vDecide('A');
		vDecide('B');
		vPlus();
	}
}

void vMain1() {
	int in;
	vInit();
	for (nCount = 0;; nCount++) {
		gotoxy(msize, 0);
		color(15, 0);
		cout << "count=" << nCount << endl;
		do {
			in = getch();
			if (in == 224) {
				in = getch();
				if (in == 72) vMove(-1, 0);
				if (in == 80) vMove(1, 0);
				if (in == 75) vMove(0, -1);
				if (in == 77) vMove(0, 1);
			}
		} while (in != 'w' && in != 's' && in != 'a' && in != 'd' && in != 'z' && in != ' ');
		if (in == 'w') vMoveNum(-1, 0, 'A');
		if (in == 's') vMoveNum(1, 0, 'A');
		if (in == 'a') vMoveNum(0, -1, 'A');
		if (in == 'd') vMoveNum(0, 1, 'A');
		if (in == 'z') {
			in = getch();
			if (in == 'w') vDiv(-1, 0, 'A');
			if (in == 's') vDiv(1, 0, 'A');
			if (in == 'a') vDiv(0, -1, 'A');
			if (in == 'd') vDiv(0, 1, 'A');
		}
		vDecide('B');
		vPlus();
	}
}

void vMain2() {
	int in;
	vInit();
	for (nCount = 0;; nCount++) {
		gotoxy(msize, 0);
		color(15, 0);
		cout << "count=" << nCount;
		vMove(-sx, -sy);
		gotoxy(msize + 1, 0);
		cout << "red ";
		do {
			in = getch();
			if (in == 'w') vMove(-1, 0);
			if (in == 's') vMove(1, 0);
			if (in == 'a') vMove(0, -1);
			if (in == 'd') vMove(0, 1);
		} while (in != 'x' && in != 'z' && in != ' ');
		if (in == 'x') {
			in = getch();
			if (in == 'w') vMoveNum(-1, 0, 'A');
			if (in == 's') vMoveNum(1, 0, 'A');
			if (in == 'a') vMoveNum(0, -1, 'A');
			if (in == 'd') vMoveNum(0, 1, 'A');
		} else if (in == 'z') {
			in = getch();
			if (in == 'w') vDiv(-1, 0, 'A');
			if (in == 's') vDiv(1, 0, 'A');
			if (in == 'a') vDiv(0, -1, 'A');
			if (in == 'd') vDiv(0, 1, 'A');
		}
		vEnd();
		////////////////////////////////
		vMove(msize - 1 - sx, msize - 1 - sy);
		gotoxy(msize + 1, 0);
		color(15, 0);
		cout << "blue";
		do {
			in = getch();
			if (in == 224) {
				in = getch();
				if (in == 72) vMove(-1, 0);
				if (in == 80) vMove(1, 0);
				if (in == 75) vMove(0, -1);
				if (in == 77) vMove(0, 1);
			}
		} while (in != 'n' && in != 'm' && in != ' ');
		if (in == 'm') {
			in = getch();
			if (in == 224) {
				in = getch();
				if (in == 72) vMoveNum(-1, 0, 'B');
				if (in == 80) vMoveNum(1, 0, 'B');
				if (in == 75) vMoveNum(0, -1, 'B');
				if (in == 77) vMoveNum(0, 1, 'B');
			}
		} else if (in == 'n') {
			in = getch();
			if (in == 224) {
				in = getch();
				if (in == 72) vDiv(-1, 0, 'B');
				if (in == 80) vDiv(1, 0, 'B');
				if (in == 75) vDiv(0, -1, 'B');
				if (in == 77) vDiv(0, 1, 'B');
			}
		}
		vPlus();
	}
}

void vInit() {
	int i, j, x, y;
	string sSeed;
	unsigned int sd = 20190622;
	cout << "seed:" << endl;
	cin >> sSeed;
	system("cls");
	for (i = 0; i < sSeed.size(); i++) sd = sd * 233 + sSeed[i];
	srand(sd);
	cMap[0][0] = 'A';
	hp[0][0] = pmain;
	vChange(0, 0);
	cMap[msize - 1][msize - 1] = 'B';
	hp[msize - 1][msize - 1] = pmain;
	vChange(msize - 1, msize - 1);
	for (i = 1; i <= mount; i++) {
		x = rand() % msize;
		y = rand() % msize;
		if (cMap[x][y] != 0) i--;
		else {
			cMap[x][y] = 'X';
			gotoxy(x, y * 3);
			color(15, 0);
			cout << " X";
		}
	}
	for (i = 1; i <= grey; i++) {
		x = rand() % msize;
		y = rand() % msize;
		if (cMap[x][y] != 0) i--;
		else {
			cMap[x][y] = 'C';
			hp[x][y] = 40 + rand() % 10;
			vChange(x, y);
		}
	}
	sx = sy = 0;
	gotoxy(0, 2);
	color(15, 0);
	cout << "<";
}

void vMove(int dx, int dy) {
	color(15, 0);
	dx += sx;
	dy += sy;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize)) return;
	gotoxy(sx, sy * 3 + 2);
	cout << " ";
	sx = dx;
	sy = dy;
	gotoxy(sx, sy * 3 + 2);
	cout << "<";
	gotoxy(msize + 1, 0);
	cout << "cMap=" << cMap[dx][dy] << ",hp=" << hp[dx][dy] << "            ";
}

void vMoveNum(int x, int y, int dx, int dy, char A) {
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	if (cMap[x][y] != a && cMap[x][y] != A) return;
	dx += x;
	dy += y;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize && cMap[dx][dy] != 'X')) return;
	char &cd = cMap[dx][dy];
	int &hd = hp[dx][dy], &hs = hp[x][y];
	if (cd != a && cd != A && hd >= hs - 1) return;
	if (cd == 0) {
		cd = a;
		hd = hs - 1;
	} else if (cd == A || cd == a) {
		hd += hs - 1;
	} else if (cd == B || cd == b) {
		if (B == 'B') cd--;
		else cd++;
		hd = hs - 1 - hd;
	} else { //'C'
		cd = A;
		hd = hs - 1 - hd;
	}
	hs = 1;
	vChange(dx, dy);
	vChange(x, y);
}

void vMoveNum(int dx, int dy, char A) {
	vMoveNum(sx, sy, dx, dy, A);
	vMove(dx, dy);
}

void vDiv(int dx, int dy, char A) {
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	if (cMap[sx][sy] != a && cMap[sx][sy] != A) return;
	dx += sx;
	dy += sy;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize && cMap[dx][dy] != 'X')) return;
	char &cd = cMap[dx][dy];
	int &hd = hp[dx][dy], &hs = hp[sx][sy], tmp = hs / 2;
	if (tmp == 0) return;
	if (cd != a && cd != A && hd >= tmp - 1) return;
	if (cd == 0) {
		cd = a;
		hd = tmp;
	} else if (cd == A || cd == a) {
		hd += tmp;
	} else if (cd == B || cd == b) {
		if (B == 'B') cd--;
		else cd++;
		hd = tmp - hd;
	} else { //'C'
		cd = A;
		hd = tmp - hd;
	}
	hs -= tmp;
	vChange(dx, dy);
	vChange(sx, sy);
	vMove(dx - sx, dy - sy);
}

void vDecide(char A) {
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	int i, j, k, x, y, dx, dy;
	dRet tmp, res;
	bool bd = false, bm = false;
	for (i = (A == 'A') ? 0 : (msize - 1); (A == 'A') ? (i <= msize - 1) : (i >= 0); (A == 'A') ? (i++) : (i--)) {
		for (j = (A == 'A') ? 0 : (msize - 1); (A == 'A') ? (j <= msize - 1) : (j >= 0); (A == 'A') ? (j++) : (j--)) {
			if (!bd && (cMap[i][j] == B || cMap[i][j] == 'C')) {
				tmp = bfs(i, j, A);
				if (tmp.dx + tmp.dy == 0) continue;
				bd = true;
				res = tmp;
			}
			if (!bd && !bm && (cMap[i][j] == A || cMap[i][j] == a) && hp[i][j] > 1) {
				for (k = 0; k < 4; k++) {
					dx = dir[k][0] + i;
					dy = dir[k][1] + j;
					if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize)) continue;
					if ((cMap[dx][dy] == 0 || cMap[dx][dy] == b) && hp[dx][dy] < hp[i][j] - 1) {
						bm = true;
						res.dx = dir[k][0];
						res.dy = dir[k][1];
						res.x = i;
						res.y = j;
						break;
					}
				}
			}
			if (bd) break;
		}
		if (bd) break;
	}
	if (!bd && !bm) return;
	vMoveNum(res.x, res.y, res.dx, res.dy, A);
}

dRet bfs(int x, int y, char A) {
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	int i, dx, dy;
	dRet ret;
	node now, nxt;
	queue<node>q;
	now.step = hp[x][y] + 2;
	now.x = x;
	now.y = y;
	q.push(now);
	memset(bVis, false, sizeof bVis);
	bVis[x][y] = true;
	while (!q.empty()) {
		now = q.front();
		q.pop();
		for (i = 0; i < 4; i++) {
			dx = dir[i][0] + now.x;
			dy = dir[i][1] + now.y;
			if (dx >= 0 && dx < msize && dy >= 0 && dy < msize && !bVis[dx][dy] && cMap[dx][dy] != 'X') {
				bVis[dx][dy] = true;
				if (cMap[dx][dy] == 0) nxt.step = now.step + 1;
				if (cMap[dx][dy] == b || cMap[dx][dy] == B || cMap[dx][dy] == 'C') nxt.step = now.step + hp[dx][dy] + 2;
				else {
					nxt.step = now.step - hp[dx][dy] + 1;
					if (nxt.step < -5 && (cMap[dx][dy] == A || cMap[dx][dy] == a)) {
						ret.dx = -dir[i][0];
						ret.dy = -dir[i][1];
						ret.x = dx;
						ret.y = dy;
						return ret;
					}
				}
				nxt.x = dx;
				nxt.y = dy;
				q.push(nxt);
			}
		}
	}
	ret.dx = ret.dy = 0;
	return ret;
}

void vPlus() {
	int i, j, aa = 0, ba = 0, al = 0, bl = 0;
	if (nCount % lcnt == 0) {
		for (i = 0; i < msize; i++) {
			for (j = 0; j < msize; j++) {
				if (cMap[i][j] == 'a' || cMap[i][j] == 'b') {
					hp[i][j]++;
					vChange(i, j);
				}
			}
		}
	}
	for (i = 0; i < msize; i++) {
		for (j = 0; j < msize; j++) {
			if (cMap[i][j] == 'A' || cMap[i][j] == 'B') {
				hp[i][j] += pmain;
				vChange(i, j);
			}
			if (cMap[i][j] == 'A' || cMap[i][j] == 'a') al++, aa += hp[i][j];
			if (cMap[i][j] == 'B' || cMap[i][j] == 'b') bl++, ba += hp[i][j];
		}
	}
	color(15, 0);
	gotoxy(msize + 2, 0);
	cout << "red army:" << aa << "   " << endl;
	cout << "red land:" << al << "   " << endl;
	cout << "blue army:" << ba << "   " << endl;
	cout << "blue land:" << bl << "   " << endl;
	vEnd();
}

void vEnd() {

	if (cMap[0][0] == 'B') {
		cout << "blue win" << endl;
		while (1);
	}
	if (cMap[msize - 1][msize - 1] == 'A') {
		cout << "red win" << endl;
		while (1);
	}
}

void vChange(int x, int y) {
	int c;
	char cm = cMap[x][y];
	if (cm == 'A') c = cA;
	if (cm == 'a') c = ca;
	if (cm == 'B') c = cB;
	if (cm == 'b') c = cb;
	if (cm == 'C') c = cC;
	gotoxy(x, y * 3);
	color(15, c);
	int val = hp[x][y];
	if (val < 10) cout << " " << val;
	else if (val < 100) cout << val;
	else if (val < 1000) cout << val / 100 << "e";
	else if (val < 10000) cout << val / 1000 << "k";
	else if (val < 100000) cout << val / 10000 << "w";
	else cout << "ju";
}

void gotoxy(int x, int y) {
	COORD c;
	c.X = y;
	c.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void color(int t, int b) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), t + b * 16);
}
