#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>

#define PWM_CHANNEL_MAX		(8) /* 0-7 */

static rt_err_t set(struct rt_device *device, int channel, struct rt_pwm_configuration *configuration)
{	
	rt_err_t result = RT_EOK;

	if(channel > (PWM_CHANNEL_MAX - 1))
	{
		result = -RT_EIO;
		goto _exit;
	}

	rt_kprintf("drv_pwm.c set channel: %d, period: %d, pulse: %d\n", channel, configuration->period, configuration->pulse); 

_exit:
	return result;
}

static rt_err_t get(struct rt_device *device, int channel, struct rt_pwm_configuration *configuration)
{
	return -RT_EIO;
}

static const struct rt_pwm_ops pwm_ops =
{
	set,
	get,
};


int rt_hw_pwm_init(void)
{
    int ret = RT_EOK;
	
	/* add pwm initial. */ 

    ret = rt_device_pwm_create("pwm", &pwm_ops, RT_NULL);

    return ret;
}
INIT_DEVICE_EXPORT(rt_hw_pwm_init);
