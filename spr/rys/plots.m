dims=[2, 10, 20, 50, 100];
pops=[500, 2000, 3000, 5000, 10000];
big_pops=[5000, 10000, 20000];
threads=[1, 2, 4];


f_GR_1_1 = [0.46, 0.58, 1.63, 6.59, 29.37]
f_AC_1_1 = [0.04, 0.6, 1.72, 6.95, 28.05 ]

plot(
    dims, f_GR_1_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_AC_1_1, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3)
grid("on")
legend("f. Griewanka", "f. Ackleya", "location", "southeast")
axis([-4 105 -4 35])
title('Czas wyk. dla roznych wymiarow zadania', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng rys1.png -F:15

f_GR_P_1_1 = [13.12, 26.07, 52.39]
f_AC_P_1_1 = [14.06, 28.05, 56.27]

plot(
    big_pops, f_GR_P_1_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    big_pops, f_AC_P_1_1, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3)
grid("on")
legend("f. Griewanka", "f. Ackleya", "location", "southeast")
axis([4000 21000 12 60])
title('Czas wyk. dla roznych rozmiarow pop.', 'fontsize', 14)
xlabel('Rozmiar populacji P', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng rys2.png -F:15


f_GR_2_1 = [ 0.05, 0.8, 2.17, 8.39, 31.52 ]
f_GR_2_2 = [ 0.04, 0.45, 1.18, 4.16, 16.2 ]
f_GR_2_4 = [ 0.05, 0.28, 0.65, 2.18, 8.36 ]

plot(
    dims, f_GR_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_GR_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    dims, f_GR_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 watek", "2 watki", "4 watki", "location", "southeast")
axis([-4 105 -4 35])
title('Czas wyk. dla roznych wymiarow f. Griewanka', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng rys3.png -F:15

f_AC_2_1 = [ 0.05, 0.81, 2.21, 8.29, 33.7 ]
f_AC_2_2 = [ 0.04, 0.46, 1.23, 4.38, 17.33 ]
f_AC_2_4 = [ 0.09, 0.28, 0.67, 2.3, 8.91 ]

plot(
    dims, f_AC_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    dims, f_AC_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    dims, f_AC_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 watek", "2 watki", "4 watki", "location", "southeast")
axis([-4 105 -4 35])
title('Czas wyk. dla roznych wymiarow f. Ackleya', 'fontsize', 14)
xlabel('Wymiar funkcji n', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng rys4.png -F:15

f_GR_P_2_1 = [ 14.58, 31.87, 72.53 ]
f_GR_P_2_2 = [ 7.47, 16.65, 37.33 ]
f_GR_P_2_4 = [ 3.85, 8.36, 19.27 ]

plot(
    big_pops, f_GR_P_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    big_pops, f_GR_P_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    big_pops, f_GR_P_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 watek", "2 watki", "4 watki", "location", "southeast")
axis([4000 21000 3 80])
title('Czas wyk. roznych rozmiarow pop. (f. Griewanka)', 'fontsize', 14)
xlabel('Rozmiar populacji P', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng rys5.png -F:15

f_AC_P_2_1 = [ 15.6, 33.81, 76.97 ]
f_AC_P_2_2 = [ 8.03, 17.3, 39.5 ]
f_AC_P_2_4 = [ 4.12, 9.8, 20.39 ]


plot(
    big_pops, f_AC_P_2_1, "-dk", "markersize", 9, "MarkerFaceColor", "k", "linewidth", 3,
    big_pops, f_AC_P_2_2, "-dr", "markersize", 9, "MarkerFaceColor", "r", "linewidth", 3,
    big_pops, f_AC_P_2_4, "-dg", "markersize", 9, "MarkerFaceColor", "g", "linewidth", 3)
grid("on")
legend("1 watek", "2 watki", "4 watki", "location", "southeast")
axis([4000 21000 3 80])
title('Czas wyk. dla roznych rozmiarow pop. (f. Ackleya)', 'fontsize', 14)
xlabel('Rozmiar populacji P', 'fontsize', 14)
ylabel('Czas wykonania t [s]', 'fontsize', 14)
print -dpng rys6.png -F:15

