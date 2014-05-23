Relatório
=========

  - Integrantes:
    - (**8516782**) Daniel Toshio Iwamoto
    - (**8536065**) Leonardo Pereira Macedo
    - (**7557797**) Gustavo Chicato Wandeur
    
    
Introdução
----------
  O objetivo dessa segunda fase foi realizar ajustes e reorganizar algumas funções com a finalidade de torná-las mais claras, além de criar outras que cuidam da interação com o usuário. Não foi necessária a criação de muitas funções, visto que conseguimos adiantar bem o projeto logo na primeira fase.
  
Resumo do que foi feito
-----------------------
  - Acrescentado sitema de pontuação.
  - Adicionado módulo Base.h, que contém definições que não se enquadram em nenhum outro modulo, e uma estrutura Corpo contendo atributos comuns à nave e aos inimigos.
  - Adicionado ação da gravidade nos projéteis.
  - Adicionado controle da nave.
  - Adicionado sistema de detecção de colisões.
  - Correção de erros decorrentes da reorganização.
  - Nave e inimigos alterados para formato cilíndrico.

Simulação
---------
  Para testar o jogo, foi criado um módulo exclusivo para isso, que chamamos criativamente de Teste.c.
  
  Esse módulo trata de simular a execução de um jogo, desta vez com interação com o usuário. Este passa um comando a ser executado durante X frames.
  A nave se desloca progressivamente pelo meio da tela com velocidade constante, enquanto inimigos fixos são gerados aleatoriamente de tempos em tempos.
  
  O teste prossegue até a nave perder todas as vidas, ou seja, sua energia chegar a zero 4 vezes (3 vidas extras por padrão).
  
  A exibição do jogo ainda ocorre por meio da leitura de números, mas agora com um detalhamento maior. Todas as funções foram testadas e, aparentemente, funcionam corretamente.
