#include "wifi_scan.h"
#include <stdio.h>  //printf
#include <unistd.h> //sl
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 2048

//convert bssid to printable hardware mac address
const char *bssid_to_string(const uint8_t bssid[BSSID_LENGTH], char bssid_string[BSSID_STRING_LENGTH])
{
	snprintf(bssid_string, BSSID_STRING_LENGTH, "%02x:%02x:%02x:%02x:%02x:%02x",
         bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
	return bssid_string;
}

const int BSS_INFOS=10; //the maximum amounts of APs (Access Points) we want to store

int main(int argc, char **argv)
{
	int pipe_fd;
	mkfifo("/tmp/data_x",0666);
	struct wifi_scan *wifi=NULL;    //this stores all the library information
	struct bss_info bss[BSS_INFOS]; //this is where we are going to keep informatoin about APs (Access Points)
	char mac[BSSID_STRING_LENGTH];  //a placeholder where we convert BSSID to printable hardware mac address
	int status, i;

	if(argc != 2)
	{
		return 0;
	}

	// initialize the library with network interface argv[1] (e.g. wlan0)
	wifi=wifi_scan_init(argv[1]);

	int j = 0;
	while(1)
	{
	char buffer[BUFFSIZE] = {'\0'};
		if ( (pipe_fd = open("/tmp/data_x", O_WRONLY)) < 0) {
			printf("Pipe error\n");
			continue;
		}
		status=wifi_scan_all(wifi, bss, BSS_INFOS);
		
		//it may happen that device is unreachable (e.g. the device works in such way that it doesn't respond while scanning)
		//you may test for errno==EBUSY here and make a retry after a while, this is how my hardware works for example
		if(status<0)
			perror("Unable to get scan data");
		else //wifi_scan_all returns the number of found stations, it may be greater than BSS_INFOS that's why we test for both in the loop
			for(i=0;i<status && i<BSS_INFOS;++i) {	
				char temp[256] = {'\0'};
				sprintf(temp, "%s,%s,%d,%u,%d#",
				   bssid_to_string(bss[i].bssid, mac), 
				   bss[i].ssid,  
				   bss[i].signal_mbm/100, 
				   bss[i].frequency,
				   bss[i].seen_ms_ago 
				);
				strcat(buffer, temp);
			}

		printf("%d \n", j);
		j++;

		if ( write(pipe_fd, buffer, strlen(buffer)) != strlen(buffer)) {
			printf("Write error \n");
		}

		close(pipe_fd);
		sleep(2);
	}
	
	//free the library resources
	wifi_scan_close(wifi);

	return 0;
}

