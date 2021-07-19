#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//                                   ENUM

/* A seguir temos os estados possíveis dos personagens da nossa barbearia.*/
typedef enum estados_personagens
{
	default_, //estado que se aplica a todos
	entrou,	  //estado que se aplica ao que entra na barbearia
	desistente,
	atendido, //estados que se aplicam aos que saem da barbearia
	ocupado,  //estado no banco de espera
	dormindo,
	trabalhando //estados do barbeiro
} estados;

/* Tipo de cabelo que o cliente possui*/
typedef enum tipo_de_cabelo
{
	curto,
	mediano,
	longo,
	black_power,
} cabelos;
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

//                                  STRUCTS

/* A struct cliente se refere tanto ao personagem que entra na barbearia, como ao que sai.
   os char se referem as partes do corpo do personagem.*/
struct Cliente
{
	int numero;
	cabelos meuCabelo;
	char cabeca[6];
	char olhos[8];
	char corpo[11];
	char lateral_dir[3];
	char lateral_esq[3];
	char pernas[6];
} cliente_default = {
	0,			   //numero
	mediano,	   //tipo de cabelo
	"     \0",	   //cabeca
	"       \0",   //olhos
	"         \0", //corpo
	"  \0",		   //lateral direita
	"  \0",		   //lateral esquerda
	"     \0"	   //pernas
};
typedef struct Cliente Cliente;

Cliente *novo_cliente(int id)
{
	Cliente *novo_cliente = malloc(sizeof(Cliente));
	int cabelo = rand() % 4;
	novo_cliente->meuCabelo = cabelo;
	novo_cliente->numero = id;
	return novo_cliente;
}

/* A struct banco_de_espera se refere aos clientes que chegam, mas ainda nao podem cortar o cabelo, portanto
   ali sentam para esperar. Os char se referem as partes do corpo do personagem.*/
struct banco_de_espera
{
	int numero;
	char cabeca[6];
	char olhos[8];
	char corpo[11];
	char lateral_dir[3];
	char lateral_esq[3];
} banco_default = {
	0,			   //numero
	"     \0",	   //cabeca
	"       \0",   //olhos
	"         \0", //corpo
	"  \0",		   //lateral direita
	"  \0"		   //lateral esquerda
};
typedef struct banco_de_espera banco;

/* A struct cadeira_do_barbeiro se ao conjunto barbeiro (que dorme ou trabalha) e cliente (que senta na cadeira).
   Os char se referem as partes do corpo do barbeiro e cliente.*/
struct cadeira_do_barbeiro
{
	int numero_cliente;
	char cabeca_cliente[6];
	char olhos_cliente[8];
	char corpo_cliente[11];
	char lateral_dir_cliente[3];
	char lateral_esq_cliente[3];

	int numero_barbeiro;
	char cabeca_barbeiro[6];
	char olhos_barbeiro[10];
	char boca_barbeiro[11];
	char lateral_dir_barbeiro[4];
	char lateral_esq_barbeiro[4];
	char pernas_barbeiro[6];
} barbeiro_default = {0, "     \0", "       \0", "         \0", "  \0", "  \0",
					  0, "     \0", "         \0", "         \0", "   \0", "   \0", "     \0"};
typedef struct cadeira_do_barbeiro barbeiro;

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//                                    FUNCOES ATUALIZADORAS

/* Funcao para atualizar o estado dos clientes (que entra ou sai). Para os que entram os estados possiveis sao entrou e default que
corresponde a nao ter o personagem. Para os que saem sao desistente (desistiu de esperar), atendido (cortou o cabelo) e default (vazio).*/
void atualiza_cliente(int estado, int numero, Cliente *cliente)
{
	switch (estado)
	{
	case entrou:
		cliente->numero = numero + 1;
		if (cliente->meuCabelo == curto)
		{
			strcpy(cliente->cabeca, "_///_\0");
		}
		else if (cliente->meuCabelo == mediano)
		{
			strcpy(cliente->cabeca, "_|||_\0");
		}
		else if (cliente->meuCabelo == longo)
		{
			strcpy(cliente->cabeca, "_@@@_\0");
		}
		else
		{
			strcpy(cliente->cabeca, "_$$$_\0");
		}
		strcpy(cliente->olhos, "| o o |\0");
		strcpy(cliente->corpo, "/|__c__|\\\0");
		strcpy(cliente->lateral_dir, "|_\0");
		strcpy(cliente->lateral_esq, "_|\0");
		strcpy(cliente->pernas, "|   |\0");
		break;
	case desistente:
		cliente->numero = numero + 1;
		if (cliente->meuCabelo == curto)
		{
			strcpy(cliente->cabeca, "_///_\0");
		}
		else if (cliente->meuCabelo == mediano)
		{
			strcpy(cliente->cabeca, "_|||_\0");
		}
		else if (cliente->meuCabelo == longo)
		{
			strcpy(cliente->cabeca, "_@@@_\0");
		}
		else
		{
			strcpy(cliente->cabeca, "_$$$_\0");
		}
		strcpy(cliente->olhos, "| > < |\0");
		strcpy(cliente->corpo, "\\|__~__|/\0");
		strcpy(cliente->lateral_dir, "|_\0");
		strcpy(cliente->lateral_esq, "_|\0");
		strcpy(cliente->pernas, "|   |\0");
		break;
	case atendido:
		cliente->numero = numero + 1;
		strcpy(cliente->cabeca, "_____\0");
		strcpy(cliente->olhos, "| ^ ^ |\0");
		strcpy(cliente->corpo, "/|__U__|\\\0");
		strcpy(cliente->lateral_dir, "|_\0");
		strcpy(cliente->lateral_esq, "_|\0");
		strcpy(cliente->pernas, "|   |\0");
		break;
	default:
		cliente->numero = 0;
		strcpy(cliente->cabeca, "     \0");
		strcpy(cliente->olhos, "       \0");
		strcpy(cliente->corpo, "         \0");
		strcpy(cliente->lateral_dir, "  \0");
		strcpy(cliente->lateral_esq, "  \0");
		strcpy(cliente->pernas, "     \0");
	}
}

/* Funcao para atualizar o estado dos bancos. Os estados possiveis sao ocupado e default. O cliente espera no banco para ser atendido.*/

void atualiza_banco(int estado, int numero, banco *banco, int posicao, cabelos meuCabelo)
{
	switch (estado)
	{
	case ocupado:
		banco[posicao].numero = numero + 1;
		if (meuCabelo == curto)
		{
			strcpy(banco[posicao].cabeca, "_///_\0");
		}
		else if (meuCabelo == mediano)
		{
			strcpy(banco[posicao].cabeca, "_|||_\0");
		}
		else if (meuCabelo == longo)
		{
			strcpy(banco[posicao].cabeca, "_@@@_\0");
		}
		else
		{
			strcpy(banco[posicao].cabeca, "_$$$_\0");
		}
		strcpy(banco[posicao].olhos, "| o o |\0");
		strcpy(banco[posicao].corpo, "/|__c__|\\\0");
		strcpy(banco[posicao].lateral_dir, "|_\0");
		strcpy(banco[posicao].lateral_esq, "_|\0");
		break;
	default:
		banco[posicao].numero = 0;
		strcpy(banco[posicao].cabeca, "     \0");
		strcpy(banco[posicao].olhos, "       \0");
		strcpy(banco[posicao].corpo, "         \0");
		strcpy(banco[posicao].lateral_dir, "  \0");
		strcpy(banco[posicao].lateral_esq, "  \0");
	}
}

/* Funcao para atualizar o estado dos barbeiros. Quando esta dormindo nao tem cliente na cadeira. Trabalhando aparece o cliente na cadeira. Default fica sem barbeiro e sem cliente sentado.*/

void atualiza_barbeiro(int estado, int numero_cliente, barbeiro *cadeira, int assento)
{
	switch (estado)
	{
	case dormindo:
		cadeira[assento].numero_cliente = 0;
		strcpy(cadeira[assento].cabeca_cliente, "     \0");
		strcpy(cadeira[assento].olhos_cliente, "       \0");
		strcpy(cadeira[assento].corpo_cliente, "         \0");
		strcpy(cadeira[assento].lateral_dir_cliente, "  \0");
		strcpy(cadeira[assento].lateral_esq_cliente, "  \0");

		cadeira[assento].numero_barbeiro = assento + 1;
		strcpy(cadeira[assento].cabeca_barbeiro, "_zzz_\0");
		strcpy(cadeira[assento].olhos_barbeiro, " | - - | \0");
		strcpy(cadeira[assento].boca_barbeiro, " |__o__| \0");
		strcpy(cadeira[assento].lateral_dir_barbeiro, "__|\0");
		strcpy(cadeira[assento].lateral_esq_barbeiro, "|_B\0");
		strcpy(cadeira[assento].pernas_barbeiro, "|   |\0");
		break;
	case trabalhando:
		cadeira[assento].numero_cliente = numero_cliente + 1;
		strcpy(cadeira[assento].cabeca_cliente, "__|__\0");
		strcpy(cadeira[assento].olhos_cliente, "| o o |\0");
		strcpy(cadeira[assento].corpo_cliente, "/|__U__|\\\0");
		strcpy(cadeira[assento].lateral_dir_cliente, "|_\0");
		strcpy(cadeira[assento].lateral_esq_cliente, "_|\0");

		cadeira[assento].numero_barbeiro = assento + 1;
		strcpy(cadeira[assento].cabeca_barbeiro, "_____\0");
		strcpy(cadeira[assento].olhos_barbeiro, "Y| o o |E\0");
		strcpy(cadeira[assento].boca_barbeiro, "\\|__c__|/\0");
		strcpy(cadeira[assento].lateral_dir_barbeiro, "__|\0");
		strcpy(cadeira[assento].lateral_esq_barbeiro, "|_B\0");
		strcpy(cadeira[assento].pernas_barbeiro, "|   |\0");
		break;
	default:
		cadeira[assento].numero_cliente = 0;
		strcpy(cadeira[assento].cabeca_cliente, "     \0");
		strcpy(cadeira[assento].olhos_cliente, "       \0");
		strcpy(cadeira[assento].corpo_cliente, "         \0");
		strcpy(cadeira[assento].lateral_dir_cliente, "  \0");
		strcpy(cadeira[assento].lateral_esq_cliente, "  \0");

		cadeira[assento].numero_barbeiro = 0;
		strcpy(cadeira[assento].cabeca_barbeiro, "     \0");
		strcpy(cadeira[assento].olhos_barbeiro, "         \0");
		strcpy(cadeira[assento].boca_barbeiro, "         \0");
		strcpy(cadeira[assento].lateral_dir_barbeiro, "   \0");
		strcpy(cadeira[assento].lateral_esq_barbeiro, "   \0");
		strcpy(cadeira[assento].pernas_barbeiro, "     \0");
	}
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

void clearScreen()
{
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

//                                 FUNCAO PARA IMPRIMIR
void imprime_estado(Cliente entrando, Cliente saindo, banco *sentado, barbeiro *cadeira)
{
	clearScreen();
	printf("                                   BARBEARIA          \n");
	printf("   ____________________________________________________________________\n");
	printf("   |Entrada|                                                           |\n");
	printf("   |_______|                                                           |\n");
	printf("   |                                                                   |\n");
	printf("   |     %s            l  %s     l  %s                        |\n",
		   entrando.cabeca, cadeira[0].cabeca_cliente, cadeira[1].cabeca_cliente);
	printf("   |    %s           l %s    l %s                       |\n",
		   entrando.olhos, cadeira[0].olhos_cliente, cadeira[1].olhos_cliente);
	printf("   |   %s          l%s   l%s                      |\n",
		   entrando.corpo, cadeira[0].corpo_cliente, cadeira[1].corpo_cliente);
	printf("   |    %s%03d%s           l %s%03d%s    l %s%03d%s                       |\n",
		   entrando.lateral_dir, entrando.numero, entrando.lateral_esq,
		   cadeira[0].lateral_dir_cliente, cadeira[0].numero_cliente, cadeira[0].lateral_esq_cliente,
		   cadeira[1].lateral_dir_cliente, cadeira[1].numero_cliente, cadeira[1].lateral_esq_cliente);
	printf("   |     %s            l~~~~~~~~    l~~~~~~~~                       |\n", entrando.pernas);
	printf("   |                      L       L    L       L                       |\n");
	printf("   |                        %s        %s                         |\n",
		   cadeira[0].cabeca_barbeiro, cadeira[1].cabeca_barbeiro);
	printf("   |                      %s    %s                       |\n",
		   cadeira[0].olhos_barbeiro, cadeira[1].olhos_barbeiro);
	printf("   |__________________    %s    %s                       |\n",
		   cadeira[0].boca_barbeiro, cadeira[1].boca_barbeiro);
	printf("   |Saida|                 %s%01d%s      %s%01d%s                        |\n",
		   cadeira[0].lateral_esq_barbeiro, cadeira[0].numero_barbeiro, cadeira[0].lateral_dir_barbeiro,
		   cadeira[1].lateral_esq_barbeiro, cadeira[1].numero_barbeiro, cadeira[1].lateral_dir_barbeiro);
	printf("   |_____|                  %s        %s                         |\n",
		   cadeira[0].pernas_barbeiro, cadeira[1].pernas_barbeiro);
	printf("   |                                                                   |\n");
	printf("   |     %s         %s     %s     %s     %s             |\n",
		   saindo.cabeca, sentado[0].cabeca, sentado[1].cabeca, sentado[2].cabeca, sentado[3].cabeca);
	printf("   |    %s       %s   %s   %s   %s            |\n",
		   saindo.olhos, sentado[0].olhos, sentado[1].olhos, sentado[2].olhos, sentado[3].olhos);
	printf("   |   %s     %s %s %s %s           |\n",
		   saindo.corpo, sentado[0].corpo, sentado[1].corpo, sentado[2].corpo, sentado[3].corpo);
	printf("   |    %s%03d%s       %s%03d%s   %s%03d%s   %s%03d%s   %s%03d%s            |\n",
		   saindo.lateral_dir, saindo.numero, saindo.lateral_esq,
		   sentado[0].lateral_dir, sentado[0].numero, sentado[0].lateral_esq,
		   sentado[1].lateral_dir, sentado[1].numero, sentado[1].lateral_esq,
		   sentado[2].lateral_dir, sentado[2].numero, sentado[2].lateral_esq,
		   sentado[3].lateral_dir, sentado[3].numero, sentado[3].lateral_esq);
	printf("   |     %s       --------- --------- --------- ---------           |\n", saindo.pernas);
	printf("   |                    /C1\\      /C2\\      /C3\\      /C4\\             |\n");
	printf("   |                                                                   |\n");
	printf("   |                   %s     %s     %s     %s             |\n",
		   sentado[4].cabeca, sentado[5].cabeca, sentado[6].cabeca, sentado[7].cabeca);
	printf("   |                  %s   %s   %s   %s            |\n",
		   sentado[4].olhos, sentado[5].olhos, sentado[6].olhos, sentado[7].olhos);
	printf("   |                 %s %s %s %s           |\n",
		   sentado[4].corpo, sentado[5].corpo, sentado[6].corpo, sentado[7].corpo);
	printf("   |                  %s%03d%s   %s%03d%s   %s%03d%s   %s%03d%s            |\n",
		   sentado[4].lateral_dir, sentado[4].numero, sentado[4].lateral_esq,
		   sentado[5].lateral_dir, sentado[5].numero, sentado[5].lateral_esq,
		   sentado[6].lateral_dir, sentado[6].numero, sentado[6].lateral_esq,
		   sentado[7].lateral_dir, sentado[7].numero, sentado[7].lateral_esq);
	printf("   |                 --------- --------- --------- ---------           |\n");
	printf("   |                    /C5\\      /C6\\      /C7\\      /C8\\             |\n");
	printf("   |___________________________________________________________________|\n");
	sleep(1);
	fflush(stdout);
	return;
}