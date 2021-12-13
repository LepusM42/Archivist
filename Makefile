EXE=archive

make :
	 
compile :
	g++ main.cpp dictionary.cpp utility.cpp -o $(EXE)
run :
	./$(EXE) $@
