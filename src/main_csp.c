#include <csp/csp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


static bool ping_run = true;
static uint16_t ping_count = 0;
static uint16_t ping_ok_count = 0;
static uint32_t ping_delay_sum = 0;

__attribute__((__unused__))
static void * ping_task(void * param) {

    printf("Ping task running\n");

    while (ping_run) {
    	int res = csp_ping(10, 1000, 1, CSP_O_CRC32);
        ping_count++;
        if (res >= 0) {
            ping_ok_count++;

            ping_delay_sum += res;
        }

		usleep(1000000);
    }

    return 0;
}

int libmain(int argc, char ** argv) {

    printf("Libmain is creating ping task\n");

	ping_run = true;
	pthread_t ping_handle;
	pthread_create(&ping_handle, NULL, ping_task, NULL);

    return 0;

}

void libinfo() {

    printf("Addin with CSP ping task.\n");
    printf("  Pings: %u  Ok: %u  Delay: %.2f\n", ping_count, ping_ok_count, ping_ok_count ? ((double)ping_delay_sum)/ping_ok_count : -1.0);

}