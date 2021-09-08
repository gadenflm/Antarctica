#include <stdio.h>
#include "hello_tvt.h"
#include <linux/time.h>

void hello_tvt(void) {
	printf("CLOCK_REALTIME:%d\n", CLOCK_REALTIME);
	printf("CLOCK_MONOTONIC:%d\n", CLOCK_MONOTONIC);
	printf("CLOCK_TAI:%d\n", CLOCK_TAI);


	return;
}