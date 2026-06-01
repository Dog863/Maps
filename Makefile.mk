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

# Arquivos fonte principais - ADICIONAR grafo.c e dijkstra.c
MAIN_SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/banco.c $(SRC_DIR)/hashfile.c \
            $(SRC_DIR)/file_reader.c $(SRC_DIR)/geo.c $(SRC_DIR)/pessoa.c \
            $(SRC_DIR)/qry.c $(SRC_DIR)/svg.c $(SRC_DIR)/grafo.c \
            $(SRC_DIR)/dijkstra.c
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

# Testes (adicionar)
TEST_GRAFO_SRCS = $(TEST_DIR)/test_grafo.c $(SRC_DIR)/grafo.c $(UNITY_DIR)/unity.c
TEST_GRAFO_OBJS = $(TEST_GRAFO_SRCS:.c=.o)
TEST_GRAFO_BIN = test_grafo

TEST_DIJKSTRA_SRCS = $(TEST_DIR)/test_dijkstra.c $(SRC_DIR)/dijkstra.c $(SRC_DIR)/grafo.c $(UNITY_DIR)/unity.c
TEST_DIJKSTRA_OBJS = $(TEST_DIJKSTRA_SRCS:.c=.o)
TEST_DIJKSTRA_BIN = test_dijkstra

# Regras principais
all: $(PROJ_NAME)

$(PROJ_NAME): $(MAIN_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -I$(SRC_DIR) -I$(UNITY_DIR) $< -o $@

# Testes
test_grafo: $(TEST_GRAFO_OBJS)
	$(CC) -o $(TEST_GRAFO_BIN) $^ $(LDFLAGS)
	./$(TEST_GRAFO_BIN)

test_dijkstra: $(TEST_DIJKSTRA_OBJS)
	$(CC) -o $(TEST_DIJKSTRA_BIN) $^ $(LDFLAGS)
	./$(TEST_DIJKSTRA_BIN)

tstall: test_grafo test_dijkstra

# Execução com dados de exemplo
run: $(PROJ_NAME)
	mkdir -p $(OUT_DIR)
	./$(PROJ_NAME) -f $(DATA_DIR)/cidade.geo -pm $(DATA_DIR)/pessoas.pm -v $(DATA_DIR)/mapa.via -q $(DATA_DIR)/consultas.qry -o $(OUT_DIR)

# Limpeza
clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(UNITY_DIR)/*.o
	rm -f $(PROJ_NAME)
	rm -f $(TEST_GRAFO_BIN) $(TEST_DIJKSTRA_BIN)
	rm -rf $(OUT_DIR)

clean-all: clean
	rm -f $(SRC_DIR)/*~ $(TEST_DIR)/*~ 2>/dev/null || true

# Help
help:
	@echo "========================================="
	@echo "COMANDOS DISPONÍVEIS"
	@echo "========================================="
	@echo "  make              - Compila o programa principal"
	@echo "  make test_grafo   - Executa testes do grafo"
	@echo "  make test_dijkstra - Executa testes do Dijkstra"
	@echo "  make tstall       - Executa todos os testes"
	@echo "  make run          - Executa com dados de exemplo"
	@echo "  make clean        - Limpa arquivos"
	@echo "  make clean-all    - Limpeza completa"
	@echo ""

.PHONY: all clean clean-all tstall test_grafo test_dijkstra run help
