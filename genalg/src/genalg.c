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

// ilość argumentów w programie
#define _PROGARGN  12

char _set_VERBOSELVL    = 0;
char _set_seed          = 0;
char _set_dim           = 0;
char _set_maxGen        = 0;
char _set_popSize       = 0;
char _set_selParamINT   = 0;
char _set_pCross        = 0;
char _set_pMut          = 0;
char _set_selParamFLOAT = 0;
char _set_funcType      = 0;
char _set_selType       = 0;

idx_t _val_selParamINT    = 0;
real_t _val_selParamFLOAT = 0.0;

static char _paramErrorMsg[] = "Blad parametrow wejsciowych";

char *_progname = "GENALG";
char *_progdec  = "algorytm genetyczny do minimalizacji funkcji Griewanka i Ackleya";
char *_progargs[_PROGARGN][3] =
    {
        {"-h       ", "Wyświetla tą wiadomość            ", "    "},
        {"-v       ", "verbose - dodatkowe komunikaty    ", "    "},
        {"-d[N]    ", "Wymiar funkcji                    ", "   2"},
        {"-e[N]    ", "Seed dla randomizacji             ", "   0"},
        {"-g[N]    ", "Maksymalna ilość generacji        ", "  50"},
        {"-k[N]    ", "Parametr metody selekcji [INT]    ", "   2"},
        {"-r[N]    ", "Parametr metody selekcji [FLOAT]  ", " 0.5"},
        {"-p[N]    ", "Rozmiar populacji                 ", "  20"},
        {"-m[p]    ", "Prawd. mutacji allela             ", "0.01"},
        {"-x[p]    ", "Prawd. krzyżowania dwóch osobników", " 0.2"},
        {"-s[R|T|B]", "Metoda selekcji osobników         ", "   B"},
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
    int padding = 32;
    printf("  *************************************************************\n");
    printf("  ******************* PARAMS **********************************\n");
    printf("  *************************************************************\n");
    printf("\n");
    printf(
        "  %*s -> %d\n"
        "  %*s -> %d\n"
        "  %*s -> %d\n"
        "  %*s -> %d\n"
        "  %*s -> %f\n"
        "  %*s -> %f\n",
        padding, "Seed", g_seed,
        padding, "Wymiar", g_dim,
        padding, "Maksymalna liczba generacji", g_maxGen,
        padding, "Rozmiar populacji", g_popSize,
        padding, "Prawd. krzyzowania", g_pCross,
        padding, "Prawd. mutacji", g_pMut
        );
    switch (g_selType) {
        case BEST:
            printf("  %*s -> %s %d %s\n",
                    padding,
                    "Metoda selekcji nowej populacji",
                    "m. wyboru",
                    g_selParam,
                    "najlepszych osobników"
                    );
            break;
        case RULETTE:
            printf("  %*s -> %s\n",
                    padding,
                    "Metoda selekcji nowej populacji","m. ruletki");
            break;
        case TOURNAMENT:
            printf("  %*s -> %s %d %s\n",
                    padding,
                    "Metoda selekcji nowej populacji",
                    "m. turnieju z",
                    g_selParam,
                    "osobników"
                    );
            break;
        default:
            printf("NIEZNANA METODA SELECKJI!\n");
    }
    switch (g_funcType) {
        case GRIEWANK:
            printf("  %*s -> %s\n",
                    padding,
                    "Funkcja do minimalziacji","f. Griewanka");
            break;
        case ACKLEY:
            printf("  %*s -> %s\n",
                    padding,
                    "Funkcja do minimalziacji","f. Ackleya");
            break;
        case KWADRATX:
            printf("  %*s -> %s\n",
                    padding,
                    "Funkcja do minimalziacji","f. kwadratowa");
            break;
        default:
            printf("NIEZNANA FUNKCJA!\n");
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

void set_char_param(const char *optarg, char *param, const char* accept, const char* msg)
{
    char val = optarg[0];
    if (strchr(accept, val) == NULL) {
        MYERR_ERR(-1, "%s: %s;\n\tpodany wartosc to: %c, mozliwe opcje zawiera: `%s'",
                _paramErrorMsg, msg, val, accept);
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
    int      def_VERBOSELVL       = 0;
    unsigned def_seed             = time(NULL);

    idx_t  def_dim                = 2;
    idx_t  def_maxGen             = 50;
    idx_t  def_popSize            = 20;
    idx_t  def_selParamINT        = 2;
    real_t def_pCross             = 0.2;
    real_t def_pMut               = 0.01;
    real_t def_selParamFLOAT      = 0.5;
    enum   Functions def_funcType = GRIEWANK;
    enum   Selections def_selType = BEST;

    // setting defaults
    if (!_set_VERBOSELVL) {
        g_VERBOSELVL = def_VERBOSELVL;
    }

    if (!_set_seed) {
        g_seed = def_seed;
    }

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

    if (!_set_selParamINT) {
        _val_selParamINT = def_selParamINT;
    }
    if (!_set_selParamFLOAT) {
        _val_selParamFLOAT = def_selParamFLOAT;
    }

    if (!_set_selType) {
        g_selType = def_selType;
    }
    switch (g_selType) {
        case BEST:
            g_selFunct = pop_select_best;
            if ((!_set_selParamFLOAT) && _set_selParamINT) {
                g_selParam = _val_selParamINT;
            } else {
                g_selParam = _val_selParamFLOAT * g_popSize;
            }
            break;
        case RULETTE:
            g_selFunct = pop_select_rulette;
            break;
        case TOURNAMENT:
            g_selFunct = pop_select_tournament;
            if ((!_set_selParamINT) && _set_selParamFLOAT) {
                g_selParam = _val_selParamFLOAT * g_popSize;
            } else {
                g_selParam = _val_selParamINT;
            }
            break;
        default:
            MYERR_ERR(-1,
                    "Nieznana metoda selecji");
    }
    if ((g_selParam < 1) && (g_selType == BEST)) {
        MYERR_ERR(-1,
            "Licza osobnikow rozpatrywanyw w "
            "m. selekci k-najlepszych [= %d], musi być >= 1",
            g_selParam);
    }
    if ((g_selParam < 2) && (g_selType == TOURNAMENT)) {
        MYERR_ERR(-1,
            "Licza osobnikow rozpatrywanyw w "
            "m. selekci turjenowe [= %d], musi być >= 2",
            g_selParam);
    }
    if (g_selParam > g_popSize) {
        if (g_VERBOSELVL) {
            MYERR_WARN("Uwaga parametr metody selekcji[%d] musi być mniejszy od\n"
                    " rozmaru populacji[%d]: Modyfikacja parametru do wartości %d",
                    g_selParam, g_popSize, g_popSize);
        }
        g_selParam = g_popSize;
    }

    //pozostałe globalne - zależne
    g_gen = 0;

    g_bitvecPerChrom = g_dim;
    g_bitPerChrom    = g_bitvecPerChrom * BIT_PER_BITVEC;

    switch (g_funcType) {
        case GRIEWANK:
            g_funcLB        = GRIEW_LB;
            g_funcUB        = GRIEW_UB;
            g_evalFunct     = griew_eval;
            g_revalFunct    = griew_reval;
            g_minimizeFunct = griewank;
            break;
        //case ACKLEY:
            //g_funcLB = ;
            //g_funcUB = ;
            //g_evalFunct = ;
            //g_minimizeFunct = ;
            //break;
        case KWADRATX:
            g_funcLB        = KWX_LB;
            g_funcUB        = KWX_UB;
            g_evalFunct     = kwx_eval;
            g_revalFunct    = kwx_reval;
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
    char selCode;
    /*char *funcOpt;*/

    while ((c = getopt(argc, argv, "hd:e:f:g:k:m:p:r:s:x:v")) != -1) {
        switch (c) {
            case 'h':
                print_help_msg();
                exit(0);
                break;
            case 'v':
                g_VERBOSELVL = 1;
                _set_VERBOSELVL = 1;
                break;
            case 'd':
                set_idx_param(optarg, &g_dim,
                        "wymiar minimalizowanej funkcji musi byc liczba naturalna wieksza od 0");
                _set_dim = 1;
                break;
            case 'e':
                set_idx_param(optarg, &g_seed,
                        "seed do randomizacji musi byc liczba naturalna wieksza od 0");
                _set_seed = 1;
                break;
            case 'g':
                set_idx_param(optarg, &g_maxGen,
                        "maksymalna liczba generacji musi byc liczba naturalna wieksza od 0");
                _set_maxGen = 1;
                break;
            case 'k':
                set_idx_param(optarg, &_val_selParamINT,
                        "parametr metody selekcji musi byc liczba naturalna wieksza od 0");
                _set_selParamINT = 1;
                break;
            case 'p':
                set_idx_param(optarg, &g_popSize,
                        "wielkosc populacji musi byc liczba naturalna wieksza od 0");
                _set_popSize = 1;
                break;
            case 'x':
                set_prob_param(optarg, &g_pCross,
                        "prawd. krzyżowania musi mieć wartość z przedziału [0, 1]");
                _set_pCross = 1;
                break;
            case 'm':
                set_prob_param(optarg, &g_pMut,
                        "prawd. mutacji musi mieć wartość z przedziału [0, 1]");
                _set_pMut = 1;
                break;
            case 'r':
                set_prob_param(optarg, &_val_selParamFLOAT,
                        "wartość zmiennoprzecinkowego parametru metody selekcji "
                        "musi przyjmować wartość z przedziału (0, 1)");
                _set_selParamFLOAT = 1;
                break;
            case 'f':
                set_func_param(optarg);
                _set_funcType = 1;
                break;
            case 's':
                set_char_param(optarg, &selCode,
                        "BRT", "dostepne sa 3 typy selecki R-ruletka, T-turniej, B-najlepsze");
                switch (selCode) {
                    case 'R':
                        g_selType = RULETTE;
                        break;
                    case 'T':
                        g_selType = TOURNAMENT;
                        break;
                    case 'B':
                        g_selType = BEST;
                        break;
                    default:
                        MYERR_ERR(-1, "Nieznany selCode");
                }
                _set_selType = 1;
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

int main(int argc, char *argv[])
{
    real_t result;
    stats_t stats;

    /* wczytanie parametrow */
    read_params(argc, argv);
    /* wartości domyślen zmiennych */
    init_globals();
    srand(g_seed);
    if (g_VERBOSELVL > 0) {
        print_params();
    }
    /* alokacja pamieci */
    /* funkcja algorytmu */
    result = galgorithm(&stats);
    /* zwrocenie wynikow */
    printf("RESULT->%f\n", g_revalFunct(result));
    /*printf("\nRESULT -> %f\n", result);*/
    return 0;
}
