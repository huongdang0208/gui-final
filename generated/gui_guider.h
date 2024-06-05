/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_tileview_main;
	lv_obj_t *screen_tileview_main_hub_screen;
	lv_obj_t *screen_tileview_main_settings;
	lv_obj_t *screen_tileview_hub;
	lv_obj_t *screen_tileview_hub_home;
	lv_obj_t *screen_tileview_hub_home_control;
	lv_obj_t *screen_imgbtn_22;
	lv_obj_t *screen_imgbtn_22_label;
	lv_obj_t *screen_cont_date;
	lv_obj_t *screen_label_47;
	lv_obj_t *screen_label_48;
	lv_obj_t *screen_cont_sensor;
	lv_obj_t *screen_imgbtn_23;
	lv_obj_t *screen_imgbtn_23_label;
	lv_obj_t *screen_label_49;
	lv_obj_t *screen_imgbtn_24;
	lv_obj_t *screen_imgbtn_24_label;
	lv_obj_t *screen_label_50;
	lv_obj_t *screen_cont_hub_menu;
	lv_obj_t *screen_label_13;
	lv_obj_t *screen_cont_24;
	lv_obj_t *screen_label_14;
	lv_obj_t *screen_imgbtn_5;
	lv_obj_t *screen_imgbtn_5_label;
	lv_obj_t *screen_cont_25;
	lv_obj_t *screen_label_15;
	lv_obj_t *screen_imgbtn_6;
	lv_obj_t *screen_imgbtn_6_label;
	lv_obj_t *screen_cont_26;
	lv_obj_t *screen_label_16;
	lv_obj_t *screen_imgbtn_7;
	lv_obj_t *screen_imgbtn_7_label;
	lv_obj_t *screen_cont_led;
	lv_obj_t *screen_cont_29;
	lv_obj_t *screen_label_17;
	lv_obj_t *screen_imgbtn_8;
	lv_obj_t *screen_imgbtn_8_label;
	lv_obj_t *screen_cont_30;
	lv_obj_t *screen_label_18;
	lv_obj_t *screen_imgbtn_9;
	lv_obj_t *screen_imgbtn_9_label;
	lv_obj_t *screen_cont_31;
	lv_obj_t *screen_label_19;
	lv_obj_t *screen_imgbtn_10;
	lv_obj_t *screen_imgbtn_10_label;
	lv_obj_t *screen_cont_32;
	lv_obj_t *screen_label_20;
	lv_obj_t *screen_imgbtn_11;
	lv_obj_t *screen_imgbtn_11_label;
	lv_obj_t *screen_cont_33;
	lv_obj_t *screen_label_21;
	lv_obj_t *screen_imgbtn_12;
	lv_obj_t *screen_imgbtn_12_label;
	lv_obj_t *screen_cont_34;
	lv_obj_t *screen_label_22;
	lv_obj_t *screen_imgbtn_13;
	lv_obj_t *screen_imgbtn_13_label;
	lv_obj_t *screen_cont_sw;
	lv_obj_t *screen_cont_41;
	lv_obj_t *screen_label_28;
	lv_obj_t *screen_sw_3;
	lv_obj_t *screen_cont_42;
	lv_obj_t *screen_label_29;
	lv_obj_t *screen_sw_4;
	lv_obj_t *screen_cont_43;
	lv_obj_t *screen_label_30;
	lv_obj_t *screen_sw_5;
	lv_obj_t *screen_cont_44;
	lv_obj_t *screen_label_31;
	lv_obj_t *screen_sw_6;
	lv_obj_t *screen_cont_45;
	lv_obj_t *screen_label_32;
	lv_obj_t *screen_sw_7;
	lv_obj_t *screen_cont_46;
	lv_obj_t *screen_label_33;
	lv_obj_t *screen_sw_8;
	lv_obj_t *screen_cont_timer;
	lv_obj_t *screen_cont_53;
	lv_obj_t *screen_label_39;
	lv_obj_t *screen_imgbtn_14;
	lv_obj_t *screen_imgbtn_14_label;
	lv_obj_t *screen_cont_54;
	lv_obj_t *screen_label_40;
	lv_obj_t *screen_imgbtn_15;
	lv_obj_t *screen_imgbtn_15_label;
	lv_obj_t *screen_cont_55;
	lv_obj_t *screen_label_41;
	lv_obj_t *screen_imgbtn_16;
	lv_obj_t *screen_imgbtn_16_label;
	lv_obj_t *screen_cont_56;
	lv_obj_t *screen_label_42;
	lv_obj_t *screen_imgbtn_17;
	lv_obj_t *screen_imgbtn_17_label;
	lv_obj_t *screen_cont_57;
	lv_obj_t *screen_label_43;
	lv_obj_t *screen_imgbtn_18;
	lv_obj_t *screen_imgbtn_18_label;
	lv_obj_t *screen_cont_58;
	lv_obj_t *screen_label_44;
	lv_obj_t *screen_imgbtn_19;
	lv_obj_t *screen_imgbtn_19_label;
	lv_obj_t *screen_cont_59;
	lv_obj_t *screen_label_45;
	lv_obj_t *screen_imgbtn_20;
	lv_obj_t *screen_imgbtn_20_label;
	lv_obj_t *screen_cont_60;
	lv_obj_t *screen_label_46;
	lv_obj_t *screen_imgbtn_21;
	lv_obj_t *screen_imgbtn_21_label;
	lv_obj_t *screen_win_1;
	lv_obj_t *screen_win_1_item0;
	lv_obj_t *screen_cont_setting_menu;
	lv_obj_t *screen_cont_2;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_cont_3;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_imgbtn_1;
	lv_obj_t *screen_imgbtn_1_label;
	lv_obj_t *screen_cont_4;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_imgbtn_2;
	lv_obj_t *screen_imgbtn_2_label;
	lv_obj_t *screen_cont_5;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_imgbtn_3;
	lv_obj_t *screen_imgbtn_3_label;
	lv_obj_t *screen_cont_6;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_imgbtn_4;
	lv_obj_t *screen_imgbtn_4_label;
	lv_obj_t *screen_cont_wifi;
	lv_obj_t *screen_cont_8;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_sw_1;
	lv_obj_t *screen_cont_9;
	lv_obj_t *screen_cont_10;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_btn_1;
	lv_obj_t *screen_btn_1_label;
	lv_obj_t *screen_cont_ble;
	lv_obj_t *screen_cont_14;
	lv_obj_t *screen_label_9;
	lv_obj_t *screen_sw_2;
	lv_obj_t *screen_cont_12;
	lv_obj_t *screen_cont_13;
	lv_obj_t *screen_label_8;
	lv_obj_t *screen_btn_2;
	lv_obj_t *screen_btn_2_label;
	lv_obj_t *screen_cont_lang;
	lv_obj_t *screen_cont_18;
	lv_obj_t *screen_label_11;
	lv_obj_t *screen_cont_16;
	lv_obj_t *screen_ddlist_1;
	lv_obj_t *screen_cont_noti;
	lv_obj_t *screen_cont_21;
	lv_obj_t *screen_label_12;
	lv_obj_t *screen_cont_20;
	lv_obj_t *screen_list_1;
	lv_obj_t *screen_list_1_item0;
	lv_obj_t *screen_list_1_item1;
	lv_obj_t *screen_list_1_item2;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_move_animation(void * var, int32_t duration, int32_t delay, int32_t x_end, int32_t y_end, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_scale_animation(void * var, int32_t duration, int32_t delay, int32_t width, int32_t height, lv_anim_path_cb_t path_cb,
                        uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                        lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_zoom_animation(void * var, int32_t duration, int32_t delay, int32_t zoom, lv_anim_path_cb_t path_cb,
                           uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                           lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_rotate_animation(void * var, int32_t duration, int32_t delay, lv_coord_t x, lv_coord_t y, int32_t rotate,
                   lv_anim_path_cb_t path_cb, uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time,
                   uint32_t playback_delay, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_screen(lv_ui *ui);
LV_IMG_DECLARE(_background_alpha_800x480);
LV_IMG_DECLARE(_itemperature_alpha_40x30);
LV_IMG_DECLARE(_ihumidity_alpha_31x28);
LV_IMG_DECLARE(_onlight_alpha_38x35);
LV_IMG_DECLARE(_switch_alpha_37x33);
LV_IMG_DECLARE(_clock_alpha_32x32);
LV_IMG_DECLARE(_offlight_alpha_54x52);
LV_IMG_DECLARE(_led_alpha_54x52);
LV_IMG_DECLARE(_offlight_alpha_54x52);
LV_IMG_DECLARE(_led_alpha_54x52);
LV_IMG_DECLARE(_offlight_alpha_54x52);
LV_IMG_DECLARE(_led_alpha_54x52);
LV_IMG_DECLARE(_offlight_alpha_54x52);
LV_IMG_DECLARE(_led_alpha_54x52);
LV_IMG_DECLARE(_offlight_alpha_54x52);
LV_IMG_DECLARE(_led_alpha_54x52);
LV_IMG_DECLARE(_offlight_alpha_54x52);
LV_IMG_DECLARE(_led_alpha_54x52);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_timer_alpha_47x41);
LV_IMG_DECLARE(_arrow_alpha_38x35);
LV_IMG_DECLARE(_arrow_alpha_38x35);
LV_IMG_DECLARE(_arrow_alpha_38x35);
LV_IMG_DECLARE(_arrow_alpha_38x35);

LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_24)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Antonio_Regular_32)
LV_FONT_DECLARE(lv_font_Alatsi_Regular_16)
LV_FONT_DECLARE(lv_font_Acme_Regular_25)
LV_FONT_DECLARE(lv_font_montserratMedium_14)
LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_20)
LV_FONT_DECLARE(lv_font_FontAwesome5_18)


#ifdef __cplusplus
}
#endif
#endif
