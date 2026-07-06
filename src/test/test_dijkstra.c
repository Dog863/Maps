#include "../unity/unity.h"
#include "dijkstra.h"
#include "grafo.h"
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

/* ==================== TESTES BÁSICOS ==================== */

void test_dijkstra_grafo_vazio(void) {
    Caminho *c = dijkstra_distancia(g, NULL, NULL);
    TEST_ASSERT_NULL(c);
}

void test_dijkstra_mesmo_vertice(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    
    Caminho *c = dijkstra_distancia(g, v1, v1);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(1, caminho_num_vertices(c));
    TEST_ASSERT_TRUE(double_equal(0.0, caminho_distancia(c)));
    TEST_ASSERT_TRUE(double_equal(0.0, caminho_tempo(c)));
    
    caminho_destruir(c);
}

void test_dijkstra_dois_vertices_conectados(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua", "-", "-", 100.0, 10.0);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Vertice *v2 = grafo_buscar_vertice(g, "v2");
    
    Caminho *c = dijkstra_distancia(g, v1, v2);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(2, caminho_num_vertices(c));
    TEST_ASSERT_TRUE(double_equal(100.0, caminho_distancia(c)));
    
    caminho_destruir(c);
}

void test_dijkstra_distancia_versus_tempo(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua", "-", "-", 200.0, 20.0);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Vertice *v2 = grafo_buscar_vertice(g, "v2");
    
    Caminho *c_dist = dijkstra_distancia(g, v1, v2);
    Caminho *c_temp = dijkstra_tempo(g, v1, v2);
    
    TEST_ASSERT_NOT_NULL(c_dist);
    TEST_ASSERT_NOT_NULL(c_temp);
    TEST_ASSERT_TRUE(double_equal(200.0, caminho_distancia(c_dist)));
    TEST_ASSERT_TRUE(double_equal(10.0, caminho_tempo(c_temp)));
    
    caminho_destruir(c_dist);
    caminho_destruir(c_temp);
}

/* ==================== MÚLTIPLOS CAMINHOS ==================== */

void test_dijkstra_caminho_mais_curto(void) {
    grafo_adicionar_vertice(g, "A", 0, 0);
    grafo_adicionar_vertice(g, "B", 50, 0);
    grafo_adicionar_vertice(g, "C", 100, 0);
    
    grafo_adicionar_aresta(g, "A", "B", "Rua1", "-", "-", 50.0, 10.0);
    grafo_adicionar_aresta(g, "B", "C", "Rua2", "-", "-", 50.0, 10.0);
    grafo_adicionar_aresta(g, "A", "C", "Rua Direta", "-", "-", 80.0, 10.0);
    
    Vertice *A = grafo_buscar_vertice(g, "A");
    Vertice *C = grafo_buscar_vertice(g, "C");
    
    Caminho *c = dijkstra_distancia(g, A, C);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(2, caminho_num_vertices(c));
    TEST_ASSERT_TRUE(double_equal(80.0, caminho_distancia(c)));
    
    Vertice *primeiro = caminho_get_vertice(c, 0);
    Vertice *segundo = caminho_get_vertice(c, 1);
    TEST_ASSERT_EQUAL_STRING("A", vertice_get_id(primeiro));
    TEST_ASSERT_EQUAL_STRING("C", vertice_get_id(segundo));
    
    caminho_destruir(c);
}

void test_dijkstra_caminho_mais_rapido(void) {
    grafo_adicionar_vertice(g, "A", 0, 0);
    grafo_adicionar_vertice(g, "B", 50, 0);
    grafo_adicionar_vertice(g, "C", 100, 0);
    
    grafo_adicionar_aresta(g, "A", "B", "Rua1", "-", "-", 50.0, 10.0);
    grafo_adicionar_aresta(g, "B", "C", "Rua2", "-", "-", 50.0, 10.0);
    grafo_adicionar_aresta(g, "A", "C", "Rua Rapida", "-", "-", 80.0, 40.0);
    
    Vertice *A = grafo_buscar_vertice(g, "A");
    Vertice *C = grafo_buscar_vertice(g, "C");
    
    Caminho *c = dijkstra_tempo(g, A, C);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(2, caminho_num_vertices(c));
    TEST_ASSERT_TRUE(double_equal(2.0, caminho_tempo(c)));
    
    caminho_destruir(c);
}

/* ==================== GRAFO COMPLEXO ==================== */

void test_dijkstra_grafo_complexo(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    grafo_adicionar_vertice(g, "v3", 200, 0);
    grafo_adicionar_vertice(g, "v4", 100, 100);
    grafo_adicionar_vertice(g, "v5", 0, 100);
    
    grafo_adicionar_aresta(g, "v1", "v2", "R1", "-", "-", 100, 10);
    grafo_adicionar_aresta(g, "v2", "v3", "R2", "-", "-", 100, 10);
    grafo_adicionar_aresta(g, "v3", "v4", "R3", "-", "-", 100, 5);
    grafo_adicionar_aresta(g, "v4", "v5", "R4", "-", "-", 100, 10);
    grafo_adicionar_aresta(g, "v1", "v5", "R5", "-", "-", 150, 30);
    grafo_adicionar_aresta(g, "v1", "v4", "R6", "-", "-", 120, 20);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Vertice *v3 = grafo_buscar_vertice(g, "v3");
    
    Caminho *c = dijkstra_distancia(g, v1, v3);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_TRUE(double_equal(200.0, caminho_distancia(c)));
    
    caminho_destruir(c);
}

void test_dijkstra_destino_inacessivel(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    grafo_adicionar_vertice(g, "v3", 200, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "Rua", "-", "-", 100, 10);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Vertice *v3 = grafo_buscar_vertice(g, "v3");
    
    Caminho *c = dijkstra_distancia(g, v1, v3);
    
    TEST_ASSERT_NULL(c);
}

/* ==================== GRAFO GRANDE ==================== */

void test_dijkstra_grafo_linear(void) {
    char id[10];
    for (int i = 0; i < 10; i++) {
        sprintf(id, "v%d", i);
        grafo_adicionar_vertice(g, id, i * 50, 0);
    }
    
    for (int i = 0; i < 9; i++) {
        char origem[10], destino[10];
        sprintf(origem, "v%d", i);
        sprintf(destino, "v%d", i + 1);
        grafo_adicionar_aresta(g, origem, destino, "Rua", "-", "-", 50, 10);
    }
    
    Vertice *v0 = grafo_buscar_vertice(g, "v0");
    Vertice *v9 = grafo_buscar_vertice(g, "v9");
    
    Caminho *c = dijkstra_distancia(g, v0, v9);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(10, caminho_num_vertices(c));
    TEST_ASSERT_TRUE(double_equal(450.0, caminho_distancia(c)));
    
    caminho_destruir(c);
}

/* ==================== TESTES DE ACESSORES ==================== */

void test_caminho_getters(void) {
    grafo_adicionar_vertice(g, "v1", 0, 0);
    grafo_adicionar_vertice(g, "v2", 100, 0);
    grafo_adicionar_vertice(g, "v3", 200, 0);
    
    grafo_adicionar_aresta(g, "v1", "v2", "R1", "-", "-", 100, 10);
    grafo_adicionar_aresta(g, "v2", "v3", "R2", "-", "-", 100, 20);
    
    Vertice *v1 = grafo_buscar_vertice(g, "v1");
    Vertice *v3 = grafo_buscar_vertice(g, "v3");
    
    // Usar dijkstra_tempo para testar o tempo
    Caminho *c = dijkstra_tempo(g, v1, v3);
    
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_INT(3, caminho_num_vertices(c));
    
    // Verificar os vértices do caminho
    Vertice *v = caminho_get_vertice(c, 0);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("v1", vertice_get_id(v));
    
    v = caminho_get_vertice(c, 1);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("v2", vertice_get_id(v));
    
    v = caminho_get_vertice(c, 2);
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_STRING("v3", vertice_get_id(v));
    
    double dist = caminho_distancia(c);
    double temp = caminho_tempo(c);
    
    printf("DEBUG: dist=%f, temp=%f\n", dist, temp);
    
    TEST_ASSERT_TRUE(fabs(dist - 200.0) < 0.1);
    TEST_ASSERT_TRUE(fabs(temp - 15.0) < 0.1);
    
    caminho_destruir(c);
}

/* ==================== MAIN ==================== */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_dijkstra_grafo_vazio);
    RUN_TEST(test_dijkstra_mesmo_vertice);
    RUN_TEST(test_dijkstra_dois_vertices_conectados);
    RUN_TEST(test_dijkstra_distancia_versus_tempo);
    RUN_TEST(test_dijkstra_caminho_mais_curto);
    RUN_TEST(test_dijkstra_caminho_mais_rapido);
    RUN_TEST(test_dijkstra_grafo_complexo);
    RUN_TEST(test_dijkstra_destino_inacessivel);
    RUN_TEST(test_dijkstra_grafo_linear);
    RUN_TEST(test_caminho_getters);
    return UNITY_END();
}