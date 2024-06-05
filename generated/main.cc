#include <hub_screen.h>

lv_style_t  style;
lv_ui guider_ui;
std::queue<Event> eventQueue;
std::mutex mtx;
std::condition_variable cv;
pthread_t hub_screen_thread;
HubScreen_t hub_screen(DEVICE_NAME);

/*push data to queue*/
void event_handle(const struct mosquitto_message *message){
    // if(hub_screen.buffer.cotroller() == Hub) {
    //     eventQueue.push(Event(Event::SyncDeivce, message->topic, hub_screen.buffer));
    // }
    // else if(hub_screen.buffer.cotroller() == Screen){
    //     if(hub_screen.buffer.has_time()){
    //         eventQueue.push(Event(Event::SyncTimer, message->topic, hub_screen.buffer));
    //     }
    //     else {
    //         eventQueue.push(Event(Event::StatusDeivce, message->topic, hub_screen.buffer));
    //     }
    // }
    eventQueue.push(Event(Event::SyncDeivce, message->topic, hub_screen.buffer));
}

/*push data to queue*/
void logic_control(std::vector<uint8_t> buff_vec, const struct mosquitto_message *message){
    if (!hub_screen.buffer.ParseFromArray(buff_vec.data(), buff_vec.size())) {
        eventQueue.push(Event(Event::ErrParse, message->topic, hub_screen.buffer));
    }else {
        if (hub_screen.buffer.receiver() == Server){
            event_handle(message);
        }
        else {
            eventQueue.push(Event(Event::ErrFormat, message->topic, hub_screen.buffer));
        }
    }
}

void* system_intergration(void* arg) {

    while (1) {
        // get data from queue
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return !eventQueue.empty(); });
        Event event = eventQueue.front();
        eventQueue.pop();
        lock.unlock();
        // handle data
        switch (event.type) {
            case Event::ErrFormat: {
                LOG_ERR("====Error format====");
                break;
            }
            case Event::ErrParse: {
                LOG_ERR("====Error parse====");
                break;
            }
            case Event::SyncDeivce: {
                LOG_INFO("====Sync device====");
                
                sync_devices(event.buffer);
                break;
            }
            case Event::SyncTimer: {
                LOG_INFO("====Sync device====");

                sync_timer(event.buffer);
                break;
            }
            case Event::StatusDeivce: {
                LOG_INFO("====Status device====");
                break;
            }
            case Event::ControlDevice: {
                LOG_INFO("====Control device====");
                break;
            }
            default:
                break;
        }
    }
    return NULL;
}

void mqtt_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    std::lock_guard<std::mutex> lock(mtx);

    LOG_INFO("<-- " << message->topic << " : " << message->payload);
    std::vector<uint8_t> vec_data((const uint8_t *)message->payload, (const uint8_t *)message->payload + message->payloadlen);
    logic_control(vec_data, message);
    
    cv.notify_one();
}


int main(int argc, char *argv[])
{
    int hor_res = 1024;
    int ver_res = 600;

    if (argc >= 3)
    {
        hor_res = atoi(argv[1]);
        ver_res = atoi(argv[2]);
    }
    else
    {
        fprintf(stderr,"Warring: base Usage: %s [w,h]\r\n"
               "base eg: %s 1024 600 ",
               argv[0], argv[0]);
    }

    /*Configuration mqtt*/ 
    __test_mqtt();
    hub_screen.transport.set_callback(mqtt_callback);
    hub_screen.transport.setup(BROKER, PORT, 45);
    hub_screen.transport.subscribe(SUB , 1);
    hub_screen.transport.connect();

    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = hor_res;
    disp_drv.ver_res = ver_res;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    evdev_init();
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = (void (*)(struct _lv_indev_drv_t *, lv_indev_data_t *))evdev_read; // defined in lv_drivers/indev/evdev.h
    lv_indev_t *lv_indev = lv_indev_drv_register(&indev_drv);
    if (!lv_indev)
    {
        printf("lv_indev rregister error %d \r\n", __LINE__);
        return 0;
    }

    //app
    ui_init_style(&style);
    init_scr_del_flag(&guider_ui);
    setup_ui(&guider_ui);
    if (pthread_create(&hub_screen_thread, NULL, system_intergration, NULL)) {
        perror("Error creating thread");
        return -1;
    }
    /*Handle LitlevGL tasks (tickless mode)*/
    while (1)
    {
        lv_task_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if (start_ms == 0)
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}