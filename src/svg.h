#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "grafo.h"
#include "quadra.h"

/**
 * @file svg.h
 * @brief Geração de arquivos SVG
 */

typedef struct svg SVG;

/**
 * Cria um novo arquivo SVG
 * @param nome_arquivo Caminho do arquivo .svg
 * @param largura Largura da página em pixels
 * @param altura Altura da página em pixels
 * @return Ponteiro para SVG ou NULL em erro
 */
SVG* svg_criar(const char *nome_arquivo, double largura, double altura);

/**
 * Fecha o arquivo SVG
 */
void svg_fechar(SVG *svg);

/**
 * Desenha um retângulo
 */
void svg_retangulo(SVG *svg, double x, double y, double w, double h,
                   const char *fill, const char *stroke, double stroke_width);

/**
 * Desenha um círculo
 */
void svg_circulo(SVG *svg, double cx, double cy, double r,
                 const char *fill, const char *stroke, double stroke_width);

/**
 * Desenha uma linha
 */
void svg_linha(SVG *svg, double x1, double y1, double x2, double y2,
               const char *stroke, double stroke_width, const char *stroke_dasharray);

/**
 * Desenha texto
 */
void svg_texto(SVG *svg, double x, double y, const char *texto, const char *fill, double font_size);

/**
 * Inicia um path (para desenhar linhas contínuas)
 */
void svg_path_begin(SVG *svg, const char *stroke, double stroke_width, const char *fill);

/**
 * Adiciona um ponto ao path
 */
void svg_path_add(SVG *svg, double x, double y);

/**
 * Finaliza o path
 */
void svg_path_end(SVG *svg);

/**
 * Cria um elemento animado que percorre um path
 */
void svg_animate_motion(SVG *svg, const char *tipo, double r, double w, double h,
                        const char *fill, const char *stroke, const char *path,
                        double duracao);

/**
 * Desenha um caminho (percurso) do Dijkstra
 */
void svg_desenhar_caminho(SVG *svg, Caminho *c, const char *stroke, double stroke_width);

/**
 * Desenha um retângulo de bounding box com transparência
 */
void svg_bounding_box(SVG *svg, double x, double y, double w, double h, const char *cor, double opacidade);

#endif
