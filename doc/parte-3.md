Relatório - Parte 3
===================

  - Integrantes:
    - (**8516782**) Daniel Toshio Iwamoto
    - (**7557797**) Gustavo Chicato Wandeur
    - (**8536065**) Leonardo Pereira Macedo

Introdução
----------
  O objetivo principal desta terceira parte foi finalizar o projeto por meio da tão esperada implementação gráfica via OpenGL. Assim, temos como resultado final um jogo básico, interativo e visualmente atrativo.
  
Resumo do que foi feito
-----------------------
  - Implementação gráfica dos elementos do jogo.
  - Texturas para as paredes, rio e fundo.
  - Modelos padronizados para nave e inimigos.
  - Reconhecimento de comandos via teclado (callbacks).
  - HUD para indicação de pontuação, vidas e energia da nave.
  - Modo primeira pessoa, onde câmera é colocada "dentro" da nave.
  - Possibilidade de exibir frames por segundo (FPS) do jogo no momento.
  - Controle da velocidade do jogo. Evita-se que o jogo rode rápido ou lento demais, usando de *frameskip* caso necessário.
  - Ajustes em algumas constantes do jogo, visando a torná-lo mais balanceado.
  - Correções de bugs que ficaram aparentes após a implementação gráfica.
 
Simulação
---------
  Ao executar o programa, o jogo é iniciado imediatamente. O usuário joga através do teclado, tentando desviar de tiros enquanto destrói inimigos para acumular pontos em seu score. Quando a nave perder as 3 vidas, o jogo é fechado automaticamente, e a pontuação final impressa na tela.

Uso
---
  Para compilar, basta digitar *make*.
  O programa é executado pelo terminal (supõe-se aqui que esteja usando Linux) da seguinte forma:

  **$ ./bin/River [código]**
  - **código**: Argumentos opcionais para dar aquela trapaceada básica.
    + [-iddqd]: A nave fica invencível.
  
As seguintes teclas podem ser usadas pelo usuário:
  - **Setas Direcionais**: Fazem a nove se mover na direção correspondente.
  - **Barra de Espaço**: Faz a nave atirar.
  - **C**: Alterna entre visão em primeira e terceira pessoa.
  - **P**: Pausa/despausa jogo.
  - **F**: Liga/desliga mostrador de FPS.
  - **Q**: Fecha o jogo.
