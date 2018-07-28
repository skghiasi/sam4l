/*
 * SARA_response_list.c
 *
 * Created: 25/07/2018 15:48:58
 *  Author: eth
 */ 

#include "SARA_response_list.h"
#include <stdio.h>

const char * returnStringCommand(SARA_command command){

	switch(command){
		case set_response_verbose:
		return("AT+CMEE=2");
		case set_response_errCode:
		return("AT+CMEE=1");
		case is_gprs_active:
		return("AT+CGACT?");
		case is_module_attached_to_network:
		return("AT+CGATT?");
		case is_device_registered:
		return("AT+CEREG?");
		case what_netOperatorCOPS:
		return("AT+COPS?");
		case open_UDP_socket:
		return("AT+USOCR=17");
		case open_TCP_socket:
		return("AT+USOCR=6");
		case connect_to_TCP_server:
		return("AT+USOCO=0,\"195.34.89.241\",7");
		case retrieve_TCP_message:
		return("AT+USORD=0,32");
		case close_socket:
		return("AT+USOCL=0");
		case connect_send_UDP_server:
			return(getUdpCommand());
		case read_UDP_message:
		return("AT+USORF=0,13");
		case what_rssi:
		return("AT+CSQ");
		
		//operation modes
		case set_full_functionality:
		return("AT+CFUN=1");
		case set_airplane_mode:
		return("AT+CFUN=4");
		case retain_tx_amplifier:
		return("AT+CFUN=15");
		
	}
}


void setUdpServerIP(int a , int b , int c , int d){
	UDP_profile.server_ip0 = a;
	UDP_profile.server_ip1 = b;
	UDP_profile.server_ip2 = c;
	UDP_profile.server_ip3 = d;
}

void setUdpServerPort(int port){
	UDP_profile.server_port = port;
}

void setSARAUDPsocket(int socket){
	UDP_profile.sara_socket = socket;
}

void setUDPStringMessage(const char * inputString){
	int size=0; 
	UDP_profile.string_command = inputString;
	while(size < 100 && inputString[size] != '\0' ) size++; 
	UDP_profile.stringLength = size; 
}

void setUDPIntMessage(int inputInt){
	sprintf(udpIntegerHolder , "%d\0" , inputInt);
	setUDPStringMessage(udpIntegerHolder);
}

const char * getUdpCommand(){
	snprintf(udpCommand,100 , "AT+USOST=%d,\"%d.%d.%d.%d\",%d,%d,\"%s\"\0", UDP_profile.sara_socket
	, UDP_profile.server_ip0
	, UDP_profile.server_ip1
	, UDP_profile.server_ip2
	, UDP_profile.server_ip3
	, UDP_profile.server_port
	, UDP_profile.stringLength
	, UDP_profile.string_command
	);
	
	const char * b = udpCommand;
	//return "AT+USOST=0,\"195.34.89.241\",7,13,\"TestNumberOne\"\0";
	return b; 
}

const char * getUdpProfile(){
	char a[100] ;
	snprintf(a ,100, "UDP IP=%d.%d.%d.%d\n\rUDP server port: %d\r\nUDP sara socket: %d\n\rmessage: \"%s\"\r\n",   UDP_profile.server_ip0
	, UDP_profile.server_ip1
	, UDP_profile.server_ip2
	, UDP_profile.server_ip3
	, UDP_profile.server_port
	, UDP_profile.sara_socket
	, UDP_profile.string_command);
	const char * b = a;
	return b;
}

const char * get_udp_send_string(){
	char * a;
	sprintf(a , "AT+USOST=%d,\"%d.%d.%d.%d\",7,13,\"TestNumberOne\"", UDP_profile.sara_socket,
	UDP_profile.server_ip0,
	UDP_profile.server_ip1,
	UDP_profile.server_ip2,
	UDP_profile.server_ip3);
	const char * result = a;
	return result;
}

