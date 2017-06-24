#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
//MAXWIDTH��MAXHEIGHT��INITLEN ���ַ���
#define Width (30)
#define Length Width			
#define INITLEN (3)                         //̰���ߵĳ�ʼ���� 


struct
{
	char *ch;
	int color;
	char type;
}
Border = { "��", 15, 1 },				//�߿�
Background = { "��", 0, 2 },			//����
Snake = { "��", 11, 3 },				//̰���߽ڵ�
Food = { "��", 12, 4 },					//ʳ��								
Food_spec = { "��",12, 5 },				//Ϊ��ʵ��̰����ÿ������food�����ĸ�food������ӷֵĹ��ܣ���Ҫ�������ʳ��
Head = { "��",11, 6 };
int Gx, Gy;//
int TIMES;
int foodSpeed = 4;
int snakespeed = 200;
//��һ���ṹ�����鱣���ͼ�еĸ�����
struct
{
	char type;
	int index;
}WholeMap[Width][Length];
//WholeMap�Ƕ�Ӧ������ͼ�����ά���飬��type��ȷ����ͼ�˴���ʲô������ʳ��or��or����orǽ����index��SnakeMapһά������±�

//̰������Ч���Χ��ͼ������
struct {
	int x;
	int y;
} SnakeMap[(Width - 2)*(Length - 2)], ApplePosition;
// SnakeMap��̰���ߺ�ƻ�����һά���飬x��y����WholeMap��ά������±�

//�����������໥����������ȷ����ͼ�δ�Ϊ��������ںδ�

int scores = 0;										//�÷�Ҳ���ǳ�ƻ��������
int snakeMapLength = (Width - 2)*(Length - 2);			//snakemaplength��snakemapһά����ĳ���
int headerIndex, tailIndex;									//��ͷ��β��Ӧ��snakeMap�е��������±꣩
int eat_count = 0;                                            //������¼̰���߳Ե�ƻ��������
int snakespeed;
HANDLE Handle;											//����̨���
														// ���ù��λ�ã�xΪ�У�yΪ��
void setPosition(int x, int y)
{
	COORD coord;											//COORD�ṹ��X��Y����ʾ�����෴��
	coord.X = 2 * y;											//��2����Ϊ���崰�ڴ�С��cols��Ҫ���Գ����ģ�
	coord.Y = x;
	SetConsoleCursorPosition(Handle, coord);
}

// ������ɫ
void setColor(int color)
{
	SetConsoleTextAttribute(Handle, color);
}



//����ʳ��
void createFood()
{
	int index, rang, x, y;
	//�����������ȷ�� snakeMap ��������� 
	srand((unsigned)time(NULL));					//�Ե�ǰʱ��timeΪ���������������
	if (tailIndex<headerIndex)
	{													//����ͷ�±������β�±꣬����ʳ��������
		rang = headerIndex - tailIndex - 1;
		index = rand() % rang + tailIndex + 1;
	}
	else													//����ͷ�±�С����β�±꣬����ʳ��������
	{
		rang = snakeMapLength - (tailIndex - headerIndex + 1);
		index = rand() % rang;
		if (index >= headerIndex) {
			index += (tailIndex - headerIndex + 1);
		}
	}
	eat_count++;
	x = SnakeMap[index].x;
	y = SnakeMap[index].y;
	Gx = x;
	Gy = y;
	setPosition(x, y);
	if (eat_count % 4 != 0 && eat_count != 0)
	{
		setColor(Food.color);
		printf("%s", Food.ch);
	}
	else
	{
		int x1, y1;
		index = 0;
		while (WholeMap[SnakeMap[index].x][SnakeMap[index].y].type != 2) {
			if (tailIndex<headerIndex)
			{													//����ͷ�±������β�±꣬����ʳ��������
				rang = headerIndex - tailIndex - 1;
				index = rand() % rang + tailIndex + 1;
			}
			else													//����ͷ�±�С����β�±꣬����ʳ��������
			{
				rang = snakeMapLength - (tailIndex - headerIndex + 1);
				index = rand() % rang;
				if (index >= headerIndex) {
					index += (tailIndex - headerIndex + 1);
				}
			}
		}
		x1 = SnakeMap[index].x;//����ͷ������
		y1 = SnakeMap[index].y;
		setPosition(x1, y1);
		setColor(Food_spec.color);
		printf("%s", Food_spec.ch);
		WholeMap[x1][y1].type = Food_spec.type;
	}
	WholeMap[x][y].type = Food.type;
}

void moveFood()
{
	srand((unsigned)time(NULL));
	bool flag = true;
	int cx, cy;
	int x, y;
	int count = 0, maxCount = 1000;
	while (flag)
	{
		int dir = rand() % 4 + 1;
		if (dir % 2 == 0) {
			cx = 0;
			cy = dir - 3;
		}
		else {
			cx = dir - 2;
			cy = 0;
		}
		if ((Gx + cx)>1 && (Gx + cx)<(Width - 2) && (Gy + cy)>1 && (Gy + cy)<(Length - 2)) {
			if (WholeMap[Gx + cx][Gy + cy].type != 2) {
				continue;
			}
			flag = false;
		}
		count++;
		if (count > maxCount) {
			cx = 0;
			cy = 0;
			break;
		}
	}

	//Ĩȥԭ����ʳ��

	setPosition(Gx, Gy);
	setColor(Background.color);
	printf("%s", Background.ch);
	WholeMap[Gx][Gy].type = Background.type;
	//��ʾ�µ�ʳ��
	Gx += cx;
	Gy += cy;
	setPosition(Gx, Gy);
	setColor(Food.color);
	printf("%s", Food.ch);
	WholeMap[Gx][Gy].type = Food.type;
}

//����
void die()
{
	int xCenter = Length % 2 == 0 ? Length / 2 : Length / 2 + 1;
	//xcenter��ycenter���Ǵ��ڵ��������꣬����������ʾ��Ϸ��������
	int yCenter = Width % 2 == 0 ? Width / 2 : Width / 2 + 1;

	setPosition(xCenter, yCenter - 5);
	setColor(0xC);

	printf("You lose! Game Over!");
	_getch();
	system("cls");
	setColor(15);
	scores = 0;
	eat_count = 0;
	main();
}

int start()
{
	int a, b = 30;
	getchar();
	printf(" \n\n\n\n\n\n\n\n\n\n                                    ̰ �� ��\n\n\n\n");
	printf("                     ��ѡ��������Ϸ�Ѷ�: 1�� ��   2�� ��ͨ   3�� ����\n");
	printf("                ");
	scanf("%d", &a);
	if (a == 1)
		b = 250;
	else if (a == 2)
		b = 150;
	else if (a == 3)
		b = 80;
	else
	{
		printf("                 ���ﰡ����1��2��3��\n");
		printf("                 ��Ȼ���������Ǿͽ������̬�Ѷ�");
		_getch();
	}
	eat_count = 0;
	return b;
}
// ���ƶ�
void move(char direction)
{
	int newHeaderX, newHeaderY;					//����ͷ������
	int Storevalue;							//����ͷ������ǰ��Ӧ������
	int StorevalueX, StorevalueY;				//����ͷ��������ǰ��Ӧ������
	int newHeaderPreType;							//����ͷ��ǰ������
	int oldTailX, oldTailY;							//����β����
	int  m, n;										// -----------------------------------------------
	m = SnakeMap[headerIndex].x;//����ͷ������
	n = SnakeMap[headerIndex].y;
	switch (direction) {
	case 'w':
		newHeaderX = SnakeMap[headerIndex].x - 1;
		newHeaderY = SnakeMap[headerIndex].y;
		break;
	case 's':
		newHeaderX = SnakeMap[headerIndex].x + 1;
		newHeaderY = SnakeMap[headerIndex].y;
		break;
	case 'a':
		newHeaderX = SnakeMap[headerIndex].x;
		newHeaderY = SnakeMap[headerIndex].y - 1;
		break;
	case 'd':
		newHeaderX = SnakeMap[headerIndex].x;
		newHeaderY = SnakeMap[headerIndex].y + 1;
		break;
	default:
		printf("%c\n", direction);
		exit(0);
		break;
	}
	//����ͷ������
	//newheaderX��newheaderY����ͷ��Ҫ����λ����globalmap��ά���������
	//	headerIndex = headerIndex == 0 ? snakeMapLen - 1 : headerIndex - 1;
	if (headerIndex == 0)
		headerIndex = snakeMapLength;
	else
		headerIndex = headerIndex - 1;
	//̰����Ĭ����snakemap���һά������ǰ������νǰ������ÿ���±��1
	// -----------------------------------------------
	//����ͷ������ǰ��Ӧ������
	Storevalue = WholeMap[newHeaderX][newHeaderY].index;
	//Storevalue������ͷ��Ҫ����λ����snakemapһά�����е��±�
	//����ͷ��������ǰ��Ӧ������
	StorevalueX = SnakeMap[headerIndex].x;
	StorevalueY = SnakeMap[headerIndex].y;
	//StorevalueX��Y������ͷԤ��Ҫ����λ����wholemap��ά�����е��±�

	//˫�������ͷ����������Ķ�Ӧ��ϵ
	SnakeMap[headerIndex].x = newHeaderX;
	SnakeMap[headerIndex].y = newHeaderY;
	//����ͷ��Ҫ����λ�õ����긳ֵ����ͷԤ�Ƴ���λ�õ�����
	WholeMap[newHeaderX][newHeaderY].index = headerIndex;
	//����ͷԤ�Ƴ���λ�õ��±긳ֵ����ͷ��Ҫ����λ�õ��±�

	//˫�����ǰ������������Ķ�Ӧ��ϵ
	SnakeMap[Storevalue].x = StorevalueX;
	SnakeMap[Storevalue].y = StorevalueY;
	//����ͷԤ�Ƴ���λ�õ����긳ֵ����ͷ��Ҫ����λ�õ�����
	WholeMap[StorevalueX][StorevalueY].index = Storevalue;
	//����ͷ��Ҫ����λ�õ��±긳ֵ����ͷԤ�Ƴ���λ�õ��±�

	//����ͷ��ǰ������
	newHeaderPreType = WholeMap[newHeaderX][newHeaderY].type;
	//����ͷ��Ҫ����λ�����ͼ�����������ȡ��
	//��������ͷ����
	WholeMap[newHeaderX][newHeaderY].type = Snake.type;
	//����ͷ��Ҫ����λ�����ͼ���������������Ϊ�ߵ�ͼ������
	// �ж��Ƿ�����ײ���Լ�
	if (newHeaderPreType == Border.type || newHeaderPreType == Snake.type)
	{
		die();
	}
	//�������ͷ
	setPosition(newHeaderX, newHeaderY);
	setColor(Snake.color);
	printf("%s", Head.ch);
	setPosition(m, n);
	setColor(Snake.color);
	printf("%s", Snake.ch);
	//�ж��Ƿ�Ե�ʳ�����Ե�ʳ��Ͳ�ɾ����β���������������β�ĳ���.
	if (newHeaderPreType == Food.type)
	{ 	 //�Ե�ʳ��
		createFood();
		//���ķ���
		setColor(15);
		setPosition(ApplePosition.x, ApplePosition.y);
		printf("%d", ++scores);
	}
	else if (newHeaderPreType == Food_spec.type) {
		scores += 2;
		setColor(15);
		setPosition(ApplePosition.x, ApplePosition.y);
		printf("%d", scores);
	}
	else
	{
		///NEW
		TIMES += 1;
		if (TIMES % foodSpeed == 0)
		{
			moveFood();
		}
		//����β����
		oldTailX = SnakeMap[tailIndex].x;
		oldTailY = SnakeMap[tailIndex].y;
		//ɾ����β
		setPosition(oldTailX, oldTailY);
		setColor(Background.color);
		printf("%s", Background.ch);
		WholeMap[oldTailX][oldTailY].type = Background.type;
		//		tailIndex = (tailIndex == 0) ? snakeMapLength - 1 : tailIndex - 1;
		if (tailIndex == 0)
			tailIndex = snakeMapLength;
		else
			tailIndex = tailIndex - 1;
	}
}

//�´��ƶ��ķ���
char nextDirection(char ch, char directionOld)
{
	int sum = ch + directionOld;
	ch = tolower(ch);
	if ((ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') && sum != 197 && sum != 234)
		//197=a+d��234=w+s�������ķ���͵�ǰ�˶������෴
	{
		return ch;
	}
	else
	{
		return directionOld;
	}
}

//��ͣ
char pause()
{
	return _getch();
}

// ��ʼ��
void init() {
	// ������ر��� 
	int x, y, i, index;
	int xCenter = Length % 2 == 0 ? Length / 2 : Length / 2 + 1;
	int yCenter = Width % 2 == 0 ? Width / 2 : Width / 2 + 1;
	//xcenter��ycenter�Ǵ��ڵ���������
	CONSOLE_CURSOR_INFO cci;
	//����̨�����Ϣ

	//�ж���������Ƿ����
	if (Width<16)
	{
		printf("'MAXWIDTH' is too small!");
		_getch();
		exit(0);
	}
	//���ô��ڴ�С 
	system("mode con: cols=106 lines=32");
	//����cols=106��Ӧsetposition������COORD.x=2*y

	//���ع��
	Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(Handle, &cci);
	cci.bVisible = 0;
	SetConsoleCursorInfo(Handle, &cci);

	//��ӡ���� 
	for (x = 0; x<Length; x++)
	{
		for (y = 0; y<Width; y++)
		{
			if (y == 0 || y == Width - 1 || x == 0 || x == Length - 1)
			{
				WholeMap[x][y].type = Border.type;								 //�������ͼƬ������
				setColor(Border.color);
				printf("%s", Border.ch);
			}
			else
			{
				index = (x - 1)*(Width - 2) + (y - 1);
				//index���ɶ�ά����wholemap��x��y�������һά����snakemap���±꣬��0��ʼ
				SnakeMap[index].x = x;
				SnakeMap[index].y = y;
				WholeMap[x][y].type = Background.type;									//�������ͼƬ������
				WholeMap[x][y].index = index;
				//����wholemap�ĳ�Աindex��ֵ��indexҲ����һά����snakemap���±�

				setColor(Background.color);
				printf("%s", Background.ch);
			}
		}
		printf("\n");
	}

	//��ʼ��̰����
	WholeMap[xCenter][yCenter - 1].type = WholeMap[xCenter][yCenter].type = WholeMap[xCenter][yCenter + 1].type = Snake.type;
	//����̰�������ڵ��������

	headerIndex = (xCenter - 1)*(Width - 2) + (yCenter - 1) - 1;
	//������ͷ��snakemap���һά�����е��±�
	tailIndex = headerIndex + 2;
	//�߳�Ĭ��Ϊ3������tailIndex=headerIndex+2
	setPosition(xCenter, yCenter - 1);
	setColor(Snake.color);
	printf("%s", Head.ch);
	for (y = yCenter; y <= yCenter + 1; y++)
	{
		printf("%s", Snake.ch);
	}
	//����ʳ��
	createFood();
	//���ó�����Ϣ
	setPosition(xCenter - 1, Width + 2);
	setColor(15);
	printf(" Apples : ");
	setPosition(xCenter, Width + 2);
	printf(" Author:������,���ղ�,�����,������");
	setPosition(xCenter + 1, Width + 2);
	ApplePosition.x = xCenter - 1;
	ApplePosition.y = Width + 8;
}

int main()
{
	int b;
	b = start();
	char charInput, direction = 'a';					           //Ĭ��̰�����ƶ��ķ���
	init();											  //���ó�ʼ������
	snakespeed = b;
	charInput = tolower(_getch());						  //tolower��������ַ�ת��ΪСд
	direction = nextDirection(charInput, direction);	           //nextdirection�����жϺ���
	int pre = eat_count;
	while (1)
	{
		if (_kbhit())
		{										//kbhit����Ƿ����û�����
			charInput = tolower(_getch());
			if (charInput == ' ')
			{									//����û�����ո񣬳�����ͣ					
				charInput = pause();
			}
			direction = nextDirection(charInput, direction);
		}
		move(direction);							//movḛ�����ƶ�����
		if (eat_count > pre) {
			snakespeed--;
			pre = eat_count;
		}
		Sleep(snakespeed);
	}

	_getch();
	return 0;
}