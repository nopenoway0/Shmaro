#ifndef ENGINE_INFO_H
#define ENGINE_INFO_H
#include <string>
#include <iostream>
#include <vector>
namespace{
	struct error{
		std::string name;
		int number;
		inline bool operator==(error a){
			return (a.number == this->number);
		}
	};
	class Engine_Info{
		public:
			Engine_Info(int rpm, double load){this->rpm = rpm; this->engine_load = load; this->errors = std::vector<error>();
				this->errors.push_back(sample_error);}
			int get_rpm() {return this->rpm;}
			double get_load() {return this->engine_load;}
			std::vector<error> get_errors(){return this->errors;}
		private:
			int rpm;
			float engine_load;
			std::vector<error> errors;

	};
}
#endif