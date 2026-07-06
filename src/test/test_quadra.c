#include "../unity/unity.h"
#include "quadra.h"
#include <string.h>
#include <math.h>

#define TOLERANCE 0.001

static ListaQuadras *lista;
static Quadra *q;

void setUp(void) {
    lista = lista_quadras_criar();
    q = quadra_criar("12345", 10.0, 20.0, 100.0, 80.0, "red", "black", 2.0);
}

void tearDown(void) {
    // IMPORTANTE: Não destruir q separadamente se ele está na lista
    // Apenas destruir a lista, que vai destruir todas as quadras
    if (lista) {
        lista_quadras_destruir(lista);
        lista = NULL;
    }
    q = NULL;  // Apenas marcar como NULL, pois já foi destruído pela lista
}

static int double_equal(double a, double b) {
    return fabs(a - b) < TOLERANCE;
}

/* ==================== TESTES DA QUADRA ==================== */

void test_quadra_criar(void) {
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_STRING("12345", quadra_get_cep(q));
    TEST_ASSERT_EQUAL_STRING("red", quadra_get_fill(q));
    TEST_ASSERT_EQUAL_STRING("black", quadra_get_stroke(q));
    TEST_ASSERT_TRUE(double_equal(2.0, quadra_get_stroke_width(q)));
}

void test_quadra_get_xywh(void) {
    double x, y, w, h;
    quadra_get_xywh(q, &x, &y, &w, &h);
    TEST_ASSERT_TRUE(double_equal(10.0, x));
    TEST_ASSERT_TRUE(double_equal(20.0, y));
    TEST_ASSERT_TRUE(double_equal(100.0, w));
    TEST_ASSERT_TRUE(double_equal(80.0, h));
}

void test_quadra_calcular_endereco_norte(void) {
    double x, y;
    quadra_calcular_endereco(q, 'N', 15, &x, &y);
    TEST_ASSERT_TRUE(double_equal(95.0, x));
    TEST_ASSERT_TRUE(double_equal(20.0, y));
}

void test_quadra_calcular_endereco_sul(void) {
    double x, y;
    quadra_calcular_endereco(q, 'S', 20, &x, &y);
    TEST_ASSERT_TRUE(double_equal(90.0, x));
    TEST_ASSERT_TRUE(double_equal(100.0, y));
}

void test_quadra_calcular_endereco_leste(void) {
    double x, y;
    quadra_calcular_endereco(q, 'L', 25, &x, &y);
    TEST_ASSERT_TRUE(double_equal(110.0, x));
    TEST_ASSERT_TRUE(double_equal(75.0, y));
}

void test_quadra_calcular_endereco_oeste(void) {
    double x, y;
    quadra_calcular_endereco(q, 'O', 30, &x, &y);
    TEST_ASSERT_TRUE(double_equal(10.0, x));
    TEST_ASSERT_TRUE(double_equal(70.0, y));
}

/* ==================== TESTES DA LISTA ==================== */

void test_lista_inserir_e_buscar(void) {
    lista_quadras_inserir(lista, q);
    Quadra *encontrada = lista_quadras_buscar(lista, "12345");
    TEST_ASSERT_NOT_NULL(encontrada);
    TEST_ASSERT_EQUAL_STRING("12345", quadra_get_cep(encontrada));
}

void test_lista_buscar_inexistente(void) {
    lista_quadras_inserir(lista, q);
    Quadra *encontrada = lista_quadras_buscar(lista, "99999");
    TEST_ASSERT_NULL(encontrada);
}

void test_lista_percorrer(void) {
    Quadra *q2 = quadra_criar("67890", 0, 0, 50, 50, "blue", "white", 1.0);
    lista_quadras_inserir(lista, q);
    lista_quadras_inserir(lista, q2);
    
    int count = 0;
    void contador(Quadra *quad, void *dados) {
        (void)quad;
        (*(int*)dados)++;
    }
    
    lista_quadras_percorrer(lista, contador, &count);
    TEST_ASSERT_EQUAL_INT(2, count);
    
    // q2 já está na lista, será destruído junto com a lista
}

/* ==================== MAIN ==================== */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_quadra_criar);
    RUN_TEST(test_quadra_get_xywh);
    RUN_TEST(test_quadra_calcular_endereco_norte);
    RUN_TEST(test_quadra_calcular_endereco_sul);
    RUN_TEST(test_quadra_calcular_endereco_leste);
    RUN_TEST(test_quadra_calcular_endereco_oeste);
    RUN_TEST(test_lista_inserir_e_buscar);
    RUN_TEST(test_lista_buscar_inexistente);
    RUN_TEST(test_lista_percorrer);
    return UNITY_END();
}