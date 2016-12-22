main:main.o optimizer.o outliner.o
	g++ optimizer.o outliner.o main.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -o main

main.o: stdafx.hpp main.cpp optimizer.o outliner.o
	g++ main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

optimizer.o: stdafx.hpp optimizer/optimizer.cpp optimizer/optimizer.hpp 
	g++ optimizer/optimizer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

outliner.o:stdafx.hpp outliner/outliner.cpp outliner/outliner.hpp 
	g++ outliner/outliner.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

#reconizer.o:ellipse_recognizer/recognizer.cpp ellipse_recognizer/recognizer.hpp 
#	g++ ellipse_recognizer/recognizer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 `pkg-config opencv --cflags` `pkg-config opencv --libs` -c

clean:
	-rm *.o
	-rm main
# example: g++ -o hello_word.exe hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
