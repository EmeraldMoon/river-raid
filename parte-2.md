Relatório
=========

  - Integrantes:
    - (**8516782**) Daniel Toshio Iwamoto
    - (**7557797**) Gustavo Chicato Wandeur
    - (**8536065**) Leonardo Pereira Macedo
    
    
    
Introdução
----------
  O objetivo dessa segunda fase foi realizar ajustes e reorganizar algumas funções com a finalidade de torná-las mais claras, além de criar outras que cuidam da interação com o usuário. Não foi necessária a criação de muitas funções, visto que conseguimos adiantar bem o projeto logo na primeira fase.
  
Resumo do que foi feito
-----------------------
  - Acrescentado sitema de pontuação: pontos são conferidos quando um inimigo é atingido ou destruído.
  - Adicionado módulo Base.h, contendo definições que não se enquadram em nenhum outro modulo, além de uma estrutura Corpo, que contém atributos comuns à nave e aos inimigos.
  - Adicionado ação da gravidade nos projéteis.
  - Adicionado controle da nave (ver "Testes" mais abaixo).
  - Adicionado sistema de detecção de colisões.
  - Correção de erros decorrentes da reorganização.
  - Nave e inimigos alterados para formato cilíndrico.

Simulação
---------
  Para testar o jogo, foi criado o arquivo Main.c.
  
  Esse módulo trata de simular a execução de um jogo, desta vez tendo interação com o usuário. Este passa um comando a ser executado durante X frames.
  A nave se desloca progressivamente pelo meio da tela com velocidade constante, enquanto inimigos fixos são gerados aleatoriamente de tempos em tempos.
  
  O teste prossegue até a nave perder todas as vidas, ou seja, sua energia chegar a zero 4 vezes (3 vidas extras por padrão).
  
  A exibição do jogo ainda ocorre por meio da leitura de números, mas agora com um detalhamento maior. Todas as funções foram testadas e, aparentemente, funcionam corretamente.

Uso
---
  O programa é executado pelo Terminal da seguinte forma:
  
  @ ./bin/River [semente] [intervalo para comandos]
  
  - semente: Valor inteiro a ser usado na geração de números aleatórios. Se nada for especificado, usa-se o tempo do sistema.
  - intervalo para comandos: Intervalo de tempo, em frames, para pedir um comando para o usuário. Se nada for especificado, usa-se 20 frames.

  As seguintes teclas podem ser usadas pelo usuário:
  - U: Move a nave para cima.
  - D: Move a nave para baixo.
  - L: Move a nave para a esquerda.
  - R: Move a nave para a direita.
  - Z: Faz a nave atirar.

Pode-se digitar mais de uma tecla de uma vez. Por exemplo, "UZ" faz a nave atirar enquanto sobe.
