#include "leitor_qry.h"
#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 1024

// Estrutura para componentes conexos
typedef struct {
    int *visitado;
    int n_vertices;
    Grafo *g;
} DadosDFS;

typedef struct contexto_qry {
    ListaQuadras *quadras;
    Grafo *grafo;
    Registros *registros;
    SVG *svg;
    FILE *txt_output;
    double largura_svg;
    double altura_svg;
} ContextoQRY;


static void dfs_componente(DadosDFS *dados, int v, int *comp_id, int id, double vl) {
    dados->visitado[v] = 1;
    comp_id[v] = id;
    
    Vertice *vert = grafo_get_vertice(dados->g, v);
    Aresta *a = vertice_get_primeira_aresta(vert, dados->g);
    while (a) {
        if (aresta_get_velocidade(a) < vl) {
            Vertice *dest = aresta_get_destino(a);
            int w = vertice_get_indice(dest);
            if (!dados->visitado[w]) {
                dfs_componente(dados, w, comp_id, id, vl);
            }
        }
        a = aresta_get_prox(a);
    }
}

static int calcular_componentes_conexos(Grafo *g, double vl, int **comp_id) {
    int n = grafo_num_vertices(g);
    if (n == 0) return 0;
    
    DadosDFS dados;
    dados.visitado = (int*)calloc(n, sizeof(int));
    dados.n_vertices = n;
    dados.g = g;
    
    *comp_id = (int*)calloc(n, sizeof(int));
    int num = 0;
    
    for (int i = 0; i < n; i++) {
        if (!dados.visitado[i]) {
            num++;
            dfs_componente(&dados, i, *comp_id, num, vl);
        }
    }
    
    free(dados.visitado);
    return num;
}

// Estrutura para aresta na AGM
typedef struct {
    int u, v;
    double peso;
    Aresta *aresta;
    Vertice *origem;
} ArestaAGM;

static int comparar_peso(const void *a, const void *b) {
    double pa = ((ArestaAGM*)a)->peso;
    double pb = ((ArestaAGM*)b)->peso;
    if (pa < pb) return -1;
    if (pa > pb) return 1;
    return 0;
}

static int find(int *pai, int x) {
    if (pai[x] != x) pai[x] = find(pai, pai[x]);
    return pai[x];
}

static void union_sets(int *pai, int *rank, int a, int b) {
    int ra = find(pai, a);
    int rb = find(pai, b);
    if (ra != rb) {
        if (rank[ra] < rank[rb]) pai[ra] = rb;
        else if (rank[ra] > rank[rb]) pai[rb] = ra;
        else { pai[rb] = ra; rank[ra]++; }
    }
}

static void calcular_agm_velocidade_lenta(Grafo *g, double vl, SVG *svg) {
    int n = grafo_num_vertices(g);
    if (n == 0) return;
    
    int num_arestas = grafo_num_arestas(g);
    ArestaAGM *arestas = (ArestaAGM*)malloc(num_arestas * sizeof(ArestaAGM));
    int num_arestas_lentas = 0;
    
    for (int i = 0; i < n; i++) {
        Vertice *orig = grafo_get_vertice(g, i);
        Aresta *a = vertice_get_primeira_aresta(orig, g);
        while (a) {
            if (aresta_get_velocidade(a) < vl) {
                arestas[num_arestas_lentas].u = i;
                arestas[num_arestas_lentas].v = vertice_get_indice(aresta_get_destino(a));
                arestas[num_arestas_lentas].peso = aresta_get_comprimento(a);
                arestas[num_arestas_lentas].aresta = a;
                arestas[num_arestas_lentas].origem = orig;
                num_arestas_lentas++;
            }
            a = aresta_get_prox(a);
        }
    }
    
    if (num_arestas_lentas == 0) {
        free(arestas);
        return;
    }
    
    qsort(arestas, num_arestas_lentas, sizeof(ArestaAGM), comparar_peso);
    
    int *pai = (int*)malloc(n * sizeof(int));
    int *rank = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) pai[i] = i;
    
    int *selecionada = (int*)calloc(num_arestas_lentas, sizeof(int));
    int num_selecionadas = 0;
    
    for (int i = 0; i < num_arestas_lentas && num_selecionadas < n - 1; i++) {
        int ru = find(pai, arestas[i].u);
        int rv = find(pai, arestas[i].v);
        if (ru != rv) {
            union_sets(pai, rank, ru, rv);
            selecionada[i] = 1;
            num_selecionadas++;
        }
    }
    
    for (int i = 0; i < num_arestas_lentas; i++) {
        if (selecionada[i]) {
            double x1 = vertice_get_x(arestas[i].origem);
            double y1 = vertice_get_y(arestas[i].origem);
            double x2 = vertice_get_x(aresta_get_destino(arestas[i].aresta));
            double y2 = vertice_get_y(aresta_get_destino(arestas[i].aresta));
            
            svg_linha(svg, x1, y1, x2, y2, "red", 3.0, NULL);
            
            double nova_vel = aresta_get_velocidade(arestas[i].aresta) * 1.5;
            aresta_set_velocidade(arestas[i].aresta, nova_vel);
        }
    }
    
    free(pai);
    free(rank);
    free(selecionada);
    free(arestas);
}

// Processar comando @o?
static void processar_comando_oreg(ContextoQRY *ctx, char *linha) {
    char cep[20];
    char face;
    int num;
    int reg;
    
    // Tentar ler com R (ex: R1, R2)
    if (sscanf(linha, "@o? R%d %s %c %d", &reg, cep, &face, &num) == 4) {
        // OK, leu com R
    }
    // Tentar ler sem R (ex: 1, 2)
    else if (sscanf(linha, "@o? %d %s %c %d", &reg, cep, &face, &num) == 4) {
        // OK, leu sem R
    } else {
        fprintf(ctx->txt_output, "Erro: formato inválido em: %s\n", linha);
        return;
    }
    
    Quadra *q = lista_quadras_buscar(ctx->quadras, cep);
    if (q) {
        double x, y;
        quadra_calcular_endereco(q, face, num, &x, &y);
        registro_armazenar(ctx->registros, reg, x, y);
        
        svg_linha(ctx->svg, x, y, x, 0, "red", 1.0, "5,5");
        
        char r_str[10];
        snprintf(r_str, sizeof(r_str), "R%d", reg);
        svg_texto(ctx->svg, x + 2, y - 3, r_str, "red", 10);
        
        fprintf(ctx->txt_output, "[*] @o? R%d %s %c %d\n", reg, cep, face, num);
        fprintf(ctx->txt_output, "Coordenada: (%.2f, %.2f)\n", x, y);
    } else {
        fprintf(ctx->txt_output, "[*] @o? R%d %s %c %d - Quadra nao encontrada\n", reg, cep, face, num);
    }
}

// Processar comando m
static void processar_comando_m(ContextoQRY *ctx, char *linha) {
    double x, y, w, h, v;
    if (sscanf(linha, "m %lf %lf %lf %lf %lf", &x, &y, &w, &h, &v) == 5) {
        grafo_atualizar_velocidade_regiao(ctx->grafo, x, y, w, h, v);
        fprintf(ctx->txt_output, "[*] m %.2f %.2f %.2f %.2f %.2f\n", x, y, w, h, v);
        fprintf(ctx->txt_output, "Velocidades atualizadas na região\n");
    }
}

// Processar comando regs
static void processar_comando_regs(ContextoQRY *ctx, char *linha) {
    double vl;
    if (sscanf(linha, "regs %lf", &vl) == 1) {
        int *comp_id;
        int n_comps = calcular_componentes_conexos(ctx->grafo, vl, &comp_id);
        
        fprintf(ctx->txt_output, "[*] regs %.2f\n", vl);
        fprintf(ctx->txt_output, "Número de componentes conexos: %d\n", n_comps);
        
        const char *cores[] = {"red", "blue", "green", "orange", "purple", 
                               "cyan", "magenta", "yellow", "pink", "brown"};
        
        for (int c = 1; c <= n_comps && c <= 10; c++) {
            double min_x = 1e9, min_y = 1e9, max_x = -1e9, max_y = -1e9;
            int n_verts = grafo_num_vertices(ctx->grafo);
            
            for (int i = 0; i < n_verts; i++) {
                if (comp_id[i] == c) {
                    Vertice *v = grafo_get_vertice(ctx->grafo, i);
                    double x = vertice_get_x(v);
                    double y = vertice_get_y(v);
                    if (x < min_x) min_x = x;
                    if (y < min_y) min_y = y;
                    if (x > max_x) max_x = x;
                    if (y > max_y) max_y = y;
                }
            }
            
            if (min_x < 1e8) {
                double bw = max_x - min_x + 20;
                double bh = max_y - min_y + 20;
                svg_bounding_box(ctx->svg, min_x - 10, min_y - 10, bw, bh, 
                                cores[(c-1) % 10], 0.3);
            }
        }
        
        free(comp_id);
    }
}

// Processar comando exp
static void processar_comando_exp(ContextoQRY *ctx, char *linha) {
    double vl;
    if (sscanf(linha, "exp %lf", &vl) == 1) {
        fprintf(ctx->txt_output, "[*] exp %.2f\n", vl);
        calcular_agm_velocidade_lenta(ctx->grafo, vl, ctx->svg);
        fprintf(ctx->txt_output, "Arestas selecionadas tiveram velocidade aumentada em 50%%\n");
    }
}

// Processar comando p?
static void processar_comando_p(ContextoQRY *ctx, char *linha) {
    char cc[20], cr[20];
    int reg1, reg2;
    int encontrou = 0;
    
    // Tentar ler com R (ex: p? R1 R2 #ff6600 #66ff00)
    if (sscanf(linha, "p? R%d R%d %s %s", &reg1, &reg2, cc, cr) == 4) {
        encontrou = 1;
    }
    // Tentar ler sem R (ex: p? 1 2 #ff6600 #66ff00)
    else if (sscanf(linha, "p? %d %d %s %s", &reg1, &reg2, cc, cr) == 4) {
        encontrou = 1;
    }
    
    if (!encontrou) {
        fprintf(ctx->txt_output, "Erro: formato inválido em: %s\n", linha);
        return;
    }
    
    double x1, y1, x2, y2;
    
    fprintf(ctx->txt_output, "[*] p? R%d R%d %s %s\n", reg1, reg2, cc, cr);
    
    if (!registro_obter(ctx->registros, reg1, &x1, &y1) ||
        !registro_obter(ctx->registros, reg2, &x2, &y2)) {
        fprintf(ctx->txt_output, "Erro: registrador não definido (R%d ou R%d)\n", reg1, reg2);
        return;
    }
    
    // Encontrar vértices mais próximos
    Vertice *v_orig = grafo_encontrar_vertice_proximo(ctx->grafo, x1, y1);
    Vertice *v_dest = grafo_encontrar_vertice_proximo(ctx->grafo, x2, y2);
    
    if (!v_orig || !v_dest) {
        fprintf(ctx->txt_output, "Erro: não foi possível encontrar vértices próximos\n");
        return;
    }
    
    // Calcular caminhos
    Caminho *curto = dijkstra_distancia(ctx->grafo, v_orig, v_dest);
    Caminho *rapido = dijkstra_tempo(ctx->grafo, v_orig, v_dest);
    
    // Caminho mais curto
    if (curto) {
        fprintf(ctx->txt_output, "\nCaminho mais curto (distância %.2f m):\n", 
                caminho_distancia(curto));
        svg_desenhar_caminho(ctx->svg, curto, cc, 3.0);
        
        // Animação do caminho mais curto
        double duracao = caminho_distancia(curto) / 200.0;
        if (duracao < 1.0) duracao = 1.0;
        if (duracao > 10.0) duracao = 10.0;
        svg_animar_caminho(ctx->svg, curto, cc, 5.0, duracao);
        
        // Descrição textual das ruas
        for (int i = 0; i < caminho_num_arestas(curto); i++) {
            Aresta *a = caminho_get_aresta(curto, i);
            if (a) {
                fprintf(ctx->txt_output, "  - Siga pela %s\n", aresta_get_nome(a));
            }
        }
    } else {
        fprintf(ctx->txt_output, "Destino inacessível para caminho mais curto\n");
    }
    
    // Caminho mais rápido
    if (rapido) {
        fprintf(ctx->txt_output, "\nCaminho mais rápido (tempo %.2f s):\n", 
                caminho_tempo(rapido));
        svg_desenhar_caminho(ctx->svg, rapido, cr, 3.0);
        
        // Animação do caminho mais rápido (baseado no tempo real)
        double duracao = caminho_tempo(rapido) / 5.0;
        if (duracao < 0.5) duracao = 0.5;
        if (duracao > 10.0) duracao = 10.0;
        svg_animar_caminho(ctx->svg, rapido, cr, 5.0, duracao);
        
        // Descrição textual das ruas
        for (int i = 0; i < caminho_num_arestas(rapido); i++) {
            Aresta *a = caminho_get_aresta(rapido, i);
            if (a) {
                fprintf(ctx->txt_output, "  - Siga pela %s\n", aresta_get_nome(a));
            }
        }
    } else {
        fprintf(ctx->txt_output, "Destino inacessível para caminho mais rápido\n");
    }
    
    // Marcadores I (Início) e F (Fim)
    svg_circulo(ctx->svg, vertice_get_x(v_orig), vertice_get_y(v_orig), 8, "green", "black", 2);
    svg_texto(ctx->svg, vertice_get_x(v_orig) - 5, vertice_get_y(v_orig) + 5, "I", "white", 10);
    
    svg_circulo(ctx->svg, vertice_get_x(v_dest), vertice_get_y(v_dest), 8, "red", "black", 2);
    svg_texto(ctx->svg, vertice_get_x(v_dest) - 5, vertice_get_y(v_dest) + 5, "F", "white", 10);
    
    // Liberar memória
    caminho_destruir(curto);
    caminho_destruir(rapido);
}

int executar_arquivo_qry(const char *nome_arquivo, ContextoQRY *ctx, const char *nome_base_svg) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo %s\n", nome_arquivo);
        return 0;
    }
    
    char linha[MAX_LINE];
    (void)nome_base_svg;  // Evitar warning
    
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) == 0) continue;
        
        if (strncmp(linha, "@o?", 3) == 0) {
            processar_comando_oreg(ctx, linha);
        }
        else if (strncmp(linha, "m ", 2) == 0 || strncmp(linha, "mv", 2) == 0) {
            processar_comando_m(ctx, linha);
        }
        else if (strncmp(linha, "regs", 4) == 0) {
            processar_comando_regs(ctx, linha);
        }
        else if (strncmp(linha, "exp", 3) == 0) {
            processar_comando_exp(ctx, linha);
        }
        else if (strncmp(linha, "p?", 2) == 0) {
            processar_comando_p(ctx, linha);
        }
    }
    
    fclose(f);
    return 1;
}
