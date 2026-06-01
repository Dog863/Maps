#include "svg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct svg {
    FILE *file;
} SVG;

SVG* svg_criar(const char *nome_arquivo, double largura, double altura) {
    SVG *svg = (SVG*)malloc(sizeof(SVG));
    if (!svg) return NULL;
    
    svg->file = fopen(nome_arquivo, "w");
    if (!svg->file) {
        free(svg);
        return NULL;
    }
    
    fprintf(svg->file, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%.0f\" height=\"%.0f\">\n", 
            largura, altura);
    fprintf(svg->file, "<style>\n");
    fprintf(svg->file, "  text { font-family: sans-serif; }\n");
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
    if (stroke_dasharray) {
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
    fprintf(svg->file, "  <path fill=\"%s\" stroke=\"%s\" stroke-width=\"%.1f\" d=\"",
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

void svg_animate_motion(SVG *svg, const char *tipo, double r, double w, double h,
                        const char *fill, const char *stroke, const char *path,
                        double duracao) {
    if (!svg) return;
    
    if (strcmp(tipo, "circle") == 0) {
        fprintf(svg->file, "  <circle r=\"%.1f\" fill=\"%s\" stroke=\"%s\">\n",
                r, fill, stroke);
        fprintf(svg->file, "    <animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" path=\"%s\"/>\n",
                duracao, path);
        fprintf(svg->file, "  </circle>\n");
    } else if (strcmp(tipo, "rect") == 0) {
        fprintf(svg->file, "  <rect width=\"%.1f\" height=\"%.1f\" fill=\"%s\" stroke=\"%s\">\n",
                w, h, fill, stroke);
        fprintf(svg->file, "    <animateMotion dur=\"%.1fs\" repeatCount=\"indefinite\" path=\"%s\"/>\n",
                duracao, path);
        fprintf(svg->file, "  </rect>\n");
    }
}

void svg_bounding_box(SVG *svg, double x, double y, double w, double h, const char *cor, double opacidade) {
    if (!svg) return;
    fprintf(svg->file, "  <rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" "
            "fill=\"%s\" fill-opacity=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n",
            x, y, w, h, cor, opacidade, cor);
}