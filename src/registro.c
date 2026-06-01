#include "registro.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct registros {
    double x[11];
    double y[11];
    int definido[11];
} Registros;

Registros* registros_criar(void) {
    Registros *r = (Registros*)malloc(sizeof(Registros));
    if (!r) return NULL;
    
    for (int i = 0; i < 11; i++) {
        r->definido[i] = 0;
        r->x[i] = 0;
        r->y[i] = 0;
    }
    
    return r;
}

void registros_destruir(Registros *r) {
    free(r);
}

int registro_armazenar(Registros *r, int idx, double x, double y) {
    if (!r || idx < 0 || idx > 10) return 0;
    
    r->x[idx] = x;
    r->y[idx] = y;
    r->definido[idx] = 1;
    
    return 1;
}

int registro_obter(Registros *r, int idx, double *x, double *y) {
    if (!r || idx < 0 || idx > 10 || !r->definido[idx]) return 0;
    
    *x = r->x[idx];
    *y = r->y[idx];
    
    return 1;
}