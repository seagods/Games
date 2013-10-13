TARGET		=Flight

$(TARGET):	Flight.o   gl2ps.o  
	g++ -g -o $(TARGET) Flight.o  gl2ps.o  \
	 `sdl-config --libs` -lGL -lGLU -lOGLFT -lm  -lmylib -lgfortran

Flight.o:   
	g++ -g  -c Flight.cpp `sdl-config --cflags`  

gl2ps.o:
	cc -g -c gl2ps.c 

clean:
	rm -f $(TARGET) *.o  core
