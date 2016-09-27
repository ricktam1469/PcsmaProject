#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fftw3.h>
#include <bits/stdc++.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class fft_compute
{
	private:

	unsigned char chunk_id[4];
    	int chunk_size;
    	unsigned char audioFormat[4];
    	unsigned char subchunk1_id[4];
    	int subchunk1_size;
    	unsigned char audio_format[2];
    	int num_channels;
    	int sample_rate;            		// sample_rate denotes the sampling rate.
    	int byte_rate;
    	unsigned char block_align[2];
    	double *magnitudeFFTShift;
    	int bits_per_sample;
    	unsigned char subchunk2_id[4];
    	int subchunk2_size;
	double *magnitudeFFT;

	// FFT variables	
	fftwf_complex *inputRawData;
  	fftwf_complex *outputFFT;
  	fftwf_plan plan_backward;
 	fftwf_plan plan_forward;
	int nPointFFT;
		
	public:

	void computeFFT(char wavName[100], bool debug_switch)
	{		
	    FILE * filePointer;			// Open wave file in read mode
	    cout<<"compute FFt 1 "<<wavName<<endl;
	    //char wavFilePath[100]; //= "/var/tmp/";
	    //memset(wavFilePath,0x00,100);
	   // strcat(wavFilePath,wavName);
	    //wavFilePath.append(wavName);
	    
	    //printf("wave file path- %s" , wavFilePath);
	    filePointer = fopen(wavName,"rb");
	    if(filePointer==NULL)
	    {
	    cout<<"Error in opening the file"<<endl;
	    } 
	    else
	    {
	    fseek (filePointer, 0, SEEK_END);   	
	    int fileSize = ftell(filePointer); 
	    rewind(filePointer); 		

	    fseek (filePointer, 16, SEEK_SET);     
	    fread(&subchunk1_size,1,4,filePointer); 
	    if(debug_switch) cout << "SubChunk Size1 : "<<subchunk1_size << endl;  

	    fseek (filePointer, 22, SEEK_SET); 
	    fread(&num_channels,1,2,filePointer);
	    if(debug_switch) cout <<"Number of channels : "<< num_channels << endl;

	    fseek (filePointer, 34, SEEK_SET); 
	    fread(&bits_per_sample,1,2,filePointer);
	    if(debug_switch) cout << "bps : "<<bits_per_sample << endl;

	    if(subchunk1_size > 16)
	    {
		char dummy[subchunk1_size-16];
		fread(dummy,1,subchunk1_size-16,filePointer);
	    }

	    fseek (filePointer, 40, SEEK_SET); 
	    fread(&subchunk2_size,1,4,filePointer);
	    if(debug_switch) cout << "SubChunk Size2 : " << subchunk2_size << endl;

	    //if(bits_per_sample==16) // Recording with S16_LE format (16 bits/ sample Little Endian Format)
	    //{
		int numOfSamples = subchunk2_size/(num_channels*(bits_per_sample/8));
		inputRawData = (fftwf_complex *) fftwf_malloc ( sizeof ( double ) * numOfSamples );

		long int counter = 0;
		short int readBit;
		double sampleValue;
		
		fseek (filePointer, 44, SEEK_SET); //Moving the pointer to RawData Start index of Wav file
		for (int index = 0; index <numOfSamples; index++)
		{
			fread(&readBit,1,2,filePointer);
			sampleValue = readBit/32768.0; 	// Converting the sample values between -1 and 1 for plotting a waveform
			inputRawData[index][0]=(double)sampleValue;
			inputRawData[index][1]=0;
		}

		fclose(filePointer); // Closing Wav file

		// FFT Input
		nPointFFT = numOfSamples; //1024;
		outputFFT = (fftwf_complex *) fftwf_malloc ( sizeof ( double ) * nPointFFT );
		magnitudeFFT = (double*)malloc(sizeof(double)*nPointFFT);
		magnitudeFFTShift = (double*)malloc(sizeof(double)*nPointFFT);

		
		// Compute FFT
		plan_forward = fftwf_plan_dft_1d ( nPointFFT, inputRawData, outputFFT, FFTW_FORWARD, FFTW_ESTIMATE );
		fftwf_execute ( plan_forward );
		if(debug_switch) cout << "Magnitude:"<<endl;
		for(int index = 0; index < nPointFFT ; index++)
		{	
  			//magnitudeFFT[index] = (-2*sqrt(pow((double)outputFFT[index][0],2) + pow((double)outputFFT[index][1],2)))/nPointFFT;
  			magnitudeFFT[index] = sqrt(pow((double)outputFFT[index][0],2) + pow((double)outputFFT[index][1],2));
  			//printf ( "  %3d  %12f  %12f %12f\n", index, outputFFT[index][0], outputFFT[index][1] ,magnitudeFFT[index] );
			if(debug_switch) cout << magnitudeFFT[index] <<endl;
		}
		//***************************fft-Shift*****************************************
		if(debug_switch) cout << " Magnitude SHIFT:"<<endl;

		for (int index = 0; index < nPointFFT/2; index++)
			magnitudeFFTShift[index]=magnitudeFFT[(nPointFFT/2)+index];
		
		for (int index = nPointFFT/2; index <nPointFFT; index++)
			magnitudeFFTShift[index]=magnitudeFFT[index - nPointFFT/2];
		
		for(int index = 0; index < nPointFFT ; index++)
			//cout << magnitudeFFTShift[index] << endl;
		
		//-----------------------------------------------------------------------------------------------------------------
		
		fftwf_destroy_plan ( plan_forward );
		fftwf_free ( inputRawData );
		fftwf_free ( outputFFT );
		free(magnitudeFFT);
		free(magnitudeFFTShift);
	}
	}
};

int main()
{
fft_compute obj_computeFFT;
obj_computeFFT.computeFFT("beep.wav",true);
return 0;
}

