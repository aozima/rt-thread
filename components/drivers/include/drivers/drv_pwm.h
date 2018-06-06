#ifndef __DRV_PWM_H_INCLUDE__
#define __DRV_PWM_H_INCLUDE__

#define PWM_CMD_ENABLE      (128 + 0)
#define PWM_CMD_SET         (128 + 1)
#define PWM_CMD_GET         (128 + 2)
//#define ....

struct rt_pwm_configuration
{
    rt_uint32_t channel; /* 0-n */
    rt_uint32_t period; /* unit:ns 1ns~4.29s:1Ghz~0.23hz */
    rt_uint32_t pulse;  /* unit:ns (pulse<=period) */
};

struct rt_device_pwm;
struct rt_pwm_ops
{
    rt_err_t (*control)(struct rt_device_pwm *device, int cmd, void *arg);
};

struct rt_device_pwm
{
    struct rt_device parent;
    const struct rt_pwm_ops *ops;
};

extern rt_err_t rt_device_pwm_register(struct rt_device_pwm *device, const char *name, const struct rt_pwm_ops *ops, const void *user_data);

#endif /* __DRV_PWM_H_INCLUDE__ */
