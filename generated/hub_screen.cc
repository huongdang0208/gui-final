#include <hub_screen.h>
#include <unistd.h>

#define SWITCH 0
#define LED 1
#define TIMER 2
#define DEVICE_1 0
#define DEVICE_2 1
#define DEVICE_3 2
#define DEVICE_4 3
#define DEVICE_5 4
#define DEVICE_6 5
#define DEVICE_7 6
#define DEVICE_8 7

static int timer_index = 0;
static int led_index = 0;
static int sw_index = 0;



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
    guider_ui.screen_cont_29,
    guider_ui.screen_cont_30,
    guider_ui.screen_cont_31,
    guider_ui.screen_cont_32,
    guider_ui.screen_cont_33,
    guider_ui.screen_cont_34
};

lv_obj_t* sw_conts[] = {
    guider_ui.screen_cont_41,
    guider_ui.screen_cont_42,
    guider_ui.screen_cont_43,
    guider_ui.screen_cont_44,
    guider_ui.screen_cont_45,
    guider_ui.screen_cont_46
};

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

HubScreen_t hub_screen("DEVICE_NAME");
 
Buffer* bufferManager = new Buffer; 

void set_device_name(uint8_t index, const char* name, uint8_t flag) {
    if(flag == LED) {
        switch (index) {
            case DEVICE_1:
                lv_label_set_text(guider_ui.screen_label_17, name);
                break;
            case DEVICE_2:
                lv_label_set_text(guider_ui.screen_label_18, name);
                break;
            case DEVICE_3:
                lv_label_set_text(guider_ui.screen_label_19, name);
                break;
            case DEVICE_4:
                lv_label_set_text(guider_ui.screen_label_20, name);
                break;
            case DEVICE_5:
                lv_label_set_text(guider_ui.screen_label_21, name);
                break;
            case DEVICE_6:
                lv_label_set_text(guider_ui.screen_label_22, name);
                break;
            default:
                // Xử lý khi index không hợp lệ
                break;
        }
    } else if(flag == SWITCH) {
        switch (index) {
            case DEVICE_1:
                lv_label_set_text(guider_ui.screen_label_28, name);
                break;
            case DEVICE_2:
                lv_label_set_text(guider_ui.screen_label_29, name);
                break;
            case DEVICE_3:
                lv_label_set_text(guider_ui.screen_label_30, name);
                break;
            case DEVICE_4:
                lv_label_set_text(guider_ui.screen_label_31, name);
                break;
            case DEVICE_5:
                lv_label_set_text(guider_ui.screen_label_32, name);
                break;
            case DEVICE_6:
                lv_label_set_text(guider_ui.screen_label_33, name);
                break;
            default:
                // Xử lý khi index không hợp lệ
                break;
        }
    }
}

void show_device(uint8_t index, uint8_t flag) {
    if(flag == LED) {
        switch (index) {
            case DEVICE_1:
                lv_obj_clear_flag(guider_ui.screen_cont_29, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_2:
                lv_obj_clear_flag(guider_ui.screen_cont_30, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_3:
                lv_obj_clear_flag(guider_ui.screen_cont_31, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_4:
                lv_obj_clear_flag(guider_ui.screen_cont_32, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_5:
                lv_obj_clear_flag(guider_ui.screen_cont_33, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_6:
                lv_obj_clear_flag(guider_ui.screen_cont_34, LV_OBJ_FLAG_HIDDEN);
                break;
            default:
                // Xử lý khi index không hợp lệ
                break;
        }
    } else if(flag == SWITCH) {
        switch (index) {
            case DEVICE_1:
                lv_obj_clear_flag(guider_ui.screen_cont_41, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_2:
                lv_obj_clear_flag(guider_ui.screen_cont_42, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_3:
                lv_obj_clear_flag(guider_ui.screen_cont_43, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_4:
                lv_obj_clear_flag(guider_ui.screen_cont_44, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_5:
                lv_obj_clear_flag(guider_ui.screen_cont_45, LV_OBJ_FLAG_HIDDEN);
                break;
            case DEVICE_6:
                lv_obj_clear_flag(guider_ui.screen_cont_46, LV_OBJ_FLAG_HIDDEN);
                break;
            default:
                // Xử lý khi index không hợp lệ
                break;
        }
    }
}
void hide_device(uint8_t index, uint8_t flag) {
    switch (flag) {
        case LED:
            switch (index) {
                case DEVICE_1:
                    lv_obj_add_flag(guider_ui.screen_cont_29, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_2:
                    lv_obj_add_flag(guider_ui.screen_cont_30, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_3:
                    lv_obj_add_flag(guider_ui.screen_cont_31, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_4:
                    lv_obj_add_flag(guider_ui.screen_cont_32, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_5:
                    lv_obj_add_flag(guider_ui.screen_cont_33, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_6:
                    lv_obj_add_flag(guider_ui.screen_cont_34, LV_OBJ_FLAG_HIDDEN);
                    break;
                default:
                    // Xử lý khi index không hợp lệ
                    break;
            }
            break;

        case SWITCH:
            switch (index) {
                case DEVICE_1:
                    lv_obj_add_flag(guider_ui.screen_cont_41, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_2:
                    lv_obj_add_flag(guider_ui.screen_cont_42, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_3:
                    lv_obj_add_flag(guider_ui.screen_cont_43, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_4:
                    lv_obj_add_flag(guider_ui.screen_cont_44, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_5:
                    lv_obj_add_flag(guider_ui.screen_cont_45, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_6:
                    lv_obj_add_flag(guider_ui.screen_cont_46, LV_OBJ_FLAG_HIDDEN);
                    break;
                default:
                    // Xử lý khi index không hợp lệ
                    break;
            }
            break;

        case TIMER:
            switch (index) {
                case DEVICE_1:
                    lv_obj_add_flag(guider_ui.screen_cont_53, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_2:
                    lv_obj_add_flag(guider_ui.screen_cont_54, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_3:
                    lv_obj_add_flag(guider_ui.screen_cont_55, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_4:
                    lv_obj_add_flag(guider_ui.screen_cont_56, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_5:
                    lv_obj_add_flag(guider_ui.screen_cont_57, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_6:
                lv_obj_add_flag(guider_ui.screen_cont_58, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_7:
                    lv_obj_add_flag(guider_ui.screen_cont_59, LV_OBJ_FLAG_HIDDEN);
                    break;
                case DEVICE_8:
                    lv_obj_add_flag(guider_ui.screen_cont_60, LV_OBJ_FLAG_HIDDEN);
                    break;
                default:
                    // Xử lý khi index không hợp lệ
                    break;
            }
            break;

        default:
            // Xử lý khi flag không hợp lệ
            break;
    }
}

void set_sw_output(uint8_t index, uint32_t state) {
    switch (index)
    {
    case DEVICE_1:
        if (state) {
            lv_obj_add_state(guider_ui.screen_sw_3, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_sw_3, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_2:
        if (state) {
            lv_obj_add_state(guider_ui.screen_sw_4, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_sw_4, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_3:
        if (state) {
            lv_obj_add_state(guider_ui.screen_sw_5, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_sw_5, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_4:
        if (state) {
            lv_obj_add_state(guider_ui.screen_sw_6, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_sw_6, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_5:
        if (state) {
            lv_obj_add_state(guider_ui.screen_sw_7, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_sw_7, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_6:
        if (state) {
            lv_obj_add_state(guider_ui.screen_sw_8, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_sw_8, LV_STATE_CHECKED);
        }
        break;

    default:
        // Xử lý khi index không hợp lệ
        break;
    }
}

void set_led_output(uint8_t index, uint32_t state) {
    switch (index)
    {
    case DEVICE_1:
        if (state) {
            lv_obj_add_state(guider_ui.screen_imgbtn_8, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_imgbtn_8, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_2:
        if (state) {
            lv_obj_add_state(guider_ui.screen_imgbtn_9, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_imgbtn_9, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_3:
        if (state) {
            lv_obj_add_state(guider_ui.screen_imgbtn_10, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_imgbtn_10, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_4:
        if (state) {
            lv_obj_add_state(guider_ui.screen_imgbtn_11, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_imgbtn_11, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_5:
        if (state) {
            lv_obj_add_state(guider_ui.screen_imgbtn_12, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_imgbtn_12, LV_STATE_CHECKED);
        }
        break;

    case DEVICE_6:
        if (state) {
            lv_obj_add_state(guider_ui.screen_imgbtn_13, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(guider_ui.screen_imgbtn_13, LV_STATE_CHECKED);
        }
        break;

    default:
        // Xử lý khi index không hợp lệ
        break;
    }
}

void set_timer(uint8_t index, const char* time) {
    switch (index)
    {
    case DEVICE_1:
        lv_label_set_text(guider_ui.screen_label_39, time);
        break;

    case DEVICE_2:
        lv_label_set_text(guider_ui.screen_label_40, time);
        break;

    case DEVICE_3:
        lv_label_set_text(guider_ui.screen_label_41, time);
        break;

    case DEVICE_4:
        lv_label_set_text(guider_ui.screen_label_42, time);
        break;

    case DEVICE_5:
        lv_label_set_text(guider_ui.screen_label_43, time);
        break;

    case DEVICE_6:
        lv_label_set_text(guider_ui.screen_label_44, time);
        break;

    case DEVICE_7:
        lv_label_set_text(guider_ui.screen_label_45, time);
        break;

    case DEVICE_8:
        lv_label_set_text(guider_ui.screen_label_46, time);
        break;
    default:
        // Xử lý khi index không hợp lệ
        break;
    }
}


void send_sw_to_hub(int index, bool status){
            Buffer buffer;
            Sw_t *new_sw = buffer.add_sw();
            new_sw->set_name(bufferManager->sw(index).name());
            bufferManager->mutable_sw(index)->set_status(status);
            new_sw->set_status(bufferManager->sw(index).status());
            new_sw->set_mac(bufferManager->sw(index).mac());
            new_sw->set_ep(bufferManager->sw(index).ep());
            buffer.set_sender(Hub);
            buffer.set_receiver(Server);
            buffer.set_mac_hub("8xff");
            uint8_t buffer_arr[255];
            buffer.SerializeToArray(buffer_arr, sizeof(buffer_arr));
            LOG_INFO(buffer.DebugString());
            hub_screen.transport.publish("hub/master/8xff", (const uint8_t*)buffer_arr, buffer.ByteSizeLong());
}

void sw_handle(int index, lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		lv_obj_t * status_obj = lv_event_get_target(e);
		int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? 1 : 0;
		switch(status) {
		case 0:
		{
            LOG_INFO("sw"<<index<<"off");
            send_sw_to_hub(index, false);

			break;
		}
		case 1:
		{
            LOG_INFO("sw"<<index<<"on");
            send_sw_to_hub(index, true);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void send_led_to_hub(int index, bool status){
            Buffer buffer;
            Led_t *new_led = buffer.add_led();
            new_led->set_name(bufferManager->led(index).name());
            bufferManager->mutable_led(index)->set_status(status);
            new_led->set_status(bufferManager->led(index).status());
            new_led->set_mac(bufferManager->led(index).mac());
            new_led->set_ep(bufferManager->led(index).ep());
            buffer.set_sender(Hub);
            buffer.set_receiver(Server);
            buffer.set_mac_hub("8xff");
            uint8_t buffer_arr[255];
            buffer.SerializeToArray(buffer_arr, sizeof(buffer_arr));
            LOG_INFO(buffer.DebugString());
            hub_screen.transport.publish("hub/master/8xff", (const uint8_t*)buffer_arr, buffer.ByteSizeLong());
}

void led_handle(int index, lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		lv_obj_t * status_obj = lv_event_get_target(e);
		int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? 1 : 0;
		switch(status) {
		case 0:
		{
            LOG_INFO("led"<<index<<"off");
            send_led_to_hub(index, false);

			break;
		}
		case 1:
		{
            LOG_INFO("led"<<index<<"on");
            send_led_to_hub(index, true);
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void sw_1_handle (lv_event_t *e)
{
    sw_handle(0, e);
}

void sw_2_handle (lv_event_t *e)
{
    sw_handle(1, e);
}
void sw_3_handle (lv_event_t *e)
{
    sw_handle(2, e);
}
void sw_4_handle (lv_event_t *e)
{
    sw_handle(3, e);
}
void sw_5_handle (lv_event_t *e)
{
    sw_handle(4, e);
}
void sw_6_handle (lv_event_t *e)
{
    sw_handle(5, e);
}

void led_1_handle (lv_event_t *e)
{
    led_handle(0, e);
}

void led_2_handle (lv_event_t *e)
{
    led_handle(1, e);
}
void led_3_handle (lv_event_t *e)
{
    led_handle(2, e);
}
void led_4_handle (lv_event_t *e)
{
    led_handle(3, e);
}
void led_5_handle (lv_event_t *e)
{
    led_handle(4, e);
}
void led_6_handle (lv_event_t *e)
{
    led_handle(5, e);
}


void events_init_hubscreen(lv_ui *ui){
    hide_device(0, LED);
    hide_device(1, LED);
    hide_device(0, SWITCH);
    hide_device(1, SWITCH);
    hide_device(2, SWITCH);
	lv_obj_add_event_cb(ui->screen_sw_3, sw_1_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_sw_4, sw_2_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_sw_5, sw_3_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_sw_6, sw_4_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_sw_7, sw_5_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_sw_8, sw_6_handle, LV_EVENT_ALL, ui);

    lv_obj_add_event_cb(ui->screen_imgbtn_8, led_1_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_9, led_2_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_10, led_3_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_11, led_4_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_12, led_5_handle, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_imgbtn_13, led_6_handle, LV_EVENT_ALL, ui);
}


void clear_buff(){
    bufferManager->clear_led();
    bufferManager->clear_sw();
    led_index = 0;
    sw_index = 0;
    for(int i = 0; i < 6; i++){
        hide_device(i, LED);
        hide_device(i, SWITCH);
    }
}

void sync_devices(Buffer buffer){
    clear_buff();
    for (int i = 0; i < buffer.led_size(); i++) {
        LOG_INFO("");
        const Led_t& led = buffer.led(i);
        Led_t *new_led = bufferManager->add_led();
        new_led->set_name(led.name());
        new_led->set_status(led.status());
        new_led->set_mac(led.mac());
        new_led->set_ep(led.ep());
        LOG_INFO(led.name());
        show_device(led_index, LED);
        LOG_INFO("show");
        set_device_name(led_index, led.name().c_str(), LED);
        LOG_INFO("set name");
        if(led.status()){
            set_led_output(led_index, LED_ON);   
            LOG_INFO("set status");
        }
        else {
            set_led_output(led_index, LED_OFF);
            LOG_INFO("set status");
        }   
        led_index++;
    }
    for (int i = 0; i < buffer.sw_size(); i++) {
        LOG_INFO("");
        const Sw_t& sw = buffer.sw(i);

        Sw_t *new_sw = bufferManager->add_sw();
        new_sw->set_name(sw.name());
        new_sw->set_status(sw.status());
        new_sw->set_mac(sw.mac());
        new_sw->set_ep(sw.ep());
        LOG_INFO(sw.name());

        show_device(sw_index, SWITCH);
        LOG_INFO("show");

        set_device_name(sw_index, sw.name().c_str(), SWITCH);
        if(sw.status()){
            set_sw_output(sw_index, SW_ON);   
            LOG_INFO("set status");
        }
        else {
            set_sw_output(sw_index, SW_OFF);
            LOG_INFO("set status");
        }   
        sw_index++;
    }
}
void add_led(const Led_t led){
    LOG_INFO("========LED SIZE:" << bufferManager->led_size() << "=========");
    bool found = true;
    for(int j = 0; j < bufferManager->led_size(); j++){
        if(led.name() == bufferManager->led(j).name()){ 
            found = false;
            return;
        }
    }
    if(found) {
        Led_t *new_led = bufferManager->add_led();
        new_led->set_name(led.name());
        new_led->set_status(led.status());
        new_led->set_mac(led.mac());
        new_led->set_ep(led.ep());

        // update device to hubscreen
        show_device(led_index, LED);
        set_device_name(led_index, led.name().c_str(), LED);
        if(led.status()){
            set_led_output(led_index, LED_ON);   
        }
        else {
            set_led_output(led_index, LED_OFF);
        }   
        led_index ++;
    }
}
void add_sw(const Sw_t sw) {
    LOG_INFO("========LED SIZE:" << bufferManager->led_size() << "=========");
    bool found = true;
    for(int j = 0; j < bufferManager->led_size(); j++){
        if(sw.name() == bufferManager->led(j).name()){ 
            found = false;
            return;
        }
    }
    if(found) {
        // add to buffer manager
        Sw_t *new_sw = bufferManager->add_sw();
        new_sw->set_name(sw.name());
        new_sw->set_status(sw.status());
        new_sw->set_mac(sw.mac());
        new_sw->set_ep(sw.ep());

        // update device to hubscreen
        show_device(sw_index, SWITCH);
        set_device_name(sw_index, sw.name().c_str(), SWITCH);
        if(sw.status()){
            set_sw_output(sw_index, SW_ON);   
        }
        else {
            set_sw_output(sw_index, SW_OFF);
        }   
        sw_index ++;
    }
}
void add_device(Buffer buffer){
    for (int i = 0; i < buffer.led_size(); i++){
        const Led_t& led = buffer.led(i);
        add_led(led);
    }
    for (int i = 0; i < buffer.sw_size(); i++){
        const Sw_t& sw = buffer.sw(i);
        add_sw(sw);
    }
}


void upgrade_sw(uint8_t size, uint8_t removedIndex) {
    for (int i = removedIndex ; i < size -1 ; ++i) {
        
        set_device_name(i, bufferManager->sw(i+1).name().c_str(), SWITCH);

        if( bufferManager->sw(i+1).status()){
            set_sw_output(i, SW_ON);   
        }
        else {
            set_sw_output(i, SW_OFF);
        }   
    }
}
void remove_sw(const Sw_t sw) {
    for(int j = 0; j < bufferManager->sw_size(); j++){
        if(sw.name() == bufferManager->sw(j).name()){ 
            upgrade_sw(bufferManager->sw_size(), j);
            bufferManager->mutable_sw()->DeleteSubrange(j, 1);
            hide_device(sw_index - 1, SWITCH);
            sw_index--;
        }
    }   
}

void upgrade_led(uint8_t size, uint8_t removedIndex) {
    for (int i = removedIndex ; i < size -1 ; ++i) {
        // ugrade device to hubscreen
        
        set_device_name(i, bufferManager->led(i+1).name().c_str(), LED);

        if( bufferManager->led(i+1).status()){
            set_led_output(i, LED_ON);   
        }
        else {
            set_led_output(i, LED_OFF);
        }   
    }
}
void remove_led(const Led_t led) {
    for(int j = 0; j < bufferManager->led_size(); j++){
        if(led.name() == bufferManager->led(j).name()){ 
            upgrade_led(bufferManager->led_size(), j);
            bufferManager->mutable_led()->DeleteSubrange(j, 1);
            hide_device(led_index - 1, LED);
            led_index --;
        }
    }   
}

void remove_device(Buffer buffer){
    for (int i = 0; i < buffer.led_size(); i++){
        const Led_t& led = buffer.led(i);
        remove_led(led);
    }
    for (int i = 0; i < buffer.sw_size(); i++){
        const Sw_t& sw = buffer.sw(i);
        remove_sw(sw);
    }
}

void sync_timer(Buffer buffer) {

    const Timer_t& time = buffer.time();
    time.day();
    std::string result = std::to_string(time.hour()) + ":" + std::to_string(time.minute()) + "-" + std::to_string(time.day()) + "/" + std::to_string(time.month());
    set_timer(timer_index++, result.c_str());
}

void sync_status(Buffer buffer) {
    if(buffer.led_size() > 0){
        const Led_t& led = buffer.led(0);
        for(int j = 0; j < bufferManager->led_size(); j++){
            if(led.name() == bufferManager->led(j).name()){
                LOG_INFO(led.name());
                if(led.status()){
                    set_led_output(j, LED_ON);   
                    LOG_INFO("On");
                }
                else {
                    set_led_output(j, LED_OFF);
                    LOG_INFO("Off");
                }  
            }
        }
    }
    else if(buffer.sw_size() > 0){
        const Sw_t& sw = buffer.sw(0);
        for(int j =0; j < bufferManager->sw_size(); j++){
            if(sw.name() == bufferManager->sw(j).name()){
                LOG_INFO(bufferManager->sw_size());
                LOG_INFO(sw.name());
                if(sw.status()){
                    set_sw_output(j, SW_ON); 
                    LOG_INFO("On");
                }
                else {
                    set_sw_output(j, SW_OFF);
                    LOG_INFO("Off");
                }   
            }
        }
    }
}