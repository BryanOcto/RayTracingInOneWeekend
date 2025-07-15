# adapted from https://makefiletutorial.com and COMP30023 Makefile tutorial

exe = ray-tracer
execs = PPM-img-output-demo ray-tracer

$(exe): $(exe).cpp
	g++ -o $(exe) $(exe).cpp

%: %.cpp
	g++ -o % %.cpp

clean:
	rm -f *.o $(execs)