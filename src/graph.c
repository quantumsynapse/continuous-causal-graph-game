#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

int graph_load(const char *path, struct graph *g)
{
    FILE *fp;
    int i;

    memset(g, 0, sizeof(*g));
    fp = fopen(path, "r");
    if (fp == NULL)
        return -1;

    if (fscanf(fp, "%d %d", &g->n_nodes, &g->n_edges) != 2) {
        fclose(fp);
        return -1;
    }

    if (g->n_nodes < 1 || g->n_nodes > MAX_NODES || g->n_edges < 0) {
        fclose(fp);
        return -1;
    }

    g->edges = calloc((size_t) g->n_edges, sizeof(struct edge));
    if (g->edges == NULL) {
        fclose(fp);
        return -1;
    }

    for (i = 0; i < g->n_edges; ++i) {
        struct edge *e = &g->edges[i];
        if (fscanf(fp, "%d %d %lf %lf",
                   &e->from, &e->to, &e->weight, &e->sensitivity) != 4) {
            graph_free(g);
            fclose(fp);
            return -1;
        }
        if (e->from < 0 || e->from >= g->n_nodes ||
            e->to < 0 || e->to >= g->n_nodes) {
            graph_free(g);
            fclose(fp);
            return -1;
        }
        g->adjacency[e->from][e->to] = e->weight;
        g->sensitivity[e->from][e->to] = e->sensitivity;
    }

    fclose(fp);
    return 0;
}

void graph_free(struct graph *g)
{
    free(g->edges);
    g->edges = NULL;
    g->n_edges = 0;
    g->n_nodes = 0;
}
