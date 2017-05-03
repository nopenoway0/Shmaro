#define BOOST_PYTHON_STATIC_LIB
#include <string>
#include <iostream>
#include <engine_info.h>
#include <boost/python.hpp>
#include <vector>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

BOOST_PYTHON_MODULE(engine_info)
{
	using namespace boost::python;
	class_<error >("error_struct")
	.def_readonly("name", &error::name)
	.def_readwrite("number", &error::number)
	;
	class_<std::vector<error> >("error_vector")
	.def(vector_indexing_suite<std::vector<error> >())
	;
	class_<Engine_Info>("Engine_Info", init<int, double>())
	.def("get_rpm", &Engine_Info::get_rpm)
	.def("get_load", &Engine_Info::get_load)
	.def("get_errors", &Engine_Info::get_errors)
	;
}