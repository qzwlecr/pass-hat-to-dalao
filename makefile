main:main.o optimizer.o outliner.o recognizer.o manual_recognition.o
	g++ -std=c++14 optimizer.o outliner.o recognizer.o main.o -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 `pkg-config opencv --cflags --libs` -o main

main.o: stdafx.hpp main.cpp
	g++ -std=c++14 main.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

optimizer.o: stdafx.hpp optimizer/optimizer.cpp optimizer/optimizer.hpp 
	g++ -std=c++14 optimizer/optimizer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

outliner.o:stdafx.hpp outliner/outliner.cpp outliner/outliner.hpp 
	g++ -std=c++14 outliner/outliner.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

recognizer.o:stdafx.hpp ellipse_recognizer/recognizer.cpp ellipse_recognizer/recognizer.hpp 
	g++ -std=c++14 ellipse_recognizer/recognizer.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 `pkg-config opencv --cflags` `pkg-config opencv --libs` -c

manual_recognition.o:ellipse_recognizer/manual_recognition.hpp ellipse_recognizer/manual_recognition.cpp stdafx.hpp
	g++ -std=c++14 ellipse_recognizer/manual_recognition.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -c

clean:
	-rm *.o
	-rm main
# example: g++ -o hello_word.exe hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
