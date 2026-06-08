PROJ_NAME = ted
CC = gcc
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall -Wextra -Wno-format-truncation -D_GNU_SOURCE
LDFLAGS = -O0 -lm

# Diretórios
SRC_DIR = src
TEST_DIR = src/test
UNITY_DIR = unity
OUT_DIR = saida
DATA_DIR = dados

# ==================== ARQUIVOS FONTE DO TRABALHO 2 ====================
# ADICIONAR dijkstra.c e todos os outros arquivos
MAIN_SRCS = $(SRC_DIR)/main.c \
            $(SRC_DIR)/grafo.c \
            $(SRC_DIR)/dijkstra.c \
            $(SRC_DIR)/quadra.c \
            $(SRC_DIR)/registro.c \
            $(SRC_DIR)/svg.c \
            $(SRC_DIR)/leitor_geo.c \
            $(SRC_DIR)/leitor_via.c \
            $(SRC_DIR)/leitor_qry.c \
            $(SRC_DIR)/utils.c

MAIN_OBJS = $(MAIN_SRCS:.c=.o)

# ==================== TESTES ====================

TEST_GRAFO_SRCS = $(TEST_DIR)/test_grafo.c $(SRC_DIR)/grafo.c $(UNITY_DIR)/unity.c
TEST_GRAFO_OBJS = $(TEST_GRAFO_SRCS:.c=.o)
TEST_GRAFO_BIN = test_grafo

TEST_DIJKSTRA_SRCS = $(TEST_DIR)/test_dijkstra.c $(SRC_DIR)/dijkstra.c $(SRC_DIR)/grafo.c $(UNITY_DIR)/unity.c
TEST_DIJKSTRA_OBJS = $(TEST_DIJKSTRA_SRCS:.c=.o)
TEST_DIJKSTRA_BIN = test_dijkstra

# ==================== REGRAS PRINCIPAIS ====================

all: $(PROJ_NAME)

$(PROJ_NAME): $(MAIN_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -I$(SRC_DIR) -I$(UNITY_DIR) $< -o $@

# ==================== TESTES ====================

test_grafo: $(TEST_GRAFO_OBJS)
	$(CC) -o $(TEST_GRAFO_BIN) $^ $(LDFLAGS)
	./$(TEST_GRAFO_BIN)

test_dijkstra: $(TEST_DIJKSTRA_OBJS)
	$(CC) -o $(TEST_DIJKSTRA_BIN) $^ $(LDFLAGS)
	./$(TEST_DIJKSTRA_BIN)

tstall: test_grafo test_dijkstra
	@echo "✅ Todos os testes concluídos!"

# ==================== EXECUÇÃO ====================

run: $(PROJ_NAME)
	mkdir -p $(OUT_DIR)
	./$(PROJ_NAME) -f $(DATA_DIR)/cidade.geo -v $(DATA_DIR)/mapa.via -q $(DATA_DIR)/consultas.qry -o $(OUT_DIR)
	@echo "========================================="
	@echo "Resultados salvos em: $(OUT_DIR)/"
	@ls -la $(OUT_DIR)/
	@echo "========================================="

run-geo-only: $(PROJ_NAME)
	mkdir -p $(OUT_DIR)
	./$(PROJ_NAME) -f $(DATA_DIR)/cidade.geo -o $(OUT_DIR)
	@echo "Resultados salvos em: $(OUT_DIR)/"

# ==================== LIMPEZA ====================

clean:
	@echo "🧹 Removendo arquivos objeto..."
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(UNITY_DIR)/*.o
	@echo "🧹 Removendo executáveis..."
	rm -f $(PROJ_NAME)
	rm -f $(TEST_GRAFO_BIN) $(TEST_DIJKSTRA_BIN)
	@echo "🧹 Removendo diretório de saída..."
	rm -rf $(OUT_DIR)
	@echo "✅ Limpeza concluída!"

clean-all: clean
	@echo "🧹 Limpeza completa..."
	rm -f $(SRC_DIR)/*~ $(TEST_DIR)/*~ 2>/dev/null || true
	rm -f *.hf *.hfd *.svg *.txt 2>/dev/null || true
	@echo "✅ Limpeza completa concluída!"

# ==================== DEPENDÊNCIAS ====================

$(SRC_DIR)/grafo.o: $(SRC_DIR)/grafo.h
$(SRC_DIR)/dijkstra.o: $(SRC_DIR)/dijkstra.h $(SRC_DIR)/grafo.h
$(SRC_DIR)/quadra.o: $(SRC_DIR)/quadra.h
$(SRC_DIR)/registro.o: $(SRC_DIR)/registro.h
$(SRC_DIR)/svg.o: $(SRC_DIR)/svg.h
$(SRC_DIR)/leitor_geo.o: $(SRC_DIR)/leitor_geo.h $(SRC_DIR)/quadra.h
$(SRC_DIR)/leitor_via.o: $(SRC_DIR)/leitor_via.h $(SRC_DIR)/grafo.h
$(SRC_DIR)/leitor_qry.o: $(SRC_DIR)/leitor_qry.h $(SRC_DIR)/grafo.h $(SRC_DIR)/quadra.h $(SRC_DIR)/registro.h $(SRC_DIR)/svg.h $(SRC_DIR)/dijkstra.h
$(SRC_DIR)/utils.o: $(SRC_DIR)/utils.h
$(SRC_DIR)/main.o: $(SRC_DIR)/grafo.h $(SRC_DIR)/quadra.h $(SRC_DIR)/registro.h $(SRC_DIR)/svg.h \
                   $(SRC_DIR)/leitor_geo.h $(SRC_DIR)/leitor_via.h $(SRC_DIR)/leitor_qry.h $(SRC_DIR)/utils.h

# ==================== HELP ====================

help:
	@echo "========================================="
	@echo "COMANDOS DISPONÍVEIS"
	@echo "========================================="
	@echo ""
	@echo "📦 COMPILAÇÃO:"
	@echo "  make              - Compila o programa principal"
	@echo "  make clean        - Remove arquivos objeto e executáveis"
	@echo "  make clean-all    - Limpeza completa"
	@echo ""
	@echo "🧪 TESTES:"
	@echo "  make test_grafo    - Testes do grafo"
	@echo "  make test_dijkstra - Testes do Dijkstra"
	@echo "  make tstall        - Executa todos os testes"
	@echo ""
	@echo "🚀 EXECUÇÃO:"
	@echo "  make run          - Executa com dados de exemplo"
	@echo "  make run-geo-only - Executa apenas geo"
	@echo ""
	@echo "📁 Os resultados são salvos em: $(OUT_DIR)/"
	@echo ""

.PHONY: all clean clean-all tstall
.PHONY: test_grafo test_dijkstra
.PHONY: run run-geo-only help
