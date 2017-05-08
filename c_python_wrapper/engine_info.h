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
	// Error codes will be stored in this struct will contain
	// Error description as "name" and corresponding numeric code
	struct error{
		std::string name;
		int number;
		inline bool operator==(error a){
			return (a.number == this->number);
		}
	};
	// Class to contain info. Incomplete will contain more information that scantool extracts
	class Engine_Info{
		public:
			Engine_Info():Engine_Info(0,0,UNITIALIZED){}

			Engine_Info(int rpm, float load):Engine_Info(rpm, load, 0){}

			Engine_Info(int rpm, float load, int flag):Engine_Info(rpm, load, false, 0, 0, 0, 0, 0, 0, 0,flag){}

			Engine_Info(int rpm, float load, bool fuel_status, float coolant_tmp, float speed, float intake_tmp, float airflow, float throttle_position, float ltrim,
				float strim, int flag){this->rpm = rpm; this->engine_load = load; this->fuel_status = fuel_status;
											this->coolant_tmp = coolant_tmp; this->speed = speed; this->intake_tmp = intake_tmp; this->airflow = airflow; this->throttle_position = throttle_position;
											this->ltrim = ltrim; this->strim = strim; this->flag = flag;}
			


			int get_rpm() {return this->rpm;}
			void set_rpm(int rpm){this->rpm = rpm;}

			float get_load() {return this->engine_load;}
			void set_load(float load) {this->engine_load = load;}

			void set_flag(int flag) {this->flag = flag;}
			int get_flag() {return this->flag;}

			std::vector<error> get_errors(){return this->errors;}
			
			float get_airflow(){return this->airflow;}
			void set_airflow(float airflow){this->airflow = airflow;}

			float get_throttle(){return this->throttle_position;}
			void set_throttle(float throttle){this->throttle_position = throttle;}

			float get_ltrim(){return this->ltrim;}
			void set_ltrim(float ltrim){this->ltrim = ltrim;}

			float get_strim(){return this->strim;}
			void set_strim(float strim){this->strim = strim;}

			float get_coolant_tmp(){return this->coolant_tmp;}
			void set_coolant_tmp(float coolant_tmp){this->coolant_tmp = coolant_tmp;}

			float get_speed(){return this->speed;}
			void set_speed(float speed){this->speed = speed;}

			float get_intake_tmp(){return this->intake_tmp;}
			void set_intake_tmp(float intake){this->intake_tmp = intake;}

			bool get_fuel_status(){return this->fuel_status;}
			void set_fuel_status(bool status){this->fuel_status = status;}

			bool operator==(Engine_Info a){
				return this->rpm == a.get_rpm() && this->engine_load == a.get_load() && this->flag == a.get_flag() && this->airflow == a.get_airflow()
					&& this-> throttle_position == a.get_throttle() && this->ltrim == a.get_ltrim() && this->strim == a.get_strim() && this->coolant_tmp == a.get_coolant_tmp()
					&& this->speed == a.get_speed() && this->intake_tmp == a.get_intake_tmp() && this->fuel_status == a.get_fuel_status();
			}
/*
			std::ostream& operator<<(std::ostream& outputStream){
				outputStream << "Engine Info Object- RPM: " << this->get_rpm() << "\nEngine Load: " << this->get_load();
				outputStream << "\nFuel Status: " << this->get_fuel_status() << "\nIntake Temperature: " << this->get_intake_tmp();
				outputStream << "\nSpeed: " << this->get_speed() << "\nCoolant Temp: " << this->get_coolant_tmp() << "\nLong Trim: ";
				outputStream << this->get_ltrim() << "\nShort Trim: " << this->get_strim() << "\nThrottle: " << this->get_throttle() << "\nAirflow: " << this->get_airflow();
				return outputStream;
			}*/
		private:
			float airflow;
			float throttle_position;
			float ltrim;
			float strim;
			float coolant_tmp;
			float speed;
			float intake_tmp;
			bool fuel_status;
			int rpm;
			float engine_load;
			std::vector<error> errors;
			int flag;
	};

	// makes class "pickelable"
	// e.g. lets python break it down into bytes and put it into shared memory
	struct engine_info_pickle_suite : boost::python::pickle_suite{
		static boost:: python::tuple getinitargs(Engine_Info& e){
			using namespace boost::python;
			return make_tuple(e.get_rpm(), e.get_load(), e.get_fuel_status(), e.get_coolant_tmp(), e.get_speed(), e.get_intake_tmp(), e.get_airflow(), e.get_throttle(), e.get_ltrim(), 
				e.get_strim(), e.get_flag());
		}
	};

}

#endif
