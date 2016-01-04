OBJ = main.o
INC = -I "./"

ray_trace: $(OBJ)
	g++ $(OBJ) -o ray_trace.exe
	rm -f $(OBJ)
	
main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) ray_trace.exe