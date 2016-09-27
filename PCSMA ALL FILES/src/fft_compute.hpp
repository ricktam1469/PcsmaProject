#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fftw3.h>
#include <bits/stdc++.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>

using namespace std;

class fft_compute
{
	private:

	int chunk_size;
    	int subchunk1_size;
    	unsigned int num_channels;
    	unsigned int sample_rate;            		// sample_rate denotes the sampling rate.
    	unsigned int byte_rate;
    	float *freqFFTData;
    	float *magnitudeFFTShift;
    	unsigned int bits_per_sample;
    	unsigned int subchunk2_size;
	float *magnitudeFFT;

	// FFT variables	
	fftwf_complex *inputRawData;
  	fftwf_complex *outputFFT;
  	fftwf_plan plan_backward;
 	fftwf_plan plan_forward;
	int nPointFFT;
		
	public:
	fft_compute()
	{
		num_channels=0;
		sample_rate=0;
		byte_rate=0;
		bits_per_sample=0;
		subchunk2_size=0;
	}
	
	float* computeFFT(char wavName[100], bool debug_switch, int startFreq,int endFreq, int bandGap)
	{	
		
	    FILE * filePointer;			// Open wave file in read mode
	    cout<<"compute FFt 1 "<<wavName<<endl;
	    filePointer = fopen(wavName,"rb");
	    //filePointer = fopen(wavName,"rb");
	    if(filePointer==NULL)
	    {
	    	cout<<"Error in opening the file"<<endl;
	    	return NULL;
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
		    cout<<"subchunk2_size : " << subchunk2_size<<endl;
		    cout<<"num_channels : " << num_channels<<endl;
		    cout<<"bits_per_sample : " << bits_per_sample<<endl;
			int numOfSamples = subchunk2_size/(num_channels*(bits_per_sample/8));
			cout<<"num of samples : " << numOfSamples<<endl;
			cout<<"before malloc.c"<<endl;
			inputRawData = (fftwf_complex *) fftwf_malloc ( sizeof ( fftwf_complex ) * numOfSamples );
			cout<<"malloc.c worked";
			long int counter = 0;
			short int readBit = 0;
			float sampleValue = 0;
		
			fseek (filePointer, 44, SEEK_SET); //Moving the pointer to RawData Start index of Wav file
			for (int index = 0; index <numOfSamples; index++)
			{
				fread(&readBit,1,2,filePointer);
				sampleValue = readBit/32768.0; 	// Converting the sample values between -1 and 1 for plotting a waveform
				inputRawData[index][0]=(float)sampleValue;
				inputRawData[index][1]=0;
			}

			fclose(filePointer); // Closing Wav file

			// FFT Input
			nPointFFT = 44100;
			cout<<"before malloc.c"<<endl;
			// FFT Raw output buffer
			outputFFT = (fftwf_complex *) fftwf_malloc ( sizeof ( fftwf_complex ) * nPointFFT );
			cout<<"malloc.c worked";
			// Compute FFT
			plan_forward = fftwf_plan_dft_1d ( nPointFFT, inputRawData, outputFFT, FFTW_FORWARD, FFTW_ESTIMATE );
			fftwf_execute ( plan_forward );
			
			if(debug_switch) cout << "Magnitude:"<< startFreq << "-" << endFreq << "-" << bandGap << endl;
			magnitudeFFT = (float*)malloc(sizeof(float)* ( endFreq - startFreq + 1 )/bandGap);
			
			for(int index=startFreq,out_index = 0; index <= endFreq; index=index+bandGap,out_index = out_index + 1)
	  		{
	  			magnitudeFFT[out_index] = sqrt(pow((float)outputFFT[index][0],2) + pow((float)outputFFT[index][1],2));
			}
			
			//Clearing Allocated Memories
			fftwf_destroy_plan ( plan_forward );
			fftwf_free ( inputRawData );
			fftwf_free ( outputFFT );
			
			//return FFT Power Spectrum
			return magnitudeFFT;
		}

	}
};
