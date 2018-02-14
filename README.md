# SIMION2ROOT
SIMION2ROOT fill the ROOT TTree with the output of the SIMION simulation.

The use of SIMION2ROOT: 
SIMION2ROOT <file path/file> <delimiter in simion file ex: ,>
if the delimiter is not present it will be considered as TAB
delimiters in SIMION are , or ; or TAB

The SIMION2ROOT first search the ------ Begin Next Fly'm ------ by searching "------"

8 lines after this line, there is line with the list of SIMION parameters writen in the output file. 
The code will create a branch for each parameter (any number of SIMION parameters) with the corresponding name and then put the data into the tree

Prerequisite:
ROOT data analysis framework (https://root.cern.ch/)
