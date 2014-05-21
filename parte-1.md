Relatório
=========

  - Integrantes:
    - (**7557797**) Gustavo Chicato Wandeur
    - (**8516782**) Daniel Toshio Iwamoto

Introdução
----------
  O principal objetivo nesta primeira parte do projeto foi montar a estrutura básica do jogo, ainda sem se preocupar muito com a parte criativa propriamente dita. Nossa meta a princípio foi resolver tudo que não necessitasse nem de controles nem da parte visual; ou seja, adiantar o máximo possível de coisas.
  
  Procuramos definir bem a interface, além de documentar com clareza o código, colocando comentários a respeito de cada atributo e função. Também separamos com cautela o que deveria ir nos respectivos .c e .h. A intenção foi não só manter o código legível e fácil de entender, mas também preparar o caminho para simplificar modificações e implementações futuras nas próximas partes do projeto.
  
  Adicionalmente, organizamos os arquivos em pastas separadas (arquivos C, headers, objetos, etc). Para compilar, basta ir até o diretório raiz do jogo e digitar "make" no terminal, o que deverá cuidar do resto.
  
Resumo do que foi feito
-----------------------
  - Todos os módulos e estruturas básicas descritas no pdf
  - Módulos adicionais, como manipulação de listas (Lista.c) e números aleatórios (Random.c)
  - Várias funções que manipulam elementos, como movimentação dos objetos, atualização do cenário, verificação de acerto, saída do campo de jogo, etc
  - Geração dinâmica de inimigos e projéteis, com características aleatórias ou pré-estipuladas
  - Definido, para cada inimigo, tempo de cooldown (ou recarga), a ser esperado entre um tiro e outro
  - Definido modelo para a precisão dos tiros, baseado em variação angular sob uma distribuição Normal
  - Alocação e liberação de memória feita com sucesso - Valgrind aponta zero vazamentos e zero erros!

Simulação
---------
  Para testar o jogo, foi criado um módulo exclusivo para isso, que chamamos criativamente de Teste.c.
  
  Esse módulo trata de simular a execução de um jogo, ainda sem interação com o usuário. A nave se desloca progressivamente pelo meio da tela com velocidade constante, enquanto inimigos fixos são gerados aleatoriamente de tempos em tempos. Cada vez que a nave for atingida, é mostrado um relatório na tela com a posição e características de cada elemento (nave, inimigos e projéteis; note que, no caso dos dois últimos, a coordenada z é relativa à nave).
  
  O teste prossegue até a nave ser destruída, ou seja, sua energia chegar a 0.
  
  Embora um monte de números na tela em pouco lembre um jogo, a simulação é essencial para averiguar o bom funcionamento do programa. Todas as funções foram testadas e, aparentemente, está tudo ok. Assim, basta agora aguardar a implementação da próxima fase do projeto.
