clear;
INPUT=$1;                                                             # get file.cpp from args
SUBSTRING=$(echo $INPUT| cut -d'.' -f 1);                             # get file without extention
g++ -c $INPUT $SECOND;                                                # \ compile
g++ $SUBSTRING.o -o game -lsfml-graphics -lsfml-window -lsfml-system; # /compile
rm $SUBSTRING.o;                                                      # remove .o file
./game;                                                               # start app