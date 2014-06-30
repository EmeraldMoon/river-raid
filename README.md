River Raid - Changelog
========================

Terceira parte
--------------

### Versão 1.1

###### 29/06:
  - Relatório feito.
  - Corrigida a pequena mudança nas projeções ortogonais ao se mudar a câmera.
  - Fog não está tão intenso a ponto de não se enxergar o céu.
  - Ampliado distância máxima em *gluPerspective()* para poder enxergar o céu na cãmera em terceira pessoa.
  - ***Grafico.c***: Pequena polida em algumas funções.
  - Pausa funcionando perfeitamente. Bug que causava encerramento do jogo fora removido.

###### 28/06:
  - ***Grafico.c***: Adicionadas funções relativas ao uso de projeções ortogonais: *ortogonalInicio()* e *ortogonalFim()*.
  - O hud e o fps não sumirão ao se mudar o tamanho da tela, mas ainda há um problema relativo à aparência das projeções na câmera em primeira pessoa.
  - ***Main.c***: Removido a opção de fornecer a semente do gerador de números aleatórios (não estava sendo muito útil). Adicionada a opção de um cheat de invincibilidade pela linha de comando.
  - Corrigido bug da nave "entrar" nas paredes ao levar em conta o ângulo máximo/mínimo horizontal da nave.
  - Constantes da nave, inimigos, etc. foram levemente modificados para melhorar a jogabilidade.
  - Nova pasta ***model*** para guardar os arquivos .obj.
  - Uso do programa ***obj2opengl.pl***, da autoria de Heiko Behrens, diminuiu bem o tempo de compilação dos modelos. Para maiores detalhes: http://heikobehrens.net/2009/08/27/obj2opengl/

### Versão 1.0

###### 22/06:
  - Adicionados modelos para a nave e inimigos! Também foram implementadas texturas neles, mas a qualidade não está muito boa...
  - ***Textura.c***: Na função *carregaTextura()*, agora é possível especificar se a textura usará mipmaps através de um novo parâmetro.
  - ***Nave.c***: Uso de uma mira em vez de um cone transparente na câmera em primeira pessoa.

###### 20/06:
  - Uso de ***issues*** no GitHub para acompanhar as próximas pendências a serem resolvidas no projeto.

###### 18/06:
  - ***Grafico.c***: Implementação de "flashes" vermelhos durante os frames de invencibilidade da nave (só para a câmera em primeira pessoa).
  - Efeito de transparência quando a nave é atingida está mais perceptível.

###### 16/06:
  - Opções muito úteis de interação com o git adicionadas ao *Makefile*.
  - Corrigida localização dos inimigos para não aparecerem "dentro" das paredes.
  - Limpeza dos *glBegin() / glEnd()* passando vértices por matrizes.

###### 14/06:
  - Tentativa de implementar luzes no jogo. Só funciona (e ainda assim, não muito bem) para os inimigos.

###### 13/06:
  - ***Textura.c***: Novo módulo, auto-explicativo.
  - ***Cenario.c***: Enfim, controle do tempo de jogo à velocidade constante funcionando com perfeição. Em computadores rápidos, o jogo rodará fluidamente, sem modo turbo; em computadores lentos, ocorre *frameskip*, ou seja, o computador desenha apenas os quadros que conseguir, "pulando" os outros, mas deixando o ritmo de jogo intacto.
  - ***Grafico.c***: Pequena funçãozinha para mostrar FPS atual na tela, ainda meio mequetrefe.
  - ***Nave.c***: Limites do campo de jogo agora levam em conta dimensões da nave - ela não sairá mais para fora do cenário.
  - Nevoeiro adicionado sobre o cenário. O efeito é visualmente interessante.
  - 2000 linhas de código atingidas.

###### 09/06:
  - ***Tiro.c***: Liberação dos tiros agora ocorre apenas quando passam da câmera, gerando um efeito "cool".
  - Frequência dos desenhos limitada a 60FPS. Jogo não deverá mais rodar na velocidade da luz em alguns PCs.
  - Tentativa de sincronizar o jogo para ter a mesma velocidade independente do fps atual. Funciona em partes, exceto quando o slowdown for grande demais.
  - Opção de mudar a câmera de terceira para primeira pessoa e vice-versa com uma tecla.
  - Possibilidade de pausar o jogo através de uma tecla, congelando as atualizações enquanto a mesma tecla não for pressionada novamente.
  - ***Grafico.c***: Corrigido o tamanho do plano na função *rio()*. Criou-se a impressão de que o rio está "mais rápido" que a nave.

###### 08/06:
  - ***Grafico.c***: Uso de mipmaps nas texturas, produzindo um efeito mais suave ao desenhá-las à distância.
  - ***Nave.c***: Adicionado efeito de transparência quando nave é atingida ou destruída.

### Versão 0.9

###### 06/06:
  - Adicionada movimentação das paredes e do rio, de forma alheia à nave. Tal engenho cria uma *sensação de movimento* do jogador em relação ao cenário.
  - Novo diretório *texture/*, usado para armanezar os arquivos *ppm* de texturas.

###### 05/06:
  - Texturas implementadas: fundo, paredes e rio. Usaremos arquivos com formato ***ppm***.
  - ***Grafico.c***: Corrigidos problemas na leitura de textura; dentre eles, aquele que deixava a imagem invertida.

###### 02/06:
  - ***Grafico.c***: Leitura de texturas a partir de um arquivo, aparentemente funcionando.
  - Mudamos para o padrão C99. A flag *-std=c99* do gcc permite fazer códigos mais próximos ao padrão Java, com suporte a comentários começados com "//", declaração de variáveis no meio do código e vetores de tamanho dinâmico (!).

###### 01/06:
  - Corrigidos inimigos que apareciam flutuando em relação ao solo.
  - Corrigida detecção de colisão entre naves e inimigos, antes com problemas na verificação de altura.
  - Corrigido mesmo problema que o anterior, porém no caso de acerto dos projéteis.

###### 31/05:
  - Comentários e documentação a respeito das funções da terceira parte concluídos.
  - *Include guards* nos headers foram todos substituídos por *#pragma once*. Ajuda a manter os cabeçalhos mais limpos.
  - Diferenciação provisória das cores dos tiros da nave e dos inimigos.
  - ***Grafico.c***: Desenho da barra de energia pôde ser simplificado usando a função *glRect()*, cujo papel é desenhar retângulos. Função *desenha()* limita-se agora apenas a cuidar da parte visual.
  - ***Cenario.c***: Função ***atualiza()***, antiga *atualizaCenario()*, passa a cuidar do callback de comandos e de outras verificações. Será basicamente o loop responsável pelo processamento interno do jogo.  

### Versão 0.8

###### 30/05:
  - ***Cores.h***: Contém constantes RGB para praticamente todas as cores em existência, junto com um macro para *glColor()* reduzido. Assim, é possível inserir cores em outros módulos de maneira mais direta e sem preocupação com comentários adicionais.
  - Corrigido problema de cooldown não estar sendo aplicado aos tiros da nave. Agora é possível atirar em velocidade normal.

###### 29/05:
  - ***Teclado.c***: Novo módulo! Cuida da verificação de teclas e interpretação de comandos.
  - Funções responsáveis pelo desenho de elementos transferidas para seus respectivos módulos.

###### 28/05:
  - ***Grafico.c***:
    - Resolvido problema do espelhamento da tela (e consequentemente, da inversão dos ângulos da nave) através de um *glScale* com valores (-1, 1, 1), que em resumo inverte o eixo X na matriz correspondente.
    - Pontuação impressa de um jeito mais direto, usando *sprintf()*, o que corrigiu o bug de "Score: 0" não aparecer no começo do jogo.
    - Cor da lifebar varia em gradiente de verde a vermelho, dependendo da vida da nave.
  - Trocados alguns tipos de variáveis para o padrão do OpenGL (GLsizei, GLdouble, GLboolean, etc).
  - Funções do OpenGL terminadas em "f" (*float*) foram trocadas por suas análogas em *double*, terminadas em "d".
  - ***Tiro.c***: Projéteis não são mais eliminados ao saírem pelo teto do cenário. 

###### 27/05:
  - ***Grafico.c***: Novo módulo para funções relacionadas ao OpenGL (parte gráfica). Dentre as várias novidades, destacam-se:
    - *desenha()*: Cuida do posicionamento da câmera e chamada de funções de atualização. Também herdou parte do loop contido no antigo *main()*.
    - *remaneja()*: Responsável por redesenhar a tela caso a janela seja redimensionada.
    - *hud()*: Desenho das vidas, energia e pontuação.
  - ***Main.c***: Função *main()* limita-se agora à inicialização do jogo e chamada do loop do OpenGL, responsável por todo o resto da execução. Também cuida da criação da janela e seu posicionamento no centro da tela. Leitura via entrada padrão (linha de comando) removida.
  - No momento, a parte gráfica se resume a usar esferas para os tiros e "esqueletos" de cone para a nave e inimigos. Há também representação na tela de vidas restantes, energia da nave e pontuação. O usuário pode controlá-la através do teclado: direcionais movimentam, barra de espaço atira.

###### 26/05:
  - ***Tiro.c***: Precisão finalmente corrigida e funcionando. Grande parte disso deve-se à função *atan2()*, extremamente útil na tarefa de encontrar valor de um ângulo baseado em coordenadas euclidianas.

Segunda parte
-------------

### Versão 0.6

###### 25/05:
  - ***Main.c***: Problemas relacionados à leitura de comandos corrigidos com sucesso.
  - ***Tiro.c***: Cálculo de precisão, antes feito erroneamente, corrigido. Ainda não parece estar funcionando de forma muito correta. Teremos uma ideia melhor do que acontece quando a parte visual for implementada.
  - Relatório feito. Segunda parte do projeto está oficialmente concluída.

###### 23/05:
  - ***Nave.c***: Dois conceitos fundamentais.
    - *Invencibilidade*: Nave torna-se imune a danos por um certo período de tempo após levar um tiro, e por um período ainda maior caso perca uma vida. A representação se dá por uma variável que decresce a cada timestep, similar ao conceito de cooldown. Quando chega a zero, invencibilidade acaba.
    - *Colisão com inimigos*: Implementação similar à da detecção dos projéteis, porém levando em conta dois corpos cilíndricos. 
  - ***Main.c***: Módulo de testes da segunda parte está pronto. Ao receber comandos do usuário, programa simula o pressionamento de tais teclas e passa as ações à nave. Ao fim do intervalo, usuário pode inserir novos comandos. O jogo segue como usual, até a nave morrer. Dessa vez aparecem novidades na tela, como pontuação, vidas, movimento, inclinação, tiros da nave, etc - englobando tudo o que foi feito nesta segunda parte.

###### 22/05:
  - Pontuação em ação no jogo. Pontos são presenteados ao jogador em caso de acerto ou destruição de inimigos. 
  - ***Tiro.h***: Novo atributo booleano *amigo*. Em caso de fogo amigo entre próprios inimigos, dano ocorre, mas pontos não são conferidos ao jogador.
  - ***Defesa.h***: Corrigido erro que fazia os inimigos atirarem no sentido contrário do eixo Oz, o que tornava praticamente impossível aos projéteis acertarem a nave.

###### 21/05:
  - Somos agora (não tão) orgulhosamente usuários do ***GitHub***. Dessa vez para valer.
  - Novo integrante adicionado ao grupo. Corrigidos erros imperdoáveis de português.
  - ***Tiro.c***: Detecção de saída do projétil passa a levar em conta limite *Z_MAX* da tela.
  - ***Nave.c, Defesa.c***: Corrigido posicionamento dos projéteis ao serem disparados. Removidas funções supérfluas.

### Versão 0.5

###### 17/05: 
  - Corrigido erros decorrentes das últimas modificações. Programa pode ser novamente compilado e rodado.
  - ***Nave.h***: Novo atributo *vidas*, representando o nº de chances do jogador.

###### 14/05:
  - ***Base.h***: Novo módulo com definições comuns que não se enquadram em nenhum outro lugar do projeto. Em especial, foi criada uma estrutura *Corpo*, com atributos básicos (posição, energia, recarga, formato) a serem usados comumente entre nave e inimigos.
  - ***Tiro.c***: Adicionada ação da gravidade sobre os projéteis.
  - ***Nave.h, Defesa.h***: Detecção de colisão passa a levar em conta nave e inimigos como corpos cilíndricos.
  - ***Nave.c***: Implementada alteração de direção da nave. Por meio de 4 teclas, o usuário pode alterar a inclinação da mesma, sob uma certa taxa, até um determinado limite. A nave tende naturalmente a voltar à posição orientada ao centro, sob uma taxa diferente.
  - ***Main.c***: Pequena função que recebe uma tecla (na verdade, uma letra), interpretando e executando o comando do usuário.

###### 10/05:
  - Somos agora orgulhosamente usuários do ***git***. Isso não só ajudará no controle e acompanhamento de versões como possibilitará uma visão mais ampla do projeto como um todo.
  - *Makefile* incrementado com novas opções.
    - ***make tar***: Cria um arquivo .tar com todos os arquivos necessários ao projeto.
    - ***make count***: Conta o número de linhas de código (por pura satisfação pessoal).

###### 09/05:
  - ***Main.c***: Alterado nome do módulo de testes, anteriormente conhecido como *Teste.c*.
  - Documentação reformulada. Descrições de funções foram agora movidas para os respectivos arquivos .h, ao passo que restaram nos .c, em geral, detalhes com respeito à implementação. Sessões também estão definidas de maneira mais clara.

Primeira parte
--------------

### Versão 0.3

###### 26/04:
  - Retirados #includes dos .h e transferidos para seus respectivos .c, nos casos em que isso fora possível.
  - ***Random.c***: Novo módulo para trabalhar com distribuições aleatórias (Uniforme discreta, Uniforme contínua e Normal).
  - ***Tiro.c***: Precisão (finalmente) implementada. Quanto menor for, maior será o desvios dos tiros em relação ao alvo designado. É calculada aleatoriamente segundo uma distribuição Normal.
  - ***Cenario.c***: Vários atributos dos inimigos agora são gerados aleatoriamente, produzindo assim maior diversidade nos testes.
  - ***Teste.c***: Informações agora são exibidas de forma mais clara na tela.
  - Adicionado pequeno bloco de comentário no começo dos módulos com um resumo do que fazem.
  - Realizado tudo isso, acho que podemos dizer que encerramos a primeira parte.

###### 24/04:
  - ***Teste.c***: Resultados agora são exibidos em uma tela por vez; espera-se o usuário apertar [Enter] e então a próxima é mostrada.
  - Revisão feita "in loco". Adicionados comentários ao lado de cada #include explicitando quais funções foram usadas de cada biblioteca.

###### 17/04:
  - Modificadas algumas contantes nos headers e corrigidas reclamações do gcc.
  - ***Tiro.c***: Corrigido bug dos projéteis serem liberados ao atingirem coordenada z da nave (ou seja, muitos eram eliminados ainda com chances de acertá-la), o que diminuiu drasticamente o tempo até nave ser destruída.
  - ***Teste.c***: Nova apresentação dos resultados. Agora o programa recebe uma semente por linha de comando ou, caso o usuário não insira nada, usa o tempo do sistema. Toda vez que a nave sofre um dano são exibidos a energia atual, sua posição, posição e tempo de recarga de todos os inimigos e, finalmente, coordenadas e velocidade de cada projétil. Ao fim do jogo, memória das listas é liberada.
  - **Bônus**: All heap blocks were freed -- no leaks are possible (!)

### Versão 0.2

###### 16/04:
  - ***Tiro.h***: Alterado tipo das coordenadas e velocidade de *int* para *double*, o que garante melhor precisão.
  - ***Tiro.c***: Projétil dos canhões são mirados, tomando como referência de direção o bico da nave.
  - ***Defesa.h, Defesa.c***: Novo atributo *recarga*, indicando tempo de espera entre um projétil e outro.
  - ***Cenario.c***: Nova função para gerar inimigos aleatoriamente no fundo do cenário.
  - ***Teste.c***: Módulo de testes (finalmente) criado. O próprio simulará a execução do programa, ainda sem interação, e imprimirá na tela o estado dos elementos a cada certo nº de frames, a ser determinado pelo usuário. A simulação acaba quando a energia da nave chegar a zero.
  - É oficial: o programa funciona.

###### 15/04:
  - ***Cenario.c***: Atualização de posições da nave e dos projéteis.

###### 14/04:
  - ***Tiro.c***: Atualização da posição dos tiros no cenário. Verificação da saída do projétil agora também conta com os limites x e y da tela.
  - ***Nave.c***: Atualização da posição da nave.
  - ***Cenario.c***: Vasculhamento de todos os inimigos/projéteis num determinado frame, eliminando os que saírem da tela ou acertarem a nave, sendo removida energia da mesma neste último caso.

###### 13/04:
  - ***Defesa.c, Tiro.c***: Adicionadas funções de manipulação de lista.
  - ***Nave.c***: Inicialização dos atributos da nave.
  - ***Tiro.c***: Funções para verificar se um determinado tiro saiu da tela ou acertou a nave.
  - A princípio, com propósito de testes, definimos tanto tiro quanto nave como corpos esféricos. Nas versões futuras isso provavelmente será removido; o OpenGL deve lidar com a verificação de colisões de formatos variados.

### Versão 0.1

###### 12/04:
  - ***Lista.c***: Criado módulo com operações básicas de lista ligada.
  - Decidido que o melhor será usar listas ligadas comuns; como as remoções não necessitam serem feitas explicitamente nas extremidades, perde-se a razão de trabalhar com filas ou pilhas.
  - Listas agora podem receber qualquer tipo de dados, sejam inimigos, projéteis ou o que mais puder haver (conseguimos isso por meio de apontadores genéricos (void *) e outras modificações).

###### 11/04:
  - ***Defesa.h, Tiro.h***: Definidos atributos básicos.

###### 09/04: 
  - Corpo do código montado, criação de alguns .h
