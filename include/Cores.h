/*------------------------------------------------------------------*
 |                                                                  |
 |  Representação em RGB de diversas cores.                         |
 |                                                                  |
 *------------------------------------------------------------------*/

#ifndef CORES_H
#define CORES_H

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

#define glColor glColor3ub
#define Cor(c)  glColor(c);

#define INDIANRED            205,  92,  92
#define LIGHTCORAL           239, 128, 128
#define SALMON               250, 128, 114
#define DARKSALMON           233, 150, 122
#define LIGHTSALMON          255, 160, 122
#define CRIMSON              220,  20,  60
#define RED                  255,   0,   0
#define FIREBRICK            180,  34,  34
#define DARKRED              139,   0,   0
#define PINK                 255, 192, 203
#define LIGHTPINK            255, 182, 193
#define HOTPINK              255, 105, 180
#define DEEPPINK             255,  20, 147
#define MEDIUMVIOLETRED      199,  21, 133
#define PALEVIOLETRED        219, 112, 147
#define LIGHTSALMON          255, 160, 122
#define CORAL                255, 127,  80
#define TOMATO               255,  99,  71
#define ORANGERED            255,  69,   0
#define DARKORANGE           255, 140,   0
#define ORANGE               255, 165,   0
#define GOLD                 255, 215,   0
#define YELLOW               255, 255,   0
#define LIGHTYELLOW          255, 255, 224
#define LEMONCHION           255, 250, 205
#define LIGHTGOLDENRODYELLOW 250, 250, 210
#define PAPAYAWHIP           255, 239, 213
#define MOCCASIN             255, 228, 181
#define PEACHPU              255, 218, 185
#define PALEGOLDENROD        238, 232, 170
#define KHAKI                240, 230, 140
#define DARKKHAKI            189, 183, 107
#define LAVENDER             230, 230, 250
#define THISTLE              216, 191, 216
#define PLUM                 221, 160, 221
#define VIOLET               238, 130, 238
#define ORCHID               218, 112, 214
#define FUCHSIA              255,   0, 255
#define MAGENTA              255,   0, 255
#define MEDIUMORCHID         186,  85, 211
#define MEDIUMPURPLE         147, 112, 220
#define BLUEVIOLET           138,  43, 226
#define DARKVIOLET           148,   0, 211
#define DARKORCHID           153,  50, 204
#define DARKMAGENTA          138,   0, 138
#define PURPLE               128,   0, 128
#define INDIGO                74,   0, 130
#define SLATEBLUE            106,  90, 205
#define DARKSLATEBLUE         71,  60, 138
#define GREENYELLOW          172, 255,  46
#define CHARTREUSE           126, 255,   0
#define LAWNGREEN            123, 251,   0
#define LIME                   0, 255,   0
#define LIMEGREEN            49,  205,  49
#define PALEGREEN            151, 250, 151
#define LIGHTGREEN           144, 237, 144
#define MEDIUMSPRINGGREEN      0, 249, 154
#define SPRINGGREEN            0, 255, 126
#define MEDIUMSEAGREEN        59, 179, 112
#define SEAGREEN              45, 138,  86
#define FORESTGREEN           33, 138,  33
#define GREEN                  0, 128,   0
#define DARKGREEN              0, 99,    0
#define YELLOWGREEN          154, 205,  49
#define OLIVEDRAB            107, 142,  34
#define OLIVE                128, 128,   0
#define DARKOLIVEGREEN        84, 107,  46
#define MEDIUMAQUAMARINE     102, 205, 170
#define DARKSEAGREEN         143, 187, 143
#define LIGHTSEAGREEN         31, 177, 170
#define DARKCYAN               0, 138, 138
#define TEAL                   0, 128, 128
#define AQUA                   0, 255, 255
#define CYAN                   0, 255, 255
#define LIGHTCYAN            223, 255, 255
#define PALETURQUOISE        174, 237, 237
#define AQUAMARINE           126, 255, 211
#define TURQUOISE             64, 223, 208
#define MEDIUMTURQUOISE       71, 209, 204
#define DARKTURQUOISE          0, 206, 209
#define CADETBLUE             95, 158, 159
#define POWDERBLUE           175, 223, 230
#define LIGHTBLUE            172, 215, 230
#define SKYBLUE              134, 206, 235
#define LIGHTSKYBLUE         134, 206, 249
#define DEEPSKYBLUE            0, 190, 255
#define DODGERBLUE            30, 144, 255
#define CORNLOWERBLUE         99, 148, 236
#define MEDIUMSLATEBLUE      122, 104, 237
#define ROYALBLUE             65, 105, 224
#define BLUE                   0,   0, 255
#define MEDIUMBLUE             0,   0, 205
#define SHADOWBLUE             0,   0, 138
#define DARKBLUE               0,   0,  60
#define NAVY                   0,   0, 128
#define MIDNIGHTBLUE          24,  24, 111
#define CORNSILK             255, 248, 220
#define BLANCHEDALMOND       255, 235, 205
#define BISQUE               255, 227, 196
#define NAVAJOWHITE          255, 222, 172
#define WHEAT                245, 222, 179
#define BURLYWOOD            222, 184, 134
#define TAN                  210, 180, 139
#define ROSYBROWN            187, 143, 143
#define SANDYBROWN           244, 163,  95
#define GOLDENROD            218, 164,  31
#define DARKGOLDENROD        184, 133,  10
#define PERU                 205, 133,  62
#define CHOCOLATE            210, 105,  30
#define SADDLEBROWN          138,  69,  19
#define SIENNA               159,  82,  44
#define BROWN                164,  42,  42
#define MAROON               128,   0,   0
#define WHITE                255, 255, 255
#define SNOW                 255, 249, 249
#define HONEYDEW             239, 255, 239
#define MINTCREAM            245, 255, 249
#define AZURE                239, 255, 255
#define ALICEBLUE            239, 248, 255
#define GHOSTWHITE           248, 248, 255
#define WHITESMOKE           245, 245, 245
#define SEASHELL             255, 245, 237
#define BEIGE                245, 245, 220
#define OLDLACE              252, 245, 230
#define FLORALWHITE          255, 249, 239
#define IVORY                255, 255, 239
#define ANTIQUEWHITE         249, 235, 214
#define LINEN                249, 239, 230
#define LAVENDERBLUSH        255, 239, 245
#define MISTYROSE            255, 227, 224
#define GAINSBORO            220, 220, 220
#define LIGHTGREY            210, 210, 210
#define SILVER               192, 192, 192
#define DARKGRAY             169, 169, 169
#define GRAY                 128, 128, 128
#define DIMGRAY              105, 105, 105
#define LIGHTSLATEGRAY       119, 135, 153
#define SLATEGRAY            111, 128, 144
#define DARKSLATEGRAY         46,  79,  79
#define BLACK                  0,   0,   0

#endif
