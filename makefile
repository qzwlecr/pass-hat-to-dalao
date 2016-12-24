main:main.o optimizer.o outliner.o recognizer.o
	g++ -std=c++14 optimizer.o outliner.o recognizer.o main.o -O2 -Dcimg_use_png -lpng -L/usr/X11R6/lib -lm -lpthread -lX11 `pkg-config opencv --cflags --libs` -o main

main.o: stdafx.hpp main.cpp optimizer.o outliner.o
	g++ -std=c++14 main.cpp -O2 -Dcimg_use_png -lpng -lz -lX11 -lpthread -c

optimizer.o: stdafx.hpp optimizer/optimizer.cpp optimizer/optimizer.hpp 
	g++ -std=c++14 optimizer/optimizer.cpp -O2 -lpng -L/usr/X11R6/lib -lm -lpthread -lX11 -c

outliner.o:stdafx.hpp outliner/outliner.cpp outliner/outliner.hpp 
	g++ -std=c++14 outliner/outliner.cpp -O2 -lpng -L/usr/X11R6/lib -lm -lpthread -lX11 -c

recognizer.o:ellipse_recognizer/recognizer.cpp ellipse_recognizer/recognizer.hpp 
	g++ -std=c++14 ellipse_recognizer/recognizer.cpp -O2 -lpng -L/usr/X11R6/lib -lm -lpthread -lX11 `pkg-config opencv --cflags` `pkg-config opencv --libs` -c

#png_reader.o:
#	g++ -std=c++14 CImgDir/png_reader.cpp -Dcimg_use_png -lpng -lz -lX11 -lpthread -c

clean:
	-rm *.o
	-rm main
# example: g++ -o hello_word.exe hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
