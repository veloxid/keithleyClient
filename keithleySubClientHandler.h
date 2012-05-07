/*
 * subClientHandler.h
 *
 *  Created on: 27.02.2012
 *      Author: Felix Bachmair
 */

#ifndef keithleySubClientHandler_H_
#define keithleySubClientHandler_H_
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>	// read, STD*_FILENO
#include <unistd.h>
#include "scpiInterpreter.h"

#include <rs232/linux_rs232.h>
#include <rs232/KEITHLEY/SourceMeter_2400/KEITHLEY_SourceMeter_2400.h>
#include <subsystem/subClientHandler.h>

class 	keithleySubClientHandler: public subClientHandler{
	//todo constructor
public:
	keithleySubClientHandler(std::string clientName="keithleyClient");
	void openConnection(std::string port="");
	void closeDevice();
	void setDevice(std::string port);
public:
	KEITHLEY_SourceMeter_2400 *keithley;
private:
	linux_rs232 *com;
	std::string port;
	bool bComOpen;
	struct tm *local;
	time_t t;
	int baudrate;
private:
//	keithleyInterface keithley;
	scpiInterpreter interpret;
public:
	void printHelp();
private:
	void openDevice();
	void initilaiseKeithley();
	void printTime();
public:
	bool analyseData(packetData_t data);
};

#endif /* keithleySubClientHandler_H_ */
