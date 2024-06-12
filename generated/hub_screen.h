#pragma once
#include <iostream>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <gui_guider.h>
#include <typedef.pb.h>
#include <mqtt.h>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string.h>
#include <string>

#define DISP_BUF_SIZE (256 * 1024)
#define DEVICE_NAME "hub-screen"
#define BROKER "3.26.238.229"
#define PORT 1883
#define SUB "hub/server/8xff"

#define LED_ON 1
#define LED_OFF 0
#define SW_ON 1
#define SW_OFF 0



class HubScreen_t {
public:
    lv_style_t  style;
    lv_ui guider_ui;
    Mqtt_t transport;
    Buffer buffer;

    HubScreen_t(const char *id) : transport(id) {
        this->buffer = Buffer();
    }
};


struct Event {
    enum EventType { 
        ErrFormat, 
        ErrParse,
        SyncDeivce,
        StatusDeivce,
        ControlDevice,    
        SyncTimer,  
        AddDevice,
        RemoveDevice
    } type;
    const Buffer buffer;
    const std::string topic;

    Event(EventType t, const std::string p, Buffer d) : type(t), topic(p), buffer(d) {}
};

void sync_devices(Buffer buffer);
void sync_timer(Buffer buffer);
void sync_status(Buffer buffer);
void add_device(Buffer buffer);
void remove_device(Buffer buffer);
void set_sw_output(uint8_t index, uint32_t state);
void events_init_hubscreen(lv_ui *ui);

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define LOG_INFO(...) std::cout << ANSI_COLOR_GREEN << "[INFO] " << ANSI_COLOR_RESET << __VA_ARGS__ << std::endl
#define LOG_ERR(...) std::cout << ANSI_COLOR_RED << "[ERR] " << ANSI_COLOR_RESET << __VA_ARGS__ << std::endl