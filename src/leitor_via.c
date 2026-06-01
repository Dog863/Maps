#include "leitor_via.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int ler_arquivo_via(const char *nome_arquivo, Grafo *g) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }
    
    char linha[MAX_LINE];
    int nv = 0;
    int lendo_vertices = 1;
    
    while (fgets(linha, MAX_LINE, f)) {
        linha[strcspn(linha, "\n")] = 0;
        
        if (strlen(linha) == 0) continue;
        
        char comando[10];
        sscanf(linha, "%s", comando);
        
        if (lendo_vertices && comando[0] == 'v') {
            char id[50];
            double x, y;
            sscanf(linha, "%*s %s %lf %lf", id, &x, &y);
            grafo_adicionar_vertice(g, id, x, y);
        }
        else if (comando[0] == 'e') {
            lendo_vertices = 0;
            char origem[50], destino[50];
            char cep_dir[20], cep_esq[20];
            char nome[100];
            double comprimento, velocidade;
            
            // Formato: e i j ldir lesq cmp vm nome
            int n = sscanf(linha, "%*s %s %s %s %s %lf %lf %[^\n]", 
                           origem, destino, cep_dir, cep_esq, 
                           &comprimento, &velocidade, nome);
            
            if (n >= 7) {
                grafo_adicionar_aresta(g, origem, destino, nome, cep_dir, cep_esq, 
                                       comprimento, velocidade);
            }
        }
        else if (comando[0] == 'n' && comando[1] == 'v') {
            sscanf(linha, "%*s %d", &nv);
        }
    }
    
    fclose(f);
    return 1;
}