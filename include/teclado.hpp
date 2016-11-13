/*------------------------------------------------------------------*
 |                                                                  |
 |  Interpretação de comandos realizados pelo teclado.              |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

#include <SFML/Window.hpp>

/*------------------------------------------------------------------*/

/*
 *  Faz a verificação de ações de teclas.
 */
void keyPressOperations(sf::Keyboard::Key tecla);
void keyOperations();

/*
 *  Devolvem condições auto-explicativas.
 */
bool estaEmPrimeiraPessoa();
bool estaPausado();
bool exibindoFPS();
