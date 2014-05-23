Relatório
=========

  - Integrantes:
    - (**7557797**) Gustavo Chicato Wandeur
    - (**8536065**) Leonardo Pereira Macedo
    - (**8516782**) Daniel Toshio Iwamoto

Introdução
----------
  O objetivo dessa segunda fase foi realizar ajustes e re-organizar algumas funções com a finalidade de torna-las mais claras, além de criar outras que cuidam da interação com o usuario. Não foi necessária a criação de muitas funções, visto que consegumios adiantar o projeto logo na primeira fase.
  
Resumo do que foi feito
-----------------------
  - Acrescentado sitema de pontuação.
  - Adicionado modulo Base.h que contém definições que não se enquadram em nenhum outro modulo e uma estrutura Corpo que contem atributos comuns a nave e aos inimigos.
  - Adicionado ação da gravidade nos projéteis.
  - Adicionado controle da nave.
  - Adicionado sistema de detecção de colisões.
  - Correção de erros decorrentes da re-organização.
  - Nave e inimigos alterados para formato cilíndrico.

Simulação
---------
  Para testar o jogo, foi criado um módulo exclusivo para isso, que chamamos criativamente de Teste.c.
  
  Esse módulo trata de simular a execução de um jogo, agora com interação com o usuário. O usario passa um comando que então é executado durante X frames.
  A nave se desloca progressivamente pelo meio da tela com velocidade constante, enquanto inimigos fixos são gerados aleatoriamente de tempos em tempos.
  
  O teste prossegue até a nave perder todas as vidas, ou seja, sua energia chegar a zero 4 vezes (3 vidas extras por padrão).
  
  A exibição do jogo ainda ocorre por meio da exibição de números, agora um detalhamento maior. Todas funções foram testadas e, aparentemente, funcionam corretamente.
