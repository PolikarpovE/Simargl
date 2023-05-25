#include <iostream>
#include <string>
#include <map>

extern "C" {
#include <modbus.h> 
}

modbus_t* CreateController() {
	modbus_t* newController;
	std::string ipAddress = "";
	while (true) {
		std::cout << "Please input correct ip address modbus controller or \"q\" to exit:" << std::endl;
		std::cin >> ipAddress;
		if (ipAddress == "q")
			return NULL;
		newController = modbus_new_tcp("192.168.10.2", 502);
		if (newController == NULL) {  // Не создался, может не работает libmodbus 
			fprintf(stderr, "Can't create modbus server - check libmodbus, exiting...: ");
			return NULL;
		}
		if (modbus_connect(newController) == -1)   // Если нет соединения
			fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		else
			break;
		
	}
	std::cout << "Connection established.\n";  // Успех
	return newController;
}

void CommandExecute(modbus_t* controller, std::string command) {
	std::map<std::string, int> commandAddress = { {"1", 8256}, {"2", 8257} };
	int resSwitchOn = 0, resSwitchOff = 0;
	resSwitchOn = modbus_write_bit(controller, commandAddress[command], 1);
	resSwitchOff = modbus_write_bit(controller, commandAddress[command], 0);
	std::cout << (resSwitchOn == 1 && resSwitchOff == 1);
}

void Welcome(modbus_t* contoller) {
	std::string userInp = "";
	while (true) {
		std::cout << "Select command: \n";
		std::cout << "1 open main GAZ\n";
		std::cout << "2 close main GAZ\n";
		std::cout << "q exit\n";
		std::cin >> userInp;
		if (userInp == "q")
			break;
		else if (userInp == "1" || userInp == "2")
			CommandExecute(contoller, userInp);
	}
}
 
int main() {
	modbus_t* activeContoller = CreateController();
	if (activeContoller == NULL) {
		std::cout << "Application terminate" << std::endl;
		modbus_free(activeContoller);
		return 0;
	}
	Welcome(activeContoller);
	std::cout << "Goodbye";
	modbus_free(activeContoller);
	return 0;
}