#include "bitvec.h"

/*
   The purpose of this function is to convert an unsigned
   binary number to reflected binary Gray code.

   The operator >> is shift right. The operator ^ is exclusive or.
   */
bitvec_t binary_to_gray(bitvec_t num)
{
    return (num >> 1) ^ num;
}

/*
   The purpose of this function is to convert a reflected binary
   Gray code number to a binary number.
   */
bitvec_t gray_to_binary(bitvec_t num)
{
    bitvec_t mask;
    for (mask = num >> 1; mask != 0; mask = mask >> 1)
    {
        num = num ^ mask;
    }
    return num;
}

real_t binary_to_real(
        bitvec_t binary)
{
    return g_funcLB + (g_funcUB - g_funcLB) * binary / MAX_BITVEC_VALUE;
}

void bitvec_xcross(
        idx_t nbit,
        bitvec_t *bv1,
        bitvec_t *bv2)
{
    if (nbit >= BIT_PER_BITVEC) {
        MYERR_ERR(-1, "%s: %s -> (%d >= %d)",
                "Blad krzyzowania wektora bitowego",
                "Zakres krzyzowania przekracza rozmiar wektora bitowego",
                nbit, BIT_PER_BITVEC);
    }
    bitvec_t old1 = (*bv1);
    bitvec_t old2 = (*bv2);
    bitvec_t backMask = (1 << nbit) - 1;
    bitvec_t frontMask = backMask ^ MAX_BITVEC_VALUE;
    (*bv1) = (backMask & old1) | (frontMask & old2);
    (*bv2) = (backMask & old2) | (frontMask & old1);
}

void bitvec_flip(
        idx_t nbit,
        bitvec_t *bv)
{
    if (nbit >= BIT_PER_BITVEC) {
        MYERR_ERR(-1, "%s: %s -> (%d >= %d)",
                "Blad przewracania bitu w wektorze bitowym",
                "Indeks przewracanego bitu przekracza rozmiar wektora bitowego",
                nbit, BIT_PER_BITVEC);
    }
    (*bv) ^= 1 << nbit;
}


char* bitvec_to_string(
        const bitvec_t *bitvec)
{
    bitvec_t bv = (*bitvec);
    char* str = (char*)malloc(sizeof(char) * (BIT_PER_BITVEC + (BIT_PER_BITVEC / BIT_PER_BYTE)));
    idx_t
        i = BIT_PER_BITVEC + (BIT_PER_BITVEC / BIT_PER_BYTE) - 1,
        j = 0;
    while(i > 0) {
        if ((j == 8) && (i > 1)) {
            str[i-1] = '.';
            j = 0;
        } else {
            str[i-1] = ('0' + (bv & 1));
            bv >>= 1;
            ++j;
        }
        --i;
    }
    str[BIT_PER_BITVEC + (BIT_PER_BITVEC / BIT_PER_BYTE) - 1] = '\0';
    return str;
}
