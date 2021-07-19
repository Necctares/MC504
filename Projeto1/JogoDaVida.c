#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TAM_Tabuleiro 1000
#define Celula_Viva 1
#define Celula_Morta 0
#define NUM_Execucoes 1

void show(void *tabuleiro)
{
    int(*tab)[TAM_Tabuleiro] = tabuleiro;
    printf("\033[H");
    for (int i = 0; i < TAM_Tabuleiro; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
            printf(tab[i][j] ? "\033[07m  \033[m" : "  ");
        printf("\033[E");
    }
    fflush(stdout);
    return;
}

void inicializador(int tabuleiro[][TAM_Tabuleiro], int Tam_Inicial)
{
    int random, ini, fim;
    ini = (TAM_Tabuleiro - Tam_Inicial) / 2;
    fim = (TAM_Tabuleiro + Tam_Inicial) / 2;
    srand(time(0));
    for (int i = 0; i < TAM_Tabuleiro; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
        {
            tabuleiro[i][j] = 0;
        }
    }
    for (int i = ini; i < fim; i++)
    {
        for (int j = ini; j < fim; j++)
        {
            random = rand();
            if (random % 2 == 0)
            {
                tabuleiro[i][j] = 1;
            }
        }
    }
}

void copiar_tabuleiro(int tab_aux[][TAM_Tabuleiro], int tabuleiro[][TAM_Tabuleiro], int linhaInicial, int linhaFinal)
{
    for (int i = linhaInicial; i < linhaFinal; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
        {
            tabuleiro[i][j] = tab_aux[i][j];
        }
    }
    return;
}

int contar_vizinho(int tabuleiro[][TAM_Tabuleiro], int posx, int posy)
{
    int num_vizinhos = 0;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if ((i || j) && (i + posx) > -1 && (i + posx) < TAM_Tabuleiro && (j + posy) > -1 && (j + posy) < TAM_Tabuleiro)
            {
                if (tabuleiro[i + posx][j + posy])
                {
                    num_vizinhos += 1;
                }
            }
        }
    }
    return num_vizinhos;
}

void executarJogoDaVida(int tabuleiro[][TAM_Tabuleiro], int linhaInicial, int linhaFinal)
{
    int tab_aux[TAM_Tabuleiro][TAM_Tabuleiro], num_vizinhos;
    for (int i = linhaInicial; i < linhaFinal; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
        {
            num_vizinhos = contar_vizinho(tabuleiro, i, j);
            if (num_vizinhos == 2)
            {
                tab_aux[i][j] = tabuleiro[i][j];
            }
            else if (num_vizinhos == 3)
            {
                tab_aux[i][j] = 1;
            }
            else
            {
                tab_aux[i][j] = 0;
            }
        }
    }
    copiar_tabuleiro(tab_aux, tabuleiro, linhaInicial, linhaFinal);
    return;
}

int main(int argc, char *argv[])
{
    printf("Sabao.\n");
    int Tam_Inicial;
    if (argc > 1)
    {
        if (atoi(argv[1]) <= TAM_Tabuleiro)
        {
            Tam_Inicial = atoi(argv[1]);
        }
        else
        {
            Tam_Inicial = TAM_Tabuleiro / 2;
        }
    }
    else
    {
        Tam_Inicial = TAM_Tabuleiro / 4;
    }
    printf("Boleiro.\n");
    int tabuleiro[TAM_Tabuleiro][TAM_Tabuleiro];
    inicializador(tabuleiro, Tam_Inicial);
    clock_t tempo = clock();
    printf("Yes.\n");
    for (int i = 0; i < NUM_Execucoes; i++)
    {
        //show(tabuleiro);
        executarJogoDaVida(tabuleiro, 0, TAM_Tabuleiro);
        //usleep(200000);
        printf("Execucao: %d.\n", i);
    }
    tempo = clock() - tempo;
    printf("Tempo de exe: %lf", ((double)tempo) / (CLOCKS_PER_SEC / 1000));
    return 0;
}