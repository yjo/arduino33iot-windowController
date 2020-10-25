#define _stringify(e) #e
#define stringify(e) _stringify(e)
#define assert(e)    ((e) ? (void)0 : ({Serial.println("!(" #e ") @" stringify(__LINE__)); while(1);}))

#define isXAfterY_wrapped(x, y) (((int32_t) (x)) - ((int32_t) (y)) >= 0)
#define soonest_wrapped(x, y) (isXAfterY_wrapped(x, y) ? (y) : (x))
#define latest_wrapped(x, y) (isXAfterY_wrapped(x, y) ? (x) : (y))

#define hasIntervalElapsed_us(interval_us) ({ static uint32_t lastRun_us = now_us; (isXAfterY_wrapped(now_us, lastRun_us + (interval_us)) ? ({lastRun_us = now_us; true;}) : false); })
#define hasIntervalElapsed_ms(interval_ms) hasIntervalElapsed_us(((uint32_t) (interval_ms)) * 1000ul)

