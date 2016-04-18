#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "wav_record.hpp"
#include "fft_compute.hpp"

using namespace std; 

int main()
{	
	ofstream file;
        
	string receivedData;
	
	int transmissionTime=0;
	int clusterStatus;
	int clusterStatus_pi;
	string ack;	
	
	wav_record object;
	fft_compute obj_computeFFT;
	int index=0,i=0;
	char *wavFileName;
	bool debug_switch;
	
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
	
	
	numberOfiterations = 2;
	whileloopbreak_counter = 1;
        file.open("acoustic_fft.txt");
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
	       char wavFilePath[256] ="../wavFiles/";
	       strcat(wavFilePath,wavFileName);
                       
	        //FFT of the Recorded wavFile
	        FFTSampledData = obj_computeFFT.computeFFT(wavFilePath,debug_switch,1,44100,1);
	        
                
	        //Compression of Audio File	 
	        if(debug_switch) cout<<"Start Compressing :"<<endl;
	        compressWavStatus = object.compressWavFile(wavFileName);      
		if(!compressWavStatus)
		{
			if(debug_switch) cout<<"Compression Failed.Please check with bzip2 :"<<endl;
			//return -1;
		}
		if(debug_switch) cout<<"End Compressing :"<<endl;
		
		 
		for(int test_index=0 ; test_index < 44100 ; test_index++)
		{
			file << FFTSampledData[test_index] << endl;
                               
                         

		}
                

                //Transfer of fft file	 
	        if(debug_switch) cout<<"Transfer Start :"<<endl;
	        transferWavStatus = object.transferToFDB("acoustic_fft.txt");      
		if(!transferWavStatus)
		{
			if(debug_switch) cout<<"Transfer Failed.Please check with scp :"<<endl;
			//return -1;
		}
		if(debug_switch) cout<<"Transfer End :"<<endl;


                if(whileloopbreak_counter == numberOfiterations)
			break;
		whileloopbreak_counter = whileloopbreak_counter + 1;
	}
	
		
	return 0;
}
