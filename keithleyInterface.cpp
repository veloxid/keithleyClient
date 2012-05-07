/*
 * keithleyInterface.cpp
 *
 *  Created on: 27.02.2012
 *      Author: Felix Bachmair
 */

#include "keithleyInterface.h"

using namespace std;
keithleyInterface::keithleyInterface() {
	// TODO Auto-generated constructor stub
	port = "/dev/ttyF1";
	this->printTime();
	isOpen = false;
	isFake = false;
	verbosity = 0;
	com = 0;
	keithley=0;
	cout<<"Created new keithley Interface..."<<endl;
}

keithleyInterface::~keithleyInterface() {
	// TODO Auto-generated destructor stub
	delete keithley;
	delete com;
}
void keithleyInterface::SetPort(std::string port){
	this->port = port;
}

void keithleyInterface::initilaiseKeithley() {
	if(keithley==0)
		return;
	keithley->EnalbeControlBeeper();
	keithley->SelectRearTerminals();
	keithley->SelectFixedVoltageSourcingMode();
	keithley->EnableConcurrentMeasurments();
	keithley->SelectRepeatFilterType();
	keithley->SetFilterCount(2);
	keithley->EnableFilter();
	keithley->EnableConcurrentMeasurments();
	keithley->SetCurrentCompliance(100e-6);
	keithley->SelectCurrentMeasureRange(20e-6);
	keithley->SetCurrentMeasurementSpeed(10);
	keithley->SetImmediateVoltageLevel(-150);

}

int keithleyInterface::printTime() {
	/*time print*/
	t = time(NULL);
	local = localtime(&t);
	if (verbosity)
		printf("\nStart time and date: %s\n", asctime(local));
	return 0;
}

int keithleyInterface::convertToInteger(uint16_t data0, uint16_t data1) {
	int result = (data0 + (data1 << 16));
	// 	printf("convert to integer: %X %X %d\n", data0,data1,result);
	return result;
}
float keithleyInterface::convertToFloat(uint16_t data0, uint16_t data1) {
	int value = convertToInteger(data0, data1);
	float result = *((float*) &value);
	// 	printf("convert to Float %f\n",result);
	return result;
}

int keithleyInterface::closeDevice() {
	delete keithley;
	keithley=0;
	delete com;
	com =0;
	return 0;
}

int keithleyInterface::openDevice() {
	if(com==0)
		com = new linux_rs232(port.c_str());
	if(keithley==0)
		keithley = new KEITHLEY_SourceMeter_2400(com);
	initilaiseKeithley();
	return 1;//todo
}

void keithleyInterface::setVerbosity(uint16_t verb) {
	this->verbosity = verb;
}

void keithleyInterface::SelectRearTerminals(){
	if(keithley!=0)keithley->SelectRearTerminals();
}

