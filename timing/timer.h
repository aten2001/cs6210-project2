#include <sys/time.h>
#include <time.h>
#include <math.h>

#define DEBUG 0

unsigned long long get_timer_nsec(struct timespec *timer)
{
	return timer->tv_sec * 1E6 + timer->tv_nsec / 1000;
}

long get_timer_diff(struct timespec *before, struct timespec *after)
{
	return get_timer_nsec(after) - get_timer_nsec(before);
}

void start_watch(struct timespec *timer)
{
	if (clock_gettime(CLOCK_MONOTONIC, timer))
		fprintf(stderr, "startWatch failed\n");

#if DEBUG
	printf("time val is %llu\n", get_timer_nsec(timer));
#endif
}

void stop_watch(struct timespec *timer)
{
	return start_watch(timer);
}

double get_stddev(long *results, int num, double m)
{
	double sum2 = 0;
	for (int i = 0; i < num; i++) {
		double diff = ((double)results[i]) - m;
		sum2 += diff * diff;
	}

	double variance = sum2 / (num - 1);

	return sqrt(variance);
}

double get_mean(long *results, int num)
{
	long sum = 0;
	for (int i = 0; i < num; i++)
		sum += results[i];

	return ((double)sum) / num;
}
