/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zephyr.h>
#include <mvs0608.h>
#include <malloc.h>
#include <gpio.h>
#include <sys_clock.h>

#define SLEEPTICKS	MSEC(200)
#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

/*
 * @file
 * @brief Hello World demo
 * Nanokernel version of hello world demo
 */


void main(void)
{
	mraa_init();
	struct nano_timer timer;
	void *timer_data[1];
	nano_timer_init(&timer, timer_data);
	mvs0608_context dev = mvs0608_init(2);
	bool abc = 0;
	while(1){
		if(mvs0608_is_colliding(dev, &abc) != UPM_SUCCESS){
			printf("an error has occured\n");
		}
		nano_timer_start(&timer, SLEEPTICKS);
		nano_timer_test(&timer, TICKS_UNLIMITED);
		printf("value retrieved: %d\n", abc);
	}
	mvs0608_close(dev);
}
