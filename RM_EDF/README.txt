//RM.o is the .o file used in compilation and running in gcc compiler
//RM.c is the .c file that contains the RM algorithm and the RM EE algorithm
//EDF.o is the .o file used in compilation and running in gcc compiler
//EDF.c is the .c file that contains the EDF algorithm
//input1.txt is the text file used to gather scheduling data
//input2.txt is another text file used to gather scheduling data


//To compile the RM algorithm, input this into the gcc compiler - "gcc RM.c -o RM.o -lm"
//After compiling, to run the RM algorithm input this - "./RM.o input1.txt RM" or "./RM.o input2.txt RM", depending on which txt file you want to use
//If you wish to run the EE version of RM, then compile RM.c the same way as before and input one of the following - "./RM.o input1.txt RM EE" or "./RM.o input2.txt RM EE"

//To compile the EDF algorithm, input this into the gcc compiler - "gcc EDF.c -o EDF.o -lm"
//After compiling, to run the EDF algorithm one of these - "./EDF.o input1.txt EDF" or "./EDF.o input2.txt EDF"
