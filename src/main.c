#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

int main(int argc, char **argv)
{
    struct graph g;
    struct parameters p;
    struct state s;
    struct metrics m;
    double u_a[MAX_NODES];
    double u_b[MAX_NODES];
    double t;
    int steps;
    int k;

    if (argc != 3) {
        fprintf(stderr, "usage: %s graph.txt config.txt\n", argv[0]);
        return 1;
    }

    if (graph_load(argv[1], &g) != 0) {
        fprintf(stderr, "graph load error\n");
        return 1;
    }

    if (parameters_load(argv[2], &p) != 0) {
        fprintf(stderr, "parameter load error\n");
        graph_free(&g);
        return 1;
    }

    steps = (int) (p.horizon / p.dt);
    if (steps < 1 || steps > MAX_STEPS) {
        fprintf(stderr, "invalid integration horizon\n");
        graph_free(&g);
        return 1;
    }

    state_init(&s, g.n_nodes);
    memset(&m, 0, sizeof(m));
    memset(u_a, 0, sizeof(u_a));
    memset(u_b, 0, sizeof(u_b));

    printf("t");
    for (k = 0; k < g.n_nodes; ++k)
        printf(",x%d", k);
    printf("\n");

    for (k = 0; k < steps; ++k) {
        int i;
        t = p.dt * (double) k;
        pressure_policy(&g, &p, &s, t, u_a);
        minimal_intervention(&g, &p, &s, u_a, u_b);
        dynamics_step(&g, &p, &s, u_a, u_b);
        metrics_update(&p, &s, u_a, u_b, &m);

        printf("%.6f", t);
        for (i = 0; i < g.n_nodes; ++i)
            printf(",%.9f", s.x[i]);
        printf("\n");
    }

    metrics_finish(&g, &s, &m);

    fprintf(stderr, "damage_integral %.12f\n", m.accumulated_damage);
    fprintf(stderr, "control_integral %.12f\n", m.accumulated_control);
    fprintf(stderr, "escalation_integral %.12f\n", m.accumulated_escalation);
    fprintf(stderr, "terminal_damage %.12f\n", m.terminal_damage);
    fprintf(stderr, "deterrence_index %.12f\n", m.deterrence_index);

    graph_free(&g);
    return 0;
}
