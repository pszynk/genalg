/*
 * Plik glowny z funkcja main
 */
/*
 * TODO:
 * - parametr do ustawiania seed reand
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include "types.h"
#include "globals.h"
#include "myerr.h"
#include "galgorithm.h"
#include "bitvec.h"
#include "population.h"

#include "griewank.h"
#include "kwadratx.h"

#define _PROGARGN  7

char _set_dim      = 0;
char _set_maxGen   = 0;
char _set_popSize  = 0;
char _set_pCross   = 0;
char _set_pMut     = 0;
char _set_funcType = 0;

static char _paramErrorMsg[] = "Blad parametrow wejsciowych";

char *_progname = "GENALG";
char *_progdec  = "algorytm genetyczny do minimalizacji funkcji Griewanka i Ackleya";
char *_progargs[_PROGARGN][3] =
    {
        {"-h       ", "Wyświetla tą wiadomość            ", "    "},
        {"-d[N]    ", "Wymiar funkcji                    ", "   2"},
        {"-g[N]    ", "Maksymalna ilość generacji        ", "  50"},
        {"-p[N]    ", "Rozmiar populacji                 ", "  20"},
        {"-x[p]    ", "Prawd. krzyżowania dwóch osobników", " 0.2"},
        {"-m[p]    ", "Prawd. mutacji allela             ", "0.01"},
        {"-f[GR|AC]", "Funkcja do minimalizacji          ", "  GA"}
    };

void print_help_msg()
{
    char *_spaces = "   ";
    printf("%s - %s\n\n" //nazwa programu - opis
            "użycie: %s [argumenty]\n\n" //nazwa programu
            "Argumenty:\n"
            "NAZWA    "  "%sOPIS                             "  "%sDOMYŚLNA\n"
            ,_progname, _progdec, _progname, _spaces, _spaces);
    int i;
    for (i = 0; i < _PROGARGN; ++i) {
        printf("%s%s%s%s%s\n",
                _progargs[i][0],
                _spaces,
                _progargs[i][1],
                _spaces,
                _progargs[i][2]
                );
    }
}

void print_params()
{
    printf("  *******************************************\n");
    printf("  ******************* PARAMS ****************\n");
    printf("  *******************************************\n");
    printf("\n");
    printf(
        "   %27s -> %d\n"
        "   %27s -> %d\n"
        "   %27s -> %d\n"
        "   %27s -> %f\n"
        "   %27s -> %f\n",
        "Wymiar", g_dim,
        "Maksymalna liczba generacji", g_maxGen,
        "Rozmiar populacji", g_popSize,
        "Prawd. krzyzowania", g_pCross,
        "Prawd. mutacji", g_pMut
        );
    switch (g_funcType) {
        case GRIEWANK:
            printf("   %27s -> %s\n",
                    "Funkcja do minimalziacji","f. Griewanka");
            break;
        case ACKLEY:
            printf("   %27s -> %s\n",
                    "Funkcja do minimalziacji","f. Ackleya");
            break;
        case KWADRATX:
            printf("   %27s -> %s\n",
                    "Funkcja do minimalziacji","f. kwadratowa");
            break;
        default:
            printf("NIEZNANA FUNKCJA!!\n");
    }
}


void set_idx_param(const char *optarg, idx_t *param, const char* msg)
{
    int val = atoi(optarg);
    if (val <= 0) {
        MYERR_ERR(-1, "%s: %s;\n\tpodany wartosc to: %d, albo niepoprawna liczba",
                _paramErrorMsg, msg, val);
    } else {
        (*param) = val;
    }
}
void set_prob_param(const char *optarg, real_t *param, const char* msg)
{
    real_t val = atof(optarg);
    if ((val < 0) || (val > 1)) {
        MYERR_ERR(-1, "%s: %s;\n\tpodany wartosc to: %f, albo niepoprawna liczba",
                _paramErrorMsg, msg, val);
    } else {
        (*param) = val;
    }
}


void set_func_param(const char *optarg)
{
    if (
        (strncmp(optarg, "gr", 2) == 0) ||
        (strncmp(optarg, "gR", 2) == 0) ||
        (strncmp(optarg, "Gr", 2) == 0) ||
        (strncmp(optarg, "GR", 2) == 0)
       ) {
        g_funcType = GRIEWANK;
    } else if (
        (strncmp(optarg, "ac", 2) == 0) ||
        (strncmp(optarg, "aC", 2) == 0) ||
        (strncmp(optarg, "Ac", 2) == 0) ||
        (strncmp(optarg, "AC", 2) == 0)
            ) {
        g_funcType = ACKLEY;
    } else {
        MYERR_ERR(-1, "%s: %s;\n\tpodana funkcja to: %s",
                _paramErrorMsg,
                "Mozliwe funkcje to Griewank (gr,gR,Gr,GR) lub Ackley (ac,aC,Ac,AC)",
                optarg);
    }
}

void init_globals()
{
    // defaults
    idx_t def_dim = 2;
    idx_t def_maxGen = 50;
    idx_t def_popSize = 20;
    real_t def_pCross = 0.2;
    real_t def_pMut = 0.01;
    //enum Functions def_funcType = GRIEWANK;
    enum Functions def_funcType = KWADRATX;

    // setting defaults
    if (!_set_dim) {
        g_dim = def_dim;
    }


    if (!_set_maxGen) {
        g_maxGen = def_maxGen;
    }
    if(g_maxGen >  MAX_GENERATIONS) {
        g_maxGen = MAX_GENERATIONS;
    }


    if (!_set_popSize) {
        g_popSize = def_popSize;
    }
    if(g_popSize > MAX_POPULATION) {
        g_popSize = MAX_POPULATION;
    }


    if (!_set_pCross) {
        g_pCross = def_pCross;
    }
    if (!_set_pMut) {
        g_pMut = def_pMut;
    }
    if (!_set_funcType) {
        g_funcType = def_funcType;
    }

    //pozostałe globalne - zależne
    g_gen = 0;

    g_bitvecPerChrom = g_dim;
    g_bitPerChrom = g_bitvecPerChrom * BIT_PER_BITVEC;

    switch (g_funcType) {
        case GRIEWANK:
            g_funcLB = GRIEW_LB;
            g_funcUB = GRIEW_UB;
            g_evalFunct = griew_eval;
            g_revalFunct = griew_reval;
            g_minimizeFunct = griewank;
            break;
        //case ACKLEY:
            //g_funcLB = ;
            //g_funcUB = ;
            //g_evalFunct = ;
            //g_minimizeFunct = ;
            //break;
        case KWADRATX:
            g_funcLB = KWX_LB;
            g_funcUB = KWX_UB;
            g_evalFunct =  kwx_eval;
            g_revalFunct = kwx_reval;
            g_minimizeFunct = kwadratx;
            break;
        default:
            MYERR_ERR(-1,
                    "Nieznana funkcja do minimalziacji");

    }
}
void read_params(int argc, char *argv[])
{
    int c;
    /*char *funcOpt;*/

    while ((c = getopt(argc, argv, "hd:f:g:m:p:x:")) != -1) {
        switch (c) {
            case 'h':
                print_help_msg();
                exit(0);
                break;
            case 'd':
                set_idx_param(optarg, &g_dim,
                        "wymiar minimalizowanej funkcji musi byc liczba naturalna wieksza od 0");
                _set_dim = 1;
                break;
            case 'g':
                set_idx_param(optarg, &g_maxGen,
                        "maksymalna liczba generacji musi byc liczba naturalna wieksza od 0");
                _set_maxGen = 1;
                break;
            case 'p':
                set_idx_param(optarg, &g_popSize,
                        "wielkosc populacji musi byc liczba naturalna wieksza od 0");
                _set_popSize = 1;
                break;
            case 'x':
                set_prob_param(optarg, &g_pCross,
                        "wielkosc populacji musi byc liczba naturalna wieksza od 0");
                _set_pCross = 1;
                break;
            case 'm':
                set_prob_param(optarg, &g_pMut,
                        "wielkosc populacji musi byc liczba naturalna wieksza od 0");
                _set_pMut = 1;
                break;
            case 'f':
                set_func_param(optarg);
                _set_funcType = 1;
                break;
            case '?':
                if (optopt == 'c')
                    MYERR_ERR(-1, "Opcja -%c wymaga argumentu.\n", optopt);
                else if (isprint (optopt))
                    MYERR_ERR(-1, "Nieznana opcja `-%c'.\n", optopt);
                else
                    MYERR_ERR(-1,"Nieznana litera opcji `\\x%x'.\n", optopt);
            default:
                abort();
        }
    }

}
//#include "individual.h"
int main_(int argc, char *argv[])
{

    real_t args[100];// = {40.0, 40.0};
    idx_t iii;
    for (iii = 0; iii < 100; ++iii) {
        args[iii] = 40.0;
    }
    printf("%f\n", griewank(args, 100));
    return 0;
    srand(time(NULL));
    read_params(argc, argv);
    /* wartości domyślen zmiennych */
    init_globals();
    print_params();
    indiv_t *indiv = indiv_create(1);
    indiv_rand(indiv);
    real_t val = indiv_eval(indiv);
    char *str = indiv_to_string(indiv);
    printf("%s\n", str);
    free(str);
    printf("%f\n", val);
    //idx_t i, count = 100000;
    //for (i = 0; i < count; ++i) {
        //idx_t rr = RANDOM_FROM(1, 10);
        //printf("%d <- %d\n", rr, i);
        //if(rr == 10) {
            //printf("JEST 10");
            //break;
        //}
    //}
    return 0;
}
int main(int argc, char *argv[])
{
    real_t result;
    stats_t stats;

    srand(time(NULL));
    /* wczytanie parametrow */
    read_params(argc, argv);
    /* wartości domyślen zmiennych */
    init_globals();
    print_params();
    /* alokacja pamieci */
    /* funkcja algorytmu */
    result = galgorithm(&stats);
    /* zwrocenie wynikow */
    printf("\n-RESULT -> %f\n", g_revalFunct(result));
    printf("\nRESULT -> %f\n", result);
    return 0;
}
