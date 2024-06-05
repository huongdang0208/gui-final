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
#define BROKER "broker.emqx.io"
#define PORT 1883
#define SUB "hub/control/app/8xff"

#define LED_ON LV_IMGBTN_STATE_RELEASED
#define LED_OFF LV_IMGBTN_STATE_CHECKED_RELEASED
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
    } type;
    const Buffer buffer;
    const std::string topic;

    Event(EventType t, const std::string p, Buffer d) : type(t), topic(p), buffer(d) {}
};

void sync_devices(Buffer buffer);
void sync_timer(Buffer buffer);
void sync_status(Buffer buffer);

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define LOG_INFO(...) std::cout << ANSI_COLOR_GREEN << "[INFO] " << ANSI_COLOR_RESET << __VA_ARGS__ << std::endl
#define LOG_ERR(...) std::cout << ANSI_COLOR_RED << "[ERR] " << ANSI_COLOR_RESET << __VA_ARGS__ << std::endl