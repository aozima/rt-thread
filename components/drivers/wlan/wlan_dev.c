/*
 *  RT-Thread Wi-Fi Device
 *
 * COPYRIGHT (C) 2014 - 2015, Shanghai Real-Thread Technology Co., Ltd
 *
 *  This file is part of RT-Thread (http://www.rt-thread.org)
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2014-09-11     Bernard      the first verion
 */

#include <rtthread.h>
#include <rtdevice.h>

#include "wlan_dev.h"
#include "wlan_cmd.h"

#define NIOCTL_SADDR    0x02

void rt_wlan_info_init(struct rt_wlan_info *info, rt_wlan_mode_t mode, rt_wlan_security_t security,
                       char *ssid)
{
    if (info == RT_NULL) return ;

    memset(info, 0x0, sizeof(struct rt_wlan_info));
    info->mode = mode;
    info->security = security;
    if (ssid)
    {
        info->ssid = rt_malloc(strlen((char *)ssid) + 1);
        if (info->ssid)
        {
            strncpy((char *)info->ssid, (char *)ssid, strlen((char *)ssid) + 1);
        }
    }
}

void rt_wlan_info_deinit(struct rt_wlan_info *info)
{
    if (info->ssid)
    {
        rt_free(info->ssid);
        info->ssid = RT_NULL;
    }

    memset(info, 0x0, sizeof(struct rt_wlan_info));
}

int rt_wlan_init(struct rt_wlan_device *device, rt_wlan_mode_t mode)
{
    int result;

    if (device == RT_NULL) return 0;

    result = rt_device_control(RT_DEVICE(device), WIFI_INIT, (void *)&mode);

    return result;
}

int rt_wlan_connect(struct rt_wlan_device *device, struct rt_wlan_info *info, char *password)
{
    int result = 0;

    if (device == RT_NULL) return -RT_EIO;

    if (info != RT_NULL)
    {
        rt_wlan_set_info(device, info);
    }

    rt_strncpy((char *)device->key, (char *)password, sizeof(device->key) - 1);

    result = rt_device_control(RT_DEVICE(device), WIFI_EASYJOIN, (void *)password);

    return result;
}

int rt_wlan_softap(struct rt_wlan_device *device, struct rt_wlan_info *info, char *password)
{
    int result = RT_EOK;

    if (device == RT_NULL) return -RT_EIO;

    if (info != RT_NULL)
    {
        rt_wlan_set_info(device, info);
    }

    rt_strncpy((char *)device->key, (char *)password, sizeof(device->key) - 1);

    result = rt_device_control(RT_DEVICE(device), WIFI_SOFTAP, (void *)password);

    return result;
}

int rt_wlan_disconnect(struct rt_wlan_device *device)
{
    int result = 0;

    if (device == RT_NULL) return -RT_EIO;

    /* save event handler */
    result = rt_device_control(RT_DEVICE(device), WIFI_DISCONNECT, RT_NULL);

    return result;
}

int rt_wlan_set_info(struct rt_wlan_device *device, struct rt_wlan_info *info)
{
    if (device->info == info) return RT_EOK; /* same info */

    if (device->info != RT_NULL)
    {
        rt_wlan_info_deinit(device->info);
        rt_free(device->info);
    }

    device->info = info;

    return RT_EOK;
}

struct rt_wlan_info *rt_wlan_get_info(struct rt_wlan_device *device)
{
    struct rt_wlan_info *info = RT_NULL;

    if (device != RT_NULL)
    {
        info = device->info;
    }

    return info;
}

int rt_wlan_scan(struct rt_wlan_device *device)
{
    int result;

    result = rt_device_control(RT_DEVICE(device), WIFI_SCAN, RT_NULL);

    return result;
}

int rt_wlan_get_rssi(struct rt_wlan_device *device)
{
    int rssi;
    int result;

    if (device == RT_NULL) return 0;
    result = rt_device_control(RT_DEVICE(device), WIFI_GET_RSSI, (void *)&rssi);

    if (result == RT_EOK) return rssi;

    return result;
}

int rt_wlan_get_mac(struct rt_wlan_device *device, rt_uint8_t hwaddr[6])
{
    int result;
    if (device == RT_NULL) return 0;
    result = rt_device_control(RT_DEVICE(device), NIOCTL_GADDR, (void *)hwaddr);
    return result;
}

int rt_wlan_set_mac(struct rt_wlan_device *device, rt_uint8_t hwaddr[6])
{
    int result;
    if (device == RT_NULL) return 0;
    result = rt_device_control(RT_DEVICE(device), NIOCTL_SADDR, (void *)hwaddr);
    return result;
}

int rt_wlan_enter_powersave(struct rt_wlan_device *device, int level)
{
    int result = 0;

    if (device == RT_NULL) return -RT_EIO;

    result = rt_device_control(RT_DEVICE(device), WIFI_ENTER_POWERSAVE, (void *)&level);

    return result;
}

void rt_wlan_set_event_callback(struct rt_wlan_device *device, rt_wlan_event_t event,
                                rt_wlan_event_handler handler)
{
    if (device == RT_NULL) return ;

    device->handler[event] = handler;
}

int rt_wlan_cfg_monitor(struct rt_wlan_device *device, rt_wlan_monitor_opition_t opition)
{
    int result = 0;

    if (device == RT_NULL) return -RT_EIO;

    result = rt_device_control(RT_DEVICE(device), WIFI_CFG_MONITOR, (void *)&opition);

    return result;
}

int rt_wlan_register_monitor(struct rt_wlan_device *device, rt_wlan_monitor_cb_t callback)
{
    int result = 0;

    if (device == RT_NULL) return -RT_EIO;

    result = rt_device_control(RT_DEVICE(device), WIFI_SET_MONITOR_CALLBACK, (void *)&callback);

    return result;
}

int rt_wlan_set_channel(struct rt_wlan_device *device, int channel)
{
    int result = 0;

    if (device == RT_NULL) return -RT_EIO;

    result = rt_device_control(RT_DEVICE(device), WIFI_SET_CHANNEL, (void *)&channel);

    return result;
}