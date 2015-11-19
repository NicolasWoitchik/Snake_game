#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define WIDTH 70
#define HEIGHT 23

#define MOVE_CIMA 72
#define MOVE_BAIXO 80
#define MOVE_ESQUERDA 75
#define MOVE_DIREITA 77

enum Direcao {
	CIMA,
	BAIXO,
	ESQUERDA,
	DIREITA
};

typedef struct Cobra {
	
	int x, y;
	Direcao d;
	struct Cobra * proximo;

}Snake;

#define WALL_LEFT_RIGHT 186
#define WALL_TOP_BOTTOM 205
#define BODY_HEAD 206
#define BODY 'O'/* 219 = ? / 233 =  */
#define FOOD 254
#define SPACE ' '

void cursor(COORD coord, char c) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("%c", c);
}
void semente(void) { 
	srand(time(NULL)); 
	rand(); 
}

COORD comida(char tela[HEIGHT][WIDTH]) {
	int x, y;
	COORD cord;
	x = rand() % HEIGHT;
	y = rand() % WIDTH;
	if (tela[x][y] == SPACE) {
		cord.X = y;
		cord.Y = x;
		tela[x][y] == FOOD;
		cursor(cord, FOOD);
		return(cord);
	}
	else {
		comida(tela);
	}
}


void inicializa_tela(char tela[HEIGHT][WIDTH]) {
	int i, j;
	for (i = 0; i < HEIGHT; i += (HEIGHT - 1))
	{
		for (j = 0; j < WIDTH; j++)
		{
			tela[i][j] = WALL_TOP_BOTTOM;
		}
	}
	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 0; j < WIDTH; j += (WIDTH - 1))
		{
			tela[i][j] = WALL_LEFT_RIGHT;
		}
	}
	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			tela[i][j] = SPACE;
		}
	}
	tela[0][0] = 201;
	tela[HEIGHT-1][0] = 200;
	tela[0][WIDTH-1] = 187;
	tela[HEIGHT-1][WIDTH-1] = 188;
}
Snake* inicializa_snake(char tela[HEIGHT][WIDTH],Snake *corpo,int p) {
	Snake *novo =(Snake*)malloc(sizeof(Snake));
	if (!novo == NULL) {
		Snake *aux = NULL;
		Snake *ax;
		
		if (p == 2) {
			novo->proximo = corpo;
			corpo = novo;
			
		}else{
			for (ax = corpo; ax->proximo != NULL; ax = ax->proximo);
			novo->proximo = ax->proximo;
			ax->proximo = novo;
		}
		novo->d = ESQUERDA;
		novo->x = (WIDTH / 2) - p;
		novo->y = HEIGHT / 2;
	}
	if (p > 2 ) {

	}
	return corpo;
}

void desenha(char tela[HEIGHT][WIDTH]) {
	system("cls");
	int i, j;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			printf("%c", tela[i][j]);
		}
		printf("\n");
	}
}

void desenhasnake(char tela[HEIGHT][WIDTH], Snake* corpo) {	
	for (Snake *in = corpo; in->proximo != NULL; in = in->proximo) {
		tela[in->y][in->x] = BODY;
	}
}

Snake * novo_corpo(char tela[HEIGHT][WIDTH], Snake * c) {
	Snake *ax;
	Snake *novo = (Snake*)malloc(sizeof(Snake));
	if (!novo == NULL) {
		for (ax = c; ax->proximo != NULL; ax = ax->proximo);

		novo->proximo = ax->proximo;
		ax->proximo = novo;
		novo->d = ax->d;
		switch (ax->d)
		{
		case CIMA:
			novo->y = ax->y+1;
			novo->x = ax->x;
			break;
		case BAIXO:
			novo->y = ax->y-1;
			novo->x = ax->x;
			break;
		case ESQUERDA:
			novo->y = ax->y;
			novo->x = ax->x+1;
			break;
		case DIREITA:
			novo->y = ax->y;
			novo->x = ax->x-1;
			break;
		default:
			break;
		}
		
	}
	return c;
}

void finaliza_tela(char tela[HEIGHT][WIDTH],int pontos,Snake * c) {
	/*char nome[WIDTH];
	COORD cord;
	FILE * fp;
	int ch;
	fopen_s(&fp, "tela_fim.txt", "r");
	
	
	system("cls");
	while ((ch = fgetc(fp)) != EOF) {
		printf("%c", ch);
	}
	
	cord.X = 14;
	cord.Y = 15;
	cursor(cord, ' ');
	scanf_s("%s", &nome);
	free(c);
	exit(0);*/
}

void move_snake(char tela[HEIGHT][WIDTH], Snake* c,Direcao d,COORD *f) {	
	COORD cord;
	COORD food = *f;
	Snake *ax;
	
	switch (d)
	{
	case CIMA:
		if (c->y == food.Y && c->x == food.X) {
			*f = comida(tela);
			tela[c->y][c->x] = SPACE;
			cord.X = c->x;
			cord.Y = c->y;
			cursor(cord, SPACE);

			c->proximo = novo_corpo(tela, c->proximo);

		}
		if (tela[c->y - 1][c->x] == SPACE && tela[c->y - 1][c->x] != BODY && tela[c->y - 1][c->x] != FOOD) {
				tela[c->y][c->x] = SPACE;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, SPACE);
				c->d = CIMA;
				if (c->proximo != NULL) {
					move_snake(tela, c->proximo, c->proximo->d,&food);
					c->proximo->d = c->d;
				}
				c->y = c->y - 1;
				tela[c->y][c->x] = BODY;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, BODY);
		}
		else if (c->y != food.Y && c->x != food.X  && tela[c->y - 1][c->x] != BODY) {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x,c);
			
		}
		break;
	case BAIXO:
		if (c->y == food.Y && c->x == food.X) {
			*f = comida(tela);
			tela[c->y][c->x] = SPACE;
			cord.X = c->x;
			cord.Y = c->y;
			cursor(cord, SPACE);

			c->proximo = novo_corpo(tela, c->proximo);

		}
		if (tela[c->y+1][c->x] == SPACE && tela[c->y+1][c->x] != BODY && tela[c->y + 1][c->x] != FOOD) {
				tela[c->y][c->x] = SPACE;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, SPACE);
				c->d = BAIXO;
				if (c->proximo != NULL) {
					move_snake(tela, c->proximo, c->proximo->d,&food);
					c->proximo->d = c->d;
				}
				else {
				}
				c->y++;
				tela[c->y][c->x] = BODY;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, BODY);
		}
		else {
			if (c->y != food.Y && c->x != food.X && tela[c->y + 1][c->x] != BODY) {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x,c);
			}
		}
		
		break;
	case ESQUERDA:
		if (c->y == food.Y && c->x == food.X) {
			*f = comida(tela);
			tela[c->y][c->x] = SPACE;
			cord.X = c->x;
			cord.Y = c->y;
			cursor(cord, SPACE);
			c->proximo = novo_corpo(tela, c->proximo);
			
		}
		if (tela[c->y][c->x - 1] == SPACE && tela[c->y][c->x - 1] != BODY && tela[c->y][c->x - 1] != FOOD) {
				tela[c->y][c->x] = SPACE;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, SPACE);
				c->d = ESQUERDA;
				if (c->proximo != NULL) {
					move_snake(tela, c->proximo, c->proximo->d,&food);
					c->proximo->d = c->d;
				}
				c->x = c->x - 1;
				tela[c->y][c->x] = BODY;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, BODY);
		}
		else {
			if (c->y != food.Y && c->x != food.X  && tela[c->y][c->x - 1] != BODY) {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x,c);
			}
		}
		
		break;
	case DIREITA:
		if (c->y == food.Y && c->x == food.X) {
			*f = comida(tela);
			tela[c->y][c->x] = SPACE;
			cord.X = c->x;
			cord.Y = c->y;
			cursor(cord, SPACE);
			c->proximo = novo_corpo(tela, c->proximo);
		}
		if (tela[c->y][c->x+1] == SPACE && tela[c->y][c->x+1] != BODY) {
				tela[c->y][c->x] = SPACE;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, SPACE);
				c->d = DIREITA;
				if (c->proximo != NULL) {
					move_snake(tela, c->proximo, c->proximo->d,&food);
					c->proximo->d = c->d;
				}
				c->x = c->x + 1;
				tela[c->y][c->x] = BODY;
				cord.X = c->x;
				cord.Y = c->y;
				cursor(cord, BODY);
		}
		else {
			if (c->y != food.Y && c->x != food.X && tela[c->y][c->x + 1] != BODY) {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela,x,c);
			}
		}
		break;

	}
	
}

int placar(int x, Snake *c) {
	x = 1;	
	for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
		x++;
	}
	return(x-3);
}

int main() {
	setlocale(LC_ALL, "UTF-8");
	char tela[HEIGHT][WIDTH];
	int y;
	Snake *corp= NULL;
	
	corp = inicializa_snake(tela, corp, 2);
	corp = inicializa_snake(tela, corp, 1);
	corp = inicializa_snake(tela, corp, 0);
	desenhasnake(tela, corp);

	inicializa_tela(tela);
	
	boolean a=true;
	COORD coord,food;
	coord.Y = HEIGHT / 2;
	coord.X = WIDTH / 2;
	int x=0;

	desenha(tela);
	food = comida(tela);
	while (a) {
		if (_kbhit()) {
			char tecla = _getch(); fflush(stdin);
			switch (tecla)
			{
				
			case MOVE_CIMA:
				move_snake(tela, corp,CIMA, &food);

				break;

			case MOVE_BAIXO:
				move_snake(tela, corp,BAIXO, &food);
				break;

			case MOVE_ESQUERDA:
				move_snake(tela, corp, ESQUERDA, &food);

				break;

			case MOVE_DIREITA:
				move_snake(tela, corp,DIREITA, &food);
				break;

			case 's':
				cursor({ 0,HEIGHT },SPACE);
				system("pause");
				printf("seu jogo foi salvo, espero");
				break;

			case 'e':
				a = false;
				break;
			}
		}
		move_snake(tela, corp, corp->d, &food);
		cursor(food, FOOD);
		Sleep(70);
		x = placar(x,corp);
		cursor({ WIDTH/2-7 ,HEIGHT-1 },185);
		printf(" PLACAR : %d %c", x,204);

	}
	return(0);
}