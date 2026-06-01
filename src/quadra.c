#include "quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct quadra {
    char cep[20];
    double x, y, w, h;
    char fill[20];
    char stroke[20];
    double stroke_width;
}Quadra;

typedef struct lista_quadras {
    Quadra **quadras;
    int num;
    int cap;
}ListaQuadras;

Quadra* quadra_criar(const char *cep, double x, double y, double w, double h,
                     const char *fill, const char *stroke, double sw) {
    Quadra *q = (Quadra*)malloc(sizeof(Quadra));
    if (!q) return NULL;
    
    strcpy(q->cep, cep);
    q->x = x;
    q->y = y;
    q->w = w;
    q->h = h;
    strcpy(q->fill, fill);
    strcpy(q->stroke, stroke);
    q->stroke_width = sw;
    
    return q;
}

void quadra_destruir(Quadra *q) {
    free(q);
}

const char* quadra_get_cep(Quadra *q) { return q ? q->cep : NULL; }

void quadra_get_xywh(Quadra *q, double *x, double *y, double *w, double *h) {
    if (q) {
        *x = q->x;
        *y = q->y;
        *w = q->w;
        *h = q->h;
    }
}

const char* quadra_get_fill(Quadra *q) { return q ? q->fill : NULL; }
const char* quadra_get_stroke(Quadra *q) { return q ? q->stroke : NULL; }
double quadra_get_stroke_width(Quadra *q) { return q ? q->stroke_width : 0; }

void quadra_calcular_endereco(Quadra *q, char face, int numero, double *x, double *y) {
    if (!q) return;
    
    double cx = q->x + q->w / 2;
    double cy = q->y + q->h / 2;
    
    // Âncora no canto sudeste (x + w, y + h)
    double ancora_x = q->x + q->w;
    double ancora_y = q->y + q->h;
    
    switch (face) {
        case 'N':  // Norte (topo)
            *x = ancora_x - numero;
            *y = q->y;
            break;
        case 'S':  // Sul (base)
            *x = ancora_x - numero;
            *y = q->y + q->h;
            break;
        case 'L':  // Leste (direita)
            *x = q->x + q->w;
            *y = ancora_y - numero;
            break;
        case 'O':  // Oeste (esquerda)
            *x = q->x;
            *y = ancora_y - numero;
            break;
        default:
            *x = cx;
            *y = cy;
    }
}

ListaQuadras* lista_quadras_criar(void) {
    ListaQuadras *l = (ListaQuadras*)malloc(sizeof(ListaQuadras));
    if (!l) return NULL;
    
    l->cap = 100;
    l->num = 0;
    l->quadras = (Quadra**)malloc(l->cap * sizeof(Quadra*));
    
    return l;
}

void lista_quadras_inserir(ListaQuadras *l, Quadra *q) {
    if (!l || !q) return;
    
    if (l->num >= l->cap) {
        l->cap *= 2;
        l->quadras = (Quadra**)realloc(l->quadras, l->cap * sizeof(Quadra*));
    }
    
    l->quadras[l->num++] = q;
}

Quadra* lista_quadras_buscar(ListaQuadras *l, const char *cep) {
    if (!l || !cep) return NULL;
    
    for (int i = 0; i < l->num; i++) {
        if (strcmp(l->quadras[i]->cep, cep) == 0) {
            return l->quadras[i];
        }
    }
    return NULL;
}

void lista_quadras_percorrer(ListaQuadras *l, void (*callback)(Quadra *q, void *dados), void *dados) {
    if (!l || !callback) return;
    
    for (int i = 0; i < l->num; i++) {
        callback(l->quadras[i], dados);
    }
}

void lista_quadras_destruir(ListaQuadras *l) {
    if (!l) return;
    
    for (int i = 0; i < l->num; i++) {
        quadra_destruir(l->quadras[i]);
    }
    
    free(l->quadras);
    free(l);
}