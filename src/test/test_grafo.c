#include "../unity/unity.h"
#include "grafo.h"
#include <string.h>
#include <math.h>

#define TOLERANCE 0.001

static Grafo *g;

void setUp(void) {
    g = grafo_criar();
}

void tearDown(void) {
    grafo_destruir(g);
}

static int double_equal(double a, double b) {
    return fabs(a - b) < TOLERANCE;
}

/* ==================== TESTES DE CRIAÇÃO ==================== */

void test_grafo_criar_returns_not_null(void) {
    TEST_ASSERT_NOT_NULL(g);
}

void test_grafo_novo_tem_zero_vertices(void) {
    TEST_ASSERT_EQUAL_INT(0, grafo_num_vertices(g));
}

void test_grafo_novo_tem_zero_arestas(void) {
    TEST_ASSERT_EQUAL_INT(0, grafo_num_arestas(g));
}

/* ==================== TESTES DE VÉRTICES ==================== */

void test_grafo_adicionar_vertice(void) {
    int result = grafo_adicionar_vertice(g, "v1", 10.0, 20.0);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, grafo_num_vertices(g));
}

void test_grafo_adicionar_vertice_duplicado_retorna_zero(void) {
    grafo_adicionar_vertice(g, "v1", 10.0, 20.0);
    int result = grafo_adicionar_vertice(g, "v1", 30.0, 40.0);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, grafo_num_vertices(g));
}

void test_grafo_adicionar_multiplos_vertices(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    grafo_adicionar_vertice(g, "v3", 0, 100);
    TEST_ASSERT_EQUAL_INT(3, grafo_num_vertices(g));
}

void test_grafo_buscar_vertice_existente(void) {
    grafo_adicionar_vertice(g, "v1", 10.0, 20.0);
    Vertice *v = grafo_buscar_vertice(g, "v1");
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("v1", vertice_get_id(v));
    TEST_ASSERT_TRUE(double_equal(10.0, vertice_get_x(v)));
    TEST_ASSERT_TRUE(double_equal(20.0, vertice_get_y(v)));
}

void test_grafo_buscar_vertice_inexistente_retorna_null(void) {
    Vertice *v = grafo_buscar_vertice(g, "nao_existe");
    TEST_ASSERT_NULL(v);
}

void test_grafo_get_vertice_por_indice(void) {
    grafo_adicionar_vertice(g, "v0", 0, 0);
    grafo_adicionar_vertice(g, "v1", 10, 10);
    grafo_adicionar_vertice(g, "v2", 20, 20);
    
    Vertice *v = grafo_get_vertice(g, 1);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("v1", vertice_get_id(v));
}

void test_grafo_encontrar_vertice_proximo(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 100);
    grafo_adicionar_vertice(g, "v3", 200, 200);
    
    Vertice *v = grafo_encontrar_vertice_proximo(g, 95, 95);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("v2", vertice_get_id(v));
}

/* ==================== TESTES DE ARESTAS ==================== */

void test_grafo_adicionar_aresta(void) {
    grafo_adicionar_vertice(g, "origem", 0, 0);
    grafo_adicionar_vertice(g, "destino", 100, 0);
    
    int result = grafo_adicionar_aresta(g, "origem", "destino", "Rua A", 
                                        "cep1", "cep2", 100.0, 10.0);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, grafo_num_arestas(g));
}

void test_grafo_adicionar_aresta_com_vertices_inexistentes(void) {
    int result = grafo_adicionar_aresta(g, "nao_existe", "destino", "Rua A", 
                                        "-", "-", 100.0, 10.0);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(0, grafo_num_arestas(g));
}

void test_grafo_aresta_armazena_dados_corretamente(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 50, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua Teste", 
                           "cep_dir", "cep_esq", 200.0, 15.5);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Aresta *a = vertice_get_primeira_aresta(v1, g);
    
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_STRING("Rua Teste", aresta_get_nome(a));
    TEST_ASSERT_TRUE(double_equal(200.0, aresta_get_comprimento(a)));
    TEST_ASSERT_TRUE(double_equal(15.5, aresta_get_velocidade(a)));
    TEST_ASSERT_EQUAL_STRING("cep_dir", aresta_get_cep_dir(a));
    TEST_ASSERT_EQUAL_STRING("cep_esq", aresta_get_cep_esq(a));
    
    Vertice *destino = aresta_get_destino(a);
    TEST_ASSERT_NOT_NULL(destino);
    TEST_ASSERT_EQUAL_STRING("v2", vertice_get_id(destino));
}

void test_grafo_multiplas_arestas_mesmo_vertice(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    grafo_adicionar_vertice(g, "v3", 0, 100);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua X", "-", "-", 100, 10);
    grafo_adicionar_aresta(g, "v1", "v3", "Rua Y", "-", "-", 100, 10);
    
    TEST_ASSERT_EQUAL_INT(2, grafo_num_arestas(g));
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Aresta *a = vertice_get_primeira_aresta(v1, g);
    
    int count = 0;
    while (a) {
        count++;
        a = aresta_get_prox(a);
    }
    TEST_ASSERT_EQUAL_INT(2, count);
}

void test_grafo_arestas_sao_direcionadas(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua", "-", "-", 100, 10);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Vertice *v2 = grafo_buscar_vertice(g, "v2");
    
    Aresta *a1 = vertice_get_primeira_aresta(v1, g);
    Aresta *a2 = vertice_get_primeira_aresta(v2, g);
    
    TEST_ASSERT_NOT_NULL(a1);
    TEST_ASSERT_NULL(a2);
}

/* ==================== TESTES DE ATUALIZAÇÃO ==================== */

void test_grafo_atualizar_velocidade_aresta(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua", "-", "-", 100, 10);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Aresta *a = vertice_get_primeira_aresta(v1, g);
    
    TEST_ASSERT_TRUE(double_equal(10.0, aresta_get_velocidade(a)));
    
    aresta_set_velocidade(a, 25.0);
    TEST_ASSERT_TRUE(double_equal(25.0, aresta_get_velocidade(a)));
}

void test_grafo_atualizar_velocidade_regiao(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    grafo_adicionar_vertice(g, "v3", 200, 100);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua1", "-", "-", 100, 10);
    grafo_adicionar_aresta(g, "v2", "v3", "Rua2", "-", "-", 100, 20);
    
    grafo_atualizar_velocidade_regiao(g, 0, 0, 150, 50, 50.0);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Aresta *a1 = vertice_get_primeira_aresta(v1, g);
    
    TEST_ASSERT_TRUE(double_equal(50.0, aresta_get_velocidade(a1)));
}

/* ==================== MAIN ==================== */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_grafo_criar_returns_not_null);
    RUN_TEST(test_grafo_novo_tem_zero_vertices);
    RUN_TEST(test_grafo_novo_tem_zero_arestas);
    RUN_TEST(test_grafo_adicionar_vertice);
    RUN_TEST(test_grafo_adicionar_vertice_duplicado_retorna_zero);
    RUN_TEST(test_grafo_adicionar_multiplos_vertices);
    RUN_TEST(test_grafo_buscar_vertice_existente);
    RUN_TEST(test_grafo_buscar_vertice_inexistente_retorna_null);
    RUN_TEST(test_grafo_get_vertice_por_indice);
    RUN_TEST(test_grafo_encontrar_vertice_proximo);
    RUN_TEST(test_grafo_adicionar_aresta);
    RUN_TEST(test_grafo_adicionar_aresta_com_vertices_inexistentes);
    RUN_TEST(test_grafo_aresta_armazena_dados_corretamente);
    RUN_TEST(test_grafo_multiplas_arestas_mesmo_vertice);
    RUN_TEST(test_grafo_arestas_sao_direcionadas);
    RUN_TEST(test_grafo_atualizar_velocidade_aresta);
    RUN_TEST(test_grafo_atualizar_velocidade_regiao);
    return UNITY_END();
}