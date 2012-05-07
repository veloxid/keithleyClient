//============================================================================
// Name        : keithleyClient.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================
#include <sys/select.h>
#include <errno.h>
#include <unistd.h>	// read, STD*_FILENO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keithleyInterface.h"
#include "keithleySubClientHandler.h"
#include <subsystem/subClientHandler.h>
#include "subsystem/error.h"
#include "subsystem/signalnames.h"
#include <signal.h>	// signal()
#include <pthread.h>
#include <rs232/linux_rs232.h>
#include <rs232/KEITHLEY/SourceMeter_2400/KEITHLEY_SourceMeter_2400.h>
//void print_sum(float x, float y)
//{
//  std::cout << "The sum is " << x+y << std::endl;
//}

keithleySubClientHandler *client;

std::string address= "/dev/F0";
bool verbosity=false;
bool isFakeDevice;
void cleanexit() {
	eprintf("clean exit\n");
	client->killClient();
//	client.closeConnection();
	std::cout<<"client: "<<client->isOk()<<std::endl;
//	wantexit = 1;
}


void sigint_handler(int sig) {
	client->killClient();
	eprintf("received %s(%d)\n", SIGNAME[sig], sig);
	client->killClient();
	client->closeConnection();
	client->closeDevice();
	std::cout<<"client: "<<client->isOk()<<std::endl;
}

void printHelp(){

}

bool readInputs(int argc,char ** argv){
	isFakeDevice = false;
	for(int i=1; i < argc; i++)
	{
		if(std::string(argv[i]) == "-h"||std::string(argv[i])=="--help")
		{
			printHelp();
			exit(0);
		}
		else if(std::string(argv[i]) == "-d")
		{
			i++;
			address = std::string(argv[i]);
			std::cout<<"Setting device Address = "<<address<<std::endl;
		}
		else if(std::string(argv[i]) == "-f")
		{
			isFakeDevice=true;
			std::cout<<"Test Program with faked keithley device"<<std::endl;
		}
		else if(std::string(argv[i]) == "-v")
		{
			verbosity=true;
			std::cout<<"Start program with verbose level"<<std::endl;
		}
	}
	if(isFakeDevice)
		address="";
	return true;
}

int main(int argc, char *argv[]) {
	readInputs(argc,argv);
	client = new keithleySubClientHandler("keithleyClient");
	client->openConnection("/dev/ttyF1");
	client->keithley->TurnOutputOn();
	sleep(2);
	client->keithley->TurnOutputOff();
	atexit(cleanexit);
	signal(SIGINT, sigint_handler);
	signal(SIGTERM, sigint_handler);
	client->subscribeAbo("/keithley");
	client->getAboFromServer();
//	client->setTimeOut(2);
////	//pthread_t p1;
//	client->sendAndReceive();
//	client->killClient();
//	client->closeDevice();
//	while(!client->isClientKilled())
//		usleep(10);
//	delete client;
	return EXIT_SUCCESS;
}
