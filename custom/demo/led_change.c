
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <gui_guider.h>
#include <pthread.h>

void* led_change_st(void* arg) {
    lv_ui *ui = (lv_ui*)arg;
    while (true) {

	    // lv_imgbtn_set_state(ui->screen_imgbtn_8, LV_IMGBTN_STATE_RELEASED);
        // sleep(5);
	    // lv_imgbtn_set_state(ui->screen_imgbtn_8, LV_IMGBTN_STATE_CHECKED_RELEASED);
        // sleep(5);
        lv_obj_add_state(ui->screen_sw_3, LV_STATE_CHECKED);
        sleep(5);
        lv_obj_clear_state(ui->screen_sw_3, LV_STATE_CHECKED);
        sleep(5);
    }
    return NULL;
}