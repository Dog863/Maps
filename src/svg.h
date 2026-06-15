#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "grafo.h"
#include "quadra.h"

typedef struct svg SVG;

// Funções básicas
SVG* svg_criar(const char *nome_arquivo, double largura, double altura);
void svg_fechar(SVG *svg);

// Formas geométricas
void svg_retangulo(SVG *svg, double x, double y, double w, double h,
                   const char *fill, const char *stroke, double stroke_width);
void svg_circulo(SVG *svg, double cx, double cy, double r,
                 const char *fill, const char *stroke, double stroke_width);
void svg_linha(SVG *svg, double x1, double y1, double x2, double y2,
               const char *stroke, double stroke_width, const char *stroke_dasharray);
void svg_texto(SVG *svg, double x, double y, const char *texto, const char *fill, double font_size);

// Caminhos (paths)
void svg_path_begin(SVG *svg, const char *stroke, double stroke_width, const char *fill);
void svg_path_add(SVG *svg, double x, double y);
void svg_path_end(SVG *svg);

// Funções para percursos
void svg_desenhar_caminho(SVG *svg, Caminho *c, const char *stroke, double stroke_width);  // ADICIONAR ESTA LINHA
void svg_animar_caminho(SVG *svg, Caminho *c, const char *cor, double raio, double duracao);
void svg_animate_motion(SVG *svg, const char *tipo, double r, double w, double h,
                        const char *fill, const char *stroke, const char *path_data,
                        double duracao);
void svg_bounding_box(SVG *svg, double x, double y, double w, double h, const char *cor, double opacidade);

#endif
