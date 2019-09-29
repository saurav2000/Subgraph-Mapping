sh compile.sh
sh run1.sh $1
./minisat $1.satinput $1.satoutput
sh run2.sh $1
gedit $1.mapping