#include "../unity/unity.h"
#include "registro.h"
#include <math.h>

#define TOLERANCE 0.001

static Registros *r;

// Função auxiliar para comparar doubles
static int double_equal(double a, double b) {
    return fabs(a - b) < TOLERANCE;
}

void setUp(void) {
    r = registros_criar();
}

void tearDown(void) {
    registros_destruir(r);
}

void test_registros_criar(void) {
    TEST_ASSERT_NOT_NULL(r);
}

void test_registro_armazenar_e_obter(void) {
    int result = registro_armazenar(r, 0, 100.0, 200.0);
    TEST_ASSERT_EQUAL_INT(1, result);
    
    double x, y;
    result = registro_obter(r, 0, &x, &y);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_TRUE(double_equal(100.0, x));
    TEST_ASSERT_TRUE(double_equal(200.0, y));
}

void test_registro_obter_nao_definido(void) {
    double x, y;
    int result = registro_obter(r, 5, &x, &y);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_registro_sobrescrever(void) {
    registro_armazenar(r, 1, 50.0, 60.0);
    registro_armazenar(r, 1, 70.0, 80.0);
    
    double x, y;
    registro_obter(r, 1, &x, &y);
    TEST_ASSERT_TRUE(double_equal(70.0, x));
    TEST_ASSERT_TRUE(double_equal(80.0, y));
}

void test_registros_multiplos(void) {
    for (int i = 0; i <= 10; i++) {
        registro_armazenar(r, i, i * 10.0, i * 20.0);
    }
    
    for (int i = 0; i <= 10; i++) {
        double x, y;
        registro_obter(r, i, &x, &y);
        TEST_ASSERT_TRUE(double_equal(i * 10.0, x));
        TEST_ASSERT_TRUE(double_equal(i * 20.0, y));
    }
}

void test_registro_indice_invalido(void) {
    int result = registro_armazenar(r, 11, 100.0, 200.0);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    double x, y;
    result = registro_obter(r, 11, &x, &y);
    TEST_ASSERT_EQUAL_INT(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_registros_criar);
    RUN_TEST(test_registro_armazenar_e_obter);
    RUN_TEST(test_registro_obter_nao_definido);
    RUN_TEST(test_registro_sobrescrever);
    RUN_TEST(test_registros_multiplos);
    RUN_TEST(test_registro_indice_invalido);
    return UNITY_END();
}