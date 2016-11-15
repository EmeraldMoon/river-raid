#include <SFML/Window.hpp>

#include "jogo.hpp"
#include "cenario.hpp"

/*------------------------------------------------------------------*/

/* Constantes para teclas */
#define TECLA_TIRO   Space
#define TECLA_TIRO2  Z
#define TECLA_CAMERA C
#define TECLA_PAUSA  P
#define TECLA_FPS    F
#define TECLA_SAIDA  Escape

#define   key(tecla) sf::Keyboard::tecla
#define press(tecla) sf::Keyboard::isKeyPressed(key(tecla))

/*-------------------*
 |   T E C L A D O   |
 *-------------------*----------------------------------------------*/

void Jogo::keyPressOperations(sf::Keyboard::Key tecla)
{
    /* Ajeitar isso */
    switch (tecla) {
    case key(TECLA_CAMERA): primeiraPessoa = not primeiraPessoa; break;
    case key(TECLA_FPS):       exibindoFPS = not exibindoFPS;    break;
    case key(TECLA_PAUSA):         pausado = not pausado;        break;
    case key(TECLA_SAIDA): janela.close(); break;
    default: break;
    }
}

void Jogo::keyOperations()
{
    static Nave &nave = Cenario::get().nave;

    if ((press(TECLA_TIRO) or press(TECLA_TIRO2))
            and nave.reduzEspera() <= 0) {
        nave.dispara();
    }
    /* Faz operação entre booleanos para encontrar sentidos */
    int sentidoVert  = press(Up)    - press(Down);
    int sentidoHoriz = press(Right) - press(Left);

    /* Atualiza direção */
    nave.atualizaVertical(sentidoVert);
    nave.atualizaHorizontal(sentidoHoriz);
}
