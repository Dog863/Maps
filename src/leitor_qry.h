#ifndef LEITOR_QRY_H
#define LEITOR_QRY_H

#include "grafo.h"
#include "quadra.h"
#include "registro.h"
#include "svg.h"

/**
 * @file leitor_qry.h
 * @brief Leitura e execução do arquivo .qry com consultas
 */

/**
 * Contexto para processamento das consultas
 */
typedef struct contexto_qry ContextoQRY;

/**
 * Executa todas as consultas do arquivo .qry
 * @param nome_arquivo Caminho do arquivo .qry
 * @param ctx Contexto com dados da cidade
 * @param nome_base_svg Base para gerar SVG combinado (ex: "cidade-q")
 * @return 1 em sucesso, 0 em erro
 */
int executar_arquivo_qry(const char *nome_arquivo, ContextoQRY *ctx, const char *nome_base_svg);

#endif
