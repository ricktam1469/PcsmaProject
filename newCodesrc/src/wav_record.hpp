#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
using namespace std; 

class wav_record
{
	private:

	struct timeval start;
	char cmd_arec_1[200],cmd_arec_complete[200]; 
	char cmd_compress[200],cmd_compress_complete[200]; 
	char cmd_sshpass_1[200],cmd_sshpass_2[200],cmd_sshpass_complete[200];
	char cmd_remove_1[200],cmd_remove_complete[200];
	char date[50];
        char time_str[50];
        char wavFileName[200];
        int index;
	time_t t;
	
	public:
	
	wav_record()
	{
	memset(cmd_arec_1,0x00,200);
	memset(cmd_arec_complete,0x00,200);
	memset(cmd_compress,0x00,200);
	memset(cmd_compress_complete,0x00,200);
	memset(cmd_sshpass_1,0x00,200);
	memset(cmd_sshpass_2,0x00,200);
	memset(cmd_sshpass_complete,0x00,200);
	memset(cmd_remove_1,0x00,200);
	memset(cmd_remove_complete,0x00,200);
	memset(wavFileName,0x00,200);
	}	
	
	char* wavFileNameGen(char *board_number)
	{	
		struct tm *timeptr=localtime(&t);
		gettimeofday(&start, NULL);
		time(&t);
		timeptr=localtime(&t);	
		gettimeofday(&start, NULL);
		time(&t);
		timeptr=localtime(&t);
		sprintf(date,"%04d_%02d_%02d",timeptr->tm_year+1900, timeptr->tm_mon +1,timeptr->tm_mday);
		sprintf(time_str,"%02d_%02d_%02d",timeptr->tm_hour, timeptr->tm_min,timeptr->tm_sec);
		sprintf(wavFileName,"%s_%s_%s.wav",date,time_str,board_number);
		return wavFileName;
	}

	bool recordWavFile(char *wav_Name)
	{
		strcpy(cmd_arec_1, "arecord -f S16_LE -c 1 -r 44100 -d 2 -D hw:1,0 ");		
		sprintf(cmd_arec_complete,"%s%s",cmd_arec_1,wav_Name);
		system(cmd_arec_complete);
		return true;
	}	
	
	bool compressWavFile(char *wav_Name)
	{	
		strcpy(cmd_compress,"bzip2 ");
		sprintf(cmd_compress_complete,"%s%s",cmd_compress,wav_Name); 
		system(cmd_compress_complete);
		return true;
	}

	bool transferToFDB(char *wav_Name)
	{	
		strcpy(cmd_sshpass_1, "sshpass -p 'ubuntu123' scp");
		strcpy(cmd_sshpass_2, " ubuntu@192.168.1.73:/home/ubuntu/UserDrive/Sensor_Database/Board_06");
		sprintf(cmd_sshpass_complete,"%s%s.bz2%s",cmd_sshpass_1,wav_Name,cmd_sshpass_2);
		system(cmd_sshpass_complete);
		return true;
	}

	bool removeWavFile(char *wav_Name)
	{	
		strcpy(cmd_remove_1,"rm /var/tmp/");
		sprintf(cmd_remove_complete,"%s%s.bz2",cmd_remove_1,wav_Name);
		system(cmd_remove_complete);
		return true;
	}
};
