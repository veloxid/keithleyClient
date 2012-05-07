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
	baudrate=9600;
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
	openDevice();
}

void keithleySubClientHandler::closeConnection(){
	if(keithley!=0)delete keithley;
	keithley=0;
	if(com!=0)delete com;
	com =0;
	bComOpen =false;
}

void keithleySubClientHandler::openDevice(){
	if(com==0)
		com = new linux_rs232(port.c_str(),baudrate);
	if(com!=0&&keithley==0)
		keithley = new KEITHLEY_SourceMeter_2400(com);
	if(com!=0){
		if(keithley!=0)
			bComOpen=true;
		else{
			delete com;
			com=0;
		}
	}

}

void  keithleySubClientHandler::setDevice(std::string port){
	if(bComOpen)
		closeConnection();
	this->port=port;
}



void keithleySubClientHandler::initilaiseKeithley(){
	if(!bComOpen)
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

void keithleySubClientHandler::printTime(){
		/*time print*/
		t = time(NULL);
		local = localtime(&t);
		printf("\nStart time and date: %s\n", asctime(local));
		return ;
}
