main:main.o 
	g++ optimizer.o main.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -o main

main.o:optimizer.o
	g++ main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

optimizer.o:optimizer/optimizer.cpp optimizer/optimizer.hpp 
	g++ optimizer/optimizer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

#outliner.o:outliner/outliner.cpp outliner/outliner.hpp 
#	g++ outliner/outliner.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

#reconizer.o:ellipse_recognizer/recognizer.cpp ellipse_recognizer/recognizer.hpp 
#	g++ ellipse_recognizer/recognizer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 `pkg-config opencv --cflags` `pkg-config opencv --libs` -c

clean:
	-rm *.o
	-rm main
# example: g++ -o hello_word.exe hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
