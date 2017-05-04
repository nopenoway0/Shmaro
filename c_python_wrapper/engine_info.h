#ifndef ENGINE_INFO_H
#define ENGINE_INFO_H
#include <string>
#include <iostream>
#include <vector>

#include <boost/python/tuple.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>

namespace{
	#define ENGINE_END 1
	#define UNITIALIZED 2
	struct error{
		std::string name;
		int number;
		inline bool operator==(error a){
			return (a.number == this->number);
		}
	};
	class Engine_Info{
		public:
			Engine_Info(int rpm, double load, int flag){this->rpm = rpm; this->engine_load = load; this->errors = std::vector<error>(); this->flag = flag;}
			Engine_Info(int rpm, double load){this->rpm = rpm; this->engine_load = load; this->errors = std::vector<error>(); this->flag = 0;}
			Engine_Info(){Engine_Info(0,0); this->flag = UNITIALIZED;}
			int get_rpm() {return this->rpm;}
			double get_load() {return this->engine_load;}
			//void set_flag(int flag) {this->flag = flag;}
			int get_flag() {return this->flag;}
			std::vector<error> get_errors(){return this->errors;}
			bool operator==(Engine_Info a){
				return this->rpm == a.get_rpm() && this->engine_load == a.get_load() && this->flag == a.get_flag();
			}
		private:
			int rpm;
			double engine_load;
			std::vector<error> errors;
			int flag;
	};

	struct engine_info_pickle_suite : boost::python::pickle_suite{
		static boost:: python::tuple getinitargs(Engine_Info& e){
			using namespace boost::python;
			return make_tuple(e.get_rpm(), e.get_load(), e.get_flag());
		}
	};

}

#endif