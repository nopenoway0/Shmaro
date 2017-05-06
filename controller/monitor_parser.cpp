#include <stdio.h>
#include <monitor_parser.h>
#include <iostream>
#include <string>
#include <engine_info.h>

int main(){
	// Populate test string
	std::string test_input("Parameter                      Current         FreezeFrame\nFuel System Status             Open            -----          \nCalculated Load Value            4.6%          -----          \nEngine Coolant Temperature      46C            -----          ");
	test_input += "\nShort term fuel trim Bank 1      0.0%         -----          \nLong term fuel trim Bank 1       6.2%         -----          ";
	test_input += "\nEngine RPM                         0RPM        -----          \nVehicle Speed                    0km/h         -----          ";
	test_input += "\nIgnition timing advance Cyl #1  0.0 deg        -----          \nIntake Air Temperature          37C            -----          ";
	test_input += "\nAir Flow Rate                    0.00gm/s      -----          \nAbsolute Throttle Position      20.4%          -----          ";
	test_input += "\nLocation of Oxygen Sensors                     -----          \nBank 1 Sensor 2 Voltage/Trim   0.430V          -----  \n";
	
	// Start test here
	std::vector<std::string> information = parse_input(test_input);
	Engine_Info eng1 = extract_engine(information);
				std::cout << "Engine Info Object- RPM: " << eng1.get_rpm() << "\nEngine Load: " << eng1.get_load();
				std::cout << "\nFuel Status: " << eng1.get_fuel_status() << "\nIntake Temperature: " << eng1.get_intake_tmp();
				std::cout << "\nSpeed: " << eng1.get_speed() << "\nCoolant Temp: " << eng1.get_coolant_tmp() << "\nLong Trim: ";
				std::cout << eng1.get_ltrim() << "\nShort Trim: " << eng1.get_strim() << "\nThrottle: " << eng1.get_throttle() << "\nAirflow: " << eng1.get_airflow();
	return 1;
}