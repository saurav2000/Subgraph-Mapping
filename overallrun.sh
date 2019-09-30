python generate.py $2 $3 $4 $5 > $1.graphs 
sh compile.sh
sh run1.sh $1
./minisat $1.satinput $1.satoutput > $1.satlog
sh run2.sh $1
python check.py $1.graphs $1.mapping