Relatório - Parte 2
===================

  - Integrantes:
    - (**8516782**) Daniel Toshio Iwamoto
    - (**7557797**) Gustavo Chicato Wandeur
    - (**8536065**) Leonardo Pereira Macedo

Introdução
----------
  O objetivo principal desta terceira fase foi a implementação gráfica do jogo, o que permite uma jogabilidade melhor e a visualização de uma forma mais *user friendly* facilitando a visualização do que foi implementado nas partes anteriores sem que o usuário precise analisar dezenas/centenas de números.
  
Resumo do que foi feito
-----------------------
  - Implementação gráfica dos elementos do jogo.
  - Controle da velocidade do jogo. Em computadores mais potentes o jogo fica na velocidade normal e nos menos potentes o jogo fica com *frameskip*.
  - Ajustes em algumas constantes do jogo visando torná-lo mais balanceado.
  - Correções de bugs que ficavam aparentes pós implementação gráfica.

Dificuldades
------------
  A maior dificuldade desta parte do projeto foi a implementação da parte gráfica, tendo em vista que foi necessária a utilização de diversas funções do OpenGL, cujos usos tiveram que ser pesquisados e estudados. O uso do método erro e tentativa foi frequente para a correção de bugs decorrentes da parte gráfica.

  A realização dos testes foi feita em diversos computadores, o que dificultava a simulação do jogo, uma vez que ele rodava normalmente em um, mas poderia rodar duas vezes mais rapido ou na metade da velocidade em outros. Para evitar este problema foi necessária a implementação do controle de velocidade do jogo.
 
Simulação
---------
  Ao executar o programa o jogo é iniciado imediatamente (ainda sem tela de menu). O usuário pode jogar através do teclado, tentando desviar de tiros enquanto destrói inimigos para obter um high-score (que por enquanto ainda não é armazenado). Quando a nave perder as 3 vidas o jogo é fechado automaticamente.

Uso
---
  O programa é executado pelo terminal (supõe-se aqui que esteja usando Linux) na seguinte forma:
  
  **$ ./bin/River [código]**
  
  - **código**: Argumentos opcionais para dar aquela trapaceada básica.
                **GOD_MODE**: Nave fica invencivel.

  As seguintes teclas podem ser usadas pelo usuário:
  - **Setas Direcionais**: Fazem a nove se mover na direção correspondente.
  - **Barra de Espaço**: Faz a nave atirar.
  - **C**: Alterna entre visão em primeira e terceira pessoa.
  - **P**: Pausa/despausa jogo.
  - **F**: Liga/desliga mostrador de FPS.
  - **Q**: Fecha o jogo.