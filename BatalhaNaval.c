/**
 * @autores:
 * Nome: Gonçalo Costa      Número de Aluno: 48243
 * Nome: Duarte Neves       Número de Aluno: 48030
 * Nome: Miguel Marques     Número de Aluno: 48011
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
    int afloat;   //0 indica que está afundada, 1 indica que não
    Position pos; //Coordenada
} StateCoord;

/**Representa um barco**/
typedef struct
{
    int afloat;          //Posições que ainda não foram afundadas
    int tSize;           //Tamanho do tipo do barco
    StateCoord coord[5]; //O barco maior tem 5 coordenadas, usando o tSize garantimos que acedemos apenas às existentes
    char type;           //Caracter que representa o tipo do barco
} Boat;

/**Representa um tabuleiro**/
typedef struct
{
    int numBoatsAfloat; //Número de barcos não afundados
    int numBoats;       //Número de barcos que foram colocados
    Boat boats[B];      //Array de barcos. Cada barco é do tipo Boat.
    char board[N][M];   //Array que contém a informação de cada posição do tabuleiro
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

    //Inicializa o array board
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
 *       mostra os barcos; se for diferente de 0 (zero) mostra.
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
        if (flag == 1) //Mostra tudo
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
 *  -1 se o tipo de barco for inválido
 *  caso contrário, devolve o tamanho do barco
 **/
int typeToSize(char type)
{
    int tamanho;
    switch (toupper(type))
    {
    case 'P':
        tamanho = 5;
        break;
    case 'N':
        tamanho = 4;
        break;
    case 'C':
        tamanho = 3;
        break;
    case 'S':
        tamanho = 2;
        break;
    default:
        tamanho = -1;
        break;
    }

    return tamanho;
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
    for(int j = 0 ; j < b->tSize; j++)
    {
        if (toupper(dir) == 'H')
        {
            b->coord[j].pos.y = xy.y+j;
            b->coord[j].pos.x = xy.x;
            b->coord[j].afloat = 1;
        }
        if(toupper(dir) == 'V')
        {
            b->coord[j].pos.x = xy.x+j;
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
 *  1 se todas as posições estão livres
 *  0 caso contrário
 * 
 **/
int check_free(int n, int m, Boat *boat, char board[n][m])
{
    int x, y;
    for(int i = 0; i < boat->tSize; i++)//Vamos fazer um loop para Verificar todas as posições do barco que estivermos a trabalhar
    {
        x = boat->coord[i].pos.x;
        y = boat->coord[i].pos.y;

        if(board[x][y] != ' ')// Se por acaso alguma das posições não for vazia quer dizer que existe um barco nessa posição visto que até este ponto não é possivel ter um '*' (afundado)
        {
            return 0;//Salta logo fora da função
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
    Position pos={x1, y1};
    init_boat(&b,type,pos,dir);
    for (int i=0; i<typeToSize(type); i++)
        {
            if (toupper(dir)=='H')
            {
                check_free(N,M,&b,board->board);
                board->board[y1+i][x1]=type;
            }
            else if (toupper(dir)=='V')
            {
                board->board[y1][x1+i]=type;
            }
                
        }

    return 1;
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
 *   'F' se não for afundado nenhum barco.
 *   Se afundar um barco devolve a letra que representa o seu tipo: 'P', 'N', 'C' ou 'S' 
 *   'I' se a coordenada for inválida.
 * 
 **/
char check_sink(int x, int y, Board *board)
{
    //Implementar

    return 'O';
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
 *   0 se a posicao já foi atacada anteriormente
 *   1 se acerta numa coordenada de um barco mas sem o afundar
 *   -1 se nao tiver sucesso
 *   -2 se a coordenada for inválida. 
 *   caso afunde um barco, devolve o número correspondente ao tamanho do barco 
 *   que afundou (valor de 2 a 5)
 * 
 **/
int target(int x, int y, Board *board)
{
    //Implementar

    return -3;
}

//int colocaNavio()
int main(void)
{
    Boat player1[B];
    Board brd;
    Position p;
    char type, dir;
    int posx, posy;

    printf("---------------------------------BATALHA NAVAL---------------------------------\n\n");
    printf("*Informações importantes:\n");
    printf("*\t·");

    for (int i = 0; i < B; i++)
    {
        printf("--------------Player 1--------------\n");
        do{//Vai fazer este loop enquanto a função check_free devolver 0 pois quer dizer que já existia um barco em qualquer um dos espaços
            printf("\tBarco nº%d\n", i + 1);
            printf("\t\t·Indique o tipo de barco:");
            scanf("%c", &type); //P tem 5 casas, N tem 4, C tem 3, S tem 2
            getchar();
            do{
                printf("\t\t·Indique a posição X: ");
                scanf("%d", &posx);
                getchar();
                if (posx<0 || posx>=N)
                    printf("Erro! insira um posição Valida\n");
            } while (posx<0 || posx>=N);//loop para validar o numero metido 
            do{
                printf("\t\t·Indique a posição Y: ");
                scanf("%d", &posy);
                getchar();
                if (posy < 0 || posy>=M)
                    printf("Erro! insira um posição Valida\n");
            } while (posy<0 || posy>=M);//loop para validar o nº metido
            p.x = posx;
            p.y = posy;
            do{
                printf("\t\t·Indique a direção do barco: ");
                scanf(" %c", &dir);
                if(dir != 'h' || dir == 'H' || dir == 'v' || dir == 'V'){
                    printf("\t\t*Por favor introduza uma letra válida. É lhe relembrado que só pode introduzir 'h' ou 'v'.*\n");
                }
            }while(dir != 'h' || dir == 'H' || dir == 'v' || dir == 'V');
            getchar();
            
            init_boat(&player1[i], type, p, dir);
        } while (check_free(N,M,&player1[i],brd.board) != 1);
        //init_boat(player2[i]);
    }

    init_board(N, M, &brd);
    print_board(N, M, brd.board, 1);

    /**Exemplo de uso da print_board e da place_boat**/
    /**Precisa de as implementar primeiro**/
    //print_board(N, M, brd.board, 0);
    //place_boat(1,3, 'H', 'P', &brd);

    return 0;
}   
