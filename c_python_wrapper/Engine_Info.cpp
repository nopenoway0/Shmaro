#define BOOST_PYTHON_STATIC_LIB
#include <string>
#include <iostream>
#include <engine_info.h>
#include <boost/python.hpp>
#include <vector>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <socket_functions.h>

// Created library shmaro for use in python
// def ...; are functions in header file
BOOST_PYTHON_MODULE(shmaro)
{
	using namespace boost::python;

	def("start_server", start_server);
	def("stop_server", stop_server);
	def ("get_list", get_list);

	class_<error >("error_struct")
	.def_readonly("name", &error::name)
	.def_readwrite("number", &error::number)
	;

	class_<std::vector<Engine_Info> >("engine_info_vector")
	.def(vector_indexing_suite<std::vector<Engine_Info> >())
	;

	class_<std::vector<error> >("error_vector")
	.def(vector_indexing_suite<std::vector<error> >())
	;

	class_<Engine_Info>("Engine_Info", init<int, double, int>())
	.def("get_rpm", &Engine_Info::get_rpm)
	.def("get_load", &Engine_Info::get_load)
	.def("get_errors", &Engine_Info::get_errors)
	.def("get_flag", &Engine_Info::get_flag)
	.def_pickle(engine_info_pickle_suite())
	;
}