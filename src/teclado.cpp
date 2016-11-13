#include <SFML/Graphics.hpp>

#include "teclado.hpp"
#include "nave.hpp"
#include "cenario.hpp"

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Constantes para teclas */
#define TECLA_TIRO   Space
#define TECLA_TIRO2  Z
#define TECLA_CAMERA C
#define TECLA_PAUSA  P
#define TECLA_FPS    F
#define TECLA_SAIDA  Escape

/* Variáveis booleanas indicadoras */
static bool primeiraPessoa = false;
static bool       exibeFPS = false;
static bool        pausado = false;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*----------------------------------------------*/

#define tecla(x) sf::Keyboard::isKeyPressed(sf::Keyboard::x)

void keyOperations()
{
    if (tecla(TECLA_CAMERA)) primeiraPessoa = not primeiraPessoa;
    if (tecla(TECLA_FPS))          exibeFPS = not exibeFPS;
    if (tecla(TECLA_PAUSA))         pausado = not pausado;
    if (tecla(TECLA_SAIDA)) Cenario::get().encerraJogo();

    static Nave &nave = Cenario::get().nave;

    if ((tecla(TECLA_TIRO) or tecla(TECLA_TIRO2))
            and nave.reduzEspera() <= 0) {
        nave.dispara();
    }
    /* Faz operação entre booleanos para encontrar sentidos */
    int sentidoVert  = tecla(Up)    - tecla(Down);
    int sentidoHoriz = tecla(Right) - tecla(Left);

    /* Atualiza direção */
    nave.atualizaVertical(sentidoVert);
    nave.atualizaHorizontal(sentidoHoriz);
}

/*------------------------------------------------------------------*/

bool estaEmPrimeiraPessoa() { return primeiraPessoa; }
bool estaPausado()          { return pausado;        }
bool exibindoFPS()          { return exibeFPS;       }
