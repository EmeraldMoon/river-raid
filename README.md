River Raid
==========

![Imagem do Jogo](doc/RiverRaid.png "Imagem ilustrativa do jogo :)")


Introdução
----------

River Raid é um *3D shooter* bastante simples, baseado nos clássicos [**River Raid**]
(https://en.wikipedia.org/wiki/River_Raid "Informações sobre o River Raid original")
(Atari 2600) e [**Star Fox 64**](https://en.wikipedia.org/wiki/Star_Fox_64
"Informações sobre Star Fox 64") (Nintendo 64). Os desenvolvedores buscavam aprender
a fazer um projeto um pouco maior (em linguagem C) enquanto tinham um primeiro
contato com programação gráfica através do OpenGL mais antigo.

O jogo foi desenvolvido durante aproximadamente 3 meses como um projeto para uma
disciplina de faculdade. Há um [**changelog**](doc/changelog.md "Link para o changelog")
com maiores detalhes sobre o processo de criação.


Uso
---

Para compilar, basta digitar no root do jogo:

    $ make

O programa é executado pelo terminal (supõe-se aqui que o usuário esteja usando
Linux) da seguinte forma:

    $ ./bin/River [código]

- **código**: Argumentos opcionais para dar aquela trapaceada básica.
  + [-iddqd]: A nave fica invencível.


Instruções
----------

O jogador controla uma nave e deve percorrer a maior distância possível num corredor
infinito. Ao longo do percurso, inimigos surgem para danificar a vida da nave e
destruí-la. A nave possui seu próprio tiro e itens aleatoriamente gerados pelo
corredor para sobreviver aos ataques. A cada inimigo destruído, o jogador acumula
pontos em seu score. Quando a nave perder todas as vidas, o jogo é fechado
automaticamente e a pontuação final é revelada.

Pela simplicidade do projeto, só há um tipo de inimigo e uma variedade de tiro para
as entidades.

Os getListaItens() existentes são:

- **Cubo Amarelo:** A nave ganha uma vida extra.

- **Cubo Verde:** Restaura um pouco da barra de vida da nave.

- **Esfera Azul:** Fornece um campo de força à nave, diminuindo o dano que ela recebe.
                   O efeito vai enfraquecendo conforme a nave é atingida.


Controles
---------

As seguintes teclas podem ser usadas pelo usuário:

- **Setas Direcionais**: Fazem a nave mover-se na direção correspondente.

- **Barra de Espaço**: Faz a nave atirar.

- **C**: Alterna entre visão em primeira e terceira pessoa.

- **P**: Pausa/despausa o jogo.

- **F**: Liga/desliga mostrador de FPS.

- **Q**: Fecha o jogo.


Agradecimentos
--------------

- [**Heiko Behrens**](http://heikobehrens.net/2009/08/27/obj2opengl/
  "Script obj2opengl do Behrens"), pelo script que possibilita transformar
  eficientemente um arquivo .obj num formato usado pelo OpenGL.

- [**Site tf3dm**](http://tf3dm.com/ "Link para o site"), pelos modelos da nave e dos
  inimigos.

- [**Swiftless**](http://www.swiftless.com/opengltuts.html "Tutoriais do Swiftless"),
  pelos tutoriais de OpenGL 2.0.

- Diversos outros sites pelas texturas usadas no jogo (elas não são de nossa autoria).
