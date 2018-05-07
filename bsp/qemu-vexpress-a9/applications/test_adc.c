#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <rtthread.h>
#include <rtdevice.h>


rt_err_t test_adc(int argc, char** argv)
{
	rt_err_t result = RT_EOK;
	int value;
	rt_device_t dev;
	int channel;
	
	dev = rt_device_find(argv[1]);
	if(!dev)
	{
		rt_kprintf("%s not found!\n", argv[1]);
		result = -RT_ERROR;
		goto _exit;
	}
	
	result = rt_device_open(dev, RT_DEVICE_FLAG_RDONLY);
	if(result != RT_EOK)
	{
		rt_kprintf("open %s faild! \n", argv[1]);
		result = -RT_EIO;
		goto _exit;
	}
	
	rt_kprintf("test adc: %s\n", argv[1]);
	for(channel=0; channel<9; channel++)
	{
		if( rt_device_read(dev, channel, &value, sizeof(value)) != sizeof(value) )
		{
			rt_kprintf("channel %d: faild! \n", channel);
			result = -RT_ERROR;
			goto _exit;
		}
		rt_kprintf("channel %d: 0x%08X \n", channel, value);
	}

_exit:
	return result;
}

#include <finsh.h>

MSH_CMD_EXPORT(test_adc, test adc);
