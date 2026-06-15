#include "svg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct svg {
    FILE *file;
} SVG;

// Função auxiliar para criar diretório
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

SVG* svg_criar(const char *nome_arquivo, double largura, double altura) {
    SVG *svg = (SVG*)malloc(sizeof(SVG));
    if (!svg) return NULL;
    
    char dir_path[512];
    strcpy(dir_path, nome_arquivo);
    char *last_slash = strrchr(dir_path, '/');
    if (last_slash) {
        *last_slash = '\0';
        criar_diretorio(dir_path);
    }
    
    svg->file = fopen(nome_arquivo, "w");
    if (!svg->file) {
        free(svg);
        return NULL;
    }
    
    fprintf(svg->file, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" "
            "width=\"%.0f\" height=\"%.0f\">\n", largura, altura);
    fprintf(svg->file, "<style>\n");
    fprintf(svg->file, "  text { font-family: sans-serif; font-size: 10px; }\n");
    fprintf(svg->file, "  .caminho { fill: none; stroke-linecap: round; stroke-linejoin: round; }\n");
    fprintf(svg->file, "</style>\n");
    
    return svg;
}

void svg_fechar(SVG *svg) {
    if (!svg) return;
    fprintf(svg->file, "</svg>\n");
    fclose(svg->file);
    free(svg);
}

void svg_retangulo(SVG *svg, double x, double y, double w, double h,
                   const char *fill, const char *stroke, double stroke_width) {
    if (!svg) return;
    fprintf(svg->file, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
            "fill=\"%s\" stroke=\"%s\" stroke-width=\"%.1f\" />\n",
            x, y, w, h, fill ? fill : "none", stroke ? stroke : "none", stroke_width);
}

void svg_circulo(SVG *svg, double cx, double cy, double r,
                 const char *fill, const char *stroke, double stroke_width) {
    if (!svg) return;
    fprintf(svg->file, "  <circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" "
            "fill=\"%s\" stroke=\"%s\" stroke-width=\"%.1f\" />\n",
            cx, cy, r, fill ? fill : "none", stroke ? stroke : "none", stroke_width);
}

void svg_linha(SVG *svg, double x1, double y1, double x2, double y2,
               const char *stroke, double stroke_width, const char *stroke_dasharray) {
    if (!svg) return;
    if (stroke_dasharray && stroke_dasharray[0] != '\0') {
        fprintf(svg->file, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                "stroke=\"%s\" stroke-width=\"%.1f\" stroke-dasharray=\"%s\" />\n",
                x1, y1, x2, y2, stroke, stroke_width, stroke_dasharray);
    } else {
        fprintf(svg->file, "  <line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                "stroke=\"%s\" stroke-width=\"%.1f\" />\n",
                x1, y1, x2, y2, stroke, stroke_width);
    }
}

void svg_texto(SVG *svg, double x, double y, const char *texto, const char *fill, double font_size) {
    if (!svg) return;
    fprintf(svg->file, "  <text x=\"%.2f\" y=\"%.2f\" fill=\"%s\" font-size=\"%.1f\">%s</text>\n",
            x, y, fill ? fill : "black", font_size, texto ? texto : "");
}

void svg_path_begin(SVG *svg, const char *stroke, double stroke_width, const char *fill) {
    if (!svg) return;
    fprintf(svg->file, "  <path class=\"caminho\" fill=\"%s\" stroke=\"%s\" stroke-width=\"%.1f\" d=\"",
            fill ? fill : "none", stroke ? stroke : "none", stroke_width);
}

void svg_path_add(SVG *svg, double x, double y) {
    if (!svg) return;
    fprintf(svg->file, "%.2f,%.2f ", x, y);
}

void svg_path_end(SVG *svg) {
    if (!svg) return;
    fprintf(svg->file, "\" />\n");
}

void svg_desenhar_caminho(SVG *svg, Caminho *c, const char *stroke, double stroke_width) {
    if (!svg || !c || caminho_num_vertices(c) < 2) return;
    
    svg_path_begin(svg, stroke, stroke_width, "none");
    
    for (int i = 0; i < caminho_num_vertices(c); i++) {
        Vertice *v = caminho_get_vertice(c, i);
        if (v) {
            if (i == 0) {
                fprintf(svg->file, "M%.2f,%.2f ", vertice_get_x(v), vertice_get_y(v));
            } else {
                fprintf(svg->file, "L%.2f,%.2f ", vertice_get_x(v), vertice_get_y(v));
            }
        }
    }
    
    fprintf(svg->file, "\" />\n");
}

// Criar animação de um elemento percorrendo o caminho
void svg_animar_caminho(SVG *svg, Caminho *c, const char *cor, double raio, double duracao) {
    if (!svg || !c || caminho_num_vertices(c) < 2) return;
    
    // Construir o path data
    char path_data[8192] = "";
    
    for (int i = 0; i < caminho_num_vertices(c); i++) {
        Vertice *v = caminho_get_vertice(c, i);
        if (v) {
            char ponto[64];
            if (i == 0) {
                snprintf(ponto, sizeof(ponto), "M%.2f,%.2f ", 
                         vertice_get_x(v), vertice_get_y(v));
            } else {
                snprintf(ponto, sizeof(ponto), "L%.2f,%.2f ", 
                         vertice_get_x(v), vertice_get_y(v));
            }
            strncat(path_data, ponto, sizeof(path_data) - strlen(path_data) - 1);
        }
    }
    
    if (strlen(path_data) == 0) return;
    
    // Criar elemento animado (círculo)
    fprintf(svg->file, "  <!-- Animação do percurso com duração %.1fs -->\n", duracao);
    fprintf(svg->file, "  <circle r=\"%.1f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1.5\">\n",
            raio, cor, cor);
    fprintf(svg->file, "    <animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" "
            "path=\"%s\"/>\n", duracao, path_data);
    fprintf(svg->file, "  </circle>\n");
}

// Função genérica para animateMotion
void svg_animate_motion(SVG *svg, const char *tipo, double r, double w, double h,
                        const char *fill, const char *stroke, const char *path_data,
                        double duracao) {
    if (!svg) return;
    
    if (strcmp(tipo, "circle") == 0) {
        fprintf(svg->file, "  <circle r=\"%.1f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\">\n",
                r, fill, stroke);
        fprintf(svg->file, "    <animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" path=\"%s\"/>\n",
                duracao, path_data);
        fprintf(svg->file, "  </circle>\n");
    } else if (strcmp(tipo, "rect") == 0) {
        fprintf(svg->file, "  <rect width=\"%.1f\" height=\"%.1f\" fill=\"%s\" stroke=\"%s\" stroke-width=\"1\">\n",
                w, h, fill, stroke);
        fprintf(svg->file, "    <animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" path=\"%s\"/>\n",
                duracao, path_data);
        fprintf(svg->file, "  </rect>\n");
    }
}

void svg_bounding_box(SVG *svg, double x, double y, double w, double h, const char *cor, double opacidade) {
    if (!svg) return;
    fprintf(svg->file, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
            "fill=\"%s\" fill-opacity=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n",
            x, y, w, h, cor, opacidade, cor);
}
