/*
 * subClientHandler.cpp
 *
 *  Created on: 27.02.2012
 *      Author: Felix Bachmair
 */

#include "keithleySubClientHandler.h"

using namespace std;

keithleySubClientHandler::keithleySubClientHandler(std::string clientName):subClientHandler(clientName){
	bComOpen=false;
	baudrate=57600;
	cout<<"BAUDRATE: "<<baudrate<<endl;
	keithley=0;
	com =0;
//	sleep(1);
//	cout<<"OPEN"<<endl;
//	com = new linux_rs232("/dev/ttyF1",57600);
//	keithley = new KEITHLEY_SourceMeter_2400(com);
//	sleep(1);
//	cout<<"ON"<<endl;
//	keithley->TurnOutputOn();
//	sleep(3);
//	cout<<"OFF"<<endl;
//	keithley->TurnOutputOff();
//	sleep(1);
//	delete keithley;
//	delete com;
//	keithley=0;
//	com=0;
//	sleep(2);
}

bool keithleySubClientHandler::analyseData(packetData_t data)
{
	if(data.aboName=="/keithley"){
		cout<<"received Data:"<<data.timeStamp<<" "<<data.data<<endl;
		//	if(data.data.find("start")!=data.data.end())
		string aboData =data.data;

		/*
		 * PROGRam
		 */
		if(aboData.find(":PROGR")!=string::npos){
			if(aboData.find(":STAR")!=string::npos){

			}
			else if(aboData.find(":CAT")!=string::npos){
				//todo
			}
			else if(aboData.find(":SEL")!=string::npos||aboData.find(":SET")!=string::npos){
				//int programNumber=extractProgramNumber(aboData);
				//jumo.setProgramNumber(programNumber);
			}
			else if (aboData.find("NEXT")!=string::npos){
				//jumo.nextStep();
			}
			else if(aboData.find("PAUS")!=string::npos){
				//					jumo.pauseProgram();
			}
			else if(aboData.find("STAT")!=string::npos){
				//todo
			}
			else if(aboData.find(":HELP?")!=string::npos){
				//todo
			}
			else if(aboData.find(":NUM?")!=string::npos){
				//todo
			}

		}
		/*
		 *
		 */
		else if(aboData.find(":HELP?")!=string::npos)
		{
			printHelp();
		}

	}

	return 1;
}

void keithleySubClientHandler::printHelp(){
	stringstream test;
}

void keithleySubClientHandler::openConnection(std::string port){
	if(port!="")
		this->port=port;
	std::cout<<"Open Connection with port: \""<<this->port<<"\""<<std::endl;
	openDevice();
	initilaiseKeithley();
}

void keithleySubClientHandler::closeDevice(){
	cout<<"close connection to COM"<<endl;
	if(keithley!=0)delete keithley;
	keithley=0;
	if(com!=0)delete com;
	com =0;
	bComOpen =false;
}

void keithleySubClientHandler::openDevice(){
	if(com==0){
		cout<<"open Com device on "<<port<<" with Baudrate: "<<baudrate<<endl;
		com = new linux_rs232("/dev/ttyF1",57600);//port.c_str(),baudrate);
	}
	if(com!=0&&keithley==0){
		keithley = new KEITHLEY_SourceMeter_2400(com);
		cout<<"NEW KEITHLEY INTERFACE"<<endl;
	}
	if(com!=0){
		if(keithley!=0){
			bComOpen=true;
		}
		else{
			delete com;
			com=0;
			cout<<"Delete com"<<endl;
		}
	}
}

void  keithleySubClientHandler::setDevice(std::string port){
	if(bComOpen)
		closeDevice();
	this->port=port;
}



void keithleySubClientHandler::initilaiseKeithley(){
	if(!bComOpen)
		return;
	cout<<"Initialise Keithley:\n\tTest IDN:\""<<keithley->GetIdentificationCode()<<"\""<<endl;
	cout<<"SERIALNUMBER: "<<keithley->GetSerialNumber()<<endl;
	keithley->GetModelType();
	cout<<"\"DONE\n\tDisableControlBeeper"<<flush;
	keithley->EnalbeControlBeeper();
	cout<<"...DONE\n\tSelectRearTerminal"<<flush;
	keithley->SelectRearTerminals();
	cout<<"...DONE\n\tSelectFixedVoltageSourcingMode"<<flush;
	keithley->SelectFixedVoltageSourcingMode();
	cout<<"...DONE\n\tEnableConcurrentMeasurments"<<flush;
	keithley->EnableConcurrentMeasurments();
	cout<<"...DONE\n\tSelectRepeatFilterType"<<flush;
	keithley->SelectRepeatFilterType();
	cout<<"...DONE\n\tSetFilterCount->2"<<flush;
	keithley->SetFilterCount(2);
	cout<<"...DONE\n\tEnableFilter"<<flush;
	keithley->EnableFilter();
	cout<<"...DONE\n\tEnableConcurrentMeasurments"<<flush;
	keithley->EnableConcurrentMeasurments();
	cout<<"...DONE\n\tSetCurrentCompliance"<<flush;
	keithley->SetCurrentCompliance(100e-6);
	cout<<"...DONE\n\tSelectCurrentMeasureRange->20e-6"<<flush;
	keithley->SelectCurrentMeasureRange(20e-6);
	cout<<"...DONE\n\tSetCurrentMeasurementSpeed->10"<<flush;
	keithley->SetCurrentMeasurementSpeed(10);
	cout<<"...DONE\n\tSetImmediateVoltageLevel->-150"<<flush;
	keithley->SetImmediateVoltageLevel(-150);
	cout<<"...DONE\n\tTurnOutputOn"<<flush;
	keithley->TurnOutputOn();
	cout<<"...DONE\n"<<endl;
}

void keithleySubClientHandler::printTime(){
		/*time print*/
		t = time(NULL);
		local = localtime(&t);
		printf("\nStart time and date: %s\n", asctime(local));
		return ;
}
