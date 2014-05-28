River Raid - Changelog
========================

##### TODO:
  - Descrever funções do *Grafico.c* e traduzir os nomes das mesmas. Resumindo, polir o módulo *Grafico*.
  - Nave às vezes não se move em duas direções simultaneamente.
  - Rever as constantes de cenário, inimigos, nave, etc. para balancear o jogo.
  - Esperar Gubi lançar a terceira parte.

Terceira parte
--------------

### Versão 0.8

###### 28/05:
  - Ângulos da nave redefinidos, sendo agora chamados de angHoriz e angVert para maior clareza.
  - Resolvido problema de espelhamento da tela através de um *glScale*.
  - O score é impresso de um jeito mais direto, usando *sprintf*.

###### 27/05:
  - ***Grafico.c***: Novo módulo para funções relacionadas ao OpenGL!
  - ***Main.c***: Possui agora o objetivo de inicializar o programa e chamar o loop do OpenGL. Algumas funções deste módulo foram usadas no *Grafico.c*.
  - No momento, a parte gráfica se resume a usar "esqueletos" de cone para a nave e inimigos, e esferas para os tiros. Há também a representação das vidas restantes, hp e score da nave. O usuário pode controlar a nave através do teclado.

###### 26/05:
  - ***Tiro.c***: Precisão finalmente corrigida e funcionando. Grande parte disso deve-se à função *atan2()*, extremamente útil na tarefa de encontrar valor de um ângulo baseado em coordenadas euclidianas.

Segunda parte
-------------

### Versão 0.6

###### 25/05:
  - ***Main.c***: Problemas relacionados à leitura de comandos corrigidos com sucesso.
  - ***Tiro.c***: Cálculo de precisão, antes feito erroneamente, corrigido. Ainda não parece estar funcionando de forma muito correta. Teremos uma ideia melhor do que acontece quando a parte visual for implementada.
  - Relatório feito. Segunda parte do projeto está oficialmente concluída.

###### 23/05:
  - ***Nave.c***: Dois conceitos fundamentais.
    - *Invencibilidade*: Nave torna-se imune a danos por um certo período de tempo após levar um tiro, e por um período ainda maior caso perca uma vida. A representação se dá por uma variável que decresce a cada timestep, similar ao conceito de cooldown. Quando chega a zero, invencibilidade acaba.
    - *Colisão com inimigos*: Implementação similar à da detecção dos projéteis, porém levando em conta dois corpos cilíndricos. 
  - ***Main.c***: Módulo de testes da segunda parte está pronto. Ao receber comandos do usuário, programa simula o pressionamento de tais teclas e passa as ações à nave. Ao fim do intervalo, usuário pode inserir novos comandos. O jogo segue como usual, até a nave morrer. Dessa vez aparecem novidades na tela, como pontuação, vidas, movimento, inclinação, tiros da nave, etc - englobando tudo o que foi feito nesta segunda parte.

###### 22/05:
  - Pontuação em ação no jogo. Pontos são presenteados ao jogador em caso de acerto ou destruição de inimigos. 
  - ***Tiro.h***: Novo atributo booleano *amigo*. Em caso de fogo amigo entre próprios inimigos, dano ocorre, mas pontos não são conferidos ao jogador.
  - ***Defesa.h***: Corrigido erro que fazia os inimigos atirarem no sentido contrário do eixo Oz, o que tornava praticamente impossível aos projéteis acertarem a nave.

###### 21/05:
  - Somos agora (não tão) orgulhosamente usuários do ***GitHub***. Dessa vez para valer.
  - Novo integrante adicionado ao grupo. Corrigidos erros imperdoáveis de português.
  - ***Tiro.c***: Detecção de saída do projétil passa a levar em conta limite *Z_MAX* da tela.
  - ***Nave.c, Defesa.c***: Corrigido posicionamento dos projéteis ao serem disparados. Removidas funções supérfluas.

### Versão 0.5

###### 17/05: 
  - Corrigido erros decorrentes das últimas modificações. Programa pode ser novamente compilado e rodado.
  - ***Nave.h***: Novo atributo *vidas*, representando o nº de chances do jogador.

###### 14/05:
  - ***Base.h***: Novo módulo com definições comuns que não se enquadram em nenhum outro lugar do projeto. Em especial, foi criada uma estrutura *Corpo*, com atributos básicos (posição, energia, recarga, formato) a serem usados comumente entre nave e inimigos.
  - ***Tiro.c***: Adicionada ação da gravidade sobre os projéteis.
  - ***Nave.h, Defesa.h***: Detecção de colisão passa a levar em conta nave e inimigos como corpos cilíndricos.
  - ***Nave.c***: Implementada alteração de direção da nave. Por meio de 4 teclas, o usuário pode alterar a inclinação da mesma, sob uma certa taxa, até um determinado limite. A nave tende naturalmente a voltar à posição orientada ao centro, sob uma taxa diferente.
  - ***Main.c***: Pequena função que recebe uma tecla (na verdade, uma letra), interpretando e executando o comando do usuário.

###### 10/05:
  - Somos agora orgulhosamente usuários do ***git***. Isso não só ajudará no controle e acompanhamento de versões como possibilitará uma visão mais ampla do projeto como um todo.
  - *Makefile* incrementado com novas opções.
    - ***make tar***: Cria um arquivo .tar com todos os arquivos necessários ao projeto.
    - ***make count***: Conta o número de linhas de código (por pura satisfação pessoal).

###### 09/05:
  - ***Main.c***: Alterado nome do módulo de testes, anteriormente conhecido como *Teste.c*.
  - Documentação reformulada. Descrições de funções foram agora movidas para os respectivos arquivos .h, ao passo que restaram nos .c, em geral, detalhes com respeito à implementação. Sessões também estão definidas de maneira mais clara.

Primeira parte
--------------

### Versão 0.3

###### 26/04:
  - Retirados #includes dos .h e transferidos para seus respectivos .c, nos casos em que isso fora possível.
  - ***Random.c***: Novo módulo para trabalhar com distribuições aleatórias (Uniforme discreta, Uniforme contínua e Normal).
  - ***Tiro.c***: Precisão (finalmente) implementada. Quanto menor for, maior será o desvios dos tiros em relação ao alvo designado. É calculada aleatoriamente segundo uma distribuição Normal.
  - ***Cenario.c***: Vários atributos dos inimigos agora são gerados aleatoriamente, produzindo assim maior diversidade nos testes.
  - ***Teste.c***: Informações agora são exibidas de forma mais clara na tela.
  - Adicionado pequeno bloco de comentário no começo dos módulos com um resumo do que fazem.
  - Realizado tudo isso, acho que podemos dizer que encerramos a primeira parte.

###### 24/04:
  - ***Teste.c***: Resultados agora são exibidos em uma tela por vez; espera-se o usuário apertar [Enter] e então a próxima é mostrada.
  - Revisão feita "in loco". Adicionados comentários ao lado de cada #include explicitando quais funções foram usadas de cada biblioteca.

###### 17/04:
  - Modificadas algumas contantes nos headers e corrigidas reclamações do gcc.
  - ***Tiro.c***: Corrigido bug dos projéteis serem liberados ao atingirem coordenada z da nave (ou seja, muitos eram eliminados ainda com chances de acertá-la), o que diminuiu drasticamente o tempo até nave ser destruída.
  - ***Teste.c***: Nova apresentação dos resultados. Agora o programa recebe uma semente por linha de comando ou, caso o usuário não insira nada, usa o tempo do sistema. Toda vez que a nave sofre um dano são exibidos a energia atual, sua posição, posição e tempo de recarga de todos os inimigos e, finalmente, coordenadas e velocidade de cada projétil. Ao fim do jogo, memória das listas é liberada.
  - **Bônus**: All heap blocks were freed -- no leaks are possible (!)

### Versão 0.2

###### 16/04:
  - ***Tiro.h***: Alterado tipo das coordenadas e velocidade de *int* para *double*, o que garante melhor precisão.
  - ***Tiro.c***: Projétil dos canhões são mirados, tomando como referência de direção o bico da nave.
  - ***Defesa.h, Defesa.c***: Novo atributo *recarga*, indicando tempo de espera entre um projétil e outro.
  - ***Cenario.c***: Nova função para gerar inimigos aleatoriamente no fundo do cenário.
  - ***Teste.c***: Módulo de testes (finalmente) criado. O próprio simulará a execução do programa, ainda sem interação, e imprimirá na tela o estado dos elementos a cada certo nº de frames, a ser determinado pelo usuário. A simulação acaba quando a energia da nave chegar a zero.
  - É oficial: o programa funciona.

###### 15/04:
  - ***Cenario.c***: Atualização de posições da nave e dos projéteis.

###### 14/04:
  - ***Tiro.c***: Atualização da posição dos tiros no cenário. Verificação da saída do projétil agora também conta com os limites x e y da tela.
  - ***Nave.c***: Atualização da posição da nave.
  - ***Cenario.c***: Vasculhamento de todos os inimigos/projéteis num determinado frame, eliminando os que saírem da tela ou acertarem a nave, sendo removida energia da mesma neste último caso.

###### 13/04:
  - ***Defesa.c, Tiro.c***: Adicionadas funções de manipulação de lista.
  - ***Nave.c***: Inicialização dos atributos da nave.
  - ***Tiro.c***: Funções para verificar se um determinado tiro saiu da tela ou acertou a nave.
  - A princípio, com propósito de testes, definimos tanto tiro quanto nave como corpos esféricos. Nas versões futuras isso provavelmente será removido; o OpenGL deve lidar com a verificação de colisões de formatos variados.

### Versão 0.1

###### 12/04:
  - ***Lista.c***: Criado módulo com operações básicas de lista ligada.
  - Decidido que o melhor será usar listas ligadas comuns; como as remoções não necessitam serem feitas explicitamente nas extremidades, perde-se a razão de trabalhar com filas ou pilhas.
  - Listas agora podem receber qualquer tipo de dados, sejam inimigos, projéteis ou o que mais puder haver (conseguimos isso por meio de apontadores genéricos (void *) e outras modificações).

###### 11/04:
  - ***Defesa.h, Tiro.h***: Definidos atributos básicos.

###### 09/04: 
  - Corpo do código montado, criação de alguns .h
