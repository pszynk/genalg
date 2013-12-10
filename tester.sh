#!/bin/bash
EXEC="$1"

function exetest {
local threads="$1"
local schedule="$2"
local kwant="$3"

local func="$4"
local sel="$5"
local gen="$6"
local dim="$7"
local popsize="$8"

local omp_schedule=""
if [ ! -z $schedule ]
then
    if [ -z $kwant ]
    then
        local omp_schedule="OMP_SCHEDULE=\"$schedule\""
    else
        local omp_schedule="OMP_SCHEDULE=\"$schedule,$kwant\""
    fi
fi

local omp_threads="OMP_NUM_THREADS=$threads"

echo "env "$omp_schedule" "$omp_threads" ./$EXEC "-f" "$func" "-s" "$sel" "-g" "$gen" "-d" "$dim" "-p" "$popsize""
#retrn=$(env $omp_schedule $omp_threads ./$EXEC "-f" "$func" "-s" "$sel" "-g" "$gen" "-d" "$dim" "-p" "$popsize")
env $omp_schedule $omp_threads ./$EXEC "-f" "$func" "-s" "$sel" "-g" "$gen" "-d" "$dim" "-p" "$popsize"

local status=$?
if [ $status -ne 0 ]
then
    echo "Błąd testu!"
fi
return $retrn
}

if [ -z "$EXEC" ]
then
    echo "Podaj nazwę programu"
    exit 1
fi

if [ ! -f "$EXEC" ]
then
    echo "Program $EXEC nie istnieje, przerywam testy"
    exit 1
fi

if [ ! -x "$EXEC" ]
then
    echo "Plik $EXEC nie jest wykonywalny, przerywam testy"
    exit 1
fi

echo "Testy programu $EXEC"
echo "Testy alg. genetycznego `date`"

popsize=0
gen=100
sel="T"

function get_res_time {
local str="$1"
local etime=$(echo "$str" | sed -n 's/TIME->\(.*\)$/\1/p')
local result=$(echo "$str" | sed -n 's/RESULT->\(.*\)$/\1/p')
echo "$result $etime"
}
otpt=$(exetest 1 "" "" "GR" "R" "1" "1" "2")
#read RES ETIME < <(get_res_time "$otpt")
#echo $RES

function calltest {
#local output=$(exetest "$1" "$2" "$3" "$4" "$5" "$6" "$7" "$8")
local output=$(exetest "$@")
read RESULT ETIME < <(get_res_time "$output")
}
#calltest 1 "" "" "GR" "R" "1" "1" "2"
#echo $RESULT
function test_run {
R_BEST=9999999
R_WORST=-1
R_AVG=0
T_AVG=0
for t in {1..5}
do
    calltest "$@"
    sleep 1
    echo "$RESULT  $ETIME"
    R_AVG=$(echo "$R_AVG" + "$RESULT" | bc)
    T_AVG=$(echo "$T_AVG" + "$ETIME" | bc )
    local new_best=$(echo "$RESULT < $R_BEST" | bc)
    local new_worst=$(echo "$RESULT > $R_WORST" | bc)
    if [ $new_best -ge 1 ]
    then
        R_BEST=$RESULT
    fi
    if [ $new_worst -ge 1 ]
    then
        R_WORST=$RESULT
    fi
done
R_AVG=$(echo "$R_AVG / 5.0" | bc -l)
T_AVG=$(echo "$T_AVG / 5.0" | bc -l)

echo "BEST  >>>>>>>>>>>>> $R_BEST"
echo "WORST >>>>>>>>>>>>> $R_WORST"
echo "AVG   >>>>>>>>>>>>> $R_AVG"
echo "TIME  >>>>>>>>>>>>> $T_AVG"
}
#test_run 1 "" "" "GR" "R" "1" "1" "2"
#etime=$(echo "$otpt" | sed -n 's/TIME->\(.*\)$/\1/p')
#result=$(echo "$otpt" | sed -n 's/RESULT->\(.*\)$/\1/p')
#echo "TIME == $etime"
#echo "RESULT == $result"
#exit 0

B_BREAK="********************************************************************************"
M_BREAK="=================================="
S_BREAK="---------------------"

#dla jednego wątku
#dla dwóch funkcji
#wymiary zadania
#wymiar populacji
#metoda selekcji

echo "$B_BREAK"
echo "JEDEN WĄTEK, obie funkcje, wszystkie wymiary - różne pop, po 5"
for func in "GR" "AC"
do
    echo "$B_BREAK"
    echo "funkcja $func"
    for dim in 2 10 20 50 100
    do
        echo "$M_BREAK"
        echo "wymiar $dim"
        case "$dim" in
            "2")
                popsize=500;;
            "10")
                popsize=2000;;
            "20")
                popsize=3000;;
            "50")
                popsize=5000;;
            "100")
                popsize=10000;;
            *)
                echo "co to za wymiar?"
                exit 1;;
        esac
        echo "populacja $popsize"
        echo "$S_BREAK"
        test_run 1 "" "" $func $sel $gen $dim $popsize
        #for T in {1..5}
        #do
            #echo "test $T"
            #exetest 1 "" "" $func $sel $gen $dim $popsize
            #sleep 0.5
        #done
    done
done

echo
echo
echo

dim=100

echo "$B_BREAK"
echo "JEDEN WĄTEK, obie funkcje, iwymiar 100, różne populacje"
for func in "GR" "AC"
do
    echo "$B_BREAK"
    echo "funkcja $func"
    for popsize in 5000 10000 20000
    do
        echo "populacja $popsize"
        echo "$S_BREAK"
        for T in {1..5}
        do
            echo "test $T"
            exetest 1 "" "" $func $sel $gen $dim $popsize
        done
    done
done

echo
echo
echo
popsize=10000
dim=100

echo "$B_BREAK"
echo "JEDEN WĄTEK, obie funkcje, iwymiar 100, populacja 10000, różne selekcje"
for func in "GR" "AC"
do
    echo "$B_BREAK"
    echo "funkcja $func"
    for sel in R B T
    do
        echo "metoda selekcji $sel"
        echo "$S_BREAK"
        for T in {1..5}
        do
            echo "test $T"
            exetest 1 "" "" $func $sel $gen $dim $popsize
        done
    done
done

echo
echo
echo

############## WIELE WĄTKÓW ##############################################################


#dla dwóch funkcj
#dla 1,2,4 wątków
#dla wyszstkich wymiarów, dopasowanie populacji

sel=R
echo "$B_BREAK"
echo "JEDEN WĄTEK, obie funkcje, wszystkie wymiary - różne pop, po 5"
for func in "GR" "AC"
do
    echo "$B_BREAK"
    echo "funkcja $func"
    for threads in 2 4
    do
        echo "$S_BREAK$M_BREAK"
        echo "watkow $threads"
        for dim in 2 10 20 50 100
        do
            echo "$M_BREAK"
            echo "wymiar $dim"
            case $dim in
                2)
                    popsize=500;;
                10)
                    popsize=2000;;
                20)
                    popsize=3000;;
                50)
                    popsize=5000;;
                100)
                    popsize=10000;;
                *)
                    echo "co to za wymiar?"
                    exit 1;;
            esac
            echo "populacja $popsize"
            echo "$S_BREAK"
            for T in {1..5}
            do
                echo "test $T"
                exetest $threads "" "" $func $sel $gen $dim $popsize
                sleep 0.5
            done
        done
    done
done

#dla dwóch funkcji
#wymiar 100
#dla 1,2,4 wątków
#różne populacje

echo
echo
echo

dim=100
sel=R

echo "$B_BREAK"
echo "WIELE WĄTKÓW, obie funkcje, wymiar 100, różne populacje"
for func in "GR" "AC"
do
    echo "$B_BREAK"
    echo "funkcja $func"
    for threads in 2 4
    do
        echo "$S_BREAK$M_BREAK"
        echo "watkow $threads"
        for popsize in 5000 10000 20000
        do
            echo "populacja $popsize"
            echo "$S_BREAK"
            for T in {1..5}
            do
                echo "test $T"
                exetest $threads "" "" $func $sel $gen $dim $popsize
            done
        done
    done
done

#static, dynamic, dynamic, static kwant?
