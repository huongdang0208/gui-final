#include <hub_screen.h>

lv_obj_t* leds[] = {
    guider_ui.screen_imgbtn_8,
    guider_ui.screen_imgbtn_9,
    guider_ui.screen_imgbtn_10,
    guider_ui.screen_imgbtn_11,
    guider_ui.screen_imgbtn_12,
    guider_ui.screen_imgbtn_13,
};

lv_obj_t* led_names[] = {
    guider_ui.screen_label_17,
    guider_ui.screen_label_18,
    guider_ui.screen_label_19,
    guider_ui.screen_label_20,
    guider_ui.screen_label_21,
    guider_ui.screen_label_22,
};

lv_obj_t* sw_names[] = {
    guider_ui.screen_label_28,
    guider_ui.screen_label_29,
    guider_ui.screen_label_30,
    guider_ui.screen_label_31,
    guider_ui.screen_label_32,
    guider_ui.screen_label_33
};

lv_obj_t* sws[] = {
    guider_ui.screen_sw_3,
    guider_ui.screen_sw_4,
    guider_ui.screen_sw_5,
    guider_ui.screen_sw_6,
    guider_ui.screen_sw_7,
    guider_ui.screen_sw_8
};

lv_obj_t* led_conts[] = {
    guider_ui.screen_cont_41,
    guider_ui.screen_cont_42,
    guider_ui.screen_cont_43,
    guider_ui.screen_cont_44,
    guider_ui.screen_cont_45,
    guider_ui.screen_cont_46
};

lv_obj_t* sw_conts[] = {
    guider_ui.screen_cont_41,
    guider_ui.screen_cont_42,
    guider_ui.screen_cont_43,
    guider_ui.screen_cont_44,
    guider_ui.screen_cont_45,
    guider_ui.screen_cont_46
};

static int timer_index = 0;
lv_obj_t* timers[] = {
    guider_ui.screen_label_39,
    guider_ui.screen_label_40,
    guider_ui.screen_label_41,
    guider_ui.screen_label_42,
    guider_ui.screen_label_43,
    guider_ui.screen_label_44,
    guider_ui.screen_label_45,
    guider_ui.screen_label_46
};

lv_obj_t* timer_conts[] = {
    guider_ui.screen_cont_53,
    guider_ui.screen_cont_54,
    guider_ui.screen_cont_55,
    guider_ui.screen_cont_56,
    guider_ui.screen_cont_57,
    guider_ui.screen_cont_58,
    guider_ui.screen_cont_59,
    guider_ui.screen_cont_60
};

Buffer *bufferManager = Buffer::New();


void set_led_output(lv_obj_t * obj , lv_imgbtn_state_t state) {
    lv_imgbtn_set_state(obj, state);
}

void set_device_name(lv_obj_t * obj , const char* name) {
    lv_label_set_text(obj, name);
}

void show_device(lv_obj_t * obj) {
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

void hide_device(lv_obj_t * obj) {
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

void set_sw_output(lv_obj_t * obj , uint32_t state) {
    if (state) {
        lv_obj_add_state(obj, LV_STATE_CHECKED);
    }else {
        lv_obj_clear_state(obj, LV_STATE_CHECKED);    
    }
}

void set_timer(lv_obj_t * obj, const char* time){
    lv_label_set_text(obj, time);
}

void sync_devices(Buffer buffer){
    LOG_INFO("sync_devices");
    for (int i = 0; i < buffer.led_size(); i++) {
        LOG_INFO("sync_devices");
        const Led_t& led = buffer.led(i);
        LOG_INFO("sync_devices");
        // save new led to bufferManager
        Led_t *new_led = bufferManager->add_led();
        LOG_INFO("sync_devices");
        new_led->set_name(led.name());
        LOG_INFO("sync_devices");
        // show in screen
        show_device(led_conts[i]);
        LOG_INFO("sync_devices");
        set_device_name(led_names[i], led.name().c_str());
        LOG_INFO("sync_devices");
        if(led.status()){
            set_led_output(leds[i], LED_ON);   
        }
        else {
            set_led_output(leds[i], LED_OFF);
        }   
    }
    for (int i = 0; i < buffer.sw_size(); i++) {
        const Sw_t& sw = buffer.sw(i);

        Sw_t *new_sw = bufferManager->add_sw();
        new_sw->set_name(sw.name());

        show_device(sw_conts[i]);
        set_device_name(sw_names[i], sw.name().c_str());
        if(sw.status()){
            set_sw_output(sws[i], SW_ON);   
        }
        else {
            set_sw_output(sws[i], SW_OFF);
        }   
    }
}

void sync_timer(Buffer buffer) {
    const Timer_t& time = buffer.time();
    time.day();
    std::string result = std::to_string(time.minute()) + ":" + std::to_string(time.hour()) + "-" + std::to_string(time.day()) + std::to_string(time.hour());

    set_timer(timers[timer_index++], result.c_str());
}

void sync_status(Buffer buffer) {
    // vì màn hình chỉ có 8 led và 6 switch thôi 
    // chơi bẩn thỉu tạo ra sẵn led và switch xong ẩn đi 
    // nào cần dùng thì show ra còn không cần thì ẩn đi 
    // nhưng ví dụ nếu ẩn led 1 thì nó không thu các led kia về vị trí đầu 
    // nên cần dev thêm hàm chuyển dịch từ led 2 thành  led 1 ... nếu led 1 bị ẩn 

    for (int i = 0; i < buffer.led_size(); i++) {
        const Led_t& led = buffer.led(i);

        for(int j =0; j < bufferManager->led_size(); j++){
            if(led.name() == bufferManager->led(j).name()){
                if(led.status()){
                    set_led_output(leds[j], LED_ON);   
                }
                else {
                    set_led_output(leds[j], LED_OFF);
                }  
            }
        }
    }

    for (int i = 0; i < buffer.sw_size(); i++) {
        const Sw_t& sw = buffer.sw(i);

        for(int j =0; j < bufferManager->sw_size(); j++){
            if(sw.name() == bufferManager->sw(j).name()){
                if(sw.status()){
                    set_sw_output(sws[i], SW_ON);   
                }
                else {
                    set_sw_output(sws[i], SW_OFF);
                }   
            }
        }
    }
}