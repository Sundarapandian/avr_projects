#ifndef _ACSPEED_H
#define _ACSPEED_H

#define INPUT_HZ             50 /* 50 HZ A/C voltage */
#define ACSPEED_MAX_RANGE   ((1000 / (INPUT_HZ)) / 2)

#ifdef __cplusplus
extern "C" {
#endif

void acspeed_start(void);
void acspeed_stop(void);
void acspeed_init(void (*) (int));
void acspeed_set(unsigned char speed);

#ifdef __cplusplus
}
#endif

#endif /* _ACSPEED_H */
