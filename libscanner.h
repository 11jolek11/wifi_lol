/*
 *  This example makes passive scan of all the networks on interface.
 *  Passive scanning may take a while and your link may be unusable for other programs during the scan.
 *  For the above reason you should not call it too often (e.g. once a second) unless you don't mind side effects.
 * 
 *  Triggering a scan needs persmission. The program will fail with operation not supported error message if you don't have them.
 *  You can use sudo for testing.
 * 
 *  Program expects wireless interface as argument, e.g:
 *  wifi-scan-all wlan0
 * 
 */
 
#include "wifi_scan.h"
#include <stdio.h>
#include <unistd.h>

//convert bssid to printable hardware mac address
const char *bssid_to_string(const uint8_t bssid[BSSID_LENGTH], char bssid_string[BSSID_STRING_LENGTH]);
const int BSS_INFOS=10; //the maximum amounts of APs (Access Points) we want to store

const char* scan(const char *device);

