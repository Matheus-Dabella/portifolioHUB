// Para executar este projeto:
// 1. Copie todo o código
// 2. Cole em um compilador de C (ex: OnlineGDB)
// 3. Compile e execute

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define BOMBA '*'
#define ESCONDIDO '#'
#define FLAG 'F'

// Cores ANSI
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define LIGHTGRAY "\033[37m"
#define RESET "\033[0m"

// Protótipos
void inicializa_tabuleiro(char board[N][N], char revealed[N][N]);
void imprime_tabuleiro(char board[N][N], char revealed[N][N]);
void coloca_bombas(char board[N][N], int primeiro_lin, int primeiro_col);
void calcula_numeros(char board[N][N]);
void floodFill(char board[N][N], char revealed[N][N], int lin, int col);
int valida_jogada(int lin, int col, char revealed[N][N]);
int checa_vitoria(char board[N][N], char revealed[N][N]);

// ------------------------ MAIN ------------------------
int main() {
    char board[N][N];
    char revealed[N][N];
    int lin, col;
    int first = 1;
    char acao;

    inicializa_tabuleiro(board, revealed);

    while (1) {
        imprime_tabuleiro(board, revealed);
        printf("Digite ação (r=revelar, f=flag) e posição (linha coluna): ");
        scanf(" %c %d %d", &acao, &lin, &col);

        if (lin < 0 || lin >= N || col < 0 || col >= N) {
            printf("Coordenadas inválidas. Tente novamente.\n");
            continue;
        }

        if (acao == 'r') {
            if (!valida_jogada(lin, col, revealed)) {
                printf("Célula já revelada ou flag. Tente outra.\n");
                continue;
            }
            if (first) {
                coloca_bombas(board, lin, col);
                calcula_numeros(board);
                first = 0;
            }

            if (board[lin][col] == BOMBA) {
                revealed[lin][col] = BOMBA;
                imprime_tabuleiro(board, revealed);
                printf(RED "💥 BOOM! Você acertou uma bomba. Fim de jogo.\n" RESET);
                break;
            } else if (board[lin][col] == '0') {
                floodFill(board, revealed, lin, col);
            } else {
                revealed[lin][col] = board[lin][col];
            }
        }
        else if (acao == 'f') {
            if (revealed[lin][col] == ESCONDIDO)
                revealed[lin][col] = FLAG;
            else if (revealed[lin][col] == FLAG)
                revealed[lin][col] = ESCONDIDO;
            else
                printf("Não é possível colocar flag aqui.\n");
        }
        else {
            printf("Ação inválida. Use 'r' ou 'f'.\n");
            continue;
        }

        if (checa_vitoria(board, revealed)) {
            imprime_tabuleiro(board, revealed);
            printf(GREEN "🎉 Parabéns! Você venceu!\n" RESET);
            break;
        }
    }

    return 0;
}

// ------------------------ FUNÇÕES ------------------------

void inicializa_tabuleiro(char board[N][N], char revealed[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            board[i][j] = '0';
            revealed[i][j] = ESCONDIDO;
        }
}

void imprime_tabuleiro(char board[N][N], char revealed[N][N]) {
    printf("\n ");
    for (int j = 0; j < N; j++) printf("%2d ", j);
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%2d ", i);
        for (int j = 0; j < N; j++) {
            char c = revealed[i][j];
            if (c == FLAG)
                printf(YELLOW "%2c " RESET, c);
            else if (c == BOMBA)
                printf(RED "%2c " RESET, c);
            else if (c >= '1' && c <= '8') {
                switch(c) {
                    case '1': printf(BLUE "%2c " RESET, c); break;
                    case '2': printf(GREEN "%2c " RESET, c); break;
                    case '3': printf(RED "%2c " RESET, c); break;
                    case '4': printf(MAGENTA "%2c " RESET, c); break;
                    case '5': printf(YELLOW "%2c " RESET, c); break;
                    case '6': printf(CYAN "%2c " RESET, c); break;
                    default: printf("%2c ", c);
                }
            } else if (c == '0')
                printf(LIGHTGRAY " . " RESET);
            else
                printf("%2c ", c);
        }
        printf("\n");
    }
    printf("\n");
}

int valida_jogada(int lin, int col, char revealed[N][N]) {
    return revealed[lin][col] == ESCONDIDO;
}

void coloca_bombas(char board[N][N], int primeiro_lin, int primeiro_col) {
    int bombas = 15;
    srand(time(NULL));
    int colocadas = 0;

    while (colocadas < bombas) {
        int i = rand() % N;
        int j = rand() % N;
        if ((i == primeiro_lin && j == primeiro_col) || board[i][j] == BOMBA)
            continue;
        board[i][j] = BOMBA;
        colocadas++;
    }
}

void calcula_numeros(char board[N][N]) {
    int dx[] = {-1,-1,-1,0,0,1,1,1};
    int dy[] = {-1,0,1,-1,1,-1,0,1};

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            if (board[i][j] == BOMBA) continue;
            int count = 0;
            for (int k = 0; k < 8; k++) {
                int ni = i + dx[k];
                int nj = j + dy[k];
                if (ni >= 0 && ni < N && nj >= 0 && nj < N)
                    if (board[ni][nj] == BOMBA) count++;
            }
            board[i][j] = '0' + count;
        }
}

// ---------------- FLOOD FILL ----------------
void floodFill(char board[N][N], char revealed[N][N], int lin, int col) {
    int dx[] = {-1,-1,-1,0,0,1,1,1};
    int dy[] = {-1,0,1,-1,1,-1,0,1};

    int fila[N*N][2];
    int inicio = 0, fim = 0;

    fila[fim][0] = lin;
    fila[fim][1] = col;
    fim++;

    while (inicio < fim) {
        int x = fila[inicio][0];
        int y = fila[inicio][1];
        inicio++;

        if (x < 0 || x >= N || y < 0 || y >= N)
            continue;
        if (revealed[x][y] != ESCONDIDO)
            continue;

        revealed[x][y] = board[x][y];

        if (board[x][y] == '0') {
            for (int k = 0; k < 8; k++) {
                int nx = x + dx[k];
                int ny = y + dy[k];
                if (nx >= 0 && nx < N && ny >= 0 && ny < N &&
                    revealed[nx][ny] == ESCONDIDO) {
                    fila[fim][0] = nx;
                    fila[fim][1] = ny;
                    fim++;
                }
            }
        }
    }
}

int checa_vitoria(char board[N][N], char revealed[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] != BOMBA && revealed[i][j] == ESCONDIDO)
                return 0;
    return 1;
}
