#ifndef LEITOR_VIA_H
#define LEITOR_VIA_H

#include "grafo.h"

/**
 * @file leitor_via.h
 * @brief Leitura do arquivo .via com o sistema viário
 */

/**
 * Lê o arquivo .via e constrói o grafo
 * @param nome_arquivo Caminho do arquivo .via
 * @param g Grafo a ser preenchido
 * @return 1 em sucesso, 0 em erro
 */
int ler_arquivo_via(const char *nome_arquivo, Grafo *g);

#endif