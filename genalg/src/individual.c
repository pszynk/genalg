#include "individual.h"

indiv_t* indiv_create(
        idx_t nr) {
    if (nr == 0) {
        return NULL;
    }
    indiv_t *indiv = (indiv_t*)malloc(sizeof(*indiv) * nr);
    idx_t i;
    for (i = 0; i < nr; ++i) {
        indiv[i].fitness = 0;
        indiv[i].genotype = chrom_create();
    }
    return indiv;
}

void indiv_destory(
        indiv_t *indiv)
{
    chrom_destroy(indiv->genotype);
    indiv->genotype = NULL;
}

void indiv_copy(
        indiv_t *to,
        const indiv_t *from)
{
    chrom_copy(to->genotype, from->genotype);
    to->fitness = from->fitness;
}

void indiv_rand(
        grstate_t *grstate,
        indiv_t *indiv)
{
    chrom_rand(grstate, indiv->genotype);
    indiv->fitness = 0;
}

real_t indiv_eval(
        indiv_t *indiv)
{
    real_t feno[g_dim];
    chrom_to_real(indiv->genotype, feno);
    indiv->fitness = eval_funct(feno, g_dim);
    return indiv->fitness;
}


void indiv_xcross(
        grstate_t *grstate,
        indiv_t *indiv1,
        indiv_t *indiv2)
{
    chrom_xcross(grstate, indiv1->genotype, indiv2->genotype);
}


void indiv_mut(
        grstate_t *grstate,
        indiv_t *indiv)
{
    chrom_mut(grstate, indiv->genotype);
}


char* indiv_to_string(
        indiv_t *indiv)
{
    char* chromStr = chrom_to_string(indiv->genotype);
    idx_t spLen = 0, strLen = (strlen(chromStr) + 40);
    char* str = (char*)malloc(sizeof(char) * strLen);
    if ((spLen = sprintf(str, "[%d] {% 15.6f} -> <%s>", g_mpiProcId, indiv->fitness, chromStr)) <= 0) {
        MYERR_ERR(-3, "Blad w funkcji individual -> string");
    }
    free(chromStr);
    return str;
}
