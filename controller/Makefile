make:
	g++ -std=c++11 -I./ -I /usr/include/python2.7 -I../c_python_wrapper/ controller.cpp -lrt -lpython2.7 -lboost_python -lboost_system

parser:
	g++ -std=c++11 -I /usr/include/python2.7 -I../c_python_wrapper/ -I./ monitor_parser.cpp -o monitor.out -lrt -lpython2.7 -lboost_python
clean:
	rm -f *.out .o
