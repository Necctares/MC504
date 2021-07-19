Integrantes:
Alan Freitas Ribeiro - 193400
Leandro Watanabe - 171860
Renan Hiroki - 176573
Leandro Silva - 121092

O problema escolhido por nós foi o do barbeiro dorminho, onde no codigo utilizamos semaforos para controlar o fluxo de execução
em uma barbearia. Em nossa implementação os barbeiros ficam na espera de clientes que vão chegando em um determinado tempo na barbearia,
os clientes sentam na cadeira de espera, e se tiver vaga em um dos barbeiros, saem da cadeira, por ordem de chegada e vão para a cadeira do barbeiro, iniciando assim o corte.
Para o controle do fluxo na barbearia, em nossa implementação, enfrentamos um problema de prioridade, onde havia disputa de ações entre um cliente
entrar na barbearia vs cliente da cadeira ir para uma cadeira de barbeiro vazia. Implementamos varias versões de codigo tentando consertar esse problema, porém, ao conseguir resolve-lo enfrentamos novos problemas, como por exemplo, o cliente terminava o corte e ficava preso na cadeira do barbeiro por muito tempo. No fim, optamos por entregar essa versão, em que existe uma disputa de ações entre o cliente da entrada e o que está no banco de espera, e há uma prioridade para os clientes que já terminaram o corte irem embora da barbearia, em que consideramos ser a mais aceitavel. (As vezes, mesmo com a barbearia cheia, o barbeiro precisa tirar uma folga :D)
No codigo, podem ser alterados os parametros N_CLIENTES, N_BARBEIROS e N_CADEIRAS, onde N_BARBEIROS deve ser respeitado o numero de 1-2 barbeiros e
N_CADEIRAS um numero entre 0-8, por questões de limitação da animação (nosso artista foi meio preguiçoso), o numero de N_CLIENTES pode ser alterado a vontade.
Alguns detalhes da implementação:
-Cada cliente possui um tipo de cabelo (atribuido aleatoriamente durante sua criação), Curto (8s), Medio(10s), Longo(12s) e Black-power(16s), que leva um determinado tempo a ser cortado.
-Se o cliente da entrada chegar e N_CADEIRAS já tiver em seu limite, ele irá embora, mesmo que na animação mostre 8, caso N_CADEIRAS for menor que 8 estas cadeiras não serão ocupadas.
