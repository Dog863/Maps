#ifndef LEITOR_GEO_H
#define LEITOR_GEO_H

#include "quadra.h"

/**
 * @file leitor_geo.h
 * @brief Leitura do arquivo .geo com as quadras da cidade
 */

/**
 * Configurações de estilo padrão das quadras
 */
typedef struct structestilo_quadra EstiloQuadra;

/**
 * Lê o arquivo .geo e cria as quadras
 * @param nome_arquivo Caminho do arquivo .geo
 * @param quadras Lista onde as quadras serão inseridas
 * @return 1 em sucesso, 0 em erro
 */
int ler_arquivo_geo(const char *nome_arquivo, ListaQuadras *quadras);

#endif