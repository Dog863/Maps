#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "grafo.h"

/**
 * @file dijkstra.h
 * @brief Algoritmo de Dijkstra para caminhos mínimos
 */

typedef struct caminho Caminho;
typedef struct info_vertice InfoVertice;

/**
 * Calcula o caminho mais curto (menor distância)
 */
Caminho* dijkstra_distancia(Grafo *g, Vertice *origem, Vertice *destino);

/**
 * Calcula o caminho mais rápido (menor tempo)
 */
Caminho* dijkstra_tempo(Grafo *g, Vertice *origem, Vertice *destino);

/**
 * Destrói o caminho e libera memória
 */
void caminho_destruir(Caminho *c);

/**
 * Número de vértices no caminho
 */
int caminho_num_vertices(Caminho *c);

/**
 * Número de arestas no caminho
 */
int caminho_num_arestas(Caminho *c);

/**
 * Obtém vértice i do caminho (0 = origem, ultimo = destino)
 */
Vertice* caminho_get_vertice(Caminho *c, int i);

/**
 * Obtém aresta i do caminho
 */
Aresta* caminho_get_aresta(Caminho *c, int i);

/**
 * Distância total do caminho
 */
double caminho_distancia(Caminho *c);

/**
 * Tempo total do caminho
 */
double caminho_tempo(Caminho *c);

#endif