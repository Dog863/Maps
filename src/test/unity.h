#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definições básicas
#define UNITY_OUTPUT_CHAR(c) putchar(c)
#define UNITY_OUTPUT_FLUSH() fflush(stdout)

// Macros de teste
#define TEST_ASSERT_TRUE(condition) \
    if (!(condition)) { \
        printf("FALHA: %s:%d - Esperado TRUE, obtido FALSE\n", __FILE__, __LINE__); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_FALSE(condition) \
    if (condition) { \
        printf("FALHA: %s:%d - Esperado FALSE, obtido TRUE\n", __FILE__, __LINE__); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    if ((expected) != (actual)) { \
        printf("FALHA: %s:%d - Esperado %d, obtido %d\n", __FILE__, __LINE__, expected, actual); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_EQUAL_PTR(expected, actual) \
    if ((expected) != (actual)) { \
        printf("FALHA: %s:%d - Ponteiros diferentes\n", __FILE__, __LINE__); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    if (strcmp(expected, actual) != 0) { \
        printf("FALHA: %s:%d - Esperado '%s', obtido '%s'\n", __FILE__, __LINE__, expected, actual); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_EQUAL_DOUBLE(expected, actual, tolerance) \
    if (((expected) - (actual)) > (tolerance) || ((actual) - (expected)) > (tolerance)) { \
        printf("FALHA: %s:%d - Esperado %f, obtido %f\n", __FILE__, __LINE__, expected, actual); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_NULL(pointer) \
    if ((pointer) != NULL) { \
        printf("FALHA: %s:%d - Ponteiro não é NULL\n", __FILE__, __LINE__); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_NOT_NULL(pointer) \
    if ((pointer) == NULL) { \
        printf("FALHA: %s:%d - Ponteiro é NULL\n", __FILE__, __LINE__); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_EQUAL_CHAR(expected, actual) \
    if ((expected) != (actual)) { \
        printf("FALHA: %s:%d - Esperado '%c', obtido '%c'\n", __FILE__, __LINE__, expected, actual); \
        unity_suite.tests_failed++; \
        return; \
    }

#define TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len) \
    if (strncmp(expected, actual, len) != 0) { \
        printf("FALHA: %s:%d - Strings diferentes\n", __FILE__, __LINE__); \
        unity_suite.tests_failed++; \
        return; \
    }

// Estrutura para suite de testes
typedef struct {
    int tests_passed;
    int tests_failed;
    int tests_total;
} UnitySuite;

static UnitySuite unity_suite = {0, 0, 0};

// Funções dummy para compatibilidade
#define setUp()
#define tearDown()

#define UNITY_BEGIN() \
    unity_suite.tests_passed = 0; \
    unity_suite.tests_failed = 0; \
    unity_suite.tests_total = 0; \
    printf("========================================\n"); \
    printf("INICIANDO TESTES\n"); \
    printf("========================================\n\n");

#define UNITY_END() \
    printf("\n========================================\n"); \
    printf("RESULTADO DOS TESTES\n"); \
    printf("========================================\n"); \
    printf("Total: %d\n", unity_suite.tests_total); \
    printf("Passou: %d\n", unity_suite.tests_passed); \
    printf("Falhou: %d\n", unity_suite.tests_failed); \
    printf("========================================\n"); \
    return (unity_suite.tests_failed == 0) ? 0 : 1;

#define RUN_TEST(test_function) \
    do { \
        unity_suite.tests_total++; \
        printf("Executando: %s... ", #test_function); \
        fflush(stdout); \
        test_function(); \
        if (unity_suite.tests_failed == 0) { \
            unity_suite.tests_passed++; \
            printf("✅ PASSOU\n"); \
        } else { \
            printf("❌ FALHOU\n"); \
        } \
    } while(0)

#endif