/*
 * SARA_response_list.h
 *
 * Created: 25/07/2018 10:50:37
 *  Author: eth
 */ 


#ifndef SARA_RESPONSE_LIST_H_
#define SARA_RESPONSE_LIST_H_


typedef struct{
	int server_ip0 ;
	int server_ip1 ; 
	int server_ip2 ; 
	int server_ip3 ; 
	
	int server_port;
	int sara_socket;
	
	const char * string_command; 
	int stringLength; 
}COMM_profile;

COMM_profile UDP_profile; 

char udpIntegerHolder[100];
char udpCommand[100]; 

typedef enum SARA_command_ {
	//communication configuration
	set_response_verbose, //CMEE=2
	set_response_errCode, //CMEE=1
	is_gprs_active,   //CGACT?
	is_module_attached_to_network , //CGATT?  
	is_device_registered, // CEREG?
	what_netOperatorCOPS,  // COPS? : getting operator information 
	
	open_UDP_socket, //USOCR=17
	open_TCP_socket, //USOCR=6
	connect_to_TCP_server, // AT+USOCO=0,"195.34.89.241",7 : IP + port ? 
	retrieve_TCP_message, // AT+USORD=0,32 : means on socket 0 
	close_socket, // AT+USOCL=0: closing socket 0 
	
	connect_send_UDP_server, // "AT+USOST=0,\"195.34.89.241\",7,13,\"TestNumberOne\"")
	read_UDP_message, //"AT+USORF=0,13"  listens for a message on socket 0  
	
	what_rssi, // AT+CSQ
	
	//operation modes
	set_full_functionality, //AT+CFUN=1
	set_airplane_mode, //AT+CFUN=4
	retain_tx_amplifier //AT+CFUN=15
	
	}SARA_command;
	
const char * get_udp_send_string();

const char * returnStringCommand(SARA_command command); 

void setUdpServerIP(int a , int b , int c , int d); 

void setUdpServerPort(int port);
void setSARAUDPsocket(int socket);
void setUDPStringMessage(const char * inputString);
void setUDPIntMessage(int inputInt);

const char * getUdpCommand();

const char * getUdpProfile();

#endif /* SARA_RESPONSE_LIST_H_ */