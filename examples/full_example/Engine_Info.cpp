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

	class_<Engine_Info>("Engine_Info", init<int, float, bool, float, float, float, float, float, float, 
				float, int>())
	.def(init<int, float, int>())
	.def(init<int,float>())
	.def(init<>())	
	.def("get_rpm", &Engine_Info::get_rpm)
	.def("set_rpm", &Engine_Info::set_rpm)
	.def("get_load", &Engine_Info::get_load)
	.def("set_load", &Engine_Info::set_load)
	.def("get_errors", &Engine_Info::get_errors)
	.def("get_flag", &Engine_Info::get_flag)
	.def("set_flag", &Engine_Info::set_flag)
	.def("get_airflow", &Engine_Info::get_airflow)
	.def("set_airflow", &Engine_Info::set_airflow)
	.def("get_throttle", &Engine_Info::get_throttle)
	.def("set_throttle", &Engine_Info::set_throttle)
	.def("get_ltrim", &Engine_Info::get_ltrim)
	.def("set_ltrim", &Engine_Info::set_ltrim)
	.def("get_strim", &Engine_Info::get_strim)
	.def("set_ltrim", &Engine_Info::set_strim)
	.def("get_coolant_tmp", &Engine_Info::get_coolant_tmp)
	.def("set_coolant_tmp", &Engine_Info::set_coolant_tmp)
	.def("get_speed", &Engine_Info::get_speed)
	.def("set_speed", &Engine_Info::set_speed)
	.def("get_intake_tmp", &Engine_Info::get_intake_tmp)
	.def("set_intake_tmp", &Engine_Info::set_intake_tmp)
	.def("get_fuel_status", &Engine_Info::get_fuel_status)
	.def("set_fuel_status", &Engine_Info::set_fuel_status)
	.def_pickle(engine_info_pickle_suite())
	;
}