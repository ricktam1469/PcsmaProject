#include <stdio.h>
#include <iostream>    
#include <stdio.h> 
#include <string.h>    
#include <string> 
#include <sys/socket.h> 
#include <sys/time.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>
#include <complex>
#include <time.h>
#include <cstring>

using namespace std;

	char *board;
    	int port,freq;
    	struct timeval start;
   	double a[50];
    	char *fftValue;
    	int k=0;
	int i=0;
 	typedef bitset<8> byte;
/**
    TCP Client class
*/
class tcp_client
{
private:
	int sock;
	std::string address;
    	int port;
    	struct sockaddr_in server;
	char date[50];
    	char time_str[50];
	char Packet_Frame[1024];
     
public:
	tcp_client()
	{
	    sock = -1;
	    port = 0;
	    address = "";
	}

	/**
	 Connect to a host on a certain port number
	*/    
	bool conn(string address, int port)
	{
		//create socket if it is not already created
		if(sock == -1)
		{
			//Create socket
			sock = socket(AF_INET , SOCK_STREAM , 0);
			if (sock == -1)
			{
		    		perror("Could not create socket");
			}
		 
			cout<<"Socket created\n";
	    	}
	    	else    {   /* OK , nothing */  }

	      	//setup address structure
	    	if(inet_addr(address.c_str()) == -1)
	    	{
			struct hostent *he;
			struct in_addr **addr_list;
		 
			//resolve the hostname, its not an ip address
			if ( (he = gethostbyname( address.c_str() ) ) == NULL)
			{
			    //gethostbyname failed
			    herror("gethostbyname");
			    cout<<"Failed to resolve hostname\n";
			     
			    return false;
			}
			 
			//Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only

			addr_list = (struct in_addr **) he->h_addr_list;

			for(int i = 0; addr_list[i] != NULL; i++)
			{

			    //strcpy(ip , inet_ntoa(*addr_list[i]) );
			    server.sin_addr = *addr_list[i];
			    cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<endl;
			    break;
			}
	    	}

	    	//plain ip address
	    	else
	    	{
			server.sin_addr.s_addr = inet_addr( address.c_str() );
	    	}
	     
	    	server.sin_family = AF_INET;
	    	server.sin_port = htons( port );

	   	//Connect to remote server
	    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	    	{

			perror("connect failed. Error");
			return 1;
	    	}
	     
	    	cout<<"Connected\n";
	   	return true;
	}

	/**
    	 Send data to the connected host
	*/    
	bool send_data(string data)
	{
		//Send some data
	    	if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
	    	{
	    		cout<<"eroor";
			perror("Send failed : ");
			return false;
	    	}
		//cout<<"rcv:"<<receive(8);
	    	cout<<"Data send "<<endl;
		cout<<"line 133"<<endl;
	    	return true;
	}
	/**
   	 Receive data from the connected host
	*/	    
	string receive(int size)
	{
		char buffer[size];
	    	string reply;
	   	//Receive a reply from the server
	    	if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
	    	{
			puts("recv failed");
	    	}

	    	reply = buffer;
		return reply;
	}


	void generateOPCFormat(int largestData)
	{
	time_t t;
        struct tm *timeptr=localtime(&t);	
        /* 
        short int NumberOfValues = 1;
	float ProcessValue = 256.0f;
	
	float GeneratedProcessValue1;
	
	char NumberOfValues_ByteArray[sizeof(float)];
	char floatByteArray[sizeof(float)]; 

		cout<<sizeof(largestData)<<endl;
		fftValue=new char[12];
		char fftData[8];
		
	    	struct tm *timeptr=localtime(&t);
	    	byte* bytes_fft[4];
		//gettimeofday(&start, NULL);
		//time(&t);
		//timeptr=localtime(&t);
		//sprintf(date,"%04d:%02d:%02d",timeptr->tm_year+1900, timeptr->tm_mon +1,timeptr->tm_mday );
		//sprintf(time_str,"%02d:%02d:%02d:%02ld",timeptr->tm_hour, timeptr->tm_min,timeptr->tm_sec,start.tv_usec/1000 );
		//sprintf(Packet_Frame,"#S,%s,%s,%s,20,%s;\n",board,date,time_str,val);
		
		
		
		
		// Extraction
                cout<<"INSIDE SENDING DATA: "<<endl;
		// NumberOfValues int -> 2 Byte Char Array
		memcpy(NumberOfValues_ByteArray, &NumberOfValues, sizeof NumberOfValues);

		// ProcessValue float -> 4 Byte Char Array
		//memcpy(floatByteArray, &sampleFFTData[index], sizeof NumberOfValues);
		//memcpy(bytes_fft, (unsigned char*)(&sampleFFTData[index]),4);
		//sprintf(fftData,"%g,",sampleFFTData[index]);
		//strcat(fftValue,floatByteArray[index]);
		sprintf(fftValue,"%c\n", floatByteArray[index]);
		
                */
		//cout<<fftValue;
		gettimeofday(&start, NULL);
		time(&t);
		timeptr=localtime(&t);
		sprintf(date,"%04d:%02d:%02d",timeptr->tm_year+1900, timeptr->tm_mon +1,timeptr->tm_mday );
		sprintf(time_str,"%02d:%02d:%02d:%02ld",timeptr->tm_hour, timeptr->tm_min,timeptr->tm_sec,start.tv_usec/1000 );
		sprintf(Packet_Frame,"%s,%s,%s,%d;\n", board, date, time_str, largestData);
	

	}

	void Sending_Data(int largestData)
	{
		cout<<"Sending data"<<endl;
		generateOPCFormat(largestData);
		send_data(Packet_Frame);
		cout<<Packet_Frame;
	    	//receive and echo reply
	    	//cout<<"----------------------------\n\n";
	    	//cout<<Rcvd_String.c_str();
	    	//cout<<"\n\n----------------------------\n\n";
		
			
		
	}
	void intialConnection()
	{
		char Initial_Msg[20];
		sprintf(Initial_Msg,"#S,RTC,%s;",board);
		send_data(Initial_Msg);
	}
	
	string* extractAck(string Rcvd_String)
	{

		char temp[64];
		int i=0;    
    		char *token;
    		strcpy(temp,Rcvd_String.c_str());
    		string* receivedDataString=new string[8];
    		token = strtok(temp, ", ");
    		
		while (token!=NULL)
		{
	    		receivedDataString[i]=token;
	    		cout << receivedDataString[i] << endl;
	    		token = strtok(NULL, ", ");
			i++;
    		}
		receivedDataString[i]=i;
		return receivedDataString;
		
				
			

	}
	
	/*void masterSwitching()
	{
		if(receivedData==plc_receive)
		{
			cout<<"INSIDE MASTER: "<<endl;		
			string delimiter=",";
			size_t pos = 0;
			string token;
			string master_pi_IP[5];
			char Master_port_string[2][64];
			int j=0;
			int position=0;
			int Master_port=0;
			while((position=plc_receive.find(delimiter))!=std::string::npos)
			{
				string new_server=plc_receive.substr(0,position);
				master_pi_IP[j]=new_server;		
				cout<<master_pi_IP[j]<<endl;
				plc_receive.erase(0,position+delimiter.length());	
				j++;
			}
			char temp[64];
			strcpy(temp,plc_receive.c_str());
			strcpy(Master_port_string[0],temp);
			Master_port=atoi(Master_port_string[0]);
			cout<<"--->>"<<Master_port<<endl;
			rpi.conn(master_pi_IP[2],Master_port);
			rpi.send_data(Initial_Msg);
			rpi.Sending_Data(c.receive(2048));
			cout<<"New Master";
			rpi.Close_Socket();		
		}
	}*/
	
	void Close_Socket()
	{
		shutdown(sock,2);
		close(sock);
		cout<<" Socket Closed"<<endl;
		sock=-1;
	}
};
 /*
int main()
{
   	ofstream file;
        tcp_client c,rpi;
	string host;
   	ifstream fin;
	fin.open("Cluster.config");
	string line;
	string receivedData;
	char config_value[4][64];

	int transmissionTime=0;
	int clusterStatus;
	int clusterStatus_pi;
	string ack;	

	while(getline(fin,line))
	{
		char temp[64];
		int pos=line.find("=");
		strcpy(temp,line.substr(pos+1).c_str());
		strcpy(config_value[i], temp);
		i++;
	}
        host=config_value[1];
        board=config_value[0];
        freq=2;
        port=atoi(config_value[2]);
        cout<<"host="<<host<<endl<<"board="<<board<<endl<<"port="<<port;
       
       	//connect to host
	c.conn(host , port);
	c.intialConnection();
	
	while(1)
	{
		
		string *ack_extract;
		receivedData=c.receive(64);
		receivedData=receivedData.substr(0,receivedData.find(";"));
		ack_extract=c.extractAck(receivedData);
		ack=ack_extract[0];
		clusterStatus=atoi(ack_extract[1].c_str());
		if(ack=="PLC_ACK")
		{
			if(clusterStatus==0)
			{
				transmissionTime=atoi(ack_extract[2].c_str());		
				c.Sending_Data(receivedData);
			}
			if(clusterStatus==1)
			{
				host=ack_extract[2];
				port=atoi(ack_extract[3].c_str());
				c.Close_Socket();
				rpi.conn(host,port);
				rpi.intialConnection();
				while(1)
				{
					string *ack_extract_pi;
					receivedData=rpi.receive(64);
					receivedData=receivedData.substr(0,receivedData.find(";"));
					ack_extract_pi=rpi.extractAck(receivedData);
					ack=ack_extract_pi[0];
					clusterStatus_pi=atoi(ack_extract_pi[1].c_str());
					host=ack_extract_pi[2];
					port=atoi(ack_extract_pi[3].c_str());
					if(ack=="Rpi72_ACK")
					{
						if(clusterStatus_pi==1)
						{		
							transmissionTime=atoi(ack_extract[2].c_str());		
							rpi.Sending_Data(receivedData);
						}
						if(clusterStatus_pi==0)
						{	//cout<<"host and port"<<host<<"   "<<port<<endl;
							//rpi.receive(32);
							rpi.Close_Socket();	
							c.conn(host,port);
							c.intialConnection();
							break;
						}
					
					}
		
					//transmissionTime=atoi(ack_extract_pi[2].c_str());		
					//rpi.Sending_Data(receivedData);	
				}
			}
		}
	}
	//cout<<"512-"<<c.receive(512);
	return 0;

}
*/
