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
	if(data.aboName!="/keithley")
		return 0;
	cout<<"received Data:"<<data.timeStamp<<" "<<data.data<<endl;
	//	if(data.data.find("start")!=data.data.end())
	string aboData =data.data;
	interpret.interpreteData(data.data);
	if (interpret.isAnswer())
		return (1);
	bool commandBeenProcessed = false;
	if(interpret.hasCommandOnPlace("CLIENT",0)){
		commandBeenProcessed = true;
	}
	else if(interpret.hasCommandOnPlace("OUTPUT",0)){
		if(interpret.isCommand())
			cout<<"OUTPUT: \""<<interpret.getComand()<<"\""<<endl;
			if(interpret.getComand()=="ON"){
				keithley->TurnOutputOn();
				cout<<"Turn Output ON "<<endl;
				commandBeenProcessed = true;
			}
			else if((interpret.getComand()=="OFF")){
				keithley->TurnOutputOff();
				cout<<"Turn Output OFF"<<endl;
				commandBeenProcessed = true;
			}
	}
	else if(interpret.hasCommandOnPlace("SOUR",0)){
		if(interpret.hasCommandOnPlace("SWE",1)){
			cout<<"SWEEEEEEEEEEP:"<<endl;
			this->sweepKeithley(0,-150,5,2,0.1);
		}
	}
	else if(interpret.hasCommandOnPlace("HELP",0)){
		printHelp();
		commandBeenProcessed = true;
	}
	if(!commandBeenProcessed){
		keithley->sendstring(data.data);
	}

	return 1;
}

void keithleySubClientHandler::printHelp(){
	stringstream help;
	help<<"HELP FOR KEITHLEY SUBSYSTEM CLIENT, by IPP, ETH ZURICH\n";
	help<<"COMMAND:   \tFUNCTION \n";
	help<<":CLIENT    \t \n";
	help<<":OUTPut VAR\tSET OUTPUT TO VAR, VAR in {ON,OFF}\n";
	help<<":HELP      \tPrint this help\n";

	this->sendToServer("/keithley",help.str().c_str());
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

void keithleySubClientHandler::sweepKeithley(double start, double stop, double step, int nCounts, double delay) {
	//missing: set bias level to 0v
	keithley->SetSourceDelay(delay);
	keithley->SelectBestSourceRanging();
	keithley->SetSweepVoltageSourceMode();//not sure: instead of SOUR:VOLT:MODE SWE
	keithley->SelectLinearSweepScale();
	keithley->SpecifySweepVoltageStartLevel(start);
	keithley->SpecifySweepVoltageStopLevel(stop);
	keithley->SpecifyLinearSweepVoltageStepLevel(step);
	int counter = (stop-start)/step;
	counter *= nCounts;
	keithley->SetTriggerCount(counter);
	keithley->TurnOutputOn();
	for(int i =0;i<counter;i++)
		cout<<i<<"\t"<<keithley->GetDataString()<<endl;
}

void keithleySubClientHandler::printTime(){
		/*time print*/
		t = time(NULL);
		local = localtime(&t);
		printf("\nStart time and date: %s\n", asctime(local));
		return ;
}
