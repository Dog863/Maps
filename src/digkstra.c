#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define INFINITY DBL_MAX

typedef struct caminho {
    Vertice **vertices;
    Aresta **arestas;
    int num_vertices;
    int num_arestas;
    double distancia_total;
    double tempo_total;
}Caminho;

typedef struct info_vertice {
    double distancia;
    double tempo;
    int anterior;
    int visitado;
} InfoVertice;

static Caminho* dijkstra_geral(Grafo *g, Vertice *origem, Vertice *destino, int usar_tempo) {
    if (!g || !origem || !destino) return NULL;
    
    int n = grafo_num_vertices(g);
    InfoVertice *info = (InfoVertice*)malloc(n * sizeof(InfoVertice));
    
    for (int i = 0; i < n; i++) {
        info[i].distancia = INFINITY;
        info[i].tempo = INFINITY;
        info[i].anterior = -1;
        info[i].visitado = 0;
    }
    
    int idx_origem = vertice_get_indice(origem);
    int idx_destino = vertice_get_indice(destino);
    
    if (usar_tempo) {
        info[idx_origem].tempo = 0;
    } else {
        info[idx_origem].distancia = 0;
    }
    
    for (int count = 0; count < n - 1; count++) {
        double min_valor = INFINITY;
        int u = -1;
        
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
        
        Vertice *v_u = grafo_get_vertice(g, u);
        Aresta *a = vertice_get_primeira_aresta(v_u, g);
        
        while (a) {
            Vertice *v_dest = aresta_get_destino(a);
            int v = vertice_get_indice(v_dest);
            
            if (!info[v].visitado) {
                double peso;
                if (usar_tempo) {
                    double vel = aresta_get_velocidade(a);
                    peso = (vel > 0) ? (aresta_get_comprimento(a) / vel) : INFINITY;
                    if (info[u].tempo + peso < info[v].tempo) {
                        info[v].tempo = info[u].tempo + peso;
                        info[v].anterior = u;
                    }
                } else {
                    peso = aresta_get_comprimento(a);
                    if (info[u].distancia + peso < info[v].distancia) {
                        info[v].distancia = info[u].distancia + peso;
                        info[v].anterior = u;
                    }
                }
            }
            a = aresta_get_prox(a);
        }
    }
    
    if (info[idx_destino].anterior == -1 && idx_origem != idx_destino) {
        free(info);
        return NULL;
    }
    
    int *indices = (int*)malloc(n * sizeof(int));
    int count = 0;
    int atual = idx_destino;
    
    while (atual != -1) {
        indices[count++] = atual;
        atual = info[atual].anterior;
    }
    
    Caminho *c = (Caminho*)malloc(sizeof(Caminho));
    c->num_vertices = count;
    c->num_arestas = count - 1;
    c->vertices = (Vertice**)malloc(count * sizeof(Vertice*));
    c->arestas = (Aresta**)malloc((count - 1) * sizeof(Aresta*));
    
    for (int i = 0; i < count; i++) {
        c->vertices[i] = grafo_get_vertice(g, indices[count - 1 - i]);
    }
    
    for (int i = 0; i < count - 1; i++) {
        Vertice *v_orig = c->vertices[i];
        Vertice *v_dest = c->vertices[i + 1];
        Aresta *a = vertice_get_primeira_aresta(v_orig, g);
        
        while (a) {
            if (aresta_get_destino(a) == v_dest) {
                c->arestas[i] = a;
                break;
            }
            a = aresta_get_prox(a);
        }
    }
    
    c->distancia_total = info[idx_destino].distancia;
    c->tempo_total = info[idx_destino].tempo;
    
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
    free(c->vertices);
    free(c->arestas);
    free(c);
}

int caminho_num_vertices(Caminho *c) { return c ? c->num_vertices : 0; }
int caminho_num_arestas(Caminho *c) { return c ? c->num_arestas : 0; }
Vertice* caminho_get_vertice(Caminho *c, int i) {
    if (!c || i < 0 || i >= c->num_vertices) return NULL;
    return c->vertices[i];
}
Aresta* caminho_get_aresta(Caminho *c, int i) {
    if (!c || i < 0 || i >= c->num_arestas) return NULL;
    return c->arestas[i];
}
double caminho_distancia(Caminho *c) { return c ? c->distancia_total : 0; }
double caminho_tempo(Caminho *c) { return c ? c->tempo_total : 0; }