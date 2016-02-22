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

typedef struct placar {
	int valor[2];
	char nome[WIDTH];
}Placar;

#define WALL_LEFT_RIGHT 186
#define WALL_TOP_BOTTOM 205
#define BODY_HEAD 206
#define BODY 'O'/* 219 = ? / 233 =  */
#define FOOD 254
#define SPACE ' '
void cursor(COORD coord, char c);
void semente(void);
COORD comida(char tela[HEIGHT][WIDTH]);
void inicializa_tela(char tela[HEIGHT][WIDTH]);
Snake* inicializa_snake(Snake *corpo, int p);
Snake* inicializa_snake_load(Snake *corpo, int p);
void desenha(char tela[HEIGHT][WIDTH]);
void desenhasnake(char tela[HEIGHT][WIDTH], Snake* corpo);
Snake * novo_corpo(char tela[HEIGHT][WIDTH], Snake * c);
void finaliza_tela(char tela[HEIGHT][WIDTH], int pontos, Snake * c);
void move_snake(char tela[HEIGHT][WIDTH], Snake* c, Direcao d, COORD *f);
void save_game(Snake * c, int pontos);
int placar(int x, Snake *c);
void load(Snake*c);
void menu(Snake*c);
void rank();

int main() {
	setlocale(LC_ALL, "UTF-8");
	char tela[HEIGHT][WIDTH];
	int y;
	Snake *corp= NULL;
	
	corp = inicializa_snake(corp, 2);
	corp = inicializa_snake(corp, 1);
	corp = inicializa_snake(corp, 0);
	
	desenhasnake(tela, corp);

	inicializa_tela(tela);
	
	boolean a=true;
	COORD coord,food;
	coord.Y = HEIGHT / 2;
	coord.X = WIDTH / 2;
	int x=0;

	food = comida(tela);
	menu(corp);
	desenha(tela);
	while (a){
		
		if (_kbhit()){
			char tecla = _getch(); 
			switch (tecla)
			{
			case MOVE_CIMA:
				if(corp->d != BAIXO)
				corp->d = CIMA;
				break;
			case MOVE_BAIXO:
				if(corp->d != CIMA)
				corp->d = BAIXO;
				break;
			case MOVE_ESQUERDA:
				if(corp->d != DIREITA)
				corp->d = ESQUERDA;
				break;
			case MOVE_DIREITA:
				if(corp->d != ESQUERDA)
				corp->d = DIREITA;
				break;
			case 'e':
				a = false;
				break;
			case 's':
				cursor({ 0,HEIGHT }, SPACE);
				save_game(corp, placar(x, corp));
				printf("seu jogo foi salvo, espero");
				break;
			}
		}
		else {
			move_snake(tela, corp, corp->d, &food);
			cursor(food, FOOD);
			x = placar(x, corp);
			cursor({ WIDTH / 2 - 7 ,HEIGHT - 1 }, 185);
			printf(" PLACAR : %d %c", x, 204);
			Sleep(90);
		}
	}
	return(0);
}

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
	tela[HEIGHT - 1][0] = 200;
	tela[0][WIDTH - 1] = 187;
	tela[HEIGHT - 1][WIDTH - 1] = 188;
}
Snake* inicializa_snake(Snake *corpo, int p) {
	Snake *novo = (Snake*)malloc(sizeof(Snake));
	if (!novo == NULL) {
		Snake *aux = NULL;
		Snake *ax;

		if (p == 2) {
			novo->proximo = corpo;
			corpo = novo;

		}
		else {
			for (ax = corpo; ax->proximo != NULL; ax = ax->proximo);
			novo->proximo = ax->proximo;
			ax->proximo = novo;
		}
		novo->d = ESQUERDA;
		novo->x = (WIDTH / 2) - p;
		novo->y = HEIGHT / 2;
	}
	if (p > 2) {

	}
	return corpo;
}

Snake* inicializa_snake_load(Snake *corpo, int p) {
	Snake *novo = (Snake*)malloc(sizeof(Snake));
	if (!novo == NULL) {
		Snake *aux = NULL;
		Snake *ax;

		if (p == 2) {
			novo->proximo = corpo;
			corpo = novo;

		}
		else {
			for (ax = corpo; ax->proximo != NULL; ax = ax->proximo);
			novo->proximo = ax->proximo;
			ax->proximo = novo;
		}
	}
	if (p > 2) {
	}
	novo->x = 0;
	novo->y = 0;
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
			novo->y = ax->y + 1;
			novo->x = ax->x;
			break;
		case BAIXO:
			novo->y = ax->y - 1;
			novo->x = ax->x;
			break;
		case ESQUERDA:
			novo->y = ax->y;
			novo->x = ax->x + 1;
			break;
		case DIREITA:
			novo->y = ax->y;
			novo->x = ax->x - 1;
			break;
		default:
			break;
		}

	}
	return c;
}

void save_game(Snake *c, int pontos) {
	FILE *fp;
	Snake *aux;
	int i;
	char nome[WIDTH];
	printf("Escreva seu nome : ");
	scanf_s("%s", nome, sizeof(nome));
	for (i = 0; nome[i] != '\0'; i++);
	nome[i] = '.';
	nome[i + 1] = 't';
	nome[i + 2] = 'x';
	nome[i + 3] = 't';
	nome[i + 4] = '\0';
	fopen_s(&fp, nome, "w");
	fprintf_s(fp, "%d\n",pontos);

	for (aux = c;aux->proximo != NULL;aux = aux->proximo){
		fprintf_s(fp, "%d-%d;%d\n", aux->x, aux->y, aux->d);
	}
	fprintf_s(fp, "%d-%d;%d\n", aux->x, aux->y, aux->d);
	fclose(fp);
}
 
void ordena(Placar *p) {
	int i, j;
	int aux;
	Placar aux2;
	if (p->nome != NULL) {
		for (i = 0; i < 9; i++) {
			for (j = i + 1; j < 10; j++) {
				if (p->valor[i] > p->valor[j]) {
					aux = p[i].valor[i];
					strcpy_s(aux2.nome, p[j].nome);
					p[i].valor[i] = p[j].valor[j];
					strcpy_s(p[i].nome, p[j].nome);
					p[j].valor[j] = aux;
					strcpy_s(p[j].nome, aux2.nome);
				}
			}
		}
	}
}

void save_score(char *nome, int pontos) {
	FILE *fp;
	Placar p[10];
	char ch,ant;
	boolean ponto = true;
	int i = 0, j = 0;
	fopen_s(&fp, "placar.txt", "w+");
	i = 0;
	while ((fscanf_s(fp,"%c",&ch)) != EOF) {
		if (ponto) {
			if (ch != ' ') {
				p[i].valor[i] = ch;
				i++;
			}
			if (ch == ' ') {

				ponto = false;
				i = 0;
			}
		}else if(ch != '\n'){
			p[i].nome[j] = ch;
			j++;
		}else if (ch == '\n'){
			j = 0;
		}
	}
	ordena(p);
	for (i = 0; i != 10 || p[i].nome == NULL; i++) {
		if (pontos > p->valor[i]) {
			p[i].nome[i] = *nome;
			p[i].valor[i] = pontos;
		}
	}
	for (i = 0; i != 10 || p[i].nome == NULL; i++) {
		fputc((int)p[i].valor, fp);
		fputc(' ', fp);
		fputs((char*)p[i].nome,fp);
	}
	fclose(fp);
}
void finaliza_tela(char tela[HEIGHT][WIDTH], int pontos, Snake * c) {
	COORD cord;
	FILE * fp;
	int ch, go;
	char nome[WIDTH];
	fopen_s(&fp, "tela_fim.txt", "r");
	system("cls");
	while ((ch = fgetc(fp)) != EOF) {
		printf("%c", ch);
	}
	cord.X = 46;
	cord.Y = 9;
	cursor(cord,  ' ');
	printf("%d pontos", pontos-3);

	cord.X = 14;
	cord.Y = 15;
	cursor(cord, ' ');
	scanf_s("%s", &nome, sizeof(nome));

	cord.X = 16;
	cord.Y = 17;
	cursor(cord, ' ');
	printf("Deseja iniciar um novo jogo ?");
	cord.X = 12;
	cord.Y = 18;
	cursor(cord, ' ');
	save_score(nome, pontos);
	printf("N para sair / S para iniciar novo jogo");
	while (true) {
		
		if (_kbhit()) {
			go = _getch();
			if (go == 's' || go == 'S') {
				main();
			}
			else{
				break;
			}

		}
	}
	
	fclose(fp);
	free(c);
	exit(0);
	
}

void move_snake(char tela[HEIGHT][WIDTH], Snake* c, Direcao d, COORD *f) {
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
			if (tela[c->y - 1][c->x] != BODY) {
				if (tela[c->y - 1][c->x] == SPACE) {
					tela[c->y][c->x] = SPACE;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, SPACE);
					c->d = CIMA;
					c->y = c->y - 1;
					tela[c->y][c->x] = BODY;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, BODY);
					if (c->proximo != NULL) {
						move_snake(tela, c->proximo, c->proximo->d, &food);
						c->proximo->d = c->d;
					}
				}
				else if (c->y != food.Y && c->x != food.X  && tela[c->y - 1][c->x] != BODY || tela[c->y + 1][c->x] == BODY) {
					int x = 1;
					for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
						x++;
					}
					finaliza_tela(tela, x, c);

				}
			}
			else {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x, c);
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
			if (tela[c->y + 1][c->x] != BODY) {
				if (tela[c->y + 1][c->x] == SPACE) {
					tela[c->y][c->x] = SPACE;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, SPACE);
					c->d = BAIXO;
					c->y++;
					tela[c->y][c->x] = BODY;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, BODY);
					if (c->proximo != NULL) {
						move_snake(tela, c->proximo, c->proximo->d, &food);
						c->proximo->d = c->d;
					}
				}
				else {
					if (c->y != food.Y && c->x != food.X && tela[c->y + 1][c->x] != BODY || tela[c->y - 1][c->x] == BODY) {
						int x = 1;
						for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
							x++;
						}
						finaliza_tela(tela, x, c);
					}
				}
			}
			else {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x, c);
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
			if (tela[c->y][c->x - 1] != BODY) {
				if (tela[c->y][c->x - 1] == SPACE) {
					tela[c->y][c->x] = SPACE;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, SPACE);
					c->d = ESQUERDA;
					c->x = c->x - 1;
					tela[c->y][c->x] = BODY;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, BODY);
					if (c->proximo != NULL) {
						move_snake(tela, c->proximo, c->proximo->d, &food);
						c->proximo->d = c->d;
					}
				}
				else {
					if (c->y != food.Y && c->x != food.X  && tela[c->y][c->x - 1] != BODY || tela[c->y][c->x + 1] == BODY) {
						int x = 1;
						for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
							x++;
						}
						finaliza_tela(tela, x, c);
					}
				}
			}
			else {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x, c);
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
			if (tela[c->y][c->x + 1] != BODY) {
				if (tela[c->y][c->x + 1] == SPACE) {
					tela[c->y][c->x] = SPACE;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, SPACE);
					c->d = DIREITA;
					c->x = c->x + 1;
					tela[c->y][c->x] = BODY;
					cord.X = c->x;
					cord.Y = c->y;
					cursor(cord, BODY);
					if (c->proximo != NULL) {
						move_snake(tela, c->proximo, c->proximo->d, &food);
						c->proximo->d = c->d;
					}
				}
				else {
					if (c->y != food.Y && c->x != food.X && tela[c->y][c->x + 1] != BODY) {
						int x = 1;
						for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
							x++;
						}
						finaliza_tela(tela, x, c);
					}
				}
			}
			else {
				int x = 1;
				for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
					x++;
				}
				finaliza_tela(tela, x, c);
			}
			break;

		}
	
}

int placar(int x, Snake *c) {
	x = 1;
	for (Snake *ax = c; ax->proximo != NULL; ax = ax->proximo) {
		x++;
	}
	return(x - 3);
}

void menu(Snake*c) {
	FILE * fp;
	int ch;
	fopen_s(&fp, "menu.txt", "r");


	system("cls");
	while ((ch = fgetc(fp)) != EOF) {
		printf("%c", ch);
	}
	boolean a = TRUE;
	while (a) {
		if (_kbhit()) {
			int key = _getch();
			if (key == 'n' || key == 'N') {
				a = false;
			}
			if (key == 'l' || key == 'L') {
				load(c);
			}
			if (key == 'r' || key == 'K') {
				rank();
			}
			if (key == 'e' || key == 'E') {
				exit(1);
			}
		}
	}
	fclose(fp);
}
void load(Snake *c) {
	free(c);
	FILE * fp;
	int ant = 0,i=0;
	char cordenada[2], ch;
	int placar = 0, controle = 0, cord = 0,x = 0,y = 0 ;
	fopen_s(&fp, "teste.txt", "r");
	if (fp == NULL) {
		printf("Arquivo nao encontrado ou nao foi possivel abrir. ");
	}
	while ((fscanf_s(fp,"%c",&ch)) != EOF) {
		if (controle == 0) {
			printf_s("%c - ", ch);
		}
		if (ant == '\n') {
			controle++;
		}
		if (controle != 0 && ant == '\n') {
			controle++;
			c = inicializa_snake_load(c,controle);
		}
		if (c != NULL && controle !=0) {
			if (ant == '\n' || ant == '-') {
				cord = 1;
			}
			if (cord == 1) {
				cordenada[i] = ch;
				i++;
			}
			if (ch == '\n') {
				cord = 0;
				strcat_s((char*)c->x, sizeof(c->x), (char*)cordenada);
			}
			if (ch == '-') {
				cord = 0;
				strcat_s((char*)c->y, sizeof(c->y), (char*)cordenada);
			}
			if (ant == ';') {
				strcat_s((char*)c->d, sizeof(c->d), (char*)ch);	
			}
		}
	
		ant = ch;
	}
	
	printf("%d\n", placar);
	system("pause");
}
void rank() {
	FILE *fp;
	int ch;
	system("cls");
	fopen_s(&fp, "./placar.txt", "w+");
	if(fp!=NULL)
	while ((ch = fgetc(fp)) != EOF) {
		printf("%c", ch);
	}
}