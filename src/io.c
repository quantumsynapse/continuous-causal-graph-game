#include <stdio.h>
#include <string.h>
#include "model.h"

static int assign_value(struct parameters *p, const char *key, double value)
{
    if (strcmp(key, "dt") == 0) p->dt = value;
    else if (strcmp(key, "horizon") == 0) p->horizon = value;
    else if (strcmp(key, "alpha") == 0) p->alpha = value;
    else if (strcmp(key, "beta") == 0) p->beta = value;
    else if (strcmp(key, "gamma") == 0) p->gamma = value;
    else if (strcmp(key, "lambda_u") == 0) p->lambda_u = value;
    else if (strcmp(key, "lambda_e") == 0) p->lambda_e = value;
    else if (strcmp(key, "lambda_c") == 0) p->lambda_c = value;
    else if (strcmp(key, "pressure_gain") == 0) p->pressure_gain = value;
    else if (strcmp(key, "recovery_gain") == 0) p->recovery_gain = value;
    else if (strcmp(key, "control_limit") == 0) p->control_limit = value;
    else if (strcmp(key, "sparsity_threshold") == 0) p->sparsity_threshold = value;
    else if (strcmp(key, "max_iterations") == 0) p->max_iterations = (int) value;
    else return -1;
    return 0;
}

int parameters_load(const char *path, struct parameters *p)
{
    FILE *fp;
    char key[64];
    double value;

    memset(p, 0, sizeof(*p));
    fp = fopen(path, "r");
    if (fp == NULL)
        return -1;

    while (fscanf(fp, "%63s %lf", key, &value) == 2) {
        if (assign_value(p, key, value) != 0) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);

    if (p->dt <= 0.0 || p->horizon <= 0.0 ||
        p->control_limit <= 0.0 || p->max_iterations <= 0)
        return -1;

    return 0;
}
