#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void obter_nome_base(const char *caminho, char *saida, int tam) {
    // Encontra o último '/' ou '\'
    const char *nome = strrchr(caminho, '/');
    if (!nome) nome = strrchr(caminho, '\\');
    if (!nome) nome = caminho;
    else nome++;
    
    // Copia até encontrar '.'
    int i = 0;
    while (nome[i] && nome[i] != '.' && i < tam - 1) {
        saida[i] = nome[i];
        i++;
    }
    saida[i] = '\0';
}

void obter_diretorio(const char *caminho, char *saida, int tam) {
    const char *ultima_barra = strrchr(caminho, '/');
    if (!ultima_barra) ultima_barra = strrchr(caminho, '\\');
    
    if (ultima_barra) {
        int len = ultima_barra - caminho;
        if (len < tam) {
            strncpy(saida, caminho, len);
            saida[len] = '\0';
        }
    } else {
        strcpy(saida, ".");
    }
}

void concatenar_caminho(const char *dir, const char *arquivo, char *saida, int tam) {
    if (!dir || !arquivo) {
        saida[0] = '\0';
        return;
    }
    
    int len_dir = strlen(dir);
    if (len_dir > 0 && dir[len_dir - 1] == '/') {
        snprintf(saida, tam, "%s%s", dir, arquivo);
    } else {
        snprintf(saida, tam, "%s/%s", dir, arquivo);
    }
}

int arquivo_existe(const char *caminho) {
    FILE *f = fopen(caminho, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}