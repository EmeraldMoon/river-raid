/*------------------------------------------------------------------*
 |                                                                  |
 |  Representação em RGB de diversas cores.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#pragma once

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*----------------------------------------*/

/* Para chamadas das funções de cor */
#define setColor      glColor3ub
#define setColorAlpha glColor4ub

/* Constantes em RGB (red, green, blue) */
#define ALICE_BLUE              239, 248, 255
#define ANTIQUE_WHITE           249, 235, 214
#define AQUA                      0, 255, 255
#define AQUAMARINE              126, 255, 211
#define AZURE                   239, 255, 255
#define BEIGE                   245, 245, 220
#define BISQUE                  255, 227, 196
#define BLACK                     0,   0,   0
#define BLANCHED_ALMOND         255, 235, 205
#define BLUE                      0,   0, 255
#define BLUE_VIOLET             138,  43, 226
#define BROWN                   164,  42,  42
#define BURLY_WOOD              222, 184, 134
#define CADET_BLUE               95, 158, 159
#define CHARTREUSE              126, 255,   0
#define CHOCOLATE               210, 105,  30
#define CORAL                   255, 127,  80
#define CORNFLOWER_BLUE          99, 148, 236
#define CORNSILK                255, 248, 220
#define CRIMSON                 220,  20,  60
#define CYAN                      0, 255, 255
#define DARK_BLUE                 0,   0,  60
#define DARK_CYAN                 0, 138, 138
#define DARK_GOLDEN_ROD         184, 133,  10
#define DARK_GRAY               169, 169, 169
#define DARK_GREEN                0, 99,    0
#define DARK_KHAKI              189, 183, 107
#define DARK_MAGENTA            138,   0, 138
#define DARK_OLIVE_GREEN         84, 107,  46
#define DARK_ORANGE             255, 140,   0
#define DARK_ORCHID             153,  50, 204
#define DARK_SALMON             233, 150, 122
#define DARK_SEA_GREEN          143, 187, 143
#define DARK_SLATE_BLUE          71,  60, 138
#define DARK_SLATE_GRAY          46,  79,  79
#define DARK_TURQUOISE            0, 206, 209
#define DARK_VIOLET             148,   0, 211
#define DARK_RED                139,   0,   0
#define DEEP_PINK               255,  20, 147
#define DEEP_SKY_BLUE             0, 190, 255
#define DIM_GRAY                105, 105, 105
#define DODGER_BLUE              30, 144, 255
#define FIRE_BRICK              180,  34,  34
#define FLORAL_WHITE            255, 249, 239
#define FOREST_GREEN             33, 138,  33
#define FUCHSIA                 255,   0, 255
#define GAINSBORO               220, 220, 220
#define GHOST_WHITE             248, 248, 255
#define GOLD                    255, 215,   0
#define GOLDEN_ROD              218, 164,  31
#define GRAY                    128, 128, 128
#define GREEN                     0, 128,   0
#define GREEN_YELLOW            172, 255,  46
#define HONEY_DEW               239, 255, 239
#define HOT_PINK                255, 105, 180
#define INDIAN_RED              205,  92,  92
#define INDIGO                   74,   0, 130
#define IVORY                   255, 255, 239
#define KHAKI                   240, 230, 140
#define LAVENDER                230, 230, 250
#define LAVENDER_BLUSH          255, 239, 245
#define LAWN_GREEN              123, 251,   0
#define LEMON_CHIFFON           255, 250, 205
#define LIGHT_BLUE              172, 215, 230
#define LIGHT_CORAL             239, 128, 128
#define LIGHT_CYAN              223, 255, 255
#define LIGHT_GOLDEN_ROD_YELLOW 250, 250, 210
#define LIGHT_GRAY              210, 210, 210
#define LIGHT_GREEN             144, 237, 144
#define LIGHT_PINK              255, 182, 193
#define LIGHT_SALMON            255, 160, 122
#define LIGHT_SEA_GREEN          31, 177, 170
#define LIGHT_SKY_BLUE          135, 206, 250
#define LIGHT_SLATE_GRAY        119, 135, 153
#define LIGHT_STELL_BLUE        176, 196, 222
#define LIGHT_YELLOW            255, 255, 224
#define LIME                      0, 255,   0
#define LIME_GREEN               49,  205, 49
#define LINEN                   249, 239, 230
#define MAGENTA                 255,   0, 255
#define MAROON                  128,   0,   0
#define MEDIUM_AQUAMARINE       102, 205, 170
#define MEDIUM_BLUE               0,   0, 205
#define MEDIUM_ORCHID           186,  85, 211
#define MEDIUM_PURPLE           147, 112, 220
#define MEDIUM_SEA_GREEN         59, 179, 112
#define MEDIUM_SLATE_BLUE       122, 104, 237
#define MEDIUM_SPRING_GREEN       0, 249, 154
#define MEDIUM_TURQUOISE         71, 209, 204
#define MEDIUM_VIOLET_RED       199,  21, 133
#define MIDNIGHT_BLUE            24,  24, 111
#define MINT_CREAM              245, 255, 249
#define MISTY_ROSE              255, 227, 224
#define MOCCASIN                255, 228, 181
#define NAVAJO_WHITE            255, 222, 172
#define NAVY                      0,   0, 128
#define OLD_LACE                252, 245, 230
#define OLIVE                   128, 128,   0
#define OLIVE_DRAB              107, 142,  34
#define ORANGE                  255, 165,   0
#define ORANGE_RED              255,  69,   0
#define ORCHID                  218, 112, 214
#define PALE_GOLDEN_ROD         238, 232, 170
#define PALE_GREEN              151, 250, 151
#define PALE_TURQUOISE          174, 237, 237
#define PALE_VIOLET_RED         219, 112, 147
#define PAPAYA_WHIP             255, 239, 213
#define PEACH_PUFF              255, 218, 185
#define PERU                    205, 133,  62
#define PINK                    255, 192, 203
#define PLUM                    221, 160, 221
#define POWDER_BLUE             175, 223, 230
#define PURPLE                  128,   0, 128
#define RED                     255,   0,   0
#define ROSY_BROWN              187, 143, 143
#define ROYAL_BLUE               65, 105, 224
#define SADDLE_BROWN            138,  69,  19
#define SALMON                  250, 128, 114
#define SANDY_BROWN             244, 163,  95
#define SEA_GREEN                45, 138,  86
#define SEA_SHELL               255, 245, 237
#define SIENNA                  159,  82,  44
#define SILVER                  192, 192, 192
#define SKY_BLUE                134, 206, 235
#define SLATE_BLUE              106,  90, 205
#define SLATE_GRAY              111, 128, 144
#define SNOW                    255, 249, 249
#define SPRING_GREEN              0, 255, 126
#define STELL_BLUE               70, 130, 180
#define TAN                     210, 180, 139
#define TEAL                      0, 128, 128
#define THISTLE                 216, 191, 216
#define TOMATO                  255,  99,  71
#define TURQUOISE                64, 223, 208
#define VIOLET                  238, 130, 238
#define WHEAT                   245, 222, 179
#define WHITE                   255, 255, 255
#define WHITE_SMOKE             245, 245, 245
#define YELLOW                  255, 255,   0
#define YELLOW_GREEN            154, 205,  49
