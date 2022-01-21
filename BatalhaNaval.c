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
int typeToSize(char type);

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
                if (typeToSize(board[i][j]) == -1)
                    printf(" %c |", board[i][j]);
                else
                    printf("   |");
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
        if (toupper(dir) == 'H') /// Touppper para dar margem de erro à aplicação de letra maiuscula ou minúscula.
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
    for (int i = 0; i < boat->tSize; i++) // Ciclo de verificação de todas as posições de inserção
    {
        if (board[boat->coord[i].pos.x][boat->coord[i].pos.y] != ' ' || boat->coord[i].pos.x >= N || boat->coord[i].pos.y >= M || boat->coord[i].pos.x < 0 || boat->coord[i].pos.y < 0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 *  Nome: barconoarray
 *  Esta função serve para sabermos qual dos barcos estamos a trabalhar no array boats
 */
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
    case 'C': /// Dois return's, uma vez que temos dois barcos iguais
        for (int i = 0; i < 3; i++)
        {
            if (x == board->boats[2].coord[i].pos.x && y == board->boats[2].coord[i].pos.y)
            {
                return 2;
            }
            if (x == board->boats[3].coord[i].pos.x && y == board->boats[3].coord[i].pos.y)
            {
                return 3;
            }
        }
    case 'S': /// Dois return's, uma vez que temos dois barcos iguais
        for (int i = 0; i < 2; i++)
        {
            if (x == board->boats[0].coord[i].pos.x && y == board->boats[i].coord[i].pos.y)
            {
                return 0;
            }
            if (x == board->boats[1].coord[i].pos.x && y == board->boats[1].coord[i].pos.y)
            {
                return 1;
            }
        }
    }
}

/**Funçao que ordena os barcos por ordem crescente de tamanho na função boats*/
void ordenabarcos(char type, Boat b, Board *board)
{
    switch (toupper(type))
    {
    case 'S':
        if (board->boats[0].afloat != typeToSize(type))
        {
            board->boats[0] = b;
        }
        else if (board->boats[1].afloat != typeToSize(type))
        {
            board->boats[1] = b;
        }
        else
        {
            break;
        }
    case 'C':
        if (board->boats[2].afloat != typeToSize(type))
        {
            board->boats[2] = b;
        }
        else if (board->boats[3].afloat != typeToSize(type))
        {
            board->boats[3] = b;
        }
        else
        {
            break;
        }
    case 'N':
        board->boats[4] = b;
        break;
    case 'P':
        board->boats[5] = b;
        break;
    }
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
    int multipleboatcheck[4] = {0, 0, 0, 0}; /// Incrementa no uso de um boat usado
    init_boat(&b, type, pos, dir);
    // Se a função typetoSize devolve -1, então, pelo default do Switch, sabemos que o tipo de barco é inválido.
    if (typeToSize(type) == -1)
    {
        return -4;
    }
    // A coordenada é inválida se sair dos limites do tabuleiro
    if (x1 > N || x1 < 0 || y1 > M || y1 < 0)
    {
        return -2;
    }
    // As únicas direções disponíveis são H(horizontal) e V(vertical)
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
                board->board[x1][y1 + i] = toupper(type);
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
                board->board[x1 + i][y1] = toupper(type);
            }
        }
    }
    else /// Default
    {
        return -3;
    }

    ordenabarcos(type, b, board);
    board->boats[barconoarray(x1, y1, board)].type = type;
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
    /// Verifica-se se afundou através do valor do afloat
    if (board->board[x][y] == 'P')
    {
        board->boats[5].afloat -= 1;
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
        board->boats[4].afloat -= 1;
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
        for (int i = 0; i < 3; i++)
        {
            if (x == board->boats[2].coord[i].pos.x && y == board->boats[2].coord[i].pos.y)
            {
                board->boats[2].afloat -= 1;
                if (board->boats[2].afloat == 0)
                {
                    return 'C';
                }
                else
                {
                    return 'F';
                }
            }
            if (x == board->boats[3].coord->pos.x && y == board->boats[3].coord[i].pos.y)
            {
                board->boats[3].afloat -= 1;
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
    }
    if (board->board[x][y] == 'S')
    {
        for (int i = 0; i < 2; i++)
        {
            if (x == board->boats[0].coord[i].pos.x && y == board->boats[1].coord[i].pos.y)
            {
                board->boats[0].afloat -= 1;
                if (board->boats[0].afloat == 0)
                {
                    return 'S';
                }
                else
                {
                    return 'F';
                }
            }
            if (x == board->boats[1].coord[i].pos.x && y == board->boats[1].coord[i].pos.y)
            {
                board->boats[1].afloat -= 1;
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
    }
    /// Coordenada inválida
    if (x < 0 || x > N || y < 0 || y > M)
    {
        return 'I';
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
    char foiafundado, targetedchar;
    int numdobarco;
    numdobarco = barconoarray(x, y, board);
    targetedchar = board->board[x][y];

    if (board->board[x][y] != ' ' && typeToSize(board->board[x][y]) < 2)
    {
        return 0;
    }

    if (board->board[x][y] == ' ')
    {
        board->board[x][y] = 'F';
        return -1;
    }

    foiafundado = check_sink(x, y, board);
    if (foiafundado == 'F')
    {
        board->board[x][y] = '*';
        return 1;
    }
    else if (foiafundado == 'I')
    {
        return -2;
    }
    else
    {
        for (int i = 0; i < typeToSize(targetedchar); i++)
        {
            x = board->boats[numdobarco].coord[i].pos.x;
            y = board->boats[numdobarco].coord[i].pos.y;
            board->board[x][y] = 'A';
        }
        board->numBoatsAfloat--;
        return typeToSize(targetedchar);
    }
}

/**Nome: sizetoplacedboats
 *
 * Funçao que recebe o size e indica qual dos barcos na funçoes*/
int sizetoplacedboats(int size)
{
    switch (size)
    {
    case 2:
        return 0;
    case 3:
        return 1;
    case 4:
        return 2;
    case 5:
        return 3;
    default:
        return -1;
    }
}

int main(void)
{
    int opcao = 0;

    printf("---------------------------------BATALHA NAVAL--------------------------------\n\n");

    do
    {
        /// Menu inicial
        printf("+-----------------------------------+\n");
        printf("|Menu inicial:                      |\n");
        printf("|\t1- Objetivos do jogo.       |\n");
        printf("|\t2- Informações importantes. |\n");
        printf("|\t3- Iniciar o jogo.          |\n");
        printf("|\t0- Sair.                    |\n");
        printf("+-----------------------------------+\n");

        printf("Opção: ");
        scanf("%d", &opcao);
        Board brd, cleanboard; /**board limpo para resetar apos cada jogo*/
        Position p;
        char type, dir, jogador[2] = {1, 2}, player1[60], player2[60];
        int posx, posy, placecheck, targetcheck, playerswap, cheating, victory; /** S, C, N, P*/
        switch (opcao)
        {
            /// Informação de objetivos de jogo
        case 1:
            printf("\n*******************************************************************************\n");
            printf("Objetivos do jogo:\n");
            printf(" O objetivo da Batalha naval é afundar o máximo de barcos ou acertar no máximo \nde espaços onde existam barcos do inimigo.");
            printf(" O jogo começa quando um dos dois \njogadores seleciona o lugar dos seis barcos existentes: dois Submarinos (S),\n dois Contratorpedeiros (C), um Porta Aviões (P) e um Navio-tanque (N).\n");
            printf(" Depois dos seis barcos serem colocados o outro jogador, dentro de quarenta\n jogadas, tem de adivinhar a posição dos barcos do outro jogador.");
            printf(" Se dento \ndo limite de jogadas não conseguir afundar todos os navios, os jogadores\n trocam de posições e, num novo tabuleiro, o jogo repete-se mas com as\n funções dos jogadores trocada.\n");
            printf("*******************************************************************************\n\n");
            break;
            /// Informações importantes
        case 2:
            printf("\n*******************************************************************************\n");
            printf("Informações importantes:\n");
            printf("\t·Número de espaços de cada barco:\n");
            printf("\t\t·Submarino (S) -> 2;\n");
            printf("\t\t·Contratorpedeiros (C) -> 3;\n");
            printf("\t\t·Navio-tanque (N) -> 4;\n");
            printf("\t\t·Porta Aviões (P) -> 5.\n");
            printf("\t·Terminologia:\n");
            printf("\t\t·F -> O jogador não acertou em nenhum barco;\n");
            printf("\t\t·A -> O jogador afundou um barco inteiro ;\n");
            printf("\t\t·* -> O jogador acertou em alguma parte de um barco.\n");
            printf("\t·Direções disponiveis:\n");
            printf("\t\t·H -> horizontal;\n");
            printf("\t\t·V -> Vertical.\n");
            printf("\t·Utilizar apenas os caracteres que lhe forem pedido.\n");
            printf("\t·Este tabuleiro tem %d colunas e %d linhas.\n", M, N);
            printf("\t·As coordenadas começam em (0,0) e vão até (%d,%d).\n", N - 1, M - 1);
            printf("\t·Este jogo foi feito com fullscreen ou 79x26 (no minimo) em mente.\n");
            printf("\t·Se quiser sair do jogo a qualquer momento prima CRTL+C.\n");
            printf("\t·Quando estiver a atacar, se introduzir as coordendas (-69,42) irá ser \n\t    lhe mostrado o tabuleiro com todas as posiçõs mas o jogador perde.\n");
            printf("\t·A coordenada x é vertical e a coordenada y é horizontal\n");
            printf("*******************************************************************************\n\n");
            break;
            /// Iniciar jogo
        case 3:
            getchar();
            printf("Nome do player1: ");
            fgets(player1, 60, stdin);
            char *pos = strchr(player1, '\n');
            if (pos != NULL)
                *pos = '\0';
            else
                player1[59] = '\n';
            printf("Nome do player2: ");
            fgets(player2, 60, stdin);
            char *cao = strchr(player2, '\n');
            if (cao != NULL)
                *cao = '\0';
            else
                player2[59] = '\n';
            do
            {
                brd = cleanboard;
                int placedboats[4] = {2, 2, 1, 1};
                placecheck = 0;
                targetcheck = -3;
                cheating = 0;
                victory = 0;
                init_board(N, M, &brd);
                printf("\n---------------Jogador %s--------------\n", player1);

                for (int i = 0; i < B; i++)
                {
                    if (placecheck == -4 || placecheck == 0)
                    {
                        if (placecheck == -4)
                        {
                            printf("\t*Por favor insira um barco válido*");
                        }
                        printf("\n+--------------------------------+\n");
                        printf("|Faltam colocar:                 |\n");
                        if (placedboats[0] > 0)
                        {
                            printf("|\t·%d Submarinos (S).       |\n", placedboats[0]);
                        }
                        if (placedboats[1] > 0)
                        {
                            printf("|\t·%d Contratorpedeiros (C).|\n", placedboats[1]);
                        }
                        if (placedboats[2] > 0)
                        {
                            printf("|\t·%d Navios Tanque (N).    |\n", placedboats[2]);
                        }
                        if (placedboats[3] > 0)
                        {
                            printf("|\t·%d Porta-Aviões (P).     |\n", placedboats[3]);
                        }
                        printf("+--------------------------------+\n");
                        do
                        {
                            printf("\t->Barco:");
                            scanf(" %c", &type);
                            getchar();
                            if (placedboats[sizetoplacedboats(typeToSize(type))] == 0)
                            {
                                printf("\t*Já inseriu todos os barcos deste tipo*\n");
                            }
                        } while (placedboats[sizetoplacedboats(typeToSize(type))] == 0);
                    }
                    do
                    {
                        if (placecheck == -1)
                        {
                            printf("\n\t*Uma das posições já se encontra ocupada*\n");
                        }
                        if (placecheck == -2 || placecheck == 0 || placecheck == -1)
                        {
                            if (placecheck == -2)
                            {
                                printf("\n\t*Uma das coordenadas inseridas é invalida*\n");
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
                        if (placecheck == 0)
                        {
                            brd.numBoats++;
                            brd.numBoatsAfloat++;
                            switch (toupper(type))
                            {
                            case 'S':
                                if (placedboats[0] > 0)
                                {
                                    placedboats[0]--;
                                }
                                else
                                    ;
                                break;
                            case 'C':
                                if (placedboats[1] > 0)
                                {
                                    placedboats[1]--;
                                }
                                else
                                    ;
                                break;
                            case 'N':
                                if (placedboats[2] > 0)
                                {
                                    placedboats[2]--;
                                }
                                break;
                            case 'P':
                                if (placedboats[3] > 0)
                                {
                                    placedboats[3]--;
                                }
                                break;
                            }
                        }
                    } while (placecheck != 0);
                    print_board(N, M, brd.board, 1);
                }
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n--------------Jogador %s--------------\n", player2);
                for (int i = 0; i < 40; i++)
                {
                    printf("Turno %d\n", i + 1);
                    print_board(N, M, brd.board, 0);
                    do
                    {
                        if (targetcheck == 0)
                        {
                            printf("\t\t*O jogador já atacou essa posição*\n");
                        }
                        if (targetcheck == -2)
                        {
                            printf("\t\t*A coordenada inserido é invalida*\n");
                        }
                        printf("\t·Indique a posição X: ");
                        scanf("%d", &posx);
                        getchar();
                        printf("\t·Indique a posição Y: ");
                        scanf("%d", &posy);
                        getchar();
                        p.x = posx;
                        p.y = posy;
                        if (p.x == -69 && p.y == 42)
                        {
                            print_board(N, M, brd.board, 1);
                            cheating = 1;
                        }
                        targetcheck = target(p.x, p.y, &brd);
                        if (targetcheck > 1)
                        {
                            printf("\n\t##################################\n");
                            printf("\t#Afundou:                        #\n");
                            switch (targetcheck)
                            {
                            case 2:
                                printf("\t#\t·Um Submarino (S)!       #\n");
                                break;
                            case 3:
                                printf("\t#\t·Um Contratorpedeiro (C)!#\n");
                                break;
                            case 4:
                                printf("\t#\t·Um Navio-Tanque(N)!     #\n");
                                break;
                            case 5:
                                printf("\t#\t·Um Porta Aviões (P)!    #\n");
                                break;
                            default:
                                break;
                            }
                            printf("\t##################################\n\n");
                        }
                    } while (targetcheck == 0 || targetcheck == -2);
                    /*if(cheating == 1)
                    {
                        printf("\n--------------O Jogador %d Ganhou--------------\n\n\n", jogador[0]);
                    }*/
                    if (cheating == 0)
                    {
                        if (brd.numBoatsAfloat == 0)
                        {
                            printf("\n--------------O Jogador %s Ganhou--------------\n\n\n", player2);
                            victory = 1;
                            break;
                        }
                    }
                }
                if (brd.numBoatsAfloat > 0 && cheating == 1)
                {
                    printf("\n~~~~~~~~~~~~~~O Jogador %s Ganhou~~~~~~~~~~~~~~\n\n\n", player1);
                    victory = 1;
                }
                if (brd.numBoatsAfloat > 0 && victory == 0)
                {
                    // printf("\n--------------O Jogador %d Ganhou--------------", jogador[0]);
                    printf("\n\n~~~~~~~~~~~~~~As Posições Serão Trocadas~~~~~~~~~~~~~~\n\n");
                    playerswap = player1;
                    player1 = player2;
                    player2 = playerswap;
                }
            } while (victory != 1);
            break;
        case 0:
            printf("\t\\Obrigado pelo seu tempo e esperemos que tenha gostado./\n");
            break;
        default:
            printf("\n*Introduza um número válido*\n\n");
            break;
        }
    } while (opcao != 0);
    /// Se a opção for 0 para o jogo
    return 0;
}
