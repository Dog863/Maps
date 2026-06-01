#ifndef QUADRA_H
#define QUADRA_H

/**
 * @file quadra.h
 * @brief Gerenciamento de quadras da cidade
 */

typedef struct quadra Quadra;
typedef struct lista_quadras ListaQuadras;

/**
 * Cria uma nova quadra
 */
Quadra* quadra_criar(const char *cep, double x, double y, double w, double h,
                     const char *fill, const char *stroke, double sw);

/**
 * Destrói uma quadra
 */
void quadra_destruir(Quadra *q);

/**
 * Obtém o CEP da quadra
 */
const char* quadra_get_cep(Quadra *q);

/**
 * Obtém coordenadas e dimensões da quadra
 */
void quadra_get_xywh(Quadra *q, double *x, double *y, double *w, double *h);

/**
 * Obtém as cores da quadra
 */
const char* quadra_get_fill(Quadra *q);
const char* quadra_get_stroke(Quadra *q);
double quadra_get_stroke_width(Quadra *q);

/**
 * Calcula coordenada de um endereço na face da quadra
 * @param q Quadra
 * @param face 'N', 'S', 'L', 'O'
 * @param numero Distância da projeção do ponto de ancoragem (canto sudeste)
 * @param x Saída: coordenada X
 * @param y Saída: coordenada Y
 */
void quadra_calcular_endereco(Quadra *q, char face, int numero, double *x, double *y);

/**
 * Cria uma lista vazia de quadras
 */
ListaQuadras* lista_quadras_criar(void);

/**
 * Insere uma quadra na lista
 */
void lista_quadras_inserir(ListaQuadras *l, Quadra *q);

/**
 * Busca uma quadra pelo CEP
 */
Quadra* lista_quadras_buscar(ListaQuadras *l, const char *cep);

/**
 * Percorre todas as quadras (para desenho no SVG)
 * @param l Lista
 * @param callback Função chamada para cada quadra
 * @param dados Dados adicionais para callback
 */
void lista_quadras_percorrer(ListaQuadras *l, void (*callback)(Quadra *q, void *dados), void *dados);

/**
 * Destrói a lista e todas as quadras
 */
void lista_quadras_destruir(ListaQuadras *l);

#endif