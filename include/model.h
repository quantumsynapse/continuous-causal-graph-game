#ifndef MODEL_H
#define MODEL_H

#include <stddef.h>

#define MAX_NODES 64
#define MAX_STEPS 200000

struct edge {
    int from;
    int to;
    double weight;
    double sensitivity;
};

struct graph {
    int n_nodes;
    int n_edges;
    struct edge *edges;
    double adjacency[MAX_NODES][MAX_NODES];
    double sensitivity[MAX_NODES][MAX_NODES];
};

struct parameters {
    double dt;
    double horizon;
    double alpha;
    double beta;
    double gamma;
    double lambda_u;
    double lambda_e;
    double lambda_c;
    double pressure_gain;
    double recovery_gain;
    double control_limit;
    double sparsity_threshold;
    int max_iterations;
};

struct state {
    double x[MAX_NODES];
    double x_next[MAX_NODES];
    double pressure[MAX_NODES];
    double control[MAX_NODES];
    double vulnerability[MAX_NODES];
};

struct metrics {
    double accumulated_damage;
    double accumulated_control;
    double accumulated_escalation;
    double terminal_damage;
    double deterrence_index;
};

int graph_load(const char *path, struct graph *g);
void graph_free(struct graph *g);
int parameters_load(const char *path, struct parameters *p);
void state_init(struct state *s, int n);
void pressure_policy(const struct graph *g, const struct parameters *p,
                     const struct state *s, double t, double *u_a);
void minimal_intervention(const struct graph *g, const struct parameters *p,
                          const struct state *s, const double *u_a,
                          double *u_b);
void dynamics_step(const struct graph *g, const struct parameters *p,
                   struct state *s, const double *u_a, const double *u_b);
void metrics_update(const struct parameters *p, const struct state *s,
                    const double *u_a, const double *u_b,
                    struct metrics *m);
void metrics_finish(const struct graph *g, const struct state *s,
                    struct metrics *m);

#endif
