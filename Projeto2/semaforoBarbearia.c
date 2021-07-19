#include <pthread.h>
#include <semaphore.h>
#include "barbearia.c"

#define N_CLIENTES 15 //Numero maximo de clientes
#define N_BARBEIROS 2 //Numero de barbeiros
#define N_CADEIRAS 8  //Numero de cadeiras para os clientes esperarem

//**** Aviso! Não mexer nas configurações abaixo!****
#define MAX_BARBEIROS 2 //Numero maximo de cadeiras de barbeiros
#define MAX_CADEIRAS 8 //Numero maximo de cadeiras para os clientes esperarem

/* Função para executar as ações dos barbeiros. */
void *barbearia(void *v);
/* Função para executar as ações dos clientes. */
void *cliente(void *v);
/* Função que executa o corte. */
void executar_corte(cabelos tipo);
/* Função que cria um novo cliente */
void *criar_cliente();

sem_t cadeiras;                      //Semaforo para controlar o estado da cadeira do cliente atual
sem_t cadeira_barbeiro[N_BARBEIROS]; //Semaforo para controlar as cadeiras dos barbeiros
sem_t cabelo_cortado[N_BARBEIROS];   //Semaforo para controlar o corte de cabelo
sem_t cliente_cadeira[N_BARBEIROS];  //Semaforo para controlar se o cliente está na cadeira do barbeiro ou não
sem_t visor;                         //Semaforo que controla a escrita dos barbeiros no visor
sem_t ler_visor;                     //Semaforo que controla a leitura do visor
sem_t estado;                        //Semaforo para controlar os estados dos personagens
sem_t prioridade_ao_sair;            //Semaforo para dar prioridade para clientes que terminaram de cortar o cabelo
int num_visor;                       //Variavel que guarda o numero atual no visor

estados estado_cliente[N_CLIENTES];     //Variavel para armazenar o estado atual do cliente de id[x]
estados estado_barbeiros[N_BARBEIROS];  //Variavel para armazenar o estado atual do barbeiro de id[x]
int cliente_cadeira_espera[N_CADEIRAS]; //Variavel para armazenar o cliente atual que ocupa a cadeira de espera x
int estado_cadeira_espera[N_CADEIRAS];  //Variavel para armazenar o estado atual da cadeira de espera x. (0 = Vazia, 1 = Ocupada)
int cliente_do_barbeiro[N_BARBEIROS]; //Variavel para armazenar o cliente atual do barbeiro de id[x]
int cabelo_a_ser_cortado[N_BARBEIROS]; //Variavel para armazenar o tipo de cabelo que cada barbeiro esta cortando atualmente

Cliente entrando; //Cliente que vai entrar na barbearia
Cliente saindo;   //Cliente que está saindo da barbearia
barbeiro cadeira[MAX_BARBEIROS];    //Cadeira do barbeiro
banco lugar[MAX_CADEIRAS];          //Bancos de espera

void *barbearia(void *v)
{
    //Armazena o id do barbeiro
    int id = *(int *)v;

    while (1)
    {
        sem_wait(&visor);
        num_visor = id;
        //printf("Barbeiro %d esta livre.\n", num_visor + 1);       //
        sem_post(&ler_visor);

        sem_wait(&cliente_cadeira[id]);
        executar_corte(cabelo_a_ser_cortado[id]);
        sem_post(&cabelo_cortado[id]);
    }
    return NULL;
}

void *cliente(void *v)
{
    Cliente *cliente_atual = (Cliente *)v;
    int id = cliente_atual->numero;
    int i;
    int cadeira_atribuida, minha_cadeira_espera;

    sleep(rand() % 20);
    sem_wait(&prioridade_ao_sair);
    sem_wait(&estado);
    //AÇÃO: Cliente entrou na barbearia
    	//printf("Cliente %d entrou na barbearia.\n", id + 1);     //
    	estado_cliente[id] = entrou;
        entrando.meuCabelo = cliente_atual->meuCabelo;                             //
    	atualiza_cliente(estado_cliente[id], id, &entrando);    //
    	imprime_estado(entrando, saindo, lugar, cadeira);      //

    if (sem_trywait(&cadeiras) == 0)
    {
        //AÇÃO: Cliente conseguiu vaga na barbearia
        int achou_cadeira = 0;
        i = 0;
        //Procura uma cadeira disponivel
        while (i < N_CADEIRAS && !achou_cadeira)
        {
            if (estado_cadeira_espera[i] == 0)
            {
                estado_cadeira_espera[i] = 1;
                cliente_cadeira_espera[i] = id;
                minha_cadeira_espera = i;
                achou_cadeira = 1;
            }
            i += 1;
        }
        //Ação: Cliente espera na cadeira
        //printf("Cliente %d esta na espera na cadeira %d.\n", id + 1, minha_cadeira_espera + 1);    //
        atualiza_banco(ocupado, id, lugar, minha_cadeira_espera, cliente_atual->meuCabelo);                                  //
        if(lugar[minha_cadeira_espera].numero == entrando.numero){                                 //
        	atualiza_cliente(default_, 0, &entrando);                                           //
        }                                                                                          //
        imprime_estado(entrando, saindo, lugar, cadeira);                                          //
        sem_post(&estado);
        sem_post(&prioridade_ao_sair);

        //Cliente espera até um barbeiro estar vago
        sem_wait(&ler_visor);
        cadeira_atribuida = num_visor;

        sem_post(&visor);
        //Cliente espera o cliente atual sair da cadeira do barbeiro
        sem_wait(&cadeira_barbeiro[cadeira_atribuida]);
        //Cliente desocupa a cadeira que ele estava esperando
        sem_wait(&estado);
        sem_post(&cliente_cadeira[cadeira_atribuida]);

        //Libera a cadeira de espera para um novo cliente
        sem_post(&cadeiras);
        //printf("Cliente %d levantou da cadeira de espera %d.\n", id + 1, minha_cadeira_espera + 1);
        atualiza_banco(default_, 0, lugar, minha_cadeira_espera, -1); //
        imprime_estado(entrando, saindo, lugar, cadeira);         //
        //Altera os estados do personagem
        estado_cliente[id] = ocupado;
        estado_barbeiros[cadeira_atribuida] = trabalhando;
        //Seta os atributos do cliente para o barbeiro correto
        cabelo_a_ser_cortado[cadeira_atribuida] = cliente_atual->meuCabelo;

        cliente_do_barbeiro[cadeira_atribuida] = id;

        estado_cadeira_espera[minha_cadeira_espera] = 0;
        //AÇÃO: Cliente começa a cortar o cabelo com o barbeiro
        //printf("Cliente %d comecou a cortar o cabelo com o Barbeiro %d.\n", id + 1, cadeira_atribuida + 1);      //
        atualiza_barbeiro(trabalhando, id, cadeira, cadeira_atribuida); //
        imprime_estado(entrando, saindo, lugar, cadeira);         //
        sem_post(&estado);

        sem_wait(&cabelo_cortado[cadeira_atribuida]);

        sem_wait(&estado);
        //AÇÃO: Cliente sai da barbearia
        estado_cliente[id] = atendido;
        estado_barbeiros[cadeira_atribuida] = dormindo;
        //printf("Cliente %d terminou de cortar o cabelo e foi embora.\n", id + 1);    //
        atualiza_barbeiro(dormindo, 0, cadeira, cadeira_atribuida);                  //
        atualiza_cliente(atendido, id, &saindo);                                     //
        imprime_estado(entrando, saindo, lugar, cadeira);
        atualiza_cliente(default_, id, &saindo);
        imprime_estado(entrando, saindo, lugar, cadeira);                 //
        sem_post(&cadeira_barbeiro[cadeira_atribuida]);

        sem_post(&estado);
    }
    else
    {
        //AÇÃO: Cliente desiste e vai embora
        estado_cliente[id] = desistente;
        atualiza_cliente(estado_cliente[id], id, &saindo);    //
        if(entrando.numero == saindo.numero){                 //
        	atualiza_cliente(default_, 0, &entrando);     //
        }                                                     //
        //printf("Cliente %d nao achou vaga e foi embora.\n", id + 1);              //
        imprime_estado(entrando, saindo, lugar, cadeira);      //
        atualiza_cliente(default_, id, &saindo);
        imprime_estado(entrando, saindo, lugar, cadeira);      //
        sem_post(&estado);
        sem_post(&prioridade_ao_sair);
    }
    return NULL;
}

void executar_corte(cabelos tipo)
{
    //Calcula o tempo total, para cada corte especifico.
    int s = 0;
    switch (tipo)
    {
    case mediano:
        s += 10;
        break;
    case longo:
        s += 12;
        break;
    case black_power:
        s += 16;
        break;
    default:
        //Corte do cabelo curto
        s +=8;
        break;
    }
    sleep(s);
    return;
}

int main()
{
    srand(time(NULL)); //Inicializa uma nova seed para a função rand.
    pthread_t thread_barbeiros[N_BARBEIROS];
    pthread_t thread_clientes;
    //Inicializa os respectivos semaforos
    sem_init(&cadeiras, 0, N_CADEIRAS);
    sem_init(&estado, 0, 1);
    sem_init(&visor, 0, 1);
    sem_init(&ler_visor, 0, 0);
    sem_init(&prioridade_ao_sair, 0, 1);
    //Inicializa as structs responsaveis pelas animações

    entrando = cliente_default; //
    saindo = cliente_default;   //
    for (int i = 0; i < MAX_BARBEIROS; i++){             //
    	atualiza_barbeiro(default_, 0, cadeira, i); //
    }                                                    //

    for (int i = 0; i < MAX_CADEIRAS; i++){              //
    	atualiza_banco(default_, 0, lugar, i, -1);      //
    }                                                    //

    //Inicializa os semaforos de controle do barbeiro e seta os estados padrão de cada personagem
    for (int i = 0; i < N_BARBEIROS; i++)
    {
        sem_init(&cadeira_barbeiro[i], 0, 1);
        sem_init(&cliente_cadeira[i], 0, 0);
        sem_init(&cabelo_cortado[i], 0, 0);

        estado_barbeiros[i] = dormindo;
        atualiza_barbeiro(estado_barbeiros[i], 0, cadeira, i); //
        imprime_estado(entrando, saindo, lugar, cadeira);      //
    }

    for (int i = 0; i < N_CLIENTES; i++)
    {
        estado_cliente[i] = default_;
    }
    //Seta todas as cadeiras de espera como disponivel
    for (int i = 0; i < N_CADEIRAS; i++)
    {
        estado_cadeira_espera[i] = 0;
    }
    //Inicializa as threads dos barbeiros
    int id_barbeiro[N_BARBEIROS];
    for (int i = 0; i < N_BARBEIROS; i++)
    {
        id_barbeiro[i] = i;
        pthread_create(&thread_barbeiros[i], NULL, (void *)barbearia, (void *)&id_barbeiro[i]);
    }
    //Inicializa a thread para gerar os clientes
    pthread_t nova_thread[N_CLIENTES];
    int checar_thread;
    int contador = 0;
    //Cria os clientes até o numero maximo de clientes permitidos
    while (contador < N_CLIENTES)
    {
        Cliente *cliente_novo = novo_cliente(contador);
        checar_thread = pthread_create(&nova_thread[contador], NULL, (void *)cliente, (void *)cliente_novo);
        //if (!checar_thread)
        //    printf("Cliente %d esta saindo de casa para cortar o cabelo.\n", contador + 1);                 //
        contador += 1;
    }

    for (int i = 0; i < contador; i++)
    {
        pthread_join(nova_thread[i], NULL);
    }

    return 0;
}
