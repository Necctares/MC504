#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TAM_Tabuleiro 30
#define Celula_Viva 1
#define Celula_Morta 0
#define NUM_Execucoes 100
#define NUM_THREAD 2

int tabuleiro[TAM_Tabuleiro][TAM_Tabuleiro];
int tab_aux[TAM_Tabuleiro][TAM_Tabuleiro];
int passo = TAM_Tabuleiro / NUM_THREAD;

//Função show adaptada do site: https://rosettacode.org/wiki/Conway%27s_Game_of_Life#C
void show()
{
    printf("\033[H");
    for (int i = 0; i < TAM_Tabuleiro; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
            printf(tabuleiro[i][j] ? "\033[07m  \033[m" : "  ");
        printf("\033[E");
    }
    fflush(stdout);
    return;
}

void inicializador(int Tam_Inicial)
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

void copiar_tabuleiro()
{
    for (int i = 0; i < TAM_Tabuleiro; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
        {
            tabuleiro[i][j] = tab_aux[i][j];
        }
    }
    return;
}

int contar_vizinho(int posx, int posy)
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

void *executarJogoDaVida(void *arg)
{
    int inicio = (int)arg;
    int num_vizinhos;
    for (int i = inicio; i < inicio + passo; i++)
    {
        for (int j = 0; j < TAM_Tabuleiro; j++)
        {
            num_vizinhos = contar_vizinho(i, j);
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
    return NULL;
}

void gerenciar_threads()
{
    int ini = 0;
    pthread_t threads[NUM_THREAD];
    pthread_create(&threads[0], NULL, executarJogoDaVida, (void *)ini);
    for (int i = 1; i < NUM_THREAD; i++)
    {
        ini += passo;
        pthread_create(&threads[i], NULL, executarJogoDaVida, (void *)ini);
    }
    for (int i = 0; i < NUM_THREAD; i++)
    {
        pthread_join(threads[i], NULL);
    }
    copiar_tabuleiro();
    return;
}

int main(int argc, char *argv[])
{
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
    inicializador(Tam_Inicial);
    for (int i = 0; i < NUM_Execucoes; i++)
    {
        show();
        gerenciar_threads();
        usleep(200000);
        printf("Execucao: %d.\n", i);
    }
    return 0;
}