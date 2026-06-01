#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.h
 * @brief Funções utilitárias gerais
 */

/**
 * Extrai o nome base do arquivo (sem extensão)
 * Ex: "caminho/arquivo.geo" -> "arquivo"
 */
void obter_nome_base(const char *caminho, char *saida, int tam);

/**
 * Extrai o diretório de um caminho
 */
void obter_diretorio(const char *caminho, char *saida, int tam);

/**
 * Concatena caminhos (diretório + nome_arquivo)
 */
void concatenar_caminho(const char *dir, const char *arquivo, char *saida, int tam);

/**
 * Verifica se um arquivo existe
 */
int arquivo_existe(const char *caminho);

#endif