#include "leitor_geo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct structestilo_quadra {
    double stroke_width;
    char fill[20];
    char stroke[20];
} EstiloQuadra;

int ler_arquivo_geo(const char *nome_arquivo, ListaQuadras *quadras) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }
    
    char linha[MAX_LINE];
    EstiloQuadra estilo_atual;
    estilo_atual.stroke_width = 1.0;
    strcpy(estilo_atual.fill, "lightgray");
    strcpy(estilo_atual.stroke, "black");
    
    while (fgets(linha, MAX_LINE, f)) {
        // Remove \n do final
        linha[strcspn(linha, "\n")] = 0;
        
        char comando[10];
        sscanf(linha, "%s", comando);
        
        if (strcmp(comando, "q") == 0) {
            char cep[20];
            double x, y, w, h;
            int n = sscanf(linha, "%*s %s %lf %lf %lf %lf", cep, &x, &y, &w, &h);
            if (n >= 5) {
                Quadra *q = quadra_criar(cep, x, y, w, h,
                                         estilo_atual.fill,
                                         estilo_atual.stroke,
                                         estilo_atual.stroke_width);
                if (q) {
                    lista_quadras_inserir(quadras, q);
                }
            }
        }
        else if (strcmp(comando, "cq") == 0) {
            double sw;
            char cfill[20], cstrk[20];
            int n = sscanf(linha, "%*s %lf %s %s", &sw, cfill, cstrk);
            if (n >= 3) {
                estilo_atual.stroke_width = sw;
                strcpy(estilo_atual.fill, cfill);
                strcpy(estilo_atual.stroke, cstrk);
            }
        }
    }
    
    fclose(f);
    return 1;
}