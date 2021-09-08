#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/prctl.h>
#include <sys/time.h>

#define THREAD_INTERVAL_MS 2
#define MAIN_INTERVAL_MS 1
struct  thread_ctrl_st
{
	uint8_t		run_flag;
	pthread_t	th_id;
	FILE*		fp;
};

static struct thread_ctrl_st th_ctrl = {0};
volatile uint64_t count;

void* sample_thread(void* args)
{
	while (th_ctrl.run_flag) {
		count ++;
		fprintf (th_ctrl.fp, "Thread: %ld\n", count);
		sleep(THREAD_INTERVAL_MS);
	}
}

void signal_exit_handler(int sig)
{
	fprintf(th_ctrl.fp, "Signal Exit!\n");
	fclose(th_ctrl.fp);
	exit(0);
}

int main()
{
	signal(SIGTERM, signal_exit_handler);
	signal(SIGINT, signal_exit_handler);

	th_ctrl.fp = fopen ("test.log", "wt");
	th_ctrl.run_flag = 1;
	int err = pthread_create(&th_ctrl.th_id, NULL, sample_thread, (void*) NULL);
	if (err) {
		printf("Error,[%s]\n", strerror(err));
	}
	fprintf(th_ctrl.fp, "pthread_create!\n");

	while (th_ctrl.run_flag) {
		count ++;
		fprintf (th_ctrl.fp, "Main: %ld\n", count);
		sleep(MAIN_INTERVAL_MS);
		if (count >= 10000) goto MAIN_EXIT;
	}

MAIN_EXIT:
	fprintf(th_ctrl.fp, "Main Exit!\n");
	return 0;
}