#include <iostream>
#include <unistd.h>
#include "mcp9808.hpp"

#include "mraa.hpp"

int main()
{
	using namespace std;

//! [Interesting]   
	int command;
	upm::MCP9808 *temp = new upm::MCP9808(6);

	do
	{
		cout << endl;
		cout << "1 - read temp \t" ;
		cout << "2 - sleep mode \t";
		cout << "3 - wake up" << endl;
		cout << "4 - set mode to " << (temp->isCelsius() == true ? "Fahrenheit" : "Celsius") << endl;
		cout << "5 - show status bits" << endl;
		cout << "6 - Set Tcrit \t" ;
		cout << "7 - Set Tupper \t" ;
		cout << "8 - Set Tlower " << endl;
		cout << "9 - Display monitor temps " << endl;
		cout << "10 - Enable alert default\t";
		cout << "11 - Enable alert interrupt" << endl;
		cout << "12 - Clear interrupt \t" ;
		cout << "13 - Clear alert mode" << endl;
		cout << "14 - Get Hysteresis\t";
		cout << "15 - Set Hysteresis" << endl;
		cout << "16 - Get Resolution\t";
		cout << "17 - Set Resolution" << endl;
		cout << "18 - Get Manufacturer ID"<< endl;
		cout << "19 - Get Device ID" << endl;
		cout << "-1 - exit" << endl;
		cout << "Enter a command: ";
		cin >> command;

		switch(command)
		{
		float t;
		case 1:
			std::cout << "Temp: " << temp->getTemp() << "° " <<  (temp->isCelsius()? "Celsius" : "Fahrenheit")<< std::endl;
			break;
		case 2:
			cout << "shutdown sensor (sleep mode)" << endl;
			temp->shutDown();
			break;
		case 3:
			cout << "wake up sensor" << endl;
			temp->shutDown(false);
			break;
		case 4:
			cout << "set mode to " << (temp->isCelsius() ? "Fahrenheit" : "Celsius") << endl;
			temp->setMode(!temp->isCelsius());
			break;
		case 5:
			cout << "Tcrit = " << temp->isTcrit();
			cout << " Tupper = " << temp->isTupper();
			cout << " Tlower = " << temp->isTlower();
			break;
		case 6:
			cout << "enter a value";
			cin >> t;
			temp->setMonitorReg(temp->CRIT_TEMP, t);
			break;
		case 7:
			cout << "enter a value";
			cin >> t;
			temp->setMonitorReg(temp->UPPER_TEMP, t);
			break;
		case 8:
			cout << "enter a value";
			cin >> t;
			temp->setMonitorReg(temp->LOWER_TEMP, t);
			break;
		case 9:
			cout << "tcrit = " << temp->getMonitorReg(temp->CRIT_TEMP) << endl;
			cout << "tupper = " << temp->getMonitorReg(temp->UPPER_TEMP) << endl;
			cout << "tlower = " << temp->getMonitorReg(temp->LOWER_TEMP) << endl;
			break;
		case 10:
			cout << "set alert mode default" ;
			temp->setAlertMode(temp->ALERTCTRL);
			break;
		case 11:
			cout << "set alert mode interrupt";
			temp->setAlertMode(temp->ALERTMODE | temp->ALERTCTRL );
			break;
		case 12:
			temp->clearInterrupt();
			break;
		case 13:
			cout << "Clear alerts" << endl;
			temp->clearAlertMode();
			break;
		case 14:
			cout << "Hysteresis: " << temp->getHysteresis() << endl;
			break;
		case 15:
			int u;
			cout << "enter 1 to 4";
			cin >> u ;
			switch(u)
			{
			case 1:
				temp->setHysteresis(temp->HYST_0);
				break;
			case 2:
				temp->setHysteresis(temp->HYST_1_5);
				break;
			case 3:
				temp->setHysteresis(temp->HYST_3_0);
				break;
			case 4:
			default:
				temp->setHysteresis(temp->HYST_6_0);
				break;
			}
			break;
		case 16:
			cout << "Resolution: " << temp->getResolution() << endl;
			break;
		case 17:
			int v;
			cout << "enter 1 to 4";
			cin >> v ;
			switch(v)
			{
			case 1:
				temp->setResolution(temp->RES_LOW);
				break;
			case 2:
				temp->setResolution(temp->RES_MEDIUM);
				break;
			case 3:
				temp->setResolution(temp->RES_HIGH);
				break;
			case 4:
			default:
				temp->setResolution(temp->RES_PRECISION);
				break;
			}
			break;
		case 18:
			cout << "Manufacturer ID: " << std::hex << temp->getManufacturer() << endl;
			break;
		case 19:
			cout << "Get device ID: " << std::hex << temp->getDevicedId() << endl;
			break;
		case -1:
			break;
		default:
			cout << endl << "That option is not available. Try again" << endl;
			break;
		}

	}while (command != -1 );
//! [Interesting]
	return MRAA_SUCCESS;
}
