#include <picoos.h>
#include <avr/sleep.h>

/* Tell the OS where are we gonna start */
extern void app_main(void *arg);
static void local_main(void *arg);
static POSIDLEFUNC_t old_idle;

/* Program main function.
 * This function starts pico]OS by initializing the pico layer.
 */
int main(void)
{
  /* initialize pico]OS (pico layer only) */
  posInit(local_main, /* ptr to function: first task that is executed */
          NULL,      /* optional argument, not used here             */
          1);        /* priority of the first task                   */

  /* Note: The call to posInit() will never return */

  return 0;
}


/**
 * This is our idle task.
 * We just sleep here to save power!
 **/
void arch_idle(void)
{
	sleep_mode();

	/* Invoke old hooks if any! */
	if(old_idle != NULL)
		old_idle();
}

/**
 * Initialization routine specific to architecture
 **/
static void arch_init(void)
{
	/* Initialize idle task */
	set_sleep_mode(SLEEP_MODE_IDLE);
	old_idle = posInstallIdleTaskHook(arch_idle);
}

/**
 * Local main function which calls the application Main
 * app_main will be the entry point to application and
 * must return.
 **/
static void local_main(void * arg)
{
	arch_init();
	app_main(arg);
}
