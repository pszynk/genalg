#include "chromosome.h"

static idx_t _random_allele()
{
    return RANDOM_FROM(1, g_bitPerChrom-1);
}

static void swap_bitvec(bitvec_t *a, bitvec_t *b)
{
    bitvec_t tmp = (*a);
    (*a) = (*b);
    (*b) = tmp;
}

chrom_t chrom_create()
{
    chrom_t chrom = malloc(sizeof(chrom_t*) * g_bitvecPerChrom);
    if (chrom == NULL) {
        MYERR_ERR(-2, "Nie udalo sie zaalokowac pamieci na chromosom: %d bajtow",
                sizeof(chrom_t*) * g_bitvecPerChrom);
    }
    return chrom;
}

void chrom_destroy(chrom_t chrom)
{
    free(chrom);
    chrom = NULL;
}

void chrom_copy(chrom_t to, const chrom_t from)
{
    memcpy(to, from, g_bitvecPerChrom * sizeof(bitvec_t));
}

void chrom_rand(chrom_t chrom)
{
    idx_t i;
    // TODO OMP tylko inicjalizacja
    for (i = 0; i < g_bitvecPerChrom; ++i) {
        chrom[i] = binary_to_gray(RANDOM_BITVEC);
    }
}

idx_t chrom_xcross(
        chrom_t chrom1,
        chrom_t chrom2)
{
    idx_t nbit = _random_allele();
    /*printf("xcross> CHROM CORSS NA BICIE %d\n", nbit);*/
    if (nbit >= g_bitPerChrom) {
        MYERR_ERR(-1, "%s: %s -> (%d >= %d)",
                "Blad krzyzowania chromosomu",
                "Zakres krzyzowania przekracza rozmiar chromosomu",
                nbit, g_bitPerChrom);
    }
    idx_t nBVec, nBitInBVec;
    nBVec = nbit / BIT_PER_BITVEC;
    /*printf("xcross> CHROM CORSS NA BITVEC %d\n", nBVec);*/
    /* zmien wektory bitowe
     *       n    ...   0
     * 1  := aaa/x/bbbbbb   2  := ccc/y/dddddd
     * 1' := ccc/x/bbbbbb   2' := aaa/y/dddddd
     */
    idx_t i;
    // TODO OMP chyba za mało razy
    for (i = nBVec + 1; i < g_bitvecPerChrom; ++i) {
        swap_bitvec(chrom1 + i, chrom2 + i);
    }
    /* zmien bity w wektorze bitowym
     *      32    ...   0
     * x  := 101...001/10   y  := 111...100/00
     * x' := 111...100/10   y' := 101...001/00
     */
    nBitInBVec = nbit % BIT_PER_BITVEC ;
    /*printf("xcross> CHROM CORSS NA BIT in VEC %d\n", nBitInBVec);*/
    bitvec_xcross(nBitInBVec,
            chrom1 + nBVec,
            chrom2 + nBVec);
    /* Ostatecznie:
     * chrom1 := ccc/x'/bbbbbb
     * chrom2 := aaa/y'/dddddd
     */
    return nbit;
}


void chrom_mut(
        chrom_t chrom)
{
    idx_t i, nBVec, nBitInBVec;
    // TODO OMP petla po bit per chrom (max 100 * 16 lub 32), wykonywana popSize * gen razy
    for (i = 0; i < g_bitPerChrom; ++i) {
        if (RANDOM_0_TO_1 <= g_pMut) {
            /*nBVec = i / g_bitPerChrom;*/
            nBVec = i / BIT_PER_BITVEC;
            nBitInBVec = i % BIT_PER_BITVEC;
            /*printf("xcross> CHROM MUT NA bVec %d / bitInVec %d \n",nBVec, nBitInBVec);*/
            bitvec_flip(
                    nBitInBVec,
                    chrom + nBVec);
        }
    }
    /*
     *if (nbit >= chromBits) {
     *    MYERR_ERR(-1, "%s: %s -> (%d >= %d)",
     *            "Blad mutowania chromosomu",
     *            "Miejsce mutowania przekracza rozmiar chromosomu",
     *            nbit, chromBits);
     *}
     */
}

void chrom_to_real(
        const chrom_t chrom,
        real_t *value)
{
    idx_t i;
    // TODO OMP petla po dim, wykonywana popSize * gen razy
    for (i = 0; i < g_bitvecPerChrom; ++i) {
        value[i] = binary_to_real(
                gray_to_binary(chrom[i]));
    }
}

char* chrom_to_string(
        const chrom_t chrom)
{
    char *bvStr = bitvec_to_string(chrom + g_bitvecPerChrom - 1);
    idx_t bvStrLen = strlen(bvStr);
    char *str = malloc(
            sizeof(char) * (
                bvStrLen * g_bitvecPerChrom
                + g_bitvecPerChrom));

    strcpy(str, bvStr);
    free(bvStr);
    int i;
    // TODO OMP wypisywanie
    for(i = g_bitvecPerChrom - 2; i >= 0; --i) {
        strcat(str, "|");
        bvStr = bitvec_to_string(chrom + i);
        strcat(str, bvStr);
        free(bvStr);
    }
    return str;
}
