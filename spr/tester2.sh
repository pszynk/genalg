MPIX="mpiexec"
BPATH="../genalg/bin/mpirelease"


function exetest {
local threads="$1"

local mpiv="$2"
local interval="$3"
local migsize="$4"

local func="$5"
local sel="$6"
local gen="$7"
local dim="$8"
local popsize="$9"


echo $MPIX "-n" $threads $BPATH "-f" "$func" "-s" "$sel" "-g" "$gen" "-d" "$dim" "-p" "$popsize" "-i" "$mpiv" "-t" "$interval" "-o" "$migsize"
$MPIX "-n" $threads $BPATH "-f" "$func" "-s" "$sel" "-g" "$gen" "-d" "$dim" "-p" "$popsize" "-i" "$mpiv" "-t" "$interval" "-o" "$migsize"

local status=$?
if [ $status -ne 0 ]
then
    echo "Błąd testu!"
fi
}
#echo "2 gr, ----------------------------------------------------------"
#exetest 2 1 10 10 "GR" "R" 100 5   500
#exetest 2 1 10 10 "GR" "R" 100 10  2000
#exetest 2 1 10 10 "GR" "R" 100 20  3000
#exetest 2 1 10 10 "GR" "R" 100 50  5000
#exetest 2 1 10 10 "GR" "R" 100 100 10000
#
#echo "4 gr, ----------------------------------------------------------"
#exetest 4 1 10 10 "GR" "R" 100 5   500
#exetest 4 1 10 10 "GR" "R" 100 10  2000
#exetest 4 1 10 10 "GR" "R" 100 20  3000
#exetest 4 1 10 10 "GR" "R" 100 50  5000
#exetest 4 1 10 10 "GR" "R" 100 100 10000
#
#echo "2 ac, ----------------------------------------------------------"
#exetest 2 1 10 10 "AC" "R" 100 5   500
#exetest 2 1 10 10 "AC" "R" 100 10  2000
#exetest 2 1 10 10 "AC" "R" 100 20  3000
#exetest 2 1 10 10 "AC" "R" 100 50  5000
#exetest 2 1 10 10 "AC" "R" 100 100 10000
#
#echo "4 ac, ----------------------------------------------------------"
#exetest 4 1 10 10 "AC" "R" 100 5   500
#exetest 4 1 10 10 "AC" "R" 100 10  2000
#exetest 4 1 10 10 "AC" "R" 100 20  3000
#exetest 4 1 10 10 "AC" "R" 100 50  5000
#exetest 4 1 10 10 "AC" "R" 100 100 10000
#
#
#echo "2 gr, pomniejszone ----------------------------------------------------------"
#exetest 2 1 10 10 "GR" "R" 100 5   250
#exetest 2 1 10 10 "GR" "R" 100 10  1000
#exetest 2 1 10 10 "GR" "R" 100 20  1500
#exetest 2 1 10 10 "GR" "R" 100 50  2500
#exetest 2 1 10 10 "GR" "R" 100 100 5000
#
#echo "4 gr, pomniejszone ----------------------------------------------------------"
#exetest 4 1 10 10 "GR" "R" 100 5   125
#exetest 4 1 10 10 "GR" "R" 100 10  500
#exetest 4 1 10 10 "GR" "R" 100 20  750
#exetest 4 1 10 10 "GR" "R" 100 50  1250
#exetest 4 1 10 10 "GR" "R" 100 100 2500
#
#
#echo "2 ac, pomniejszone ----------------------------------------------------------"
#exetest 2 1 10 10 "AC" "R" 100 5   250
#exetest 2 1 10 10 "AC" "R" 100 10  1000
#exetest 2 1 10 10 "AC" "R" 100 20  1500
#exetest 2 1 10 10 "AC" "R" 100 50  2500
#exetest 2 1 10 10 "AC" "R" 100 100 5000
#
#echo "4 ac, pomniejszone ----------------------------------------------------------"
#exetest 4 1 10 10 "AC" "R" 100 5   125
#exetest 4 1 10 10 "AC" "R" 100 10  500
#exetest 4 1 10 10 "AC" "R" 100 20  750
#exetest 4 1 10 10 "AC" "R" 100 50  1250
#exetest 4 1 10 10 "AC" "R" 100 100 2500
#
#
#echo "2 gr, migracja,  ----------------------------------------------------------"
#exetest 2 2 15 10 "GR" "R" 100 100 5000
#exetest 2 2 15 10 "GR" "R" 100 100 10000
#exetest 2 2 15 10 "GR" "R" 100 100 20000
#
#echo "4 gr, migracja,  ----------------------------------------------------------"
#exetest 4 2 15 10 "GR" "R" 100 100 5000
#exetest 4 2 15 10 "GR" "R" 100 100 10000
#exetest 4 2 15 10 "GR" "R" 100 100 20000
#
#
#echo "2 ac, migracja,  ----------------------------------------------------------"
#exetest 2 2 15 10 "AC" "R" 100 100 5000
#exetest 2 2 15 10 "AC" "R" 100 100 10000
#exetest 2 2 15 10 "AC" "R" 100 100 20000
#
#echo "4 ac, migracja,  ----------------------------------------------------------"
#exetest 4 2 15 10 "AC" "R" 100 100 5000
#exetest 4 2 15 10 "AC" "R" 100 100 10000
#exetest 4 2 15 10 "AC" "R" 100 100 20000
#
#
#echo "2 gr, migracja bset,  ----------------------------------------------------------"
#exetest 2 2 15 10 "GR" "B" 100 100 5000
#exetest 2 2 15 10 "GR" "B" 100 100 10000
#exetest 2 2 15 10 "GR" "B" 100 100 20000
#
#echo "4 gr, migracja bset,  ----------------------------------------------------------"
#exetest 4 2 15 10 "GR" "B" 100 100 5000
#exetest 4 2 15 10 "GR" "B" 100 100 10000
#exetest 4 2 15 10 "GR" "B" 100 100 20000
#
#
#echo "2 ac, migracja bset,  ----------------------------------------------------------"
#exetest 2 2 15 10 "AC" "B" 100 100 5000
#exetest 2 2 15 10 "AC" "B" 100 100 10000
#exetest 2 2 15 10 "AC" "B" 100 100 20000
#
#echo "4 ac, migracja bset,  ----------------------------------------------------------"
#exetest 4 2 15 10 "AC" "B" 100 100 5000
#exetest 4 2 15 10 "AC" "B" 100 100 10000
#exetest 4 2 15 10 "AC" "B" 100 100 20000
echo "4 gr, migracja wzrost interwalu,  ----------------------------------------------------------"
exetest 4 2 1 10 "GR" "B" 200 100 2000
exetest 4 2 2 10 "GR" "B" 200 100 2000
exetest 4 2 5 10 "GR" "B" 200 100 2000
exetest 4 2 10 10 "GR" "B" 200 100 2000
exetest 4 2 15 10 "GR" "B" 200 100 2000
exetest 4 2 25 10 "GR" "B" 200 100 2000
exetest 4 2 40 10 "GR" "B" 200 100 2000
exetest 4 2 60 10 "GR" "B" 200 100 2000
exetest 4 2 80 10 "GR" "B" 200 100 2000
exetest 4 2 100 10 "GR" "B" 200 100 2000

echo "4 gr, migracja wzrost liczby,  ----------------------------------------------------------"
exetest 4 2 15 1 "GR" "B" 200 100 2000
exetest 4 2 15 5 "GR" "B" 200 100 2000
exetest 4 2 15 10 "GR" "B" 200 100 2000
exetest 4 2 15 20 "GR" "B" 200 100 2000
exetest 4 2 15 30 "GR" "B" 200 100 2000
exetest 4 2 15 50 "GR" "B" 200 100 2000
exetest 4 2 15 70 "GR" "B" 200 100 2000
exetest 4 2 15 100 "GR" "B" 200 100 2000
exetest 4 2 15 120 "GR" "B" 200 100 2000
exetest 4 2 15 140 "GR" "B" 200 100 2000
exetest 4 2 15 160 "GR" "B" 200 100 2000
exetest 4 2 15 180 "GR" "B" 200 100 2000
exetest 4 2 15 200 "GR" "B" 200 100 2000
exetest 4 2 15 220 "GR" "B" 200 100 2000
exetest 4 2 15 250 "GR" "B" 200 100 2000


echo "4 gr, migracja co tyrniej 10 50,  ----------------------------------------------------------"
exetest 4 2 10 50 "GR" "R" 200 100 2000
exetest 4 2 10 50 "GR" "B" 200 100 2000
exetest 4 2 10 50 "GR" "T" 200 100 2000

exetest 1 1 10 50 "GR" "R" 200 100 2000
exetest 1 1 10 50 "GR" "B" 200 100 2000
exetest 1 1 10 50 "GR" "T" 200 100 2000
