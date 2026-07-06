#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#ifndef INFINITY
#define INFINITY DBL_MAX
#endif

typedef struct caminho {
    Vertice **vertices;
    Aresta **arestas;
    int num_vertices;
    int num_arestas;
    double distancia_total;
    double tempo_total;
} Caminho;

typedef struct info_vertice {
    double distancia;
    double tempo;
    int anterior;
    int visitado;
} InfoVertice;

static Caminho* dijkstra_geral(Grafo *g, Vertice *origem, Vertice *destino, int usar_tempo) {
    if (!g || !origem || !destino) return NULL;
    
    int n = grafo_num_vertices(g);
    int idx_origem = vertice_get_indice(origem);
    int idx_destino = vertice_get_indice(destino);
    
    // CASO ESPECIAL: origem == destino
    if (idx_origem == idx_destino) {
        Caminho *c = (Caminho*)malloc(sizeof(Caminho));
        if (!c) return NULL;
        
        c->num_vertices = 1;
        c->num_arestas = 0;
        c->vertices = (Vertice**)malloc(sizeof(Vertice*));
        if (!c->vertices) {
            free(c);
            return NULL;
        }
        c->vertices[0] = origem;
        c->arestas = NULL;
        c->distancia_total = 0.0;
        c->tempo_total = 0.0;
        return c;
    }
    
    // Alocar array de informações dos vértices
    InfoVertice *info = (InfoVertice*)malloc(n * sizeof(InfoVertice));
    if (!info) return NULL;
    
    // Inicializar informações
    for (int i = 0; i < n; i++) {
        info[i].distancia = INFINITY;
        info[i].tempo = INFINITY;
        info[i].anterior = -1;
        info[i].visitado = 0;
    }
    
    // Definir o valor inicial para a origem
    info[idx_origem].distancia = 0;
    info[idx_origem].tempo = 0;
    
    // Algoritmo de Dijkstra usando o peso escolhido (distância ou tempo)
    for (int count = 0; count < n - 1; count++) {
        double min_valor = INFINITY;
        int u = -1;
        
        // Encontrar o vértice não visitado com menor valor
        for (int i = 0; i < n; i++) {
            if (!info[i].visitado) {
                double valor = usar_tempo ? info[i].tempo : info[i].distancia;
                if (valor < min_valor) {
                    min_valor = valor;
                    u = i;
                }
            }
        }
        
        if (u == -1 || u == idx_destino) break;
        
        info[u].visitado = 1;
        
        // Relaxar as arestas do vértice u
        Vertice *v_u = grafo_get_vertice(g, u);
        Aresta *a = vertice_get_primeira_aresta(v_u, g);
        
        while (a) {
            Vertice *v_dest = aresta_get_destino(a);
            int v = vertice_get_indice(v_dest);
            
            if (!info[v].visitado) {
                double peso_distancia = aresta_get_comprimento(a);
                double peso_tempo = 0;
                double vel = aresta_get_velocidade(a);
                if (vel > 0) {
                    peso_tempo = aresta_get_comprimento(a) / vel;
                } else {
                    peso_tempo = INFINITY;
                }
                
                // Atualizar distância
                if (info[u].distancia + peso_distancia < info[v].distancia) {
                    info[v].distancia = info[u].distancia + peso_distancia;
                    if (!usar_tempo) {
                        info[v].anterior = u;
                    }
                }
                
                // Atualizar tempo
                if (info[u].tempo + peso_tempo < info[v].tempo) {
                    info[v].tempo = info[u].tempo + peso_tempo;
                    if (usar_tempo) {
                        info[v].anterior = u;
                    }
                }
            }
            a = aresta_get_prox(a);
        }
    }
    
    // Verificar se o destino foi alcançado
    if (info[idx_destino].anterior == -1 && idx_origem != idx_destino) {
        free(info);
        return NULL;
    }
    
    // Reconstruir o caminho (do destino até a origem)
    int *indices = (int*)malloc(n * sizeof(int));
    if (!indices) {
        free(info);
        return NULL;
    }
    
    int count = 0;
    int atual = idx_destino;
    
    while (atual != -1) {
        indices[count++] = atual;
        atual = info[atual].anterior;
    }
    
    // Criar a estrutura do caminho
    Caminho *c = (Caminho*)malloc(sizeof(Caminho));
    if (!c) {
        free(indices);
        free(info);
        return NULL;
    }
    
    c->num_vertices = count;
    c->num_arestas = count - 1;
    c->vertices = (Vertice**)malloc(count * sizeof(Vertice*));
    if (!c->vertices) {
        free(c);
        free(indices);
        free(info);
        return NULL;
    }
    
    c->arestas = (Aresta**)malloc((count - 1) * sizeof(Aresta*));
    if (!c->arestas && count - 1 > 0) {
        free(c->vertices);
        free(c);
        free(indices);
        free(info);
        return NULL;
    }
    
    // Preencher os vértices (invertendo a ordem)
    for (int i = 0; i < count; i++) {
        c->vertices[i] = grafo_get_vertice(g, indices[count - 1 - i]);
    }
    
    // Reconstruir as arestas
    for (int i = 0; i < count - 1; i++) {
        Vertice *v_orig = c->vertices[i];
        Vertice *v_dest = c->vertices[i + 1];
        int idx_orig = vertice_get_indice(v_orig);
        int encontrou = 0;
        
        Vertice *v_temp = grafo_get_vertice(g, idx_orig);
        if (v_temp == v_orig) {
            Aresta *a_temp = vertice_get_primeira_aresta(v_temp, g);
            while (a_temp && !encontrou) {
                if (aresta_get_destino(a_temp) == v_dest) {
                    c->arestas[i] = a_temp;
                    encontrou = 1;
                }
                a_temp = aresta_get_prox(a_temp);
            }
        }
        
        if (!encontrou) {
            c->arestas[i] = NULL;
        }
    }
    
    // Preencher distância e tempo total
    c->distancia_total = info[idx_destino].distancia;
    c->tempo_total = info[idx_destino].tempo;
    
    // CORREÇÃO: Se valores forem INFINITY, definir como 0
    if (c->distancia_total == INFINITY || isnan(c->distancia_total)) {
        c->distancia_total = 0;
    }
    if (c->tempo_total == INFINITY || isnan(c->tempo_total)) {
        c->tempo_total = 0;
    }
    
    // Liberar memória temporária
    free(indices);
    free(info);
    
    return c;
}

Caminho* dijkstra_distancia(Grafo *g, Vertice *origem, Vertice *destino) {
    return dijkstra_geral(g, origem, destino, 0);
}

Caminho* dijkstra_tempo(Grafo *g, Vertice *origem, Vertice *destino) {
    return dijkstra_geral(g, origem, destino, 1);
}

void caminho_destruir(Caminho *c) {
    if (!c) return;
    if (c->vertices) free(c->vertices);
    if (c->arestas) free(c->arestas);
    free(c);
}

int caminho_num_vertices(Caminho *c) { 
    return c ? c->num_vertices : 0; 
}

int caminho_num_arestas(Caminho *c) { 
    return c ? c->num_arestas : 0; 
}

Vertice* caminho_get_vertice(Caminho *c, int i) {
    if (!c || i < 0 || i >= c->num_vertices) return NULL;
    return c->vertices[i];
}

Aresta* caminho_get_aresta(Caminho *c, int i) {
    if (!c || i < 0 || i >= c->num_arestas) return NULL;
    return c->arestas[i];
}

double caminho_distancia(Caminho *c) { 
    return c ? c->distancia_total : 0; 
}

double caminho_tempo(Caminho *c) { 
    return c ? c->tempo_total : 0; 
}
