// 程序运行3秒以后，每隔1秒打印当前时间。同时发送指定的信号。
/* 1.CLOCK_REALTIME:  Systemwide realtime clock.
 * 一个可设置的系统范围内的实时时钟，计算从1970年1月1日午夜0点起的毫秒数 */
/* 2.CLOCK_MONOTONIC: Represents monotonic time. Cannot be set.
 * 一个不可设置的单调增加时钟，它测度系统启动后某个非特定时间点起的时间 */
/* 3.CLOCK_PROCESS_CPUTIME_ID: High resolution per-process timer.
 * 这个时钟测量调用进程（包含其中所有线程）所消耗的 (用户和系统) CPU 时间 */
/* 4.CLOCK_THREAD_CPUTIME_ID Thread-specific timer.
 * 这个时钟测量调用线程所消耗的 (用户和系统) CPU 时间*/
/* 5.CLOCK_REALTIME_HR: High resolution version of CLOCK_REALTIME. */
/* 6.CLOCK_MONOTONIC_HR: High resolution version of CLOCK_MONOTONIC. */

//int timer_create(clockid_t clock_id, struct sigevent *evp, timer_t *timerid); /* 创建定时器 */
//int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value,struct itimerspec *old_value); /* 初始化定时器 */
//int timer_gettime(timer_t timerid,struct itimerspec *value); /* 获得一个活动定时器的剩余时间 */
//int timer_delete(timer_t timerid); /* 删除定时器 */



#include <signal.h>
#include <time.h>
#include <stdio.h>

void handle()
{
	time_t t;
	char p[32];
	time(&t);

	strftime(p, sizeof(p), "%T", localtime(&t));
	printf("time is %s\n", p);
}

int main()
{
	struct sigevent evp;
	struct itimerspec ts;
	timer_t timer;
	int ret;
	
	evp.sigev_value.sival_ptr = &timer;
	evp.sigev_notify = SIGEV_SIGNAL;
	evp.sigev_signo = SIGUSR1;
	signal(SIGUSR1, handle);
	ret = timer_create(CLOCK_REALTIME, &evp, &timer);
	if(ret)
	    perror("timer_create");
	//间隔时间
	ts.it_interval.tv_sec = 1;
	ts.it_interval.tv_nsec = 0;
	//延迟时间
	ts.it_value.tv_sec = 3;
	ts.it_value.tv_nsec = 0;
	ret = timer_settime(timer, 0, &ts, NULL);
	if( ret )
	    perror("timer_settime");
	for(;;);
}
