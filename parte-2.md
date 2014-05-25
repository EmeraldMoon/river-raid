Relatório - Parte 2
===================

  - Integrantes:
    - (**8516782**) Daniel Toshio Iwamoto
    - (**7557797**) Gustavo Chicato Wandeur
    - (**8536065**) Leonardo Pereira Macedo

Introdução
----------
  O objetivo principal desta segunda parte do projeto foi dar continuidade às modificações adiantadas na primeira fase. Como boa parte da dinâmica já estava implementada, bastou dar luz aos detalhes que faltavam e criar uma simulação mais interativa do jogo, desta vez por meio do recebimento de comandos do usuário.


Resumo do que foi feito
-----------------------
  - Acrescentado sistema de pontuação: pontos são conferidos quando um inimigo é atingido ou destruído.
  - Ação da gravidade sobre os projéteis.
  - Controle interativo da nave (ver "Testes" mais abaixo).
  - Adicionado sistema de detecção de colisões.
  - Nave e inimigos alterados para terem formato cilíndrico.
  - Efeito de invulnerabilidade: nave fica imune por um período de tempo após receber dano ou ser destrúida.
  - Sistema de vidas para a nave: quando todas acabarem, game over.

Simulação
---------
  Para testar o programa, foi atualizado o módulo Main.c. Como anteriormente, simulamos a execução do jogo, porém desta vez havendo interação com o usuário. O programa recebe comandos em forma de letras a cada certo número fixo de timesteps, simulando o pressionamento contínuo das teclas de ação até chegar a hora de receber os próximos comandos.
  
  Diferentemente da primeira fase, a nave não ficará parada esperando a morte chegar; agora pode mover-se e atirar de volta nos inimigos. Em caso de acerto, são conferidos 10 pontos, e em caso de conseguir destruir uma torre, 1000 pontos. Fogo amigo é permitido, ou seja, os inimigos podem danificar seus semelhantes, mas pontos serão conferidos à nave somente se o dano partir de tiros dela mesma (sem vida fácil no score aqui).
  
  A movimentação da nave é um pouco mais complexa. Os comandos de movimento não "movem" a nave, apenas alteram a sua direção, sendo que a inclinação atual da nave decide o quanto ela deve mover-se para cima, para baixo ou para os lados. O caminho que os tiros seguem também dependem da direção da nave no momento. Caso o usuário pare de inserir comandos de movimento, a nave tende a voltar rapidamente para a direção orientada ao centro.
  
  O teste prossegue até a nave perder todas as vidas, ou seja, sua energia chegar a zero (três vezes, por padrão). Como usual, são exibidos um monte de números correspondentes aos atributos dos elementos, com duas novidades: a energia dos inimigos e um campo "amigo", que basicamente diz se o tiro foi disparado pela nave ou não.

Uso
---
  O programa é executado pelo terminal (supõe-se aqui que esteja usando Linux) na seguinte forma:
  
  **$ ./bin/River [semente] [intervalo para comandos]**
  
  - **semente**: Valor inteiro a ser usado na geração de números aleatórios. Se nada for especificado, usa-se o relógio do sistema.
  - **intervalo para comandos**: Intervalo de tempo, em timesteps, entre comandos a serem recebidos do usuário. Se nada for especificado, usa-se como padrão o valor 20.

  As seguintes teclas podem ser usadas pelo usuário:
  - **U**: Move a nave para cima.
  - **D**: Move a nave para baixo.
  - **L**: Move a nave para a esquerda.
  - **R**: Move a nave para a direita.
  - **Z**: Faz a nave atirar.

  Pode-se digitar mais de uma tecla de uma vez. Por exemplo, "UZ" faz a nave atirar enquanto sobe. Espaços ou outras letras são ignorados. Comandos vazios fazem a nave não executar nada.

  Também é possível receber um arquivo com linhas de comandos via entrada padrão; porém, caso os arquivo acabe antes do fim da execução do programa, a nave permanecerá sem realizar ação nenhuma até ser destruída.
