#ifndef REGISTRO_H
#define REGISTRO_H

/**
 * @file registro.h
 * @brief Registradores R0..R10 para referências geográficas
 */

typedef struct registros Registros;

/**
 * Cria estrutura para 11 registradores (R0 a R10)
 */
Registros* registros_criar(void);

/**
 * Destrói a estrutura
 */
void registros_destruir(Registros *r);

/**
 * Armazena uma posição no registrador
 * @param r Estrutura
 * @param idx Número do registrador (0 a 10)
 * @param x Coordenada X
 * @param y Coordenada Y
 * @return 1 se sucesso, 0 se erro
 */
int registro_armazenar(Registros *r, int idx, double x, double y);

/**
 * Obtém uma posição do registrador
 * @param r Estrutura
 * @param idx Número do registrador (0 a 10)
 * @param x Saída: coordenada X
 * @param y Saída: coordenada Y
 * @return 1 se encontrado, 0 se não definido
 */
int registro_obter(Registros *r, int idx, double *x, double *y);

#endif