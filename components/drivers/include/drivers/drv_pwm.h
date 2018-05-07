#ifndef __DRV_PWM_H_INCLUDE__
#define __DRV_PWM_H_INCLUDE__

struct rt_pwm_configuration
{
	rt_uint32_t mode; /* 0: off, 1: cycle. 2: nano second. */
	rt_uint32_t period; /* 1hz~4Ghz or 1ns~4.29s:0.23hz~1Ghz  */
	rt_uint32_t pulse;  /* max: 4Ghz */
};

struct rt_pwm_ops
{
    rt_err_t (*configure)(struct rt_device *device, int channel, struct rt_pwm_configuration *configuration);
};

struct rt_device_pwm
{
    struct rt_device parent;
    const struct rt_pwm_ops *ops;
};

#endif /* __DRV_PWM_H_INCLUDE__ */
