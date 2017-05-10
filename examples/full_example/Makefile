make:
	g++ -std=c++11 -I./ -I /usr/include/python2.7 -I../c_python_wrapper/ controller.cpp -o controller -lrt -lpython2.7 -lboost_python -lpthread -lboost_system
lib:
	g++ -std=c++11 -I /usr/include/python2.7 -I./ -fPIC -c Engine_Info.cpp -lpython2.7 -lboost_python 
	g++ -std=c++11 -shared Engine_Info.o -o shmaro.so -lpython2.7 -lboost_python -lboost_system
	mv shmaro.so /usr/lib/python2.7
clean:
	rm -f *.out .o
