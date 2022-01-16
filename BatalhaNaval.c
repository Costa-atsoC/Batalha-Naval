/**
 * @autores:
 * Nome: Gonçalo Costa Número de Aluno: 48243
 * Nome: Duarte Neves Número de Aluno: 48030
 * Nome: Miguel Marques Número de Aluno: 48011
 **/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define B 6
#define N 8
#define M 8

/**Representa uma coordenada*/
typedef struct
{
    int x;
    int y;
} Position;

/**Associa cada coordenada do barco com um estado**/
typedef struct
{
    int afloat;   // 0 indica que está afundada, 1 indica que não
    Position pos; // Coordenada
} StateCoord;

/**Representa um barco**/
typedef struct
{
    int afloat;          // Posições que ainda não foram afundadas
    int tSize;           // Tamanho do tipo do barco
    StateCoord coord[5]; // O barco maior tem 5 coordenadas, usando o tSize garantimos que acedemos apenas às existentes
    char type;           // Caracter que representa o tipo do barco
} Boat;

/**Representa um tabuleiro**/
typedef struct
{
    int numBoatsAfloat; // Número de barcos não afundados
    int numBoats;       // Número de barcos que foram colocados
    Boat boats[B];      // Array de barcos. Cada barco é do tipo Boat.
    char board[N][M];   // Array que contém a informação de cada posição do tabuleiro
} Board;

/**
 * NOTA IMPORTANTE:
 * Não pode alterar nenhuma das assinaturas das funções abaixo.
 * Este ficheiro tem de ser considerado em conjunção com o enunciado do TP2.
 **/

/**
 * Function: init_board
 *
 * Inicializa um Board.
 *
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * b: apontador para o Board que vai ser inicializado
 *
 **/
void init_board(int n, int m, Board *b)
{
    b->numBoatsAfloat = 0;
    b->numBoats = 0;

    // Inicializa o array board
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            b->board[i][j] = ' ';
        }
    }
}

/**
 * Function: print_board
 *
 * Mostra no ecrã o estado atual do tabuleiro.
 * Se a flag é 0 (zero) não mostra os barcos, se for 1, mostra.
 * Quando a flag é 1, para indicar a posição de cada barco, é colocada em todas
 * as posições do barco a letra que representa o seu tipo.
 *
 * n: número de linhas do tabuleiro
 * m: número de colunas do tabuleiro
 * board: estado do tabuleiro
 * flag: indicador do modo de visualização do tabuleiro -- se for 0 (zero) não
 * mostra os barcos; se for diferente de 0 (zero) mostra.
 */

void print_board(int n, int m, char board[n][m], int flag)
{
    printf("+");
    for (int j = 0; j < n; j++)
    {
        printf("---+");
    }
    printf("\n");

    for (int i = 0; i < m; i++)
    {
        printf("|");
        if (flag == 1) // Mostra tudo
        {
            for (int j = 0; j < n; j++)
            {
                printf(" %c |", board[i][j]);
            }
            printf("\n");
        }
        else // Esconde os barcos
        {
            for (int j = 0; j < n; j++)
            {
                printf(" |");
            }
            printf("\n");
        }

        printf("+");
        for (int j = 0; j < n; j++)
        {
            printf("---+");
        }
        printf("\n");
    }
}

/**
 * Function: typeToSize
 *
 * Dado o tipo do barco devolve o seu tamanho.
 *
 * type: tipo do barco ('P', 'N', 'C', ou 'S')
 *
 * return
 * -1 se o tipo de barco for inválido
 * caso contrário, devolve o tamanho do barco
 **/
int typeToSize(char type)
{
    switch (toupper(type))
    {
    case 'P':
        return 5;
    case 'N':
        return 4;
    case 'C':
        return 3;
    case 'S':
        return 2;
    default:
        return -1;
    }
}

/**
 * Function: init_boat
 *
 * Dado um apontador para um barco, o tipo, a posição inicial e a direcção,
 * inicializa o barco com todas as sua posições, tamanho e tipo.
 * Por exemplo, se o tipo do barco é 'C' (contratorpedeiro),
 * a posição inicial (1,1), e a direcção 'H', então o barco fica inicializado
 * com os seguintes valores
 *
 * afloat = 3;
 * tSize = 3;
 * coord = [(1,(1,1)),(1,(1,2)),(1,(1,3))]; Onde (s,(x,y)) representa um elemento do tipo StateCoord
 * type = 'C';
 *
 * b: barco que vai ser inicializado
 * type: tipo do barco
 * xy: posição inicial do barco
 * dir: direcção do barco
 *
 **/
void init_boat(Boat *b, char type, Position xy, char dir)
{
    b->tSize = typeToSize(type);
    b->afloat = typeToSize(type);
    for (int j = 0; j < b->tSize; j++)
    {
        if (toupper(dir) == 'H')
        {
            b->coord[j].pos.y = xy.y + j;
            b->coord[j].pos.x = xy.x;
            b->coord[j].afloat = 1;
        }
        if (toupper(dir) == 'V')
        {
            b->coord[j].pos.x = xy.x + j;
            b->coord[j].pos.y = xy.y;
            b->coord[j].afloat = 1;
        }
    }
}

/**
 * Function: check_free
 *
 * Verifica se estão livres todas as posições que serão ocupadas por um
 * barco no tabuleiro.
 *
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * boat: barco a colocar no tabuleiro
 * board: estado atual do tabuleiro
 *
 * returns:
 * 1 se todas as posições estão livres
 * 0 caso contrário
 *
 **/
int check_free(int n, int m, Boat *boat, char board[n][m])
{
    for (int i = 0; i < boat->tSize; i++) // Vamos fazer um loop para Verificar todas as posições do barco que estivermos a trabalhar
    {
        if (board[boat->coord[i].pos.x][boat->coord[i].pos.y] != ' ' || boat->coord[i].pos.x>N || boat->coord[i].pos.y>M || boat->coord[i].pos.x<0 || boat->coord[i].pos.y<0 )
        {
            return 0;
        }
    }
    return 1;
}

/**
 * Function: place_boat
 *
 * Cria e coloca, se tal for possível, um barco no tabuleiro.
 *
 * x1: coordenada x inicial do barco
 * y1: coordenada y inicial do barco
 * dir: dirrecção do barco ('H' ou 'V')
 * type: o tipo de barco
 * board: estado atual do tabuleiro
 *
 * returns:
 * 0 se o barco for colocado com sucesso.
 * -1 se alguma das posições já está ocupada.
 * -2 se as coordenadas forem inválidas.
 * -3 se a direcção for inválida.
 * -4 se o tipo de barco for inválido.
 **/
int place_boat(int x1, int y1, int dir, char type, Board *board)
{
    Boat b;
    Position pos = {x1, y1};

    init_boat(&b, type, pos, dir);
    if (typeToSize(type) == -1)
    {
        return -4;
    }

    if (x1 > N || x1 < 0 || y1 > M || y1 < 0)
    {
        return -2;
    }

    if (toupper(dir) == 'H')
    {
        if (check_free(N, M, &b, board->board) == 0)
        {
            return -1;
        }
        else
        {
            for (int i = 0; i < typeToSize(type); i++)
            {
                board->board[y1 + i][x1] = type;
            }
        }
    }
    else if (toupper(dir) == 'V')
    {
        if (check_free(N, M, &b, board->board) == 0)
        {
            return -1;
        }
        else
        {
            for (int i = 0; i < typeToSize(type); i++)
            {
                board->board[y1][x1 + i] = type;
            }
        }
    }
    else
    {
        return -3;
    }

    return 0;
}

/**
 * Function: check_sink
 *
 * Verifica se ao atacar a posição (x,y) algum barco é afundado.
 *
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 *
 * returns:
 * 'F' se não for afundado nenhum barco.
 * Se afundar um barco devolve a letra que representa o seu tipo: 'P', 'N', 'C' ou 'S'
 * 'I' se a coordenada for inválida.
 *
 **/
char check_sink(int x, int y, Board *board) // Teremos todos juntos e de mãos dadas verificar este exbelto código
{
    if (board->board[x][y] == 'P')
    {
        board->boats->afloat -= 1;
        if (board->boats[5].afloat == 0)
        {
            return 'P';
        }
        else
        {
            return 'F';
        }
    }
    if (board->board[x][y] == 'N')
    {
        board->boats->afloat -= 1;
        if (board->boats[4].afloat == 0)
        {
            return 'N';
        }
        else
        {
            return 'F';
        }
    }
    if (board->board[x][y] == 'C')
    {
        if (x == board->boats[2].coord->pos.x && y == board->boats[2].coord->pos.y)
        {
            board->boats->afloat -= 1;
            if (board->boats[2].afloat == 0)
            {
                return 'C';
            }
            else
            {
                return 'F';
            }
        }
        if (x == board->boats[3].coord->pos.x && y == board->boats[3].coord->pos.y)
        {
            board->boats->afloat -= 1;
            if (board->boats[3].afloat == 0)
            {
                return 'C';
            }
            else
            {
                return 'F';
            }
        }
    }
    if (board->board[x][y] == 'S')
    {
        if (x == board->boats[0].coord->pos.x && y == board->boats[1].coord->pos.y)
        {
            board->boats->afloat -= 1;
            if (board->boats[0].afloat == 0)
            {
                return 'S';
            }
            else
            {
                return 'F';
            }
        }
        if (x == board->boats[1].coord->pos.x && y == board->boats[1].coord->pos.y)
        {
            board->boats->afloat -= 1;
            if (board->boats[1].afloat == 0)
            {
                return 'S';
            }
            else
            {
                return 'F';
            }
        }
    }
    if (x < 0 || x > N || y < 0 || y > M)
    {
        return 'I';
    }
}

/**Esta função serve para sabermos qual coordenada no array coords estamos a trabalhar*/
int coordnoarray(int x, int y, Board *board)
{
}
/** Esta função serve para sabermos qual dos barcos estamos a trabalhar no array boats*/
int barconoarray(int x, int y, Board *board)
{
    switch (board->board[x][y])
    {
    case 'P':
        return 5;
        break;
    case 'N':
        return 4;
        break;
    case 'C':
        if (x == board->boats[2].coord->pos.x && y == board->boats[2].coord->pos.y)
        {
            return 2;
        }
        if (x == board->boats[3].coord->pos.x && y == board->boats[3].coord->pos.y)
        {
            return 3;
        }
    case 'S':
        if (x == board->boats[0].coord->pos.x && y == board->boats[0].coord->pos.y)
        {
            return 0;
        }
        if (x == board->boats[1].coord->pos.x && y == board->boats[1].coord->pos.y)
        {
            return 1;
        }
    }
}

/**
 * Function: target
 *
 * Ataca a coordenada (x,y) no tabuleiro.
 * Se a coordenada (x,y) é parte de um barco, então coloca um '*' nessa
 * posição '*'; caso contrário coloca a letra 'F' nessa posição; caso afunde
 * um barco coloca em todas as posições desse barco a letra 'A'.
 *
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 *
 * returns:
 * 0 se a posicao já foi atacada anteriormente
 * 1 se acerta numa coordenada de um barco mas sem o afundar
 * -1 se nao tiver sucesso
 * -2 se a coordenada for inválida.
 * caso afunde um barco, devolve o número correspondente ao tamanho do barco
 * que afundou (valor de 2 a 5)
 *
 **/
int target(int x, int y, Board *board)
{
    char chardoboat;

    if (x < 0 || y < 0 || x > N || y > M) /// Se o player 2 introduzir coordenadas fora do tabuleiro
    {
        return -2;
    }

    if (board->board[x][y] != ' ' || typeToSize(board->board[x][y]) < 2)
    {
        return 0;
    }

    else if (board->boats[barconoarray(x, y, board)].afloat != 0)
    {
        chardoboat = check_sink(x, y, board);
        if (chardoboat == 'F')
        {
            return 1;
        }
        else
        {
            return typeToSize(chardoboat);
        }
    }

    if (board->board[x][y] == ' ')
    {
        return -1;
    }
}

int main(void)
{
    Board brd;
    Position p;
    char type, dir;
    int opcao, posx, posy, placecheck = 0;

    printf("---------------------------------BATALHA NAVAL---------------------------------\n\n");

    do
    {
        printf("+-----------------------------------+\n");
        printf("|Menu inicial:                      |\n");
        printf("|\t1- Objetivos do jogo.            |\n");
        printf("|\t2- Informações importantes.      |\n");
        printf("|\t3- Iniciar o jogo.               |\n");
        printf("|\t0- Sair.                         |\n");
        printf("+-----------------------------------+\n");

        printf("Opção: ");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            printf("Objetivos do jogo:\n");
            printf(" O objetivo da Batalha naval é afundar o máximo de barcos ou acertar no máximo de espaços onde existam barcos do inimigo.");
            printf(" O jogo começa quando um dos dois jogadores seleciona o lugar dos seis barcos existentes: dois Submarinos (S), dois Contratorpedeiros (C), um Porta Aviões (P) e um Navio-tanque (N).");
            printf(" Depois dos seis barcos serem colocados o outro jogador, dentro de quarenta jogadas, tem de adivinhar a posição dos barcos do outro jogador.");
            printf(" Se dento do limite de jogadas não conseguir afundar todos os navios passa passa a vez ao proximo jogador onde, num tabuleiro novo, volta a repetir o processo.\n");
            break;
        case 2:
            printf("Informações importantes:\n");
            printf("\t·Utilizar apenas os caracteres que lhe forem pedido.\n");
            printf("\t·Este tabuleiro tem %d colunas e %d linhas.\n", M, N);
            printf("\t·Li.\n");
            break;
        case 3:
            init_board(N, M, &brd);
            printf("--------------Player 1--------------\n");

            for (int i = 0; i < B; i++)
            { // Vai fazer este loop enquanto a função check_free devolver 0 pois quer dizer que já existia um barco em qualquer um dos espaços
                if (i == 0)
                {
                    printf("\t\tAtualmente faltam-lhe introduzir: dois Submarinos (S), dois Contratorpedeiros (C), um Porta Aviões (P) e um Navio-tanque (N).\n\t\tEstá a introduzir as informações para o 1º submarino.\n");
                    type = 'S';
                }
                if (i == 1)
                {
                    printf("\n\t\tAtualmente faltam-lhe introduzir: um Submarino (S), dois Contratorpedeiros (C), um Porta Aviões (P) e um Navio-tanque (N).\n\t\tEstá a introduzir as informações para o 2º submarino.\n");
                    type = 'S';
                }
                if (i == 2)
                {
                    printf("\n\t\tAtualmente faltam-lhe introduzir: dois Contratorpedeiros (C), um Porta Aviões (P) e um Navio-tanque (N).\n\t\tEstá a introduzir as informações para o 1º Contratorpedeiro.\n");
                    type = 'C';
                }
                if (i == 3)
                {
                    printf("\n\t\tAtualmente faltam-lhe introduzir: um Contratorpedeiro (C), um Porta Aviões (P) e um Navio-tanque (N).\n\t\tEstá a introduzir as informações para o 2º Contratorpedeiro.\n");
                    type = 'C';
                }
                if (i == 4)
                {
                    printf("\n\t\tAtualmente faltam-lhe introduzir: um Navio-tanque (N) e um Porta Aviões (P).\n\t\tEstá a introduzir as informações para o Navio-tanque.\n");
                    type = 'N';
                }
                if (i == 5)
                {
                    printf("\n\t\tAtualmente faltam-lhe introduzir: um Porta Aviões (P).\n\t\tEstá a introduzir as informações para o Porta Aviões.\n");
                    type = 'P';
                }
                /*
                printf("\tBarco nº%d \n", i + 1);
                printf("\t\t·Indique o tipo de barco:");
                scanf("%c", &type); //P tem 5 casas, N tem 4, C tem 3, S tem 2
                getchar();*/
                do
                {
                    if (placecheck == -1)
                    {
                        printf("\n\t\tUma das posições já se encontra ocupada\n");
                    }
                    if (placecheck == -2 || placecheck == 0 || placecheck == -1)
                    {
                        if (placecheck == -2)
                        {
                            printf("\n\t\tUma das coordenadas inseridas é invalida\n");
                        }
                        printf("\t\t·Indique a posição X: ");
                        scanf("%d", &posx);
                        getchar();
                        printf("\t\t·Indique a posição Y: ");
                        scanf("%d", &posy);
                        getchar();
                        p.x = posx;
                        p.y = posy;
                    }
                    if (placecheck == -3 || placecheck == 0 || placecheck == -1)
                    {
                        if (placecheck == -3)
                        {
                            printf("\n\t\tA direção é invalida\n");
                        }
                        printf("\t\t·Indique a direção do barco: ");
                        scanf(" %c", &dir);
                        getchar();
                    }
                    placecheck = place_boat(p.x, p.y, dir, type, &brd);
                } while (placecheck != 0);
            }
            printf("--------------Player 2--------------\n");

            print_board(N, M, brd.board, 1);

            /**Exemplo de uso da print_board e da place_boat**/
            /**Precisa de as implementar primeiro**/
            // print_board(N, M, brd.board, 0);
            // place_boat(1,3, 'H', 'P', &brd);
        }
    } while (opcao != 0);

    return 0;
}