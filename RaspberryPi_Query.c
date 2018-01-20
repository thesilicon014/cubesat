#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdint.h>

FILE *fp;
int fp2;

void intHandler(int dummy) {
    fclose(fp);
    exit(1);
}

int main (){

	signal(SIGINT, intHandler);
	char path[100];
	getcwd(path, 100);
	char time_s[100];
	time_t rawtime;
 	struct tm * timeinfo;
 	time(&rawtime);
 	timeinfo = localtime(&rawtime);
 	strftime(time_s, 100, "/%a:%T.csv", timeinfo);
	strcat(path, time_s);

	fp = fopen(path,"w+");

	struct timespec tim, tim2;
    	tim.tv_sec = 0;
    	tim.tv_nsec = 100000000;

    	double elapsed_time;
    	struct timespec tw1, tw2;

	fp2 = serialOpen("/dev/ttyS0", 9600);
	wiringPiSetup();
	clock_gettime(CLOCK_MONOTONIC, &tw1);
	int i;
	int j;
	char string[100];

	serialFlush(fp2);

	while(1){

		serialPutchar(fp2, 'Q');
		delay(300);
		clock_gettime(CLOCK_MONOTONIC, &tw2);
		elapsed_time = 1000.0*tw2.tv_sec + 1e-6*tw2.tv_nsec - (1000.0*tw1.tv_sec + 1e-6*tw1.tv_nsec);
		fprintf(fp, "%f,", elapsed_time);
		int first = 1;
		j = 0;
		while(serialDataAvail(fp2)){
			char c = serialGetchar(fp2);
			if(c!= 0){
				if(first){
					first = 0;
				}else{
					string[j] = c;
					j++;
				}
			}
		}
		fprintf(fp, "%s\n", string);

	}
	return 0;
}
