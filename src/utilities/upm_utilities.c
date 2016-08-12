#include <upm_utilities.h>

void* upm_malloc(int mem_map, int size){
    void *mem;
#if defined(linux)
    mem = malloc(size);
    if(mem == NULL){
        printf("unable to allocate memory");
    }
    else{
        printf("memory allocated successfully\n");
    }
#elif defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
    kmemory_map_t map_name = (kmemory_map_t) mem_map;
    if(task_mem_map_alloc(map_name, &mem, TICKS_UNLIMITED) == RC_OK){
        printf("memory allocated successfully\n");
    }
    else{
        printf("unable to allocate memory");
        mem = NULL;
    }
#endif
    return mem;
}

void upm_free(int mem_map, void* ptr){
#if defined(linux)
    free(ptr);
#elif defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
    kmemory_map_t map_name = (kmemory_map_t) mem_map;
    task_mem_map_free(map_name, &ptr);
#endif
}

void upm_delay(int time){
#if defined(linux)
    sleep(time);
#elif defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, SECONDS(time));
    nano_timer_test(&timer, TICKS_UNLIMITED);
#endif
}

void upm_delay_ms(int time){
#if defined(linux)
    usleep(1000 * time);
#elif defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, MSEC(time));
    nano_timer_test(&timer, TICKS_UNLIMITED);
#endif
}

void upm_delay_us(int time){
#if defined(linux)
    usleep(time);
#elif defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);
    nano_timer_start(&timer, USEC(time));
    nano_timer_test(&timer, TICKS_UNLIMITED);
#endif
}
