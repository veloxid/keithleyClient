/*
 * keithleyInterface.h
 *
 *  Created on: 20.04.2012
 *      Author: Felix Bachmair
 */

#ifndef KEITHLEYINTERFACE_H_
#define KEITHLEYINTERFACE_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <rs232/linux_rs232.h>
#include <rs232/KEITHLEY/SourceMeter_2400/KEITHLEY_SourceMeter_2400.h>
//#include <rs232/rs232_interface.h>
//
class keithleyInterface {
public:
	keithleyInterface();
	virtual ~keithleyInterface();
	void SetPort(std::string port);
	int openDevice();
	int closeDevice();
	void setVerbosity(uint16_t verb);

	void SelectRearTerminals();
private:
	void initilaiseKeithley();
private:
	int getValues(const char *action, float *outputvalue);
	float convertToFloat(uint16_t data0, uint16_t data1);
	int convertToInteger(uint16_t data0, uint16_t data1);
private:
	bool isFake;
	int verbosity;
	bool isOpen;
	int printTime();
	struct tm *local;
	time_t t;
public:
	KEITHLEY_SourceMeter_2400 *keithley;
private:
	linux_rs232 *com;
	std::string port;

};

#endif /* KEITHLEYINTERFACE_H_ */
