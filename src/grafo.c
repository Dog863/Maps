#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct vertice {
    char id[50];
    double x, y;
    int indice;
}Vertice;

typedef struct aresta {
    char nome[100];
    char cep_dir[20];
    char cep_esq[20];
    double comprimento;
    double velocidade;
    Vertice *destino;
    Aresta *prox;
}Aresta;

typedef struct grafo {
    Vertice **vertices;
    int num_vertices;
    int cap_vertices;
    Aresta **adjacencias;
    int num_arestas;
}Grafo;

Grafo* grafo_criar(void) {
    Grafo *g = (Grafo*)malloc(sizeof(Grafo));
    if (!g) return NULL;
    
    g->cap_vertices = 100;
    g->num_vertices = 0;
    g->num_arestas = 0;
    
    g->vertices = (Vertice**)malloc(g->cap_vertices * sizeof(Vertice*));
    g->adjacencias = (Aresta**)malloc(g->cap_vertices * sizeof(Aresta*));
    
    for (int i = 0; i < g->cap_vertices; i++) {
        g->adjacencias[i] = NULL;
    }
    
    return g;
}

void grafo_destruir(Grafo *g) {
    if (!g) return;
    
    for (int i = 0; i < g->num_vertices; i++) {
        Aresta *a = g->adjacencias[i];
        while (a) {
            Aresta *prox = a->prox;
            free(a);
            a = prox;
        }
    }
    
    for (int i = 0; i < g->num_vertices; i++) {
        free(g->vertices[i]);
    }
    
    free(g->vertices);
    free(g->adjacencias);
    free(g);
}

int grafo_adicionar_vertice(Grafo *g, const char *id, double x, double y) {
    if (!g) return 0;
    
    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->vertices[i]->id, id) == 0) {
            return 0;
        }
    }
    
    if (g->num_vertices >= g->cap_vertices) {
        g->cap_vertices *= 2;
        g->vertices = (Vertice**)realloc(g->vertices, g->cap_vertices * sizeof(Vertice*));
        g->adjacencias = (Aresta**)realloc(g->adjacencias, g->cap_vertices * sizeof(Aresta*));
    }
    
    Vertice *v = (Vertice*)malloc(sizeof(Vertice));
    strcpy(v->id, id);
    v->x = x;
    v->y = y;
    v->indice = g->num_vertices;
    
    g->vertices[g->num_vertices] = v;
    g->adjacencias[g->num_vertices] = NULL;
    g->num_vertices++;
    
    return 1;
}

int grafo_adicionar_aresta(Grafo *g, const char *origem, const char *destino,
                           const char *nome, const char *cep_dir, 
                           const char *cep_esq, double comprimento, 
                           double velocidade) {
    if (!g) return 0;
    
    Vertice *v_origem = NULL;
    int idx_origem = -1;
    
    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->vertices[i]->id, origem) == 0) {
            v_origem = g->vertices[i];
            idx_origem = i;
            break;
        }
    }
    
    Vertice *v_destino = NULL;
    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->vertices[i]->id, destino) == 0) {
            v_destino = g->vertices[i];
            break;
        }
    }
    
    if (!v_origem || !v_destino) return 0;
    
    Aresta *a = (Aresta*)malloc(sizeof(Aresta));
    strcpy(a->nome, nome);
    strcpy(a->cep_dir, cep_dir);
    strcpy(a->cep_esq, cep_esq);
    a->comprimento = comprimento;
    a->velocidade = velocidade;
    a->destino = v_destino;
    
    a->prox = g->adjacencias[idx_origem];
    g->adjacencias[idx_origem] = a;
    g->num_arestas++;
    
    return 1;
}

Vertice* grafo_buscar_vertice(Grafo *g, const char *id) {
    if (!g) return NULL;
    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->vertices[i]->id, id) == 0) {
            return g->vertices[i];
        }
    }
    return NULL;
}

Vertice* grafo_encontrar_vertice_proximo(Grafo *g, double x, double y) {
    if (!g || g->num_vertices == 0) return NULL;
    
    Vertice *mais_proximo = g->vertices[0];
    double menor_dist = hypot(x - mais_proximo->x, y - mais_proximo->y);
    
    for (int i = 1; i < g->num_vertices; i++) {
        double dist = hypot(x - g->vertices[i]->x, y - g->vertices[i]->y);
        if (dist < menor_dist) {
            menor_dist = dist;
            mais_proximo = g->vertices[i];
        }
    }
    
    return mais_proximo;
}

const char* vertice_get_id(Vertice *v) { return v ? v->id : NULL; }
double vertice_get_x(Vertice *v) { return v ? v->x : 0; }
double vertice_get_y(Vertice *v) { return v ? v->y : 0; }
int vertice_get_indice(Vertice *v) { return v ? v->indice : -1; }

Aresta* vertice_get_primeira_aresta(Vertice *v, Grafo *g) {
    if (!v || !g) return NULL;
    int idx = v->indice;
    if (idx < 0 || idx >= g->num_vertices) return NULL;
    return g->adjacencias[idx];
}

Vertice* aresta_get_destino(Aresta *a) { return a ? a->destino : NULL; }
const char* aresta_get_nome(Aresta *a) { return a ? a->nome : NULL; }
const char* aresta_get_cep_dir(Aresta *a) { return a ? a->cep_dir : NULL; }
const char* aresta_get_cep_esq(Aresta *a) { return a ? a->cep_esq : NULL; }
double aresta_get_comprimento(Aresta *a) { return a ? a->comprimento : 0; }
double aresta_get_velocidade(Aresta *a) { return a ? a->velocidade : 0; }
void aresta_set_velocidade(Aresta *a, double velocidade) { if (a) a->velocidade = velocidade; }
Aresta* aresta_get_prox(Aresta *a) { return a ? a->prox : NULL; }

int grafo_num_vertices(Grafo *g) { return g ? g->num_vertices : 0; }
int grafo_num_arestas(Grafo *g) { return g ? g->num_arestas : 0; }
Vertice* grafo_get_vertice(Grafo *g, int indice) {
    if (!g || indice < 0 || indice >= g->num_vertices) return NULL;
    return g->vertices[indice];
}

void grafo_atualizar_velocidade_regiao(Grafo *g, double x, double y, 
                                        double w, double h, double nova_velocidade) {
    if (!g) return;
    
    double x2 = x + w;
    double y2 = y + h;
    
    for (int i = 0; i < g->num_vertices; i++) {
        Aresta *a = g->adjacencias[i];
        while (a) {
            Vertice *origem = g->vertices[i];
            Vertice *destino = a->destino;
            
            double ox = vertice_get_x(origem);
            double oy = vertice_get_y(origem);
            double dx = vertice_get_x(destino);
            double dy = vertice_get_y(destino);
            
            double mx = (ox + dx) / 2;
            double my = (oy + dy) / 2;
            
            if (mx >= x && mx <= x2 && my >= y && my <= y2) {
                a->velocidade = nova_velocidade;
            }
            
            a = a->prox;
        }
    }
}

void grafo_obter_todas_arestas(Grafo *g, Aresta*** arestas, int* num_arestas,
                                Vertice*** origens, int** origens_idx) {
    if (!g) {
        *arestas = NULL;
        *num_arestas = 0;
        *origens = NULL;
        *origens_idx = NULL;
        return;
    }
    
    *num_arestas = g->num_arestas;
    *arestas = (Aresta**)malloc(g->num_arestas * sizeof(Aresta*));
    *origens = (Vertice**)malloc(g->num_arestas * sizeof(Vertice*));
    *origens_idx = (int*)malloc(g->num_arestas * sizeof(int));
    
    int idx = 0;
    for (int i = 0; i < g->num_vertices; i++) {
        Aresta *a = g->adjacencias[i];
        while (a) {
            (*arestas)[idx] = a;
            (*origens)[idx] = g->vertices[i];
            (*origens_idx)[idx] = i;
            idx++;
            a = a->prox;
        }
    }
}

void grafo_liberar_lista_arestas(Aresta** arestas, Vertice** origens, int* origens_idx) {
    free(arestas);
    free(origens);
    free(origens_idx);
}
