# adapted from https://makefiletutorial.com and COMP30023 Makefile tutorial

exe = PPM-img-output-demo
execs = PPM-img-output-demo

$(exe): $(exe).cpp
	g++ -o $(exe) $(exe).cpp

%: %.cpp
	g++ -o % %.cpp

clean:
	rm -f *.o $(execs)