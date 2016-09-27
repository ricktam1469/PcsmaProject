#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
//#include "wav_record.h"
//#include "fft_compute.h"
#include "wav_record.hpp"
#include "fft_compute.hpp"
#include "client_server.hpp"
using namespace std; 

int main()
{	
	ofstream file;
        tcp_client c,rpi;
	string host;
	string receivedData;
	char config_value[4][64];
	int transmissionTime=0;
	int clusterStatus;
	int clusterStatus_pi;
	string ack;	
	
	wav_record object;
	fft_compute obj_computeFFT;
	int index=0,i=0;
	char *wavFileName;
	bool debug_switch;
	ifstream fin;
	fin.open("Cluster.config");
	string line;
	char *boardNumber;
	float *FFTSampledData;
	char wavFilePath[256 ]="/var/tmp/"; // path for tmpfs in RAM
		
	// Output Status
	bool recordOutputStatus;
	bool compressWavStatus;
	bool transferWavStatus;
	bool deleteWavStatus;
	
	// While loop break counter
	int whileloopbreak_counter = 0;
	int numberOfiterations =  0;

         float largestData=0.0f;
        int largestDataindex=0;
	
	// Reading Config.config and Configuring Pi paramters	
	while(getline(fin,line))
	{
		char temp[64];
		int pos_1=line.find("=");
		int pos_2=line.find(";");
		strcpy(temp,line.substr((pos_1+1),(pos_2-pos_1) ).c_str());
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
		
	boardNumber=config_value[0];
	if(strcmp(config_value[3],"ON"))
		debug_switch=true;
	else
		debug_switch=false;
	
	numberOfiterations = 2;
	whileloopbreak_counter = 1;
        file.open("abc.txt");
	while(1)
	{      
		
                if(debug_switch) cout << "Iteration Number : " << whileloopbreak_counter << endl;
		
		// Creating wavFileName
		wavFileName=object.wavFileNameGen("RPi1");
		if(debug_switch) cout << " WavFileName : " << wavFileName << endl;
		
		// Recording Audio Data (Creates wavFile in /var/tmp TempFilesystem in RAM)
	        if(debug_switch) cout << "Record Start" << endl;
		recordOutputStatus = object.recordWavFile(wavFileName);
		if(!recordOutputStatus)
		{
			if(debug_switch) cout<<"Recording Failed. Please check with arecord tool." << endl << endl;
			return -1;
		}
	        if(debug_switch) cout << "Record End" << endl << endl;
	        
               
	       //Append wavFileName with the path of tmpfs(RAM)
	       //char wavFilePath[256] ="/var/tmp/";
	       //strcat(wavFilePath,wavFileName);
	        
	        //FFT of the Recorded wavFile
	        FFTSampledData = obj_computeFFT.computeFFT(wavFileName,debug_switch,1,44100,1);
	        
                
	        //Compression of Audio File	 
	        if(debug_switch) cout<<"Start Compressing :"<<endl;
	        compressWavStatus = object.compressWavFile(wavFileName);      
		if(!compressWavStatus)
		{
			if(debug_switch) cout<<"Compression Failed.Please check with bzip2 :"<<endl;
			//return -1;
		}
		if(debug_switch) cout<<"End Compressing :"<<endl;
		
		/* //Transfer of Audio File	 
	        if(debug_switch) cout<<"Transfer Start :"<<endl;
	        transferWavStatus = object.transferToFDB(wavFileName);      
		if(!transferWavStatus)
		{
			if(debug_switch) cout<<"Transfer Failed.Please check with scp :"<<endl;
			//return -1;
		}
		if(debug_switch) cout<<"Transfer End :"<<endl;
		
		 //Deletion of Audio File from RAM	 
	        if(debug_switch) cout<<"Start Deleting :"<<endl;
	        //deleteWavStatus = object.removeWavFile(wavFileName);      
		if(!deleteWavStatus)
		{
			if(debug_switch) cout<<"Deletion Failed.Please check with rm :"<<endl;
			//return -1;
		}
		if(debug_switch) cout<<"End Deleting :"<<endl;
		*/
                

                
	        
		for(int test_index=0 ; test_index < 44100 ; test_index++)
		{
			file << FFTSampledData[test_index] << endl;
                               
                         

		}
                largestData=FFTSampledData[0] ;
                for(int test_index1=1 ; test_index1 < 44100/2 ; test_index1++)
		{
                        if(largestData<FFTSampledData[test_index1]){
                                   largestData=FFTSampledData[test_index1];
                                   largestDataindex=test_index1;
cout<<"@@@@@"<<largestDataindex<<endl;
                         } 
               }
                cout<<"--->"<<largestDataindex;
                c.Sending_Data(largestDataindex);

		if(whileloopbreak_counter == numberOfiterations)
			break;
		whileloopbreak_counter = whileloopbreak_counter + 1;
	}
	
		
	return 0;
}
