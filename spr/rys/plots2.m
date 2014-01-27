dims=[2, 10, 20, 50, 100];
pops=[500, 2000, 3000, 5000, 10000];
big_pops=[5000, 10000, 20000];
threads=[1, 2, 4];




f_GR_2_1 = [ 0.05, 0.8, 2.17, 8.39, 31.52 ]


f_GR_2_2 = [ 0.13 1.29  3.3 11.66 46.0]
f_GR_2_4 = [ 0.13 1.27 3.31 11.77 46.48  ]

plot(
    dims, f_GR_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_GR_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    dims, f_GR_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 proces", "2 procesy", "4 procesy", "location", "southeast")
axis([-4 105 -4 50])
title('Czas wyk. dla roznych wymiarow f. Griewanka', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng mpi_gr_rpop.png -F:15

f_AC_2_1 = [ 0.05, 0.81, 2.21, 8.29, 33.7 ]
f_AC_2_2 = [ 0.04 0.46 3.23 11.38 37.33  ]
f_AC_2_4 = [ 0.09 1.29 3.36 12.2 40.91  ]

plot(
    dims, f_AC_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_AC_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    dims, f_AC_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 proces", "2 procesy", "4 procesy", "location", "southeast")
axis([-4 105 -4 45])
title('Czas wyk. dla roznych wymiarow f. Ackleya', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng mpi_ac_rpop.png -F:15

#--------------------------------------------------------------------------------------------

f_GR_W_1 = [ 0.05, 0.8, 2.17, 8.39, 31.52 ]


f_GR_W_2 = [ 0.05 0.49  1.34 5.02 19.63]
f_GR_W_4 = [ 0.03 0.21 0.76 2.29 8.98  ]

plot(
    dims, f_GR_W_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_GR_W_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    dims, f_GR_W_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 proces", "2 procesy", "4 procesy", "location", "southeast")
axis([-4 105 -4 35])
title('Czas wyk. dla roznych wymiarow f. Griewanka', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng mpi_gr_Wpop.png -F:15

f_AC_W_1 = [ 0.05, 0.81, 2.21, 8.29, 33.7 ]
f_AC_W_2 = [ 0.06 0.51 1.37 5.19 20.64  ]
f_AC_W_4 = [ 0.02 0.23 0.61 2.39 9.54  ]

plot(
    dims, f_AC_W_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_AC_W_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    dims, f_AC_W_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 proces", "2 procesy", "4 procesy", "location", "southeast")
axis([-4 105 -4 35])
title('Czas wyk. dla roznych wymiarow f. Ackleya', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng mpi_ac_Wpop.png -F:15

#--------------------------------------------------------------------------------------------


f_GR_P_2_1 = [ 14.58, 31.87, 72.53 ]
f_GR_P_2_2 = [ 19.64 45.77 117.44 ]
f_GR_P_2_4 = [ 19.76 46.36 119.63 ]


plot(
    big_pops, f_GR_P_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    big_pops, f_GR_P_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    big_pops, f_GR_P_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 proces", "2 procesy", "4 procesy", "location", "southeast")
axis([4000 21000 0 130])
title('Czas wyk. roznych rozmiarow pop. (f. Griewanka)', 'fontsize', 14)
xlabel('Rozmiar populacji P', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng mpi_gr_dpop_ru.png -F:15

f_AC_P_2_1 = [ 15.6, 33.81, 76.97 ]
f_AC_P_2_2 = [ 20.56 47.62 122.2 ]
f_AC_P_2_4 = [ 20.8 48.31 123.54  ]


plot(
    big_pops, f_AC_P_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    big_pops, f_AC_P_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    big_pops, f_AC_P_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 proces", "2 procesy", "4 procesy", "location", "southeast")
axis([4000 21000 0 130])
title('Czas wyk. dla roznych rozmiarow pop. (f. Ackleya)', 'fontsize', 14)
xlabel('Rozmiar populacji P', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng mpi_ac_dpop_ru.png -F:15

#--------------------------------------------------------------------------------------------


intervals = [1 2 5 10 15 25 40 60 80 100]
f_GR_Blad_interval = [123.4 129.24 123.56 150.72 133.17 156.47 154.06 144.90 126.74 143.94 ]

plot(intervals, f_GR_Blad_interval, "-dr", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
axis([0 110 120 160])
title('Wyniki minim. dla roznych interwalow migracji (f. Griewanka)', 'fontsize', 14)
xlabel('Interwal migracji osobnikow', 'fontsize', 14)
ylabel('Wynik algorytmu genetycznego', 'fontsize', 14)
print -dpng mpi_gr_intervals.png -F:15


migrations = [1 5 10 20 30 50 70 100 120 140 160 180 200 220 250]
f_GR_Blad_migrations = [154.55 155.06 147.48 143.35 151.64 143.77 142.41  137.51 128.73  119.96 118.18 128.02 147.74 145.92   149.47]

plot(migrations, f_GR_Blad_migrations, "-dg", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3)
grid("on")
axis([0 260 110 160])
title('Wyniki minim. dla roznych wielkosci migracji (f. Griewanka)', 'fontsize', 14)
xlabel('Wielkosc jednej migracji osobnikow', 'fontsize', 14)
ylabel('Wynik algorytmu genetycznego', 'fontsize', 14)
print -dpng mpi_gr_migrations.png -F:15