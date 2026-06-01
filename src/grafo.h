#ifndef GRAFO_H
#define GRAFO_H

/**
 * @file grafo.h
 * @brief Implementação de grafo direcionado para sistema viário
 */

typedef struct grafo Grafo;
typedef struct vertice Vertice;
typedef struct aresta Aresta;

/**
 * Cria um novo grafo vazio
 */
Grafo* grafo_criar(void);

/**
 * Destrói o grafo e libera toda memória
 */
void grafo_destruir(Grafo *g);

/**
 * Adiciona um vértice ao grafo
 */
int grafo_adicionar_vertice(Grafo *g, const char *id, double x, double y);

/**
 * Adiciona uma aresta direcionada ao grafo
 */
int grafo_adicionar_aresta(Grafo *g, const char *origem, const char *destino,
                           const char *nome, const char *cep_dir, 
                           const char *cep_esq, double comprimento, 
                           double velocidade);

/**
 * Busca um vértice pelo ID
 */
Vertice* grafo_buscar_vertice(Grafo *g, const char *id);

/**
 * Encontra o vértice mais próximo de uma coordenada
 */
Vertice* grafo_encontrar_vertice_proximo(Grafo *g, double x, double y);

/**
 * Obtém o ID do vértice
 */
const char* vertice_get_id(Vertice *v);

/**
 * Obtém coordenada X do vértice
 */
double vertice_get_x(Vertice *v);

/**
 * Obtém coordenada Y do vértice
 */
double vertice_get_y(Vertice *v);

/**
 * Obtém o índice do vértice
 */
int vertice_get_indice(Vertice *v);

/**
 * Obtém a primeira aresta da lista de adjacência de um vértice
 */
Aresta* vertice_get_primeira_aresta(Vertice *v, Grafo *g);

/**
 * Obtém o destino da aresta
 */
Vertice* aresta_get_destino(Aresta *a);

/**
 * Obtém o nome da rua
 */
const char* aresta_get_nome(Aresta *a);

/**
 * Obtém o CEP da direita
 */
const char* aresta_get_cep_dir(Aresta *a);

/**
 * Obtém o CEP da esquerda
 */
const char* aresta_get_cep_esq(Aresta *a);

/**
 * Obtém o comprimento
 */
double aresta_get_comprimento(Aresta *a);

/**
 * Obtém a velocidade
 */
double aresta_get_velocidade(Aresta *a);

/**
 * Altera a velocidade
 */
void aresta_set_velocidade(Aresta *a, double velocidade);

/**
 * Obtém a próxima aresta na lista
 */
Aresta* aresta_get_prox(Aresta *a);

/**
 * Número de vértices
 */
int grafo_num_vertices(Grafo *g);

/**
 * Número de arestas
 */
int grafo_num_arestas(Grafo *g);

/**
 * Obtém vértice por índice
 */
Vertice* grafo_get_vertice(Grafo *g, int indice);

/**
 * Atualiza velocidade das arestas dentro de uma região
 */
void grafo_atualizar_velocidade_regiao(Grafo *g, double x, double y, 
                                        double w, double h, double nova_velocidade);

/**
 * Obtém todas as arestas (para componentes conexos)
 */
void grafo_obter_todas_arestas(Grafo *g, Aresta*** arestas, int* num_arestas,
                                Vertice*** origens, int** origens_idx);

/**
 * Libera uma lista de arestas obtida por grafo_obter_todas_arestas
 */
void grafo_liberar_lista_arestas(Aresta** arestas, Vertice** origens, int* origens_idx);

#endif