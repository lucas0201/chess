#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define FALSE 0
#define TRUE 1
#define TABLE_SIZE 127
#define boolean char

typedef struct game_position GamePos;
typedef struct hash_table HashTable;
typedef struct position Position;
typedef struct piece Piece;
typedef struct chess Chess;

struct game_position {
	char* fen;	//FEN
	char r;		//Numero de repeticoes
	GamePos* next;	//Proximo no
};

struct hash_table {
	GamePos** game;	//Lista
	GamePos* new;	//Mais recente alteracao
	int m;		//Tamanho da tabela
};

typedef enum {		//Identificacao das pecas
	BP = 1,		//As pecas pretas assumem valores impares, as pecas brancas, valores pares
	WP,
	BN,
	WN,
	BB,
	WB,
	BR,
	WR,
	BQ,
	WQ,
	BK,
	WK,
	EMPTY = -1	//Nenhuma peca
} Piecename;

typedef enum {		//Situacao de jogo
	PLAY,
	W_WINS,
	B_WINS,
	STALEMATE,
	FIFTY,
	MATERIAL,
	REPETITION
} Gamesit;

struct position {	//Posicao
	char file;
	char rank;
	char x;		//Tipo de ocupacao: 0 - normal, 'x' - captura, 'e' - en passant, maiusculo - captura e promocao, outros - promocao
};

struct piece {					//Peca
	Piecename id;				//Identificacao
	Position** pos;				//Vetor com a posicao atual e com os possiveis movimentos para a jogada
	char m;					//Numero de movimentos possiveis calculados
	void (*move)(Piece*, Chess*, boolean);	//Funcao de movimentacao
};

struct chess {				//Estrutura para o jogo de xadrez
	HashTable* record;		//Posições do jogo
	Piece* board[8][8];		//Tabuleiro com as pecas, posicao vazia assume valor NULL
	Piece* king[2];			//Reis
	char n_pieces;			//Numero total de pecas no jogo
	char* castling;			//String de roque do FEN
	Position en_passant;		//Posicao de captura en passant
	char turn;			//Turno: 0 - pecas brancas, 1 - pecas pretas
	char mid_turns;			//Numero de meios-turnos
	int n_turns;			//Numero de turnos
};

/*Insere um novo caractere numa dada posicao em uma string.
	Parametros
		char** str	string
		char c		caractere
		char i		posicao
*/
void strinsc(char** str, char c, char i);

/*Remove um caractere de uma string.
	Parametros
		char* str	string
		char c		caractere
	Retorno
		TRUE se o caractere foi encontrado, FALSE caso contrario
*/
boolean strrmc(char* str, char c);

/*Converte um inteiro para uma string.
	Parametros
		int i	inteiro
	Retorno
		string com o numero
*/
char* itoa(int i);

/*Verifica se dois códigos FEN correspondem a mesma posicao.
	Parametros
		char* fen1	primeiro codigo FEN
		char* fen2	segundo codigo FEN
	Retorno
		TRUE se correspondem a mesma posicao, FALSE caso contrario
*/
boolean gamecmp(char* fen1, char* fen2);

/*Inicializa uma tabela hash de codigos FEN.
	Parametros
		int m		tamanho da tabela
	Retorno
		tabela hash
*/
HashTable* initialize_hashtable(int m);

/*Finaliza uma tabela hash.
	Parametros
		HashTable* ht		tabela
*/
void finalize_hashtable(HashTable* ht);

/*Calcula a o codigo hash de um codigo FEN.
	Parametros
		char* s		FEN
		int m		tamanho da tabela hash
	Retorno
		codigo hash
*/
int hash(char* s, int m);

/*Insere uma posicao numa tabela hash, ou incrementa o contador de repeticao se a posicao ja se encontra na tabela.
	Parametros
		char* s		FEN
		HashTable* ht	tabela
*/
void insert_hashtable(char* s, HashTable* ht);

/*Inicializa um registro Position.
	Parametros
		char file	file
		char rank	rank
		char x		x
	Retorno	
		estrutura alocada e ja com seus valores definidos
*/
Position* initialize_position(char file, char rank, char x);

/*Desaloca um registro Position.
	Parametros
		Position* pos	elemento a ser desalocado
*/
void finalize_position(Position* pos);

/*Compara dois registros Positions.
	Parametros
		Position* p1	elemento um
		Position* p2	elemento dois
	Retorno
		0, se sao iguais, um valor maior que zero se p1 tem menos prioridade que p2 e um valor menor que zero caso contrario
*/
char cmp_position(Position* p1, Position* p2);

/*Retorna a mediana entre tres posicoes.
	Parametros
		Position** v	vetor com os elementos
		char n		numero de elementos
	Retorno
		indice da mediana
*/
char median(Position** v, char n);

/*Ordena um vetor de registros Positions atraves do algoritmo Quicksort.
	Parametros
		Position** pos		vetor com os elementos
		char n			numero de elementos
*/
void sort_position(Position** pos, char n);

/*Realiza uma busca binaria por um registro Position num vetor.
	Parametros
		Position** v		vetor
		char n			numero de elementos
		Position* key		chave de busca
	Retorno
		TRUE caso a chave seja encontrada, FALSE caso contrario
*/
boolean search_position(Position** v, char n, Position* key);

/*Efetua uma copia de um registro posicao.
	Parametros
		Position* des		destino, se NULL e feita uma alocacao
		Position* src		fonte
	Retorno
		copia
*/
Position* cpy_position(Position* dest, Position* src);

/*Inicializa um registro Piece, efetuando uma alocacao.
	Parametros
		Piecename id	id
		char file	file
		char rank	rank
	Retorno
		Piece alocada
*/
Piece* initialize_piece(Piecename id, char file, char rank);

/*Desaloca um registro Piece.
	Parametros
		Piece* piece	elemento a ser desalocado
*/
void finalize_piece(Piece* piece);

/*Gera o id de uma peca a partir de um caractere.
	Parametros
		char c		caractere
	Retorno
		id da peca
*/
Piecename genName_piece(char c);

/*Gera o caractere correspondente ao id da peca.
	Parametros
		Piecename id	id da peca
	Retorno
		caractere
*/
char genChar_piece(Piecename id);

/*Retorna a pontuacao referente ao id da peca.
	Parametros
		Piecename id	id da peca
	Retorno
		pontuacao
*/
int score_piece(Piecename id);

/*Verifica e o id e de uma peca branca.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e branca, FALSE caso contrario
*/
boolean iswhite(Piecename id);

/*Verifica e o id e de uma peca preta.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e preta, FALSE caso contrario
*/
boolean isblack(Piecename id);

/*Verifica se e rei.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e peao, FALSE caso contrario
*/
boolean isking(Piecename id);

/*Verifica se e rainha.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e peao, FALSE caso contrario
*/
boolean isqueen(Piecename id);

/*Verifica se a regra de movimentacao da peca compoe a regra de movimentacao de uma rainha.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e peao, FALSE caso contrario
*/
boolean isqueenMove(Piecename id);

/*Verifica se e torre.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e torre, FALSE caso contrario
*/
boolean isrook(Piecename id);

/*Verifica se e peao.
	Parametros
		Piecename id	id
	Retorno
		TRUE se e peao, FALSE caso contrario
*/
boolean ispawn(Piecename id);

/*Muda a cor de uma peca.
	Parametros
		Piecename	id
*/
Piecename invert_piece(Piecename id);

/*Verifica se a peca pode ser movimentada no turno atual.
	Parametros
		Piece* piece	peca
		Chess* chess	registro Chess do jogo
	Retorno
		TRUE se sim, FALSE se nao
*/
boolean onturn(Piece* piece, Chess* chess);

/*Verifica se o peao ja foi movido.
	Parametros
		Piece* pawn	peao
	Retorno
		TRUE se ja foi movido, FALSE caso contrario
*/
boolean moved(Piece* pawn);

/*Compara dois registros Piece.
	Parametros
		Piece* p1	elemento um
		Piece* p2	elemento dois
	Retorno
		0, se sao iguais, um valor maior que zero se p1 tem menos prioridade que p2 e um valor menor que zero caso contrario
*/
char cmp_piece(Piece* p1, Piece* p2);

/*Efetua uma copia de uma peca.
	Parametros
		Position* des		destino, se NULL e feita uma alocacao
		Position* src		fonte
	Retorno
		copia
*/
Piece* cpy_piece(Piece* dest, Piece* src);

/*Procura num vetor de registros Piece por uma posicao de movimentacao de um tipo de peca.
	Parametros
		Position* key		posicao chave de busca
		Piece** piece		vetor com os registros de pecas
		char n			numero total de elementos do vetor
		char k			indice da peca chave
	Retorno
		vetor com os registros Piece em que a chave foi encontrada.
*/
Piece** searchPositions_piece(Position* key, Piece** piece, char n, char k);

/*Adiciona uma posicao de movimento, se possivel, a uma peca.
	Parametros
		Piece* piece	registro da peca
		Chess* chess	registro Chess do jogo
		boolean tk	opcao para verificar ou nao ameaca ao rei
		char file	coluna
		char rank	linha
	Retorno
		TRUE se a posicao esta vazia, FALSE caso contrario
*/
boolean insertMove_piece(Piece* piece, Chess* chess, boolean tk, char file, char rank);

/*Funcoes de movimentacao. Geram os movimentos possiveis para suas pecas.
	Parametros
		Piece*		registro Piece da peca
		Chess*		registro Chess do jogo
		boolean tk	opcao para verificar ou nao ameaca ao rei
*/
void king(Piece* king, Chess* chess, boolean tk);
void queen(Piece* queen, Chess* chess, boolean tk);
void rook(Piece* rook, Chess* chess, boolean tk);
void bishop(Piece* bishop, Chess* chess, boolean tk);
void knight(Piece* knight, Chess* chess, boolean tk);
void pawn(Piece* pawn, Chess* chess, boolean tk);

/*Verifica se uma dada posicao esta ameacao por alguma peca de cor especificada.
	Parametros
		Chess* chess		registro chess
		char file		coluna
		char rank		linha
		char player		cor
	Retorno
		TRUE se ameacada, FALSE caso contrario
*/
boolean threat(Chess* chess, char file, char rank, char player);

/*Verifica se um movimento ameaca o rei.
	Parametros
		Chess* chess	registro Chess
		Piece* piece	peca a ser movida
		char file	coluna da posicao alvo
		char rank	linha da posicao alvo
	Retorno
		TRUE se o movimento ameaca o rei, FALSE caso contrario
*/
boolean threatKing(Chess* chess, Piece* piece, char file, char rank);

/*Inicializa um registro Chess, efetuando uma alocacao, a partir um codigo FEN.
	Parametros
		char* fen	codigo FEN
	Retorno
		registro Chess
*/
Chess* initialize_chess(char* fen);

/*Desaloca um registro Chess.
	Parametros
		Chess* chess	registro Chess
*/
void finalize_chess(Chess* chess);

/*Gera o codigo FEN de um jogo de xadrez.
	Parametros
		Chess* chess	registro chess
	Retorno
		FEN
*/
char* genFen_chess(Chess* chess);

/*Efetua a troca de duas posicoes no tabuleiro.
	Parametros
		Chess* chess	registro Chess
		char file1	coluna da primeira posicao
		char rank1	linha da primeira posicao
		char file2	coluna da segunda posicao
		char rank2	linha da segunda posicao
*/
void swapPiece_chess(Chess* chess, char file1, char rank1, char file2, char rank2);

/*Atualiza o vetor de posicoes de uma com as posicoes possiveis para o turno. Os vetor e ordenado.
	Parametros
		Piece* piece	peca
		Chess* chess	registro Chess
*/
void updateMovesPositions_piece(Piece* piece, Chess* chess);

/*Atualiza o vetor de posicoes de cada peca com as posicoes possiveis para o turno. Os vetores sao ordenados.
	Parametros
		Chess* chess	registro Chess
*/
void updateMovesPositions_chess(Chess* chess, char wb);

/*Grava o codigo FEN do jogo numa tabela hash, se uma posicao igual ja foi gravada o codigo nao e inserido e o contador de repeticao e incrementado.
	Parametros
		Chess* chess	registro Chess
	Retorno
		codigo FEN
*/
char* recordGame_chess(Chess* chess);

/*Analisa a situacao de um jogo de xadrez.
	Parametros
		Chess* chess		registro Chess
		char wb			pecas brancas (0) ou pretas (1)
	Retorno
		situacao do jogo
*/
Gamesit sit_chess(Chess* chess);

/*Realiza um movimento num jogo de xadrez.
	Parametros
		Chess* chess	registro Chess do jogo
		Piece* piece	peca
		Position* dest	posicao destino
	Retorno
		TRUE se o movimento foi realizado, FALSE se o movimento e invalido
*/
boolean makeMove_chess(Chess* chess, Piece* piece, Position* dest);

/*Desfaz um movimento, a partir de dados do estado anterior.
	Parametros
		Chess* chess		registro Chess
		Piece* dest		peca na posicao destino
		Piece* piece		peca do movimento
		int mid_turns		numero de meios-turnos anterior
		char* castling		string de roque anterior
		Position* en_passant	registro de movimento en_passant anterior
*/
void backMove_chess(Chess* chess, Piece* dest, Piece* piece, int mid_turns, char* castling, Position* en_passant);

/*Calcula a pontuacao de um movimento.
	Parametros
		Chess* chess		registro Chess
		Piece* piece		peca
		Position* dest		posicao destino
	Retorno
		pontuacao
*/
double moveScore_chess(Chess* chess, Piece* piece, Position* dest);

/*Gera um movimento utilizando uma metrica de decisao.
	Parametros
		Chess* chess		registro Chess
		Piece** piece		recipiente para a peca ser movida
		Position* dest		recipiente para a posicao destino
	Retorno
		TRUE se ha um movimento possivel, FALSE caso contrario
*/
boolean genMoveAI_chess(Chess* chess, Piece** piece, Position* dest);

/*Define uma posicao destino e uma peca a ser movida a partir da leitura de uma anotacao em notacao algebrica simplificada.
	Parametros
		FILE* fp		entrada
		Chess* chess		registro Chess do jogo
		Piece** piece		ponteiro para a peca
		Position dest		posicao destino
	Retorno
		TRUE em caso de sucesso, FALSE em caso de insucesso e -1 se houve falha na leitura do arquivo
*/
char readPieceMove_chess(FILE* fp, Chess* chess, Piece** piece, Position* dest);

void strinsc(char** str, char c, char i) {
	char j;
	j = strlen(*str);
	*str = (char*) realloc(*str, (j+2)*sizeof(char));
	while(j >= i) {
		(*str)[j+1] = (*str)[j];
		j--;
	}
	(*str)[i] = c;
}

boolean strrmc(char* str, char c) {
	char i;
	for(i=0; str[i] != c && str[i] != '\0'; i++);
	if(str[i] == '\0')
		return FALSE;
	do {
		str[i] = str[i+1];
		i++;
	} while(str[i] != '\0');
	return TRUE;
}

char* itoa(int i) {
	int n, t, z;
	char* a;

	n = 0;
	if(i<0) {					//Sinal
		a = (char*) malloc(sizeof(char));
		a[n++] = '-';
	}
	else
		a = (char*) calloc(1, sizeof(char));

	t = 1;
	z = 0;
	do {
		t *= 10;
		z += !(i%t);
	} while(i/t);

	for(t/=10; i; t/=10) {
		a[n++] = i/t + '0';
		i -= (i/t)*t;
		a = (char*) realloc(a, (n+1)*sizeof(char));
		a[n] = '\0';
	}

	while(z>0) {		//Zeros finais
		a[n++] = '0';
		a = (char*) realloc(a, (n+1)*sizeof(char));
		a[n] = '\0';
		z--;
	}

	return a;
}

boolean gamecmp(char* fen1, char* fen2) {
	char i, j;
	i=j=0;
	do {				//Verifica somente, no maximo, ate o quarto espaco
		if(fen1[i] != fen2[i])
			return FALSE;
		i++;
		j += fen1[i] == ' ';
	} while(j<4);
	return TRUE;
}

HashTable* initialize_hashtable(int m) {
	HashTable* ht;
	ht = (HashTable*) malloc(sizeof(HashTable));
	ht->game = (GamePos**) calloc(m, sizeof(GamePos*));
	ht->new = NULL;
	ht->m = m;
	return ht;
}

void finalize_hashtable(HashTable* ht) {
	int i;
	GamePos* aux;
	GamePos* tmp;
	for(i=0; i<ht->m; i++) {
		aux = ht->game[i];
		while(aux != NULL) {
			tmp = aux;
			aux = aux->next;
			free(tmp->fen);
			free(tmp);
		}
	}
	free(ht->game);
	free(ht);
}

int hash(char* s, int m) {
	int i, j;
	unsigned int h;
	h = 0;
	for(i=j=0; j<4; i++, j+=s[i] == ' ')
		h = (31*h + (unsigned int) s[i]) % INT_MAX;
	return h%m;
}

void insert_hashtable(char* s, HashTable* ht) {
	int h;
	GamePos* t;
	h = hash(s, ht->m);
	if(ht->game[h] == NULL) {
		ht->game[h] = (GamePos*) malloc(sizeof(GamePos));
		ht->game[h]->fen = s;
		ht->game[h]->r = 1;
		ht->game[h]->next = NULL;
		ht->new = ht->game[h];
	}
	else {
		t = ht->game[h];
		while(t->next != NULL) {
			if(gamecmp(t->fen, s)) {
				t->r++;
				ht->new = t;
				return;
			}
			t = t->next;
		}
		if(gamecmp(t->fen, s)) {
			t->r++;
			ht->new = t;
			return;
		}
		t->next = (GamePos*) malloc(sizeof(GamePos));
		t->next->fen = s;
		t->next->r = 1;
		t->next->next = NULL;
		ht->new = t->next;
	}
}

Position* initialize_position(char file, char rank, char x) {
	Position* pos;
	pos = (Position*) malloc(sizeof(Position));
	pos->file = file;
	pos->rank = rank;
	pos->x = x;
	return pos;
}

void finalize_position(Position* pos) {
	free(pos);
}

char cmp_position(Position* p1, Position* p2) {
	if(p1->file != p2->file)
		return p1->file - p2->file;
	if(p1->rank != p2->rank)
		return p1->rank - p2->rank;
	return genName_piece(p1->x) - genName_piece(p2->x);	//Posicoes de peao em promocao
}

char median(Position** v, char n) {
	char mid;
	mid = n/2;

	if(cmp_position(v[0], v[mid]) >= 0 && cmp_position(v[0], v[n-1]) <= 0) return 0;
	if(cmp_position(v[0], v[n-1]) >= 0 && cmp_position(v[0], v[mid]) <= 0) return 0;

	if(cmp_position(v[mid], v[0]) >= 0 && cmp_position(v[mid], v[n-1]) <= 0) return mid;
	if(cmp_position(v[mid], v[n-1]) >= 0 && cmp_position(v[mid], v[0]) <= 0) return mid;

	if(cmp_position(v[n-1], v[mid]) >= 0 && cmp_position(v[mid], v[n-1]) <= 0) return n-1;
	//if(cmp_position(v[n-1], v[0]) >= 0 && cmp_position(v[n-1], v[mid]) <= 0)
	return n-1;;
}

void sort_position(Position** pos, char n) {
	char i, j;
	Position* pivot;
	Position* tmp;

	if(n < 2)		//Se o vetor possui menos que 2 elementos
		return;

	i = median(pos, n);	//Pivo = mediana entre 3 elementos: o do comeco, do meio e do fim
	pivot = pos[i];
	pos[i] = pos[n-1];
	pos[n-1] = pivot;
	i = 0;
	j = n-2;
	do {							//Arruma o vetor
		while(cmp_position(pos[i], pivot) < 0)
			i++;
		while(cmp_position(pos[j], pivot) >= 0 && j >= 0)
			j--;
		if(i<j) {
			tmp = pos[i];
			pos[i] = pos[j];
			pos[j] = tmp;
		}
	} while(i < j);
	pos[n-1] = pos[i];
	pos[i] = pivot;

	sort_position(pos, i);		//Chama recursivamente
	sort_position(pos+i+1, n-i-1);
}

boolean search_position(Position** v, char n, Position* key) {
	char i, mid;
	char c;

	c = TRUE;	//Se nenhuma comparacao for feita, retorna FALSE
	i = 0;		//inicio
	n--;		//fim
	mid = n/2;	//meio
	while(i <= n && (c = cmp_position(v[mid], key))) {
		i = c > 0 ? i : mid+1;
		n = c > 0 ? mid-1 : n;
		mid = (n+i)/2;
	}
	return !c;
}

Position* cpy_position(Position* dest, Position* src) {
	if(src == NULL)
		return NULL;
	if(dest == NULL)
		dest = (Position*) malloc(sizeof(Position));
	dest->file = src->file;
	dest->rank = src->rank;
	dest->x = src->x;
	return dest;
}

Piece* initialize_piece(Piecename id, char file, char rank) {
	Piece* piece;
	piece = (Piece*) malloc(sizeof(Piece));
	piece->id = id;
	piece->pos = (Position**) malloc(sizeof(Position*));
	piece->pos[0] = initialize_position(file, rank, 0);	//Posicao inicial
	piece->m = 0;						//0 movimentos calculados
	switch(id) {		//Funcao de movimentacao

		case WK:
		case BK: piece->move = king;
			 break;

		case WQ:
		case BQ: piece->move = queen;
			 break;

		case WR:
		case BR: piece->move = rook;
			 break;

		case WB:
		case BB: piece->move = bishop;
			 break;

		case WN:
		case BN: piece->move = knight;
			 break;

		case WP:
		case BP: piece->move = pawn;
			 break;

		default: piece->move = NULL;
	}
	return piece;
}

void finalize_piece(Piece* piece) {
	char i;
	for(i=0; i<=piece->m; i++)
		finalize_position(piece->pos[i]);
	free(piece->pos);
	free(piece);
}

Piecename genName_piece(char c) {
	switch(c) {
		case 'K': return WK;
		case 'k': return BK;
		case 'Q': return WQ;
		case 'q': return BQ;
		case 'R': return WR;
		case 'r': return BR;
		case 'B': return WB;
		case 'b': return BB;
		case 'N': return WN;
		case 'n': return BN;
		case 'P': return WP;
		case 'p': return BP;
		default: return 0;
	}
}

char genChar_piece(Piecename id) {
	switch(id) {
		case WK: return 'K';
		case BK: return 'k';
		case WQ: return 'Q';
		case BQ: return 'q';
		case WR: return 'R';
		case BR: return 'r';
		case WB: return 'B';
		case BB: return 'b';
		case WN: return 'N';
		case BN: return 'n';
		case WP: return 'P';
		case BP: return 'p';
		default: return 0;
	}
}

int score_piece(Piecename id) {
	switch(id) {

		case WK:
		case BK: return 50000;

		case WQ:
		case BQ: return 1000;

		case WR:
		case BR: return 550;

		case WB:
		case BB:
		case WN:
		case BN: return 325;

		case WP:
		case BP: return 100;

		default: return 0;
	}
}

boolean iswhite(Piecename id) {
	return !(id%2);
}

boolean isblack(Piecename id) {
	return id%2;
}

boolean isking(Piecename id) {
	return id == WK || id == BK;
}

boolean isqueen(Piecename id) {
	return id == WQ || id == BQ;
}

boolean isqueenMove(Piecename id) {
	return id >= BB && id <= WQ;
}

boolean isrook(Piecename id) {
	return id == WR || id == BR;
}

boolean ispawn(Piecename id) {
	return id== WP || id == BP;
}

Piecename invert_piece(Piecename id) {
	return id + (id%2 ? 1 : -1);
}

boolean onturn(Piece* piece, Chess* chess) {
	return piece->id%2 == chess->turn;
}

boolean moved(Piece* pawn) {
	return pawn->pos[0]->rank%6 != pawn->id-1;
}

char cmp_piece(Piece* p1, Piece* p2) {
	if(p1->id != p2->id)
		return p1->id - p2->id;
	else
		if(p1->pos[0]->file != p2->pos[0]->file)
			return p1->pos[0]->file - p2->pos[0]->file;
		else
			return p1->pos[0]->rank - p2->pos[0]->rank;
}

Piece* cpy_piece(Piece* dest, Piece* src) {
	char i;
	if(src == NULL)
		return NULL;
	if(dest == NULL)
		dest = (Piece*) malloc(sizeof(Piece));
	dest->id = src->id;
	dest->m = src->m;
	dest->pos = (Position**) malloc((src->m+1)*sizeof(Position*));
	for(i=0; i<=src->m; i++)
		dest->pos[i] = cpy_position(NULL, src->pos[i]);
	dest->move = src->move;
	return dest;
}

Piece** searchPositions_piece(Position* key, Piece** piece, char n, char k) {
	char i, j;		//Contadores
	Piece** res;		//Resultado

	res = NULL;
	j = 0;							//Passa pelo vetor enquanto nao encontra peca de tipo diferente
	for(i=0; i<n && piece[i]->id == piece[k]->id; i++) {				//Nao busca na peca chave
		if(i != k && search_position(piece[i]->pos+1, piece[i]->m, key)) {
			res = (Piece**) realloc(res, (j+1)*sizeof(Piece*));		//Chave encontrada
			res[j++] = piece[i];
		}
	}
	res = (Piece**) realloc(res, (j+1)*sizeof(Piece*));			//Terminador
	res[j] = NULL;
	return res;
}

boolean insertMove_piece(Piece* piece, Chess* chess, boolean tk, char file, char rank) {
	if(chess->board[rank][file] != NULL) {					//Posicao nao vazia
		if(chess->board[rank][file]->id%2 != piece->id%2 && (!tk || !threatKing(chess, piece, file, rank))) {//Verifica a cor
			piece->pos = realloc(piece->pos, (++piece->m+1)*sizeof(Position*));	//Movimento de captura
			piece->pos[piece->m] = initialize_position(file, rank, 'x');
		}
		return FALSE;
	}
	if(!tk || !threatKing(chess, piece, file, rank)) {				//Posicao vazia
		piece->pos = realloc(piece->pos, (++piece->m+1)*sizeof(Position*));
		piece->pos[piece->m] = initialize_position(file, rank, 0);
	}
	return TRUE;
}

void king(Piece* king, Chess* chess, boolean tk) {
	char i, j;

	for(i=king->pos[0]->rank-1; i<=king->pos[0]->rank+1; i++)		//Posicoes ao redor do rei
		for(j=king->pos[0]->file-1; j<=king->pos[0]->file+1; j++) {
			if(i<0 || i>=8 || j<0 || j>=8 || (i == king->pos[0]->rank && j == king->pos[0]->file))//Esta fora ou e a mesma pos.
				continue;
			insertMove_piece(king, chess, tk, j, i);
		}

	if(tk && !threatKing(chess, king, king->pos[0]->file, king->pos[0]->rank)) {		//Roque
		if(iswhite(king->id)) {
			if((NULL != strchr(chess->castling, 'Q')) && chess->board[0][3] == NULL && chess->board[0][2] == NULL && chess->board[0][1] == NULL && !threatKing(chess, king, 3, 0))
					insertMove_piece(king, chess, tk, 2, 0);
			if((NULL != strchr(chess->castling, 'K')) && chess->board[0][5] == NULL && chess->board[0][6] == NULL && !threatKing(chess, king, 5, 0))
				insertMove_piece(king, chess, tk, 6, 0);
		}
		else {
			if((NULL != strchr(chess->castling, 'q')) && chess->board[7][3] == NULL && chess->board[7][2] == NULL && chess->board[7][1] == NULL && !threatKing(chess, king, 3, 7))
				insertMove_piece(king, chess, tk, 2, 7);
			if((NULL != strchr(chess->castling, 'k')) && chess->board[7][5] == NULL && chess->board[7][6] == NULL && !threatKing(chess, king, 5, 7))
				insertMove_piece(king, chess, tk, 6, 7);
		}
	}
}

void queen(Piece* queen, Chess* chess, boolean tk) {
	rook(queen, chess, tk);
	bishop(queen, chess, tk);
}

void rook(Piece* rook, Chess* chess, boolean tk) {
	char i;

	for(i=rook->pos[0]->file-1; i>=0 && insertMove_piece(rook, chess, tk, i, rook->pos[0]->rank); i--);	//Esquerda
	for(i=rook->pos[0]->rank+1; i<8 && insertMove_piece(rook, chess, tk, rook->pos[0]->file, i); i++);	//Para cima
	for(i=rook->pos[0]->rank-1; i>=0 && insertMove_piece(rook, chess, tk, rook->pos[0]->file, i); i--);	//Para baixo
	for(i=rook->pos[0]->file+1; i<8 && insertMove_piece(rook, chess, tk, i, rook->pos[0]->rank); i++);	//Direita
}

void bishop(Piece* bishop, Chess* chess, boolean tk) {
	char i, j;

	//Diagonal: Para cima e para a esquerda
	for(i=bishop->pos[0]->rank+1, j=bishop->pos[0]->file-1; i<8 && j>=0 && insertMove_piece(bishop, chess, tk, j, i); i++, j--);
	//Diagonal: Para baixo e para a esquerda
	for(i=bishop->pos[0]->rank-1, j=bishop->pos[0]->file-1; i>=0 && j>=0 && insertMove_piece(bishop, chess, tk, j, i); i--, j--);
	//Diagonal: Para cima e para a direita
	for(i=bishop->pos[0]->rank+1, j=bishop->pos[0]->file+1; i<8 && j<8 && insertMove_piece(bishop, chess, tk, j, i); i++, j++);
	//Diagonal: Para baixo e para a direita
	for(i=bishop->pos[0]->rank-1, j=bishop->pos[0]->file+1; i>=0 && j<8 && insertMove_piece(bishop, chess, tk, j, i); i--, j++);
}

void knight(Piece* knight, Chess* chess, boolean tk) {
	char i, j;
	char rank, file;

	for(i=-2; i<=2; i++) {
		if(i) {	//i diferente de zero
			rank = knight->pos[0]->rank+i;	//Anda 1 ou 2 linhas, para baixo ou para cima
			for(j=-2; j<=2; j++)
				if(j && (j-i)%2) {	//j diferente de zero e modulo de j diferente de modulo de i
					file = knight->pos[0]->file+j;	//Anda 1 ou 2 colunas, esquerda ou direita
					if(rank>=0 && rank<8 && file>=0 && file<8)	//Linha e coluna valida
						insertMove_piece(knight, chess, tk, file, rank);
				}
		}
	}
}

void pawn(Piece* pawn, Chess* chess, boolean tk) {
	char i, j;

	i = iswhite(pawn->id) ? 1 : -1;		//Cor do peao: anda para cima ou para baixo

	if(pawn->pos[0]->rank+i < 0 || pawn->pos[0]->rank+i >= 8)	//Peao na primeira ou na ultima linha: nenhum movimento
		return;

	for(j=-1; j<=1; j++) {							//Verifica as 3 posicoes de movimento na proxima linha
		if(pawn->pos[0]->file+j >= 0 && pawn->pos[0]->file+j < 8) {

			//En passant
			if(chess->en_passant.x == 'e' && pawn->pos[0]->file+j == chess->en_passant.file && pawn->pos[0]->rank+i == chess->en_passant.rank) {
				if(tk && threatKing(chess, pawn, chess->en_passant.file, chess->en_passant.rank))	//Ameaca ao rei
					continue;
				pawn->pos = realloc(pawn->pos, (++pawn->m+1)*sizeof(Position*));
				pawn->pos[pawn->m] = initialize_position(chess->en_passant.file, chess->en_passant.rank, 'e');
				continue;
			}

			//(j diferente de zero e posicao nao vazia: possivel captura) || (j igual a zero (1 casa a frente) e posicao vazia)
			if((j && chess->board[pawn->pos[0]->rank+i][pawn->pos[0]->file+j] != NULL) || (!j && chess->board[pawn->pos[0]->rank+i][pawn->pos[0]->file] == NULL)) {
				insertMove_piece(pawn, chess, tk, pawn->pos[0]->file + j, pawn->pos[0]->rank + i);
				if(!((pawn->pos[0]->rank+i)%7)) {//Para linha 0 ou 7 -> Promocao
					pawn->pos[pawn->m]->x = genChar_piece(WN);
					insertMove_piece(pawn, chess, tk, pawn->pos[0]->file + j, pawn->pos[0]->rank + i);
					pawn->pos[pawn->m]->x = genChar_piece(WB);
					insertMove_piece(pawn, chess, tk, pawn->pos[0]->file + j, pawn->pos[0]->rank + i);
					pawn->pos[pawn->m]->x = genChar_piece(WR);
					insertMove_piece(pawn, chess, tk, pawn->pos[0]->file + j, pawn->pos[0]->rank + i);
					pawn->pos[pawn->m]->x = genChar_piece(WQ);
				}
			}
		}
	}

	//Avanco de duas casas
	if(!moved(pawn) && chess->board[pawn->pos[0]->rank+i][pawn->pos[0]->file] == NULL && chess->board[pawn->pos[0]->rank+2*i][pawn->pos[0]->file] == NULL)
		insertMove_piece(pawn, chess, tk, pawn->pos[0]->file, pawn->pos[0]->rank + 2*i);
}

boolean threat(Chess* chess, char file, char rank, char player) {
	char i, j;
	Piece* piece_tmp;
	Piece* aux;

	piece_tmp = chess->board[rank][file];
	for(i=player+1; i<=WK; i+=2) {				//Verificacao de ameaca, de peao a dama
		if(isqueen((Piecename) i))	//Dama: movimentos da torre e do bispo
			continue;
		aux = initialize_piece((Piecename) i, file,rank);//Peca na posicao
		aux->move(aux, chess, FALSE);		//Calculo de posicoes possiveis para a peca
		aux->id = invert_piece(aux->id);	//Inverte a cor
		for(j=1; j<=aux->m; j++) {		//Movimentos possiveis para a peca na posicao
			//Posicao de captura, entao verifica se e em uma peca do mesmo tipo ou uma dama, no caso da torre e do bispo
			if(aux->pos[j]->x == 'x' || (aux->pos[j]->x && aux->pos[j]->x != 'e' && chess->board[aux->pos[j]->rank][aux->pos[j]->file] != NULL))
				if(chess->board[aux->pos[j]->rank][aux->pos[j]->file]->id == aux->id || (isqueenMove((Piecename) i) && chess->board[aux->pos[j]->rank][aux->pos[j]->file]->id == (aux->id+2+2*(i<=6)))) {
					finalize_piece(aux);					//Se e, entao a posicao esta amecada
					chess->board[rank][file] = piece_tmp;
					return TRUE;
				}
		}
		finalize_piece(aux);
	}
	chess->board[rank][file] = piece_tmp;
	return FALSE;
}

boolean threatKing(Chess* chess, Piece* piece, char file, char rank) {
	char player;
	char file_tmp;
	char rank_tmp;
	Piece* piece_tmp;

	player = isblack(piece->id);
	file_tmp = piece->pos[0]->file;		//Efetua o movimento no tabuleiro
	rank_tmp = piece->pos[0]->rank;
	piece_tmp = chess->board[rank][file];
	chess->board[rank][file] = NULL;
	swapPiece_chess(chess, file, rank, file_tmp, rank_tmp);
	if(threat(chess, chess->king[player]->pos[0]->file, chess->king[player]->pos[0]->rank, !player)) {
		swapPiece_chess(chess, file, rank, file_tmp, rank_tmp);				//Desfaz o movimento
		chess->board[rank][file] = piece_tmp;
		return TRUE;
	}
	swapPiece_chess(chess, file, rank, file_tmp, rank_tmp);
	chess->board[rank][file] = piece_tmp;
	return FALSE;
}

Chess* initialize_chess(char* fen) {
	char i, j, k, l;
	char* aux;
	Chess* chess;
	chess = (Chess*) malloc(sizeof(Chess));

	chess->record = initialize_hashtable(TABLE_SIZE);		//Tabela hash de codigos FEN
	insert_hashtable(fen, chess->record);

	j = 7;
	k = 0;
	chess->n_pieces = 0;
	for(i=0; fen[i] != ' '; i++) {
		if(isalpha(fen[i])) {					//Ha uma peca na posicao
			chess->board[j][k] = initialize_piece(genName_piece(fen[i]), k, j);
			if(isking(chess->board[j][k]->id))
				chess->king[isblack(chess->board[j][k]->id)] = chess->board[j][k];
			k++;
			chess->n_pieces++;
		}
		else {
			if(isdigit(fen[i])) {			//Posicao vazia (numero)
				for(l=0; l < fen[i]-'0'; l++)
					chess->board[j][k++] = NULL;
			}
			else {		//Outra linha ('/')
				j--;
				k = 0;
			}
		}
	}

	chess->turn = fen[i+1] == 'b';		//Turno

	chess->castling = (char*) malloc(5*sizeof(char));		//Roque
	j = 0;
	i += 3;
	if(fen[i] != '-')
		do {
			chess->castling = (char*) realloc(chess->castling, (j+2)*sizeof(char));
			chess->castling[j++] = fen[i++];
		} while(fen[i] != ' ');
	chess->castling[j] = '\0';

	if(fen[++i] == '-')			//En passant
		chess->en_passant.x = 0;
	else {
		chess->en_passant.file = fen[i] - 'a';
		chess->en_passant.rank = fen[++i] - '1';
		chess->en_passant.x = 'e';
	}

	aux = (char*) malloc(sizeof(char));		//Numero de meios-turnos
	j = 0;
	for(i+=2; fen[i]!=' '; i++) {
		aux[j++] = fen[i];
		aux = realloc(aux, (j+1)*sizeof(char));
	}
	aux[j] = '\0';
	chess->mid_turns = atoi(aux);
	free(aux);

	aux = (char*) malloc(sizeof(char));		//Numero de turnos
	j = 0;
	for(i++; fen[i]!='\0'; i++) {
		aux[j++] = fen[i];
		aux = realloc(aux, (j+1)*sizeof(char));
	}
	aux[j] = '\0';
	chess->n_turns = atoi(aux);
	free(aux);

	updateMovesPositions_chess(chess, chess->turn);	//Calculo dos movimentos possiveis para as pecas no turno

	return chess;
}

void finalize_chess(Chess* chess) {
	char i, j;
	finalize_hashtable(chess->record);
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			if(chess->board[i][j] != NULL)
				finalize_piece(chess->board[i][j]);
	free(chess->castling);
	free(chess);
}

char* genFen_chess(Chess* chess) {
	char i, j, k, l;
	char* aux;
	char* fen;
	fen = NULL;
	l = 0;
	for(i=7; i>=0; i--) {					//Tabuleiro
		for(j=0; j<8; j++) {
			if(chess->board[i][j] != NULL) {			//Peca
				fen = (char*) realloc(fen, (l+1)*sizeof(char));
				fen[l++] = genChar_piece(chess->board[i][j]->id);
			}
			else {							//Posicao vazia
				k = 0;						//Contagem de posicoes vazia consecutivas na mesma linha
				do {
					j++;
					k++;
				} while(j < 8 && chess->board[i][j] == NULL);
				fen = (char*) realloc(fen, (l+1)*sizeof(char));
				fen[l++] = k+'0';
				j--;
			}
		}
		fen = (char*) realloc(fen, (l+1)*sizeof(char));		//Outra linha
		fen[l++] = '/';
	}
	l--;	//Retira o ultimo simbolo '/'
	fen = (char*) realloc(fen, (l+4+strlen(chess->castling)+!chess->castling[0])*sizeof(char));//Alocacao para os proximos caracteres
	fen[l++] = ' ';
	fen[l++] = chess->turn ? 'b' : 'w';	//Turno
	fen[l++] = ' ';
	fen[l] = '\0';				//Roque
	strcat(fen, chess->castling);
	fen[l] = fen[l] ? fen[l] : '-';		//Se a string de roque estiver vazia
	l += strlen(chess->castling)+!chess->castling[0];
	fen[l++] = ' ';
	if(chess->en_passant.x) {					//Casa alvo para realizar um en passant
		fen = (char*) realloc(fen, (l+4)*sizeof(char));
		fen[l++] = chess->en_passant.file + 'a';
		fen[l++] = chess->en_passant.rank + '1';
	}
	else {
		fen = (char*) realloc(fen, (l+3)*sizeof(char));
		fen[l++] = '-';
	}
	fen[l++] = ' ';
	fen[l] = '\0';					//Numero de meios-turnos
	aux = itoa((int) chess->mid_turns);
	l += strlen(aux);
	fen = (char*) realloc(fen, (l+3)*sizeof(char));
	strcat(fen, aux);
	free(aux);
	fen[l++] = ' ';
	fen[l] = '\0';				//Numero de turnos
	aux = itoa(chess->n_turns);
	l += strlen(aux);
	fen = (char*) realloc(fen, (l+1)*sizeof(char));
	strcat(fen, aux);
	free(aux);
	return fen;
}

void swapPiece_chess(Chess* chess, char file1, char rank1, char file2, char rank2) {
	Piece* tmp;
	if(chess->board[rank1][file1] != NULL) {
		chess->board[rank1][file1]->pos[0]->file = file2;
		chess->board[rank1][file1]->pos[0]->rank = rank2;
	}
	if(chess->board[rank2][file2] != NULL) {
		chess->board[rank2][file2]->pos[0]->file = file1;
		chess->board[rank2][file2]->pos[0]->rank = rank1;
	}
	tmp = chess->board[rank1][file1];
	chess->board[rank1][file1] = chess->board[rank2][file2];
	chess->board[rank2][file2] = tmp;
}

void updateMovesPositions_piece(Piece* piece, Chess* chess) {
	char i;
	for(i=1; i<=piece->m; i++)				//Apaga os movimentos anteriores
		finalize_position(piece->pos[i]);
	piece->pos = (Position**) realloc(piece->pos, sizeof(Position*));
	piece->m = 0;
	piece->move(piece, chess, TRUE);		//Geracao dos movimentos
	sort_position(piece->pos+1, piece->m);		//Ordenacao
}

void updateMovesPositions_chess(Chess* chess, char player) {
	char i, j;
	for(i=0; i<8; i++)			//Encontra no tabuleiro as pecas
		for(j=0; j<8; j++)
			if(chess->board[i][j] != NULL && isblack(chess->board[i][j]->id) == player)
				updateMovesPositions_piece(chess->board[i][j], chess);
}

char* recordGame_chess(Chess* chess) {
	char* fen;
	fen = genFen_chess(chess);
	insert_hashtable(fen, chess->record);
	return fen;
}

Gamesit sit_chess(Chess* chess) {
	char i, j;

	if(chess->record->new->r > 2)	//Tripla repeticao
		return REPETITION;

	if(chess->n_pieces <= 3) {	//Ha 3, ou menos, pecas no jogo
		if(chess->n_pieces <= 2)	//Ha apenas 2 reis
				return MATERIAL;
		for(i=0; i<8; i++)		//Procura por um bispo ou cavalo alem dos dois reis
			for(j=0; j<8; j++)
				if(chess->board[i][j] != NULL && chess->board[i][j]->id >= 3 && chess->board[i][j]->id <= 6)
						return MATERIAL;
	}

	for(i=0; i<8; i++)		//Verifica se ha algum movimento possivel
		for(j=0; j<8; j++)
			if(chess->board[i][j] != NULL && onturn(chess->board[i][j], chess))
				if(chess->board[i][j]->m) {
					i = 9;
					break;
				}

	if(i != 10)	//Nenhum movimento possivel no turno
		return threatKing(chess, chess->king[chess->turn], chess->king[chess->turn]->pos[0]->file, chess->king[chess->turn]->pos[0]->rank) ? !chess->turn+1 : STALEMATE;

	if(chess->mid_turns >= 50)	//Regra dos 50 movimentos
		return FIFTY;

	return PLAY;	//Nenhuma condicao de vitoria ou empate satisfeita
}

boolean makeMove_chess(Chess* chess, Piece* piece, Position* dest) {
	//Verifica se o movimento e possivel
	if(search_position(piece->pos+1, piece->m, dest)) {

		chess->en_passant.x = 0;				//Movimento en passant indisponivel

									//Tratamento do roque
		if(chess->castling[0]) {				//Verifica se ainda ha caractere de roque
			//Movimento de um rei
			if(isking(piece->id)) {
				if(iswhite(piece->id)) {		//Retira o caractere correspondente da string de roque
					if(strrmc(chess->castling, 'K') && dest->file == 6 && dest->rank == 0) {//Roque
						swapPiece_chess(chess, 7, 0, 5, 0);					//Movimenta a torre
						strrmc(chess->castling, 'Q');
					}
					else
						if(strrmc(chess->castling, 'Q') && dest->file == 2 && dest->rank == 0) {
							swapPiece_chess(chess, 0, 0, 3, 0);
							strrmc(chess->castling, 'K');
						}
				}
				else {
					if(strrmc(chess->castling, 'k') && dest->file == 6 && dest->rank == 7) {
						swapPiece_chess(chess, 7, 7, 5, 7);
						strrmc(chess->castling, 'q');
					}
					else
						if(strrmc(chess->castling, 'q') && dest->file == 2 && dest->rank == 7) {
							swapPiece_chess(chess, 0, 7, 3, 7);
							strrmc(chess->castling, 'k');
						}
				}
			}
			else {
				//Movimento de uma torre
				if(isrook(piece->id)) {
					if(iswhite(piece->id) && !piece->pos[0]->rank) {
						if(piece->pos[0]->file == 7)
							strrmc(chess->castling, 'K');
						else
							if(!piece->pos[0]->file)
								strrmc(chess->castling, 'Q');
					}
					else {
						if(piece->pos[0]->rank == 7) {
							if(piece->pos[0]->file == 7)
								strrmc(chess->castling, 'k');
							else
								if(!piece->pos[0]->file)
									strrmc(chess->castling, 'q');
						}
					}
				}
			}
			//Verifica se uma torre foi capturada
			if(chess->board[dest->rank][dest->file] != NULL && isrook(chess->board[dest->rank][dest->file]->id)) {
				if(iswhite(chess->board[dest->rank][dest->file]->id) && !chess->board[dest->rank][dest->file]->pos[0]->file) {
					if(chess->board[dest->rank][dest->file]->pos[0]->file == 7)
						strrmc(chess->castling, 'K');
					else
						if(!chess->board[dest->rank][dest->file]->pos[0]->file)
							strrmc(chess->castling, 'Q');
				}
				else {
					if(chess->board[dest->rank][dest->file]->pos[0]->file == 7) {
						if(chess->board[dest->rank][dest->file]->pos[0]->file == 7)
							strrmc(chess->castling, 'k');
						else
							if(!chess->board[dest->rank][dest->file]->pos[0]->file)
								strrmc(chess->castling, 'q');
					}
				}
			}
		}

		if(chess->board[dest->rank][dest->file] != NULL) {		//Captura
			finalize_piece(chess->board[dest->rank][dest->file]);
			chess->board[dest->rank][dest->file] = NULL;
			chess->n_pieces--;
			chess->mid_turns = 0;
		}
		else {
			if(ispawn(piece->id)) {				//Peao
				chess->mid_turns = 0;
				if(dest->x == 'e') {			//En passant
					finalize_piece(chess->board[dest->rank+(iswhite(piece->id) ? -1 : 1)][dest->file]);
					chess->board[dest->rank+(iswhite(piece->id) ? -1 : 1)][dest->file] = NULL;
					chess->n_pieces--;
				}
				else {
					if(abs(dest->rank - piece->pos[0]->rank) == 2) {	//Movimento en passant disponivel
						chess->en_passant.file = dest->file;
						chess->en_passant.rank = dest->rank + (chess->turn ? 1 : -1);
						chess->en_passant.x = 'e';
					}
				}
			}
			else {
				chess->mid_turns++;	//A peca nao e um peao ou o movimento nao e de captura
			}
		}

		swapPiece_chess(chess, piece->pos[0]->file, piece->pos[0]->rank, dest->file, dest->rank);	//Movimenta a peca
		switch(dest->x) {
			case 'Q': piece->id = WQ - isblack(piece->id);		//Promocao
				  piece->move = queen;
				  break;
			case 'R': piece->id = WR - isblack(piece->id);
				  piece->move = rook;
				  break;
			case 'B': piece->id = WB - isblack(piece->id);
				  piece->move = bishop;
				  break;
			case 'N': piece->id = WN - isblack(piece->id);
				  piece->move = knight;
				  break;
		}
		chess->n_turns += chess->turn;				//Incrementa o numero de turnos se for turno 'b'
		chess->turn = !chess->turn;				//Mudanca de turno
		updateMovesPositions_chess(chess, chess->turn);		//Atualiza os vetores de movimentos possiveis
		return TRUE;
	}

	return FALSE;
}

void backMove_chess(Chess* chess, Piece* dest, Piece* piece, int mid_turns, char* castling, Position* en_passant) {
	chess->turn = !chess->turn;
	if(isking(piece->id)) {		//Rei movido
		if(castling[0]) {				//Movimento de roque
			if(chess->turn) {
				if(NULL != strchr(castling, 'k') && dest->pos[0]->file == 6 && dest->pos[0]->rank == 7)	//Roque
					swapPiece_chess(chess, 7, 7, 5, 7);					//Movimenta a torre
				else
					if(NULL != strchr(castling, 'q') && dest->pos[0]->file == 2 && dest->pos[0]->rank == 7)
						swapPiece_chess(chess, 0, 7, 3, 7);
			}
			else {
				if(NULL != strchr(castling, 'K') && dest->pos[0]->file == 6 && dest->pos[0]->rank == 0)
					swapPiece_chess(chess, 7, 0, 5, 0);
				else
					if(NULL != strchr(castling, 'Q') && dest->pos[0]->file == 2 && dest->pos[0]->rank == 0)
						swapPiece_chess(chess, 0, 0, 3, 0);
			}
		}
		chess->king[chess->turn] = piece;	//Atualiza o ponteiro do rei
	}
	chess->board[piece->pos[0]->rank][piece->pos[0]->file] = piece;
	if(NULL != chess->board[dest->pos[0]->rank][dest->pos[0]->file])
	finalize_piece(chess->board[dest->pos[0]->rank][dest->pos[0]->file]);
	if(dest->id != EMPTY) {
		chess->board[dest->pos[0]->rank][dest->pos[0]->file] = dest;
		chess->n_pieces++;
	}
	else
		chess->board[dest->pos[0]->rank][dest->pos[0]->file] = NULL;
	strcpy(chess->castling, castling);			//Roque
	cpy_position(&chess->en_passant, en_passant);		//En Passant
	chess->mid_turns = mid_turns;				//Meios-turnos
	chess->n_turns -= chess->turn;				//Turnos
	updateMovesPositions_chess(chess, chess->turn);		//Movimentos
}

double moveScore_chess(Chess* chess, Piece* piece, Position* dest) {
	char i, j;
	double a, b, va, vb;
	Piece* aux;
	Piece* dest_piece;
	char castling[5];
	Position en_passant;
	char mid_turns;

	aux = cpy_piece(NULL, piece);							//Salva os dados para desfazer o movimento
	chess->board[piece->pos[0]->rank][piece->pos[0]->file] = aux;
	if(isking(aux->id))
		chess->king[chess->turn] = aux;
	if(chess->board[dest->rank][dest->file] == NULL)
		dest_piece = initialize_piece(EMPTY, dest->file, dest->rank);
	else
		dest_piece = cpy_piece(NULL, chess->board[dest->rank][dest->file]);
	strcpy(castling, chess->castling);
	cpy_position(&en_passant, &chess->en_passant);
	mid_turns = chess->mid_turns;

	makeMove_chess(chess, aux, dest);			//Efetua o movimento (calculado os movimentos das pecas do proximo turno)
	updateMovesPositions_chess(chess, !chess->turn);	//Calcula o movimento das pecas do turno

	a = 0;				//Calculo da pontuacao
	b = 1;
	for(i=0; i<8; i++) {
		for(j=0; j<8; j++) {
			if(chess->board[i][j] != NULL) {
				va = vb = score_piece(chess->board[i][j]->id);
				if(onturn(chess->board[i][j], chess))
					vb /= 2;
				else
					va /= 2;
			}
			else
				va = vb = 50;
			a += threat(chess, j, i, !chess->turn)*va;
			b += threat(chess, j, i, chess->turn)*vb;
		}
	}

	backMove_chess(chess, dest_piece, piece, mid_turns, castling, &en_passant);	//Desfaz o movimento
	if(dest_piece->id == EMPTY)		//Desaloca peca vazia auxiliar
		finalize_piece(dest_piece);

	return a/b;
}

boolean moveAI_chess(Chess* chess, Piece** piece, Position* dest) {
	char i, j, k;
	double tmp, max;
	Position aux;

	max = -1;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			if(chess->board[i][j] != NULL && onturn(chess->board[i][j], chess)) {	//Procura pelas pecas
				//Calculo da pontuacao dos movimentos da peca
				for(k=1; k<=chess->board[i][j]->m; k++) {
					cpy_position(&aux, chess->board[i][j]->pos[k]);
					tmp = moveScore_chess(chess, chess->board[i][j], &aux);
					//Atualiza o maximo de acordo com as regras de ordenacao
					if(tmp >= max && (tmp != max || cmp_piece(chess->board[i][j], *piece) < 0)) {
						max = tmp;
						*piece = chess->board[i][j];	//Peca do movimento
						cpy_position(dest, &aux);	//Posicao destino
					}
				}
			}
	return max != -1;
}

char readPieceMove_chess(FILE* fp, Chess* chess, Piece** piece, Position* dest) {
	char* move;
	size_t b;

	move = NULL;			//Leitura
	if(-1 == getline(&move, &b, fp)) {
		free(move);
		return -1;
	}
	b = (strlen(move)-1);
	if(b>=0)
		move[b] = move[b] == '\n' ? '\0' : move[b];

	//Verifica se a string contem uma anotacao de movimento plausivel
	if((b != 4 && b != 5) || move[0] < 'a' || move[0] > 'h' || move[1] < '1' || move[1] > '8' || move[2] < 'a' || move[2] > 'h' || move[3] < '1' || move[3] > '8' || chess->board[move[1]-'1'][move[0]-'a'] == NULL) {
		free(move);
		return FALSE;
	}

	*piece = chess->board[move[1]-'1'][move[0]-'a'];	//Peca
	dest->file = move[2]-'a';				//Posicao destino
	dest->rank = move[3]-'1';				//Captura ou captura en passant, promocao ou nenhuma das anteriores
	dest->x = !move[4] && chess->board[dest->rank][dest->file] != NULL ? (ispawn((*piece)->id) && chess->en_passant.x && dest->file == chess->en_passant.file && dest->rank == chess->en_passant.rank ? 'e' : 'x') : move[4];

	free(move);
	return TRUE;
}

int main(int argc, char* argv[]) {
	char* fen;	//String com um codigo fen
	size_t b;
	Chess* chess;	//Registro Chess do jogo
	Piece* piece;	//Peca de movimentacao
	Position dest;	//Posicao destino de movimentacao
	Gamesit sit;

	fen = NULL;
	getline(&fen, &b, stdin);	//Leitura do codigo
	b = strlen(fen) - 1;		//Retira o \n
	fen[b] = fen[b] == '\n' ? '\0' : fen[b];
	chess = initialize_chess(fen);		//Inicializacao da estrutura em memoria
	while(PLAY == (sit = sit_chess(chess))) {
		printf("%s\n", fen);
		if(chess->record->new->r > 1)	//Repeticao, a string nao foi gravada
			free(fen);
		if(chess->turn) {
			if(moveAI_chess(chess, &piece, &dest))
				makeMove_chess(chess, piece, &dest);
		}
		else {		//Enquanto for possivel ler e a string lida nao constituir um movimento valido
			while(-1 != (b = readPieceMove_chess(stdin, chess, &piece, &dest)) && (!b || !makeMove_chess(chess, piece, &dest)))
				printf("Movimento invalido. Tente novamente.\n");
			if(b == -1)
				break;
		}
		fen = recordGame_chess(chess);	//Grava o estado do jogo
	}
	if(PLAY != sit)
		printf("%s\n", fen);
	switch(sit) {
		case W_WINS: printf("Xeque-mate -- Vitoria: BRANCO\n");
			     break;

		case B_WINS: printf("Xeque-mate -- Vitoria: PRETO\n");
			     break;

		case STALEMATE:	printf("Empate -- Afogamento\n");
				break;

		case FIFTY: printf("Empate -- Regra dos 50 Movimentos\n");
			    break;

		case MATERIAL: printf("Empate -- Falta de Material\n");
			       break;

		case REPETITION: printf("Empate -- Tripla Repeticao\n");
				 free(fen);//A string nao foi gravada
	}
	finalize_chess(chess);
	return 0;
}
