#include <math.h>
#include <string.h>
#include "model.h"

void state_init(struct state *s, int n)
{
    int i;
    memset(s, 0, sizeof(*s));
    for (i = 0; i < n; ++i) {
        s->x[i] = 0.05 + 0.01 * (double) (i % 5);
        s->vulnerability[i] = 0.20 + 0.05 * (double) (i % 7);
    }
}

void dynamics_step(const struct graph *g, const struct parameters *p,
                   struct state *s, const double *u_a, const double *u_b)
{
    int i, j;

    for (i = 0; i < g->n_nodes; ++i) {
        double propagation = 0.0;
        double controlled = 0.0;
        double dx;

        for (j = 0; j < g->n_nodes; ++j) {
            propagation += g->adjacency[j][i] * s->x[j];
            controlled += g->sensitivity[j][i] * u_b[j];
        }

        dx = p->alpha * propagation +
             u_a[i] * (1.0 + s->vulnerability[i]) -
             p->recovery_gain * s->x[i] -
             p->gamma * u_b[i] -
             0.25 * controlled;

        s->x_next[i] = s->x[i] + p->dt * dx;
        if (s->x_next[i] < 0.0)
            s->x_next[i] = 0.0;

        s->pressure[i] = u_a[i];
        s->control[i] = u_b[i];
    }

    for (i = 0; i < g->n_nodes; ++i)
        s->x[i] = s->x_next[i];
}

void metrics_update(const struct parameters *p, const struct state *s,
                    const double *u_a, const double *u_b,
                    struct metrics *m)
{
    int i;
    double damage = 0.0;
    double control = 0.0;
    double escalation = 0.0;

    for (i = 0; i < MAX_NODES; ++i) {
        damage += s->x[i] * s->x[i];
        control += u_b[i] * u_b[i];
        escalation += u_a[i] * u_b[i];
    }

    m->accumulated_damage += p->dt * damage;
    m->accumulated_control += p->dt * control;
    m->accumulated_escalation += p->dt * escalation;
}

void metrics_finish(const struct graph *g, const struct state *s,
                    struct metrics *m)
{
    int i;
    double d = 0.0;
    double p = 0.0;

    for (i = 0; i < g->n_nodes; ++i) {
        d += s->x[i] * s->x[i];
        p += s->pressure[i];
    }

    m->terminal_damage = d;
    m->deterrence_index = 1.0 / (1.0 + d + p);
}
