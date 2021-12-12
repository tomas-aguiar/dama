#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAY_CHAR   7

int main(int argc, char **argv)
{
    // Verificar os argumentos de entrada
    if (argc > 3)
    {
        fprintf(stderr, "Número errado de argumentos.\n");
        return EXIT_FAILURE;
    }
    char *inputMethod = argv[1];

    // Abrir o arquivo para leitura ou ler do teclado
    FILE *file = (inputMethod == "f") && (argv[2] != NULL) 
        ? fopen(argv[2], "r")
        : stdin;
    if (file == NULL)
    {
        fprintf(stderr, "Arquivo não encontrado.\n");
        return EXIT_FAILURE;
    }
    
    // Receber o tamanho do tabuleiro
    char bdSizeBuffer[2];
    fgets(bdSizeBuffer, 2 * sizeof(char), file);
    int boardSize = atoi(bdSizeBuffer);
    if (boardSize < 0) boardSize = 0;

    // Receber o posicionamento das peças e montar o tabuleiro
    char *positioning = malloc(boardSize * sizeof(char));
    char **board = malloc(boardSize * sizeof(char*));
    for (int i = 0; i < boardSize; i++)
    {
        board[i] = malloc(boardSize * sizeof(char));
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = ' ';
        }
    }
    for (int i = 0; i < boardSize; i++)
    {
        fgets(positioning, boardSize * sizeof(char), file);
        if ((int) strlen(positioning) <= 0)
        {
            fprintf(stderr, "Linha de posicionamento errada %d.\n", (i + 1));
            return EXIT_FAILURE;
        }
        for (int j = 0; j < boardSize; j += 2)
        {
            board[i][j] = positioning[j];
        }
    }
    free(positioning);
    positioning = NULL;

    // Receber as jogadas
    int **plays = malloc(sizeof(int*));
    char playLine[MAX_PLAY_CHAR];
    for (int i = 0; !feof(file); i++)
    {
        fgets(playLine, MAX_PLAY_CHAR * sizeof(char), file);
        if ((int) strlen(playLine) <= 0)
        {
            fprintf(stderr, "Linha de jogada errada.\n");
            return EXIT_FAILURE;
        }
        plays = realloc(plays, (i + 1) * sizeof(int*));
        plays[i] = malloc(MAX_PLAY_CHAR * sizeof(int));
        for (int j = 0; j < MAX_PLAY_CHAR; j += 2)
        {
            plays[i][j] = playLine[j] - '0';
        }
    }
    if (file != stdin)
        fclose(file);
    file = NULL;

    // Calcular o resultado
    int black = 0;
    int white = 0;
    for (int i = 0; i < (sizeof(plays) / sizeof(plays[0])); i++)
    {
        int x0 = plays[i][0];
        int y0 = plays[i][1];
        int xf = plays[i][2];
        int yf = plays[i][3];

        if ((xf == x0) || (yf == y0))
            continue;

        if ((board[x0][y0] != 'p') || (board[x0][y0] != 'b'))
            continue;

        if (board[xf][yf] != 'o')
            continue;

        if (board[xf][yf] != ' ')
            continue;

        int xDir = ((xf - x0) > 0) ? 1 : -1;
        int yDir = ((yf - y0) > 0) ? 1 : -1;
        if ((board[x0 + xDir][y0 + yDir] != 'o') && (board[x0 + xDir][y0 + yDir] != board[x0][y0]))
        {
            if ((board[x0][y0] == 'p') && (board[x0 + xDir][y0 + yDir] == 'b'))
                black++;
            else if ((board[x0][y0] == 'b') && (board[x0 + xDir][y0 + yDir] == 'p'))
                white++;

            if ((yf == (boardSize - 1)))
            {
                if (board[x0][y0] == 'p')
                    black++;
                else if (board[x0][y0] == 'b')
                    white++;
            }

            board[xf][yf] = board[x0][y0];
            board[x0][y0] = ' ';
            board[x0 + xDir][y0 + yDir] = ' ';
        }
    }
    free(board);
    board = NULL;
    free(plays);
    plays = NULL;

    // Imprimir o resultado
    fprintf(stdout, "PRETAS %d\n", black);
    fprintf(stdout, "BRANCAS %d\n", white);
    if (black > white)
        fprintf(stdout, "PRETAS VENCERAM\n");
    else if (black < white)
        fprintf(stdout, "BRANCAS VENCERAM\n");
    else
        fprintf(stdout, "EMPATE");

    return EXIT_SUCCESS;
}
