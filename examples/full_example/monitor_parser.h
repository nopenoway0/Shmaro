#ifndef MONITOR_PARSER_H
#define MONITOR_PARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <engine_info.h>
#include <stdio.h>

namespace{
	std::vector<std::string> parse_input(std::string in){
		in = std::regex_replace(in, std::regex("\\s\\s+"), "  ");
		in = std::regex_replace(in, std::regex("(\\s*(-)+\\s+)|\n"), "  ");
		std::regex e("\\s\\s");
		std::sregex_token_iterator iter(in.begin(), in.end(), e, -1);
		std::sregex_token_iterator end;
		std::vector<std::string> tokens = std::vector<std::string>();

		int skip_first_three = 0;
		for(; iter != end;++iter){
			//std::cout << "Token: " << *iter << "\n";
			if(skip_first_three > 2) tokens.push_back(*iter);
			skip_first_three += 1;
		}
		// Create vector to push tokens onto
		// Parsing starts here
		return tokens;
	}

	Engine_Info extract_engine(std::vector<std::string> input){
		Engine_Info engine = Engine_Info(0,0,0);
		std::cout << std::endl;
		//engine.set_flag(0);
		for(int x = 2; x < input.size(); x+=2){
			std::string temp = input.at(x);
			std::string label = input.at(x-1);
			if(strstr(label.c_str(), "Engine RPM") != NULL) engine.set_rpm(std::stoi(temp.substr(0, temp.length()-3)));
			else if(strstr(label.c_str(), "Calculated Load Value") != NULL) engine.set_load(std::stof(temp.substr(0, temp.length()-1)));
			else if(strstr(label.c_str(), "Fuel System Status")){
				if(strstr(temp.c_str(), "Open") == NULL) engine.set_fuel_status(true);
				else engine.set_fuel_status(false);
			}
			else if(strstr(label.c_str(), "Engine Coolant Temperature")) engine.set_coolant_tmp(std::stof(temp.substr(0, temp.length()-1)));
			else if(strstr(label.c_str(), "Vehicle Speed")) engine.set_speed(std::stof(temp.substr(0, temp.length()-4)));
			else if(strstr(label.c_str(), "Intake Air Temperature")) engine.set_intake_tmp(std::stof(temp.substr(0, temp.length()-1)));
			else if(strstr(label.c_str(), "Air Flow Rate")) engine.set_airflow(std::stof(temp.substr(0, temp.length()-4)));
			else if(strstr(label.c_str(), "Absolute Throttle Position")) engine.set_throttle(std::stof(temp.substr(0, temp.length()-1)));
			else if(strstr(label.c_str(), "Short term fuel trim Bank")) engine.set_strim(std::stof(temp.substr(0, temp.length()-1)));
			else if(strstr(label.c_str(), "Long term fuel trim Bank")) engine.set_ltrim(std::stof(temp.substr(0, temp.length()-1)));
			//else if(strstr(label.c_str(), "Calculated Load Value")) engine.set_load(std::stof(temp.substr(0, temp.length()-1)));*/

		}
		return engine;
	}
}


#endif
