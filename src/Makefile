PROJ_NAME = ted
CC = gcc
CFLAGS = -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall -Wextra -Wno-format-truncation -D_GNU_SOURCE
LDFLAGS = -O0 -lm

# Diretórios
SRC_DIR = .
TEST_DIR = ./test
UNITY_DIR = ../unity
OUT_DIR = ../saida
DATA_DIR = ../dados

# Caminho completo do executável
EXEC = $(SRC_DIR)/$(PROJ_NAME)

# ==================== ARQUIVOS FONTE ====================
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

TEST_QUADRA_SRCS = $(TEST_DIR)/test_quadra.c $(SRC_DIR)/quadra.c $(UNITY_DIR)/unity.c
TEST_QUADRA_OBJS = $(TEST_QUADRA_SRCS:.c=.o)
TEST_QUADRA_BIN = test_quadra

TEST_REGISTRO_SRCS = $(TEST_DIR)/test_registro.c $(SRC_DIR)/registro.c $(UNITY_DIR)/unity.c
TEST_REGISTRO_OBJS = $(TEST_REGISTRO_SRCS:.c=.o)
TEST_REGISTRO_BIN = test_registro

TEST_UTILS_SRCS = $(TEST_DIR)/test_utils.c $(SRC_DIR)/utils.c $(UNITY_DIR)/unity.c
TEST_UTILS_OBJS = $(TEST_UTILS_SRCS:.c=.o)
TEST_UTILS_BIN = test_utils

# ==================== REGRAS PRINCIPAIS ====================

all: $(EXEC)

$(EXEC): $(MAIN_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	@echo "✅ Executável criado em: $(EXEC)"

%.o: %.c
	$(CC) -c $(CFLAGS) -I$(SRC_DIR) -I$(UNITY_DIR) $< -o $@

# ==================== TESTES ====================

test_grafo: $(TEST_GRAFO_OBJS)
	$(CC) -o $(TEST_GRAFO_BIN) $^ $(LDFLAGS)
	./$(TEST_GRAFO_BIN)

test_dijkstra: $(TEST_DIJKSTRA_OBJS)
	$(CC) -o $(TEST_DIJKSTRA_BIN) $^ $(LDFLAGS)
	./$(TEST_DIJKSTRA_BIN)

test_quadra: $(TEST_QUADRA_OBJS)
	$(CC) -o $(TEST_QUADRA_BIN) $^ $(LDFLAGS)
	./$(TEST_QUADRA_BIN)

test_registro: $(TEST_REGISTRO_OBJS)
	$(CC) -o $(TEST_REGISTRO_BIN) $^ $(LDFLAGS)
	./$(TEST_REGISTRO_BIN)

test_utils: $(TEST_UTILS_OBJS)
	$(CC) -o $(TEST_UTILS_BIN) $^ $(LDFLAGS)
	./$(TEST_UTILS_BIN)

tstall: test_grafo test_dijkstra test_quadra test_registro test_utils
	@echo "✅ Todos os testes concluídos!"

# ==================== EXECUÇÃO ====================

run-c1: $(EXEC)
	mkdir -p $(OUT_DIR)
	cd .. && ./src/$(PROJ_NAME) -f $(DATA_DIR)/c1.geo -v $(DATA_DIR)/c1-v.via -q $(DATA_DIR)/consultas.qry -o $(OUT_DIR)
	@echo "========================================="
	@echo "Resultados salvos em: $(OUT_DIR)/"
	@ls -la $(OUT_DIR)/
	@echo "========================================="

# ==================== LIMPEZA ====================

clean:
	@echo "🧹 Removendo arquivos objeto..."
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(UNITY_DIR)/*.o
	@echo "🧹 Removendo executáveis..."
	rm -f $(EXEC)
	rm -f $(TEST_GRAFO_BIN) $(TEST_DIJKSTRA_BIN) $(TEST_QUADRA_BIN)
	rm -f $(TEST_REGISTRO_BIN) $(TEST_UTILS_BIN)
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
	@echo "  make test_grafo   - Testes do grafo"
	@echo "  make test_dijkstra - Testes do Dijkstra"
	@echo "  make test_quadra  - Testes da quadra"
	@echo "  make test_registro - Testes do registro"
	@echo "  make test_utils   - Testes das utils"
	@echo "  make tstall       - Executa todos os testes"
	@echo ""
	@echo "🚀 EXECUÇÃO:"
	@echo "  make run-c1       - Executa com c1.geo e c1-v.via"
	@echo ""

.PHONY: all clean clean-all tstall
.PHONY: test_grafo test_dijkstra test_quadra test_registro test_utils
.PHONY: run-c1 help
