#include <math.h>
#include "model.h"

static double clamp(double x, double lo, double hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

void pressure_policy(const struct graph *g, const struct parameters *p,
                     const struct state *s, double t, double *u_a)
{
    int i;
    double cycle;

    cycle = 0.65 + 0.35 * sin(0.15 * t);
    for (i = 0; i < g->n_nodes; ++i) {
        double signal = s->vulnerability[i] + 0.5 * s->x[i];
        u_a[i] = p->pressure_gain * cycle * clamp(signal, 0.0, 1.0);
    }
}

void minimal_intervention(const struct graph *g, const struct parameters *p,
                          const struct state *s, const double *u_a,
                          double *u_b)
{
    int i, j, k;
    double score[MAX_NODES];
    double best;
    int best_idx;

    for (i = 0; i < g->n_nodes; ++i) {
        double downstream = 0.0;
        double incoming = 0.0;
        for (j = 0; j < g->n_nodes; ++j) {
            downstream += fabs(g->adjacency[i][j]) *
                          (1.0 + g->sensitivity[i][j]);
            incoming += fabs(g->adjacency[j][i]);
        }
        score[i] = u_a[i] * (1.0 + s->x[i]) *
                   (1.0 + downstream + 0.5 * incoming);
        u_b[i] = 0.0;
    }

    for (k = 0; k < g->n_nodes; ++k) {
        best = p->sparsity_threshold;
        best_idx = -1;
        for (i = 0; i < g->n_nodes; ++i) {
            if (score[i] > best) {
                best = score[i];
                best_idx = i;
            }
        }
        if (best_idx < 0)
            break;
        u_b[best_idx] = clamp(p->beta * best /
                              (p->lambda_u + p->lambda_e * u_a[best_idx] + 1e-12),
                              0.0, p->control_limit);
        score[best_idx] = -1.0;
        if ((double) (k + 1) >= sqrt((double) g->n_nodes))
            break;
    }
}
