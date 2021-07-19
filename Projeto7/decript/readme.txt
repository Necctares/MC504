Grupo formado por:
1 - Alan Freitas Ribeiro, RA 193400
2 - Renan Hiroki Bastos, RA 176573
3 - Leandro Setsuo Watanabe, RA 171860
4 - Leandro Helio Ferreira da Silva, RA 121092

Link do video = https://youtu.be/KHA92rJPTaU

Implementação

Decidimos implementar uma ideia basica para testar o driver, onde o usuario passa comandos (int) e o driver devolve uma ação referente a este inteiro.
Onde temos a seguinte range (0-6):
0 - Direita, 1 - Esquerda, 2 - Cima, 3 - Baixo, 4 - Abra a caixa, 5 - Aperte o botao, 6 - Corra

Implementamos a função de escrita e leitura, além do IOCTL.
Para o IOCTL implementamos duas funções:
DECRIPT_SET_COMMAND _IOW('q', 1, decript_arg_t *), função para setar um novo comando.
DECRIPT_GET_INSTRUCT _IOR('q', 2, decript_arg_t *), função para pegar a ação referente ao comando atual.
Obs: Caso não seja setado nenhum comando antes de pegar a ação referente, ele devolverá o valor default referente ao comando (0) => Direita.
