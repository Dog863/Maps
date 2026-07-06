#include "../unity/unity.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
void setUp(void) {
    // Não precisa fazer nada para testes de utils
}

void tearDown(void) {
    // Não precisa fazer nada para testes de utils
}

void test_obter_nome_base(void) {
    char saida[100];
    
    obter_nome_base("caminho/arquivo.geo", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("arquivo", saida);
    
    obter_nome_base("teste.txt", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("teste", saida);
    
    obter_nome_base("arquivo", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("arquivo", saida);
}

void test_obter_diretorio(void) {
    char saida[100];
    
    obter_diretorio("caminho/arquivo.geo", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("caminho", saida);
    
    obter_diretorio("arquivo.geo", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING(".", saida);
}

void test_concatenar_caminho(void) {
    char saida[200];
    
    concatenar_caminho("dir", "arquivo.txt", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("dir/arquivo.txt", saida);
    
    concatenar_caminho("dir/", "arquivo.txt", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("dir/arquivo.txt", saida);
    
    concatenar_caminho(".", "teste", saida, sizeof(saida));
    TEST_ASSERT_EQUAL_STRING("./teste", saida);
}

void test_arquivo_existe(void) {
    // Criar um arquivo temporário
    FILE *f = fopen("temp_test.txt", "w");
    fprintf(f, "teste");
    fclose(f);
    
    TEST_ASSERT_EQUAL_INT(1, arquivo_existe("temp_test.txt"));
    TEST_ASSERT_EQUAL_INT(0, arquivo_existe("nao_existe.txt"));
    
    remove("temp_test.txt");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_obter_nome_base);
    RUN_TEST(test_obter_diretorio);
    RUN_TEST(test_concatenar_caminho);
    RUN_TEST(test_arquivo_existe);
    return UNITY_END();
}