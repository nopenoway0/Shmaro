make:
	g++ -I /usr/include/python2.7 -I./ -fPIC -c Engine_Info.cpp -lpython2.7 -lboost_python 
	g++ -shared Engine_Info.o -o engine_info.so -lpython2.7 -lboost_python 
install:
	mv engine_info.so /usr/lib/python2.7
clean:
	rm *.out *.o *.so
