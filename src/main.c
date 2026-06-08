#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>

#include "grafo.h"
#include "quadra.h"
#include "registro.h"
#include "svg.h"
#include "leitor_geo.h"
#include "leitor_via.h"
#include "leitor_qry.h"
#include "utils.h"

#define LARGURA_SVG 1500
#define ALTURA_SVG 1100

// Estrutura de contexto para passar para as funções de desenho
typedef struct {
    SVG *svg;
    ListaQuadras *quadras;
} ContextoDesenho;

typedef struct contexto_qry {
    ListaQuadras *quadras;
    Grafo *grafo;
    Registros *registros;
    SVG *svg;
    FILE *txt_output;
    double largura_svg;
    double altura_svg;
} ContextoQRY;

// Função auxiliar para criar diretório recursivamente
static void criar_diretorio(const char *path) {
    char tmp[512];
    char *p = NULL;
    snprintf(tmp, sizeof(tmp), "%s", path);
    
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    mkdir(tmp, 0755);
}

// Função callback para desenhar uma quadra no SVG
static void desenhar_quadra_callback(Quadra *q, void *dados) {
    ContextoDesenho *ctx = (ContextoDesenho*)dados;
    if (!ctx || !ctx->svg) return;
    
    double x, y, w, h;
    quadra_get_xywh(q, &x, &y, &w, &h);
    
    svg_retangulo(ctx->svg, x, y, w, h, 
                  quadra_get_fill(q), 
                  quadra_get_stroke(q), 
                  quadra_get_stroke_width(q));
    
    // Escreve o CEP dentro da quadra
    char texto[50];
    snprintf(texto, sizeof(texto), "%s", quadra_get_cep(q));
    svg_texto(ctx->svg, x + 5, y + 15, texto, "black", 10);
}

// Função para desenhar todas as quadras
static void desenhar_todas_quadras(SVG *svg, ListaQuadras *quadras) {
    ContextoDesenho ctx = {svg, quadras};
    lista_quadras_percorrer(quadras, desenhar_quadra_callback, &ctx);
}

// Função para desenhar o grafo (todas as arestas)
static void desenhar_grafo(SVG *svg, Grafo *g) {
    if (!g || !svg) return;
    
    int n = grafo_num_vertices(g);
    for (int i = 0; i < n; i++) {
        Vertice *orig = grafo_get_vertice(g, i);
        Aresta *a = vertice_get_primeira_aresta(orig, g);
        
        while (a) {
            Vertice *dest = aresta_get_destino(a);
            double x1 = vertice_get_x(orig);
            double y1 = vertice_get_y(orig);
            double x2 = vertice_get_x(dest);
            double y2 = vertice_get_y(dest);
            
            // Desenha aresta (rua)
            svg_linha(svg, x1, y1, x2, y2, "gray", 1.0, NULL);
            
            // Nome da rua no meio (opcional)
            double mx = (x1 + x2) / 2;
            double my = (y1 + y2) / 2;
            svg_texto(svg, mx, my - 5, aresta_get_nome(a), "gray", 8);
            
            a = aresta_get_prox(a);
        }
    }
}

// Exibe ajuda
static void mostrar_ajuda(const char *prog) {
    printf("========================================\n");
    printf("TED - Sistema de Informações Geográficas\n");
    printf("========================================\n\n");
    printf("Uso: %s [opções]\n", prog);
    printf("Opções:\n");
    printf("  -e <dir>      Diretório base de entrada (padrão: diretório atual)\n");
    printf("  -f <arquivo>  Arquivo .geo (quadras) - obrigatório\n");
    printf("  -v <arquivo>  Arquivo .via (sistema viário)\n");
    printf("  -q <arquivo>  Arquivo .qry (consultas)\n");
    printf("  -o <dir>      Diretório de saída (padrão: .)\n");
    printf("  -h            Mostra esta ajuda\n");
    printf("\nExemplos:\n");
    printf("  %s -f cidade.geo -o saida\n", prog);
    printf("  %s -f cidade.geo -v mapa.via -q consultas.qry -o saida\n", prog);
    printf("========================================\n");
}

int main(int argc, char *argv[]) {
    char *dir_entrada = ".";
    char *arquivo_geo = NULL;
    char *arquivo_via = NULL;
    char *arquivo_qry = NULL;
    char *dir_saida = ".";
    
    // Parse de argumentos
    int opt;
    while ((opt = getopt(argc, argv, "e:f:v:q:o:h")) != -1) {
        switch (opt) {
            case 'e':
                dir_entrada = optarg;
                break;
            case 'f':
                arquivo_geo = optarg;
                break;
            case 'v':
                arquivo_via = optarg;
                break;
            case 'q':
                arquivo_qry = optarg;
                break;
            case 'o':
                dir_saida = optarg;
                break;
            case 'h':
                mostrar_ajuda(argv[0]);
                return 0;
            default:
                mostrar_ajuda(argv[0]);
                return 1;
        }
    }
    
    // Verifica arquivo obrigatório
    if (!arquivo_geo) {
        fprintf(stderr, "Erro: Arquivo .geo não especificado (use -f)\n");
        mostrar_ajuda(argv[0]);
        return 1;
    }
    
    // Criar diretório de saída antes de qualquer operação
    criar_diretorio(dir_saida);
    
    // Constrói caminhos completos
    char caminho_geo[512];
    char caminho_via[512];
    char caminho_qry[512];
    char caminho_svg[512];
    char caminho_txt[512];
    
    concatenar_caminho(dir_entrada, arquivo_geo, caminho_geo, sizeof(caminho_geo));
    
    if (arquivo_via) {
        concatenar_caminho(dir_entrada, arquivo_via, caminho_via, sizeof(caminho_via));
    }
    
    if (arquivo_qry) {
        concatenar_caminho(dir_entrada, arquivo_qry, caminho_qry, sizeof(caminho_qry));
    }
    
    // Obtém nome base para arquivos de saída
    char nome_base[256];
    obter_nome_base(arquivo_geo, nome_base, sizeof(nome_base));
    
    if (arquivo_qry) {
        char qry_base[256];
        obter_nome_base(arquivo_qry, qry_base, sizeof(qry_base));
        
        // Usar variável temporária para evitar overlapping
        char temp_base[256];
        strcpy(temp_base, nome_base);
        snprintf(nome_base, sizeof(nome_base), "%s-%s", temp_base, qry_base);
    }
    
    concatenar_caminho(dir_saida, nome_base, caminho_svg, sizeof(caminho_svg));
    strcat(caminho_svg, ".svg");
    
    concatenar_caminho(dir_saida, nome_base, caminho_txt, sizeof(caminho_txt));
    strcat(caminho_txt, ".txt");
    
    printf("========================================\n");
    printf("TED - Sistema de Informações Geográficas\n");
    printf("========================================\n");
    printf("Arquivo geo: %s\n", caminho_geo);
    if (arquivo_via) printf("Arquivo via: %s\n", caminho_via);
    if (arquivo_qry) printf("Arquivo qry: %s\n", caminho_qry);
    printf("Saida SVG: %s\n", caminho_svg);
    printf("Saida TXT: %s\n", caminho_txt);
    printf("========================================\n");
    
    // ==================== INICIALIZAÇÃO ====================
    
    // Criar lista de quadras
    ListaQuadras *quadras = lista_quadras_criar();
    if (!quadras) {
        fprintf(stderr, "Erro: não foi possível criar lista de quadras\n");
        return 1;
    }
    
    // Ler arquivo .geo
    printf("Lendo arquivo .geo...\n");
    if (!ler_arquivo_geo(caminho_geo, quadras)) {
        fprintf(stderr, "Erro ao ler arquivo .geo\n");
        lista_quadras_destruir(quadras);
        return 1;
    }
    
    // Criar grafo (se houver arquivo .via)
    Grafo *grafo = NULL;
    if (arquivo_via) {
        printf("Lendo arquivo .via...\n");
        grafo = grafo_criar();
        if (!ler_arquivo_via(caminho_via, grafo)) {
            fprintf(stderr, "Erro ao ler arquivo .via\n");
            grafo_destruir(grafo);
            lista_quadras_destruir(quadras);
            return 1;
        }
        printf("Grafo carregado: %d vértices, %d arestas\n", 
               grafo_num_vertices(grafo), grafo_num_arestas(grafo));
    }
    
    // Criar registradores
    Registros *registros = registros_criar();
    if (!registros) {
        fprintf(stderr, "Erro: não foi possível criar registradores\n");
        if (grafo) grafo_destruir(grafo);
        lista_quadras_destruir(quadras);
        return 1;
    }
    
    // Criar SVG (o diretório já foi criado)
    printf("Criando arquivo SVG...\n");
    SVG *svg = svg_criar(caminho_svg, LARGURA_SVG, ALTURA_SVG);
    if (!svg) {
        fprintf(stderr, "Erro: não foi possível criar arquivo SVG: %s\n", caminho_svg);
        registros_destruir(registros);
        if (grafo) grafo_destruir(grafo);
        lista_quadras_destruir(quadras);
        return 1;
    }
    
    // Desenhar quadras
    printf("Desenhando quadras...\n");
    desenhar_todas_quadras(svg, quadras);
    
    // Desenhar grafo (se existir)
    if (grafo) {
        printf("Desenhando grafo...\n");
        desenhar_grafo(svg, grafo);
    }
    
    // Processar consultas (se houver arquivo .qry)
    FILE *txt_output = NULL;
    if (arquivo_qry) {
        printf("Processando consultas...\n");
        
        txt_output = fopen(caminho_txt, "w");
        if (!txt_output) {
            fprintf(stderr, "Erro: não foi possível criar arquivo TXT: %s\n", caminho_txt);
        } else {
            // Criar contexto para consultas
            ContextoQRY ctx;
            ctx.quadras = quadras;
            ctx.grafo = grafo;
            ctx.registros = registros;
            ctx.svg = svg;
            ctx.txt_output = txt_output;
            ctx.largura_svg = LARGURA_SVG;
            ctx.altura_svg = ALTURA_SVG;
            
            if (!executar_arquivo_qry(caminho_qry, &ctx, nome_base)) {
                fprintf(stderr, "Erro ao processar consultas\n");
            }
            
            fclose(txt_output);
        }
    }
    
    // Finalizar
    printf("Finalizando...\n");
    svg_fechar(svg);
    registros_destruir(registros);
    if (grafo) grafo_destruir(grafo);
    lista_quadras_destruir(quadras);
    
    printf("========================================\n");
    printf("Processamento concluído com sucesso!\n");
    printf("Arquivos gerados:\n");
    printf("  - %s\n", caminho_svg);
    if (arquivo_qry) printf("  - %s\n", caminho_txt);
    printf("========================================\n");
    
    return 0;
}