#include <cstdio>   /* sprintf */
#include <cmath>    /* ceil */
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

#include "jogo.hpp"
#include "nave.hpp"
#include "cenario.hpp"
#include "teclado.hpp"
#include "modelo.hpp"
#include "cores.hpp"

/*------------------------------------------------------------------*/

/* Ponteiro (infelizmente) para acessar neste módulo */
static Nave *nave;

/*-------------*
 |   J O G O   |
 *-------------*----------------------------------------------------*/

Jogo::Jogo(int argc, char *argv[])
{
    /* Tratamento de argumentos via linha de comando */
    bool godMode = false, debug = false, noDepth = false;
    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if      (arg == "-iddqd") godMode = true;
        else if (arg ==     "-d")   debug = true;
        else if (arg ==     "-l") noDepth = true;
    }
    /* Configurações gráficas padrões */
    sf::ContextSettings settings;
    settings.depthBits = noDepth ? 0 : 24;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 4;

    /* Cria janela de jogo */
    janela.create(sf::VideoMode(1280, 720), "River Raid",
                  sf::Style::Default, settings);
    janela.setFramerateLimit(60);
    janela.setVerticalSyncEnabled(true);

    /* Ativa efeitos de transparência */
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Permite desenhar modelos de vértices */
    glEnableClientState(GL_VERTEX_ARRAY);

    /* Nevoeiro sobre o cenário
       (só aceita valores float, infelizmente). */
    /*const GLfloat cor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    glEnable(GL_FOG);
    glFogf(GL_FOG_DENSITY, 0.0001f);
    glFogfv(GL_FOG_COLOR, cor);*/

    /* static evita que instância seja destruída */
    static Cenario cenario(godMode, debug);

    nave = &cenario.nave;

    /* Carrega fonte do arquivo */
    fonte.loadFromFile("zorque.ttf");

    /* Passa controle do resto do jogo ao SFML */
    loop();    
}

/*------------------------------------------------------------------*/

void Jogo::loop()
{
    while (janela.isOpen()) {

        desenha();

        /* Teclas de efeito contínuo */
        keyOperations();

        /* Trata possíveis eventos do SFML */
        sf::Event evento;
        while (janela.pollEvent(evento)) {
            switch (evento.type) {
            case sf::Event::Closed:
                /* Fechamento da janela (Alt+F4, etc) */
                janela.close();
                break;
            case sf::Event::KeyPressed:
                /* Pressionamento não contínuo de tecla */
                keyPressOperations(evento.key.code);
                break;
            case sf::Event::Resized:
                /* Redimensionamento da janela */
                remodela(evento.size.width, evento.size.height);
                break;
            default: break;
            }
        }
        /* Atualiza jogo e elementos do cenário */
        if (not Cenario::get().atualiza()) janela.close();
    }
    printf("Score final: %d\n", nave->getScore());
}

/*
 *  Loop principal da parte visual. Cuida do posicionamento da câmera,
 *  controle dos buffers e chamada de funções de atualização.
 */
void Jogo::desenha()
{
    /* Faz a limpeza dos buffers e carrega matriz identidade */
    janela.clear();
    glLoadIdentity();

    /* Configura a posição da câmera.
       (ponto de visão, ponto de fuga, vertical da câmera) */
    if (estaEmPrimeiraPessoa()) {
        gluLookAt(nave->getX(), nave->getY(), nave->getZ(),
                  nave->getX(), nave->getY(), nave->getZ() + Cenario::Z_DIST,
                  0.0, 1.0, 0.0);
    } else {
        gluLookAt(0.0, Cenario::Y_MAX/2.0, nave->getZ() - DIST_CAMERA,
                  0.0, Cenario::Y_MAX/2.0, nave->getZ() + Cenario::Z_DIST,
                  0.0, 1.0, 0.0);
    }
    /* Desenha cenário e elementos de jogo */
    Cenario::get().desenha();

    /* Desenha elementos fixos da interface da tela.
       pushGLStates pode ser meio custoso... observar. */
    janela.pushGLStates();
    if (exibindoFPS()) exibeFps();
    exibeHud();
    janela.popGLStates();;

    janela.display();
}

/*------------------------------------------------------------------*/

/*
 *  Redesenha a área de jogo quando (e enquanto)
 *  janela for redimensionada.
 */
void Jogo::remodela(int largura, int altura)
{
    /* Define tamanho do retângulo de visão */
    glViewport(0, 0, largura, altura);

    /* Espelha horizontalmente a imagem no eixo x */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glScaled(-1.0, 1.0, 1.0);

    /* (ângulo de visão, proporção de tela, distâncias min e max) */
    gluPerspective(90.0, (GLdouble) largura/altura, 1.0, Cenario::Z_DIST);

    /* Volta ao modo original */
    glMatrixMode(GL_MODELVIEW);
}

/*------------------------------------------------------------------*/

/*
 *  Mostra na tela os indicadores básicos do jogo:
 *  vidas restantes, HP e pontuação.
 */
void Jogo::exibeHud()
{
    float x = 0.10 * janela.getSize().x;
    float y = 0.15 * janela.getSize().y;

    /* Desenha vidas extras da nave */
    static sf::CircleShape losango(20.0, 4);
    for (int i = 0; i < nave->getVidas(); i++) {
        double deltaX = 2.5 * losango.getRadius();
        losango.setPosition(x + (i * deltaX), y);
        janela.draw(losango); 
    }
    /* Desenha caixa da lifebar */
    y += 3.0 * losango.getRadius();
    static sf::RectangleShape caixa({200.0, 10.0});
    caixa.setPosition(x, y);
    caixa.setFillColor(sf::Color::Blue);
    janela.draw(caixa);

    /* Desenha lifebar com parcial do HP */
    float k = (float) nave->getHP() / nave->getHPMax();
    sf::RectangleShape lifebar({k * caixa.getSize().x,
                                    caixa.getSize().y});
    lifebar.setPosition(x, y);
    lifebar.setFillColor(k > 0.25 ? sf::Color::Green : sf::Color::Red);
    janela.draw(lifebar);

    /* Imprime pontuação (e, se for o caso, mensagem de pausa) */
    y += 2.5 * lifebar.getSize().y;
    sf::String str = "Score: " + std::to_string(nave->getScore());
    if (estaPausado()) str += " (pausa)";
    sf::Text texto(str, fonte, 20);
    texto.setPosition(x, y);
    janela.draw(texto);
}

/*
 *  Exibe o número de quadros por segundo que o jogo está
 *  desenhando no momento, caso a opção esteja ativada.
 */
void Jogo::exibeFps()
{
    float x = 0.80 * janela.getSize().x;
    float y = 0.15 * janela.getSize().y;

    /* FPS só é alterado na tela a cada tantos timesteps */
    static int fps, cont = 20;
    cont += getDelayTempo() * 10/1000.0;
    if (cont >= 20) {
        fps = 1000.0/(getDelayTempo() - 1);
        cont = cont % 20;
    }
    /* Imprime fps na tela */
    sf::String mostrador = std::to_string(fps > 60 ? 60 : fps) + " fps";
    sf::Text texto(mostrador, fonte, 20);
    texto.setPosition({x, y});
    janela.draw(texto);
}
