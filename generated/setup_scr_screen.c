/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include <pthread.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"

void setup_scr_screen(lv_ui *ui)
{
	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->screen, 800, 480);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_tileview_main
	ui->screen_tileview_main = lv_tileview_create(ui->screen);
	ui->screen_tileview_main_hub_screen = lv_tileview_add_tile(ui->screen_tileview_main, 0, 0, LV_DIR_BOTTOM);
	ui->screen_tileview_main_settings = lv_tileview_add_tile(ui->screen_tileview_main, 0, 1, LV_DIR_TOP);
	lv_obj_set_pos(ui->screen_tileview_main, 0, 0);
	lv_obj_set_size(ui->screen_tileview_main, 800, 480);
	lv_obj_set_scrollbar_mode(ui->screen_tileview_main, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_tileview_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_main, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_tileview_main, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_tileview_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_main, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_tileview_main, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_main, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_main, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_tileview_main, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_main, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



	//Write codes screen_tileview_hub
	ui->screen_tileview_hub = lv_tileview_create(ui->screen_tileview_main_hub_screen);
	ui->screen_tileview_hub_home = lv_tileview_add_tile(ui->screen_tileview_hub, 0, 0, LV_DIR_RIGHT);
	ui->screen_tileview_hub_home_control = lv_tileview_add_tile(ui->screen_tileview_hub, 1, 0, LV_DIR_LEFT);
	lv_obj_set_pos(ui->screen_tileview_hub, 0, 0);
	lv_obj_set_size(ui->screen_tileview_hub, 800, 480);
	lv_obj_set_scrollbar_mode(ui->screen_tileview_hub, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_tileview_hub, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_hub, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_hub, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_tileview_hub, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_tileview_hub, Part: LV_PART_SCROLLBAR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_tileview_hub, 255, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_tileview_hub, lv_color_hex(0xeaeff3), LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_tileview_hub, LV_GRAD_DIR_NONE, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_tileview_hub, 0, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);



	//Write codes screen_imgbtn_22
	ui->screen_imgbtn_22 = lv_imgbtn_create(ui->screen_tileview_hub_home);
	lv_obj_add_flag(ui->screen_imgbtn_22, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_22, LV_IMGBTN_STATE_RELEASED, NULL, &_background_alpha_800x480, NULL);
	ui->screen_imgbtn_22_label = lv_label_create(ui->screen_imgbtn_22);
	lv_label_set_text(ui->screen_imgbtn_22_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_22_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_22_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_22, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_22, 0, 0);
	lv_obj_set_size(ui->screen_imgbtn_22, 800, 480);

	//Write style for screen_imgbtn_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_22, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_22, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_22, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_22, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_22, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_22, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_22, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_22, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_22, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_22, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_22, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_22, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_22, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_22, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_22, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_22, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_22, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_22, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_date
	ui->screen_cont_date = lv_obj_create(ui->screen_tileview_hub_home);
	lv_obj_set_pos(ui->screen_cont_date, 472, 280);
	lv_obj_set_size(ui->screen_cont_date, 326, 200);
	lv_obj_set_scrollbar_mode(ui->screen_cont_date, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_date, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_date, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_47
	ui->screen_label_47 = lv_label_create(ui->screen_cont_date);
	lv_label_set_text(ui->screen_label_47, "11 May, 2024");
	lv_label_set_long_mode(ui->screen_label_47, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_47, 127, 91);
	lv_obj_set_size(ui->screen_label_47, 183, 32);

	//Write style for screen_label_47, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_47, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_47, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_47, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_47, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_47, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_47, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_48
	ui->screen_label_48 = lv_label_create(ui->screen_cont_date);
	lv_label_set_text(ui->screen_label_48, "9:00 AM");
	lv_label_set_long_mode(ui->screen_label_48, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_48, 139, 137);
	lv_obj_set_size(ui->screen_label_48, 183, 32);

	//Write style for screen_label_48, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_48, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_48, &lv_font_Antonio_Regular_32, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_48, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_48, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_48, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_48, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_sensor
	ui->screen_cont_sensor = lv_obj_create(ui->screen_tileview_hub_home);
	lv_obj_set_pos(ui->screen_cont_sensor, 455, -1);
	lv_obj_set_size(ui->screen_cont_sensor, 344, 60);
	lv_obj_set_scrollbar_mode(ui->screen_cont_sensor, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_sensor, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_sensor, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_23
	ui->screen_imgbtn_23 = lv_imgbtn_create(ui->screen_cont_sensor);
	lv_obj_add_flag(ui->screen_imgbtn_23, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_23, LV_IMGBTN_STATE_RELEASED, NULL, &_itemperature_alpha_40x30, NULL);
	ui->screen_imgbtn_23_label = lv_label_create(ui->screen_imgbtn_23);
	lv_label_set_text(ui->screen_imgbtn_23_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_23_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_23_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_23, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_23, 113, 3);
	lv_obj_set_size(ui->screen_imgbtn_23, 40, 30);

	//Write style for screen_imgbtn_23, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_23, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_23, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_23, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_23, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_23, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_23, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_23, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_23, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_23, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_23, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_23, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_23, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_23, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_23, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_23, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_23, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_23, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_23, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_23, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_label_49
	ui->screen_label_49 = lv_label_create(ui->screen_cont_sensor);
	lv_label_set_text(ui->screen_label_49, "25°C");

	// pthread_t thread_id;
    // pthread_create(&thread_id, NULL, sensor_thread, (void*)&ui);
	
	lv_label_set_long_mode(ui->screen_label_49, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_49, 149, 6);
	lv_obj_set_size(ui->screen_label_49, 63, 30);

	//Write style for screen_label_49, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_49, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_49, &lv_font_Alatsi_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_49, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_49, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_49, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_49, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_49, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_24
	ui->screen_imgbtn_24 = lv_imgbtn_create(ui->screen_cont_sensor);
	lv_obj_add_flag(ui->screen_imgbtn_24, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_24, LV_IMGBTN_STATE_RELEASED, NULL, &_ihumidity_alpha_31x28, NULL);
	ui->screen_imgbtn_24_label = lv_label_create(ui->screen_imgbtn_24);
	lv_label_set_text(ui->screen_imgbtn_24_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_24_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_24_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_24, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_24, 233, 4);
	lv_obj_set_size(ui->screen_imgbtn_24, 31, 28);

	//Write style for screen_imgbtn_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_24, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_24, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_24, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_24, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_24, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_24, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_24, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_24, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_24, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_24, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_24, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_24, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_24, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_24, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_label_50
	ui->screen_label_50 = lv_label_create(ui->screen_cont_sensor);
	lv_label_set_text(ui->screen_label_50, "70%");
	lv_label_set_long_mode(ui->screen_label_50, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_50, 272, 6);
	lv_obj_set_size(ui->screen_label_50, 63, 30);

	//Write style for screen_label_50, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_50, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_50, &lv_font_Alatsi_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_50, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_50, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_50, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_50, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_50, 0, LV_PART_MAIN|LV_STATE_DEFAULT);



	//Write codes screen_cont_hub_menu
	ui->screen_cont_hub_menu = lv_obj_create(ui->screen_tileview_hub_home_control);
	lv_obj_set_pos(ui->screen_cont_hub_menu, 0, 0);
	lv_obj_set_size(ui->screen_cont_hub_menu, 175, 480);
	lv_obj_set_scrollbar_mode(ui->screen_cont_hub_menu, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_hub_menu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_hub_menu, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_hub_menu, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_hub_menu, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_hub_menu, LV_BORDER_SIDE_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_hub_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_hub_menu, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_hub_menu, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_hub_menu, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_hub_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_hub_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_hub_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_hub_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_hub_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_13
	ui->screen_label_13 = lv_label_create(ui->screen_cont_hub_menu);
	lv_label_set_text(ui->screen_label_13, "Home");
	lv_label_set_long_mode(ui->screen_label_13, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_13, 0, 0);
	lv_obj_set_size(ui->screen_label_13, 175, 57);

	//Write style for screen_label_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_13, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_13, &lv_font_Acme_Regular_25, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_13, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_label_13, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_label_13, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_13, 13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_24
	ui->screen_cont_24 = lv_obj_create(ui->screen_cont_hub_menu);
	lv_obj_set_pos(ui->screen_cont_24, 7, 76);
	lv_obj_set_size(ui->screen_cont_24, 161, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_24, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_24, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_24, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_24, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_24, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_24, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_24, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_24, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_24, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_24, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_14
	ui->screen_label_14 = lv_label_create(ui->screen_cont_24);
	lv_label_set_text(ui->screen_label_14, "Lights");
	lv_label_set_long_mode(ui->screen_label_14, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_14, 27, 5);
	lv_obj_set_size(ui->screen_label_14, 120, 32);

	//Write style for screen_label_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_14, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_14, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_14, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_14, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_5
	ui->screen_imgbtn_5 = lv_imgbtn_create(ui->screen_cont_24);
	lv_obj_add_flag(ui->screen_imgbtn_5, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_5, LV_IMGBTN_STATE_RELEASED, NULL, &_onlight_alpha_38x35, NULL);
	ui->screen_imgbtn_5_label = lv_label_create(ui->screen_imgbtn_5);
	lv_label_set_text(ui->screen_imgbtn_5_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_5_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_5_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_5, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_5, 0, 0);
	lv_obj_set_size(ui->screen_imgbtn_5, 38, 35);

	//Write style for screen_imgbtn_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_5, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_5, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_5, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_5, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_5, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_5, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_5, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_5, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_5, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_5, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_5, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_5, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_25
	ui->screen_cont_25 = lv_obj_create(ui->screen_cont_hub_menu);
	lv_obj_set_pos(ui->screen_cont_25, 8, 133);
	lv_obj_set_size(ui->screen_cont_25, 161, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_25, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_25, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_25, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_25, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_25, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_25, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_25, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_25, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_25, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_25, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_15
	ui->screen_label_15 = lv_label_create(ui->screen_cont_25);
	lv_label_set_text(ui->screen_label_15, "Switches");
	lv_label_set_long_mode(ui->screen_label_15, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_15, 34, 5);
	lv_obj_set_size(ui->screen_label_15, 120, 32);

	//Write style for screen_label_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_15, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_15, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_15, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_15, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_6
	ui->screen_imgbtn_6 = lv_imgbtn_create(ui->screen_cont_25);
	lv_obj_add_flag(ui->screen_imgbtn_6, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_6, LV_IMGBTN_STATE_RELEASED, NULL, &_switch_alpha_37x33, NULL);
	ui->screen_imgbtn_6_label = lv_label_create(ui->screen_imgbtn_6);
	lv_label_set_text(ui->screen_imgbtn_6_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_6_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_6_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_6, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_6, 3, 5);
	lv_obj_set_size(ui->screen_imgbtn_6, 37, 33);

	//Write style for screen_imgbtn_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_6, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_6, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_6, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_6, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_6, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_6, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_6, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_6, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_6, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_6, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_6, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_26
	ui->screen_cont_26 = lv_obj_create(ui->screen_cont_hub_menu);
	lv_obj_set_pos(ui->screen_cont_26, 8, 188);
	lv_obj_set_size(ui->screen_cont_26, 160, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_26, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_26, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_26, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_26, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_26, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_26, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_26, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_26, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_26, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_26, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_16
	ui->screen_label_16 = lv_label_create(ui->screen_cont_26);
	lv_label_set_text(ui->screen_label_16, "Set timer");
	lv_label_set_long_mode(ui->screen_label_16, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_16, 27, 5);
	lv_obj_set_size(ui->screen_label_16, 120, 32);

	//Write style for screen_label_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_16, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_16, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_16, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_16, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_7
	ui->screen_imgbtn_7 = lv_imgbtn_create(ui->screen_cont_26);
	lv_obj_add_flag(ui->screen_imgbtn_7, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_7, LV_IMGBTN_STATE_RELEASED, NULL, &_clock_alpha_32x32, NULL);
	ui->screen_imgbtn_7_label = lv_label_create(ui->screen_imgbtn_7);
	lv_label_set_text(ui->screen_imgbtn_7_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_7_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_7_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_7, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_7, 4, 5);
	lv_obj_set_size(ui->screen_imgbtn_7, 32, 32);

	//Write style for screen_imgbtn_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_7, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_7, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_7, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_7, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_7, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_7, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_7, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_7, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_7, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_7, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_7, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_led
	ui->screen_cont_led = lv_obj_create(ui->screen_tileview_hub_home_control);
	lv_obj_set_pos(ui->screen_cont_led, 204, 26);
	lv_obj_set_size(ui->screen_cont_led, 563, 453);
	lv_obj_set_scrollbar_mode(ui->screen_cont_led, LV_SCROLLBAR_MODE_AUTO);
	lv_obj_add_flag(ui->screen_cont_led, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_led, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_led, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_29
	ui->screen_cont_29 = lv_obj_create(ui->screen_cont_led);
	lv_obj_set_pos(ui->screen_cont_29, 25, 20);
	lv_obj_set_size(ui->screen_cont_29, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_29, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_29, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_29, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_29, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_29, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_29, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_29, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_17
	ui->screen_label_17 = lv_label_create(ui->screen_cont_29);
	lv_label_set_text(ui->screen_label_17, "Light 1");
	lv_label_set_long_mode(ui->screen_label_17, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_17, 85, 13);
	lv_obj_set_size(ui->screen_label_17, 120, 32);

	//Write style for screen_label_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_17, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_17, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_17, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_17, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_8
	ui->screen_imgbtn_8 = lv_imgbtn_create(ui->screen_cont_29);
	lv_obj_add_flag(ui->screen_imgbtn_8, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_8, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_54x52, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_8, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_led_alpha_54x52, NULL);
	ui->screen_imgbtn_8_label = lv_label_create(ui->screen_imgbtn_8);
	lv_label_set_text(ui->screen_imgbtn_8_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_8_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_8_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_8, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_8, 25, 1);
	lv_obj_set_size(ui->screen_imgbtn_8, 54, 52);

	//Write style for screen_imgbtn_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_8, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_8, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_8, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_8, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_8, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_8, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_8, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_8, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_8, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_8, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_8, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_8, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_30
	ui->screen_cont_30 = lv_obj_create(ui->screen_cont_led);
	lv_obj_set_pos(ui->screen_cont_30, 297, 21);
	lv_obj_set_size(ui->screen_cont_30, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_30, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_30, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_30, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_30, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_30, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_30, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_30, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_30, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_18
	ui->screen_label_18 = lv_label_create(ui->screen_cont_30);
	lv_label_set_text(ui->screen_label_18, "Light 2");
	lv_label_set_long_mode(ui->screen_label_18, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_18, 85, 13);
	lv_obj_set_size(ui->screen_label_18, 120, 32);

	//Write style for screen_label_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_18, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_18, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_18, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_18, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_9
	ui->screen_imgbtn_9 = lv_imgbtn_create(ui->screen_cont_30);
	lv_obj_add_flag(ui->screen_imgbtn_9, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_9, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_54x52, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_9, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_led_alpha_54x52, NULL);
	ui->screen_imgbtn_9_label = lv_label_create(ui->screen_imgbtn_9);
	lv_label_set_text(ui->screen_imgbtn_9_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_9_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_9_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_9, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_9, 25, 1);
	lv_obj_set_size(ui->screen_imgbtn_9, 54, 52);

	//Write style for screen_imgbtn_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_9, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_9, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_9, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_9, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_9, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_9, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_9, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_9, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_9, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_9, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_9, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_9, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_9, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_9, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_9, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_9, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_31
	ui->screen_cont_31 = lv_obj_create(ui->screen_cont_led);
	lv_obj_set_pos(ui->screen_cont_31, 25, 96);
	lv_obj_set_size(ui->screen_cont_31, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_31, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_31, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_31, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_31, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_31, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_31, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_31, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_31, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_31, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_19
	ui->screen_label_19 = lv_label_create(ui->screen_cont_31);
	lv_label_set_text(ui->screen_label_19, "Light 3");
	lv_label_set_long_mode(ui->screen_label_19, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_19, 85, 13);
	lv_obj_set_size(ui->screen_label_19, 120, 32);

	//Write style for screen_label_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_19, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_19, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_19, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_19, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_10
	ui->screen_imgbtn_10 = lv_imgbtn_create(ui->screen_cont_31);
	lv_obj_add_flag(ui->screen_imgbtn_10, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_10, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_54x52, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_10, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_led_alpha_54x52, NULL);
	ui->screen_imgbtn_10_label = lv_label_create(ui->screen_imgbtn_10);
	lv_label_set_text(ui->screen_imgbtn_10_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_10_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_10_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_10, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_10, 25, 1);
	lv_obj_set_size(ui->screen_imgbtn_10, 54, 52);

	//Write style for screen_imgbtn_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_10, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_10, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_10, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_10, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_10, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_10, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_10, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_10, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_10, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_10, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_10, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_32
	ui->screen_cont_32 = lv_obj_create(ui->screen_cont_led);
	lv_obj_set_pos(ui->screen_cont_32, 297, 96);
	lv_obj_set_size(ui->screen_cont_32, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_32, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_32, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_32, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_32, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_32, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_32, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_32, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_32, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_32, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_20
	ui->screen_label_20 = lv_label_create(ui->screen_cont_32);
	lv_label_set_text(ui->screen_label_20, "Light 4");
	lv_label_set_long_mode(ui->screen_label_20, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_20, 86, 13);
	lv_obj_set_size(ui->screen_label_20, 120, 32);

	//Write style for screen_label_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_20, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_20, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_20, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_20, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_11
	ui->screen_imgbtn_11 = lv_imgbtn_create(ui->screen_cont_32);
	lv_obj_add_flag(ui->screen_imgbtn_11, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_11, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_54x52, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_11, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_led_alpha_54x52, NULL);
	ui->screen_imgbtn_11_label = lv_label_create(ui->screen_imgbtn_11);
	lv_label_set_text(ui->screen_imgbtn_11_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_11_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_11_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_11, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_11, 25, 1);
	lv_obj_set_size(ui->screen_imgbtn_11, 54, 52);

	//Write style for screen_imgbtn_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_11, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_11, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_11, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_11, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_11, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_11, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_11, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_11, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_11, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_11, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_11, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_33
	ui->screen_cont_33 = lv_obj_create(ui->screen_cont_led);
	lv_obj_set_pos(ui->screen_cont_33, 25, 169);
	lv_obj_set_size(ui->screen_cont_33, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_33, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_33, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_33, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_33, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_33, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_33, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_33, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_33, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_33, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_21
	ui->screen_label_21 = lv_label_create(ui->screen_cont_33);
	lv_label_set_text(ui->screen_label_21, "Light 5");
	lv_label_set_long_mode(ui->screen_label_21, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_21, 85, 13);
	lv_obj_set_size(ui->screen_label_21, 120, 32);

	//Write style for screen_label_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_21, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_21, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_21, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_21, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_21, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_21, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_12
	ui->screen_imgbtn_12 = lv_imgbtn_create(ui->screen_cont_33);
	lv_obj_add_flag(ui->screen_imgbtn_12, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_12, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_54x52, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_12, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_led_alpha_54x52, NULL);
	ui->screen_imgbtn_12_label = lv_label_create(ui->screen_imgbtn_12);
	lv_label_set_text(ui->screen_imgbtn_12_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_12_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_12_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_12, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_12, 25, 1);
	lv_obj_set_size(ui->screen_imgbtn_12, 54, 52);

	//Write style for screen_imgbtn_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_12, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_12, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_12, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_12, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_12, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_12, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_12, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_12, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_12, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_12, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_12, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_12, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_12, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_34
	ui->screen_cont_34 = lv_obj_create(ui->screen_cont_led);
	lv_obj_set_pos(ui->screen_cont_34, 296, 169);
	lv_obj_set_size(ui->screen_cont_34, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_34, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_34, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_34, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_34, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_34, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_34, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_34, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_34, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_34, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_34, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_34, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_22
	ui->screen_label_22 = lv_label_create(ui->screen_cont_34);
	lv_label_set_text(ui->screen_label_22, "Light 6");
	lv_label_set_long_mode(ui->screen_label_22, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_22, 85, 12);
	lv_obj_set_size(ui->screen_label_22, 120, 32);

	//Write style for screen_label_22, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_22, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_22, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_22, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_22, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_22, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_22, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_22, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_13
	ui->screen_imgbtn_13 = lv_imgbtn_create(ui->screen_cont_34);
	lv_obj_add_flag(ui->screen_imgbtn_13, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_13, LV_IMGBTN_STATE_RELEASED, NULL, &_offlight_alpha_54x52, NULL);
	lv_imgbtn_set_src(ui->screen_imgbtn_13, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_led_alpha_54x52, NULL);
	ui->screen_imgbtn_13_label = lv_label_create(ui->screen_imgbtn_13);
	lv_label_set_text(ui->screen_imgbtn_13_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_13_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_13_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_13, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_13, 25, 1);
	lv_obj_set_size(ui->screen_imgbtn_13, 54, 52);

	//Write style for screen_imgbtn_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_13, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_13, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_13, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_13, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_13, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_13, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_13, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_13, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_13, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_13, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_13, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_13, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_13, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_13, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_sw
	ui->screen_cont_sw = lv_obj_create(ui->screen_tileview_hub_home_control);
	lv_obj_set_pos(ui->screen_cont_sw, 204, 26);
	lv_obj_set_size(ui->screen_cont_sw, 563, 453);
	lv_obj_set_scrollbar_mode(ui->screen_cont_sw, LV_SCROLLBAR_MODE_AUTO);
	lv_obj_add_flag(ui->screen_cont_sw, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_sw, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_sw, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_41
	ui->screen_cont_41 = lv_obj_create(ui->screen_cont_sw);
	lv_obj_set_pos(ui->screen_cont_41, 25, 20);
	lv_obj_set_size(ui->screen_cont_41, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_41, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_41, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_41, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_41, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_41, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_41, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_41, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_41, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_41, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_28
	ui->screen_label_28 = lv_label_create(ui->screen_cont_41);
	lv_label_set_text(ui->screen_label_28, "Switch 1");
	lv_label_set_long_mode(ui->screen_label_28, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_28, 88, 13);
	lv_obj_set_size(ui->screen_label_28, 120, 32);

	//Write style for screen_label_28, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_28, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_28, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_28, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_28, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_28, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_28, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_28, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_3
	ui->screen_sw_3 = lv_switch_create(ui->screen_cont_41);
	lv_obj_set_pos(ui->screen_sw_3, 35, 16);
	lv_obj_set_size(ui->screen_sw_3, 50, 25);

	//Write style for screen_sw_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_3, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_3, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_3, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_3, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_3, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_3, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_3, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_3, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_3, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_3, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_3, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_3, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_3, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_42
	ui->screen_cont_42 = lv_obj_create(ui->screen_cont_sw);
	lv_obj_set_pos(ui->screen_cont_42, 300, 20);
	lv_obj_set_size(ui->screen_cont_42, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_42, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_42, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_42, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_42, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_42, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_42, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_42, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_42, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_42, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_29
	ui->screen_label_29 = lv_label_create(ui->screen_cont_42);
	lv_label_set_text(ui->screen_label_29, "Switch 2");
	lv_label_set_long_mode(ui->screen_label_29, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_29, 88, 13);
	lv_obj_set_size(ui->screen_label_29, 120, 32);

	//Write style for screen_label_29, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_29, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_29, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_29, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_29, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_29, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_29, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_4
	ui->screen_sw_4 = lv_switch_create(ui->screen_cont_42);
	lv_obj_set_pos(ui->screen_sw_4, 35, 16);
	lv_obj_set_size(ui->screen_sw_4, 50, 25);

	//Write style for screen_sw_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_4, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_4, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_4, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_4, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_4, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_4, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_4, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_4, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_4, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_4, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_4, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_4, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_4, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_43
	ui->screen_cont_43 = lv_obj_create(ui->screen_cont_sw);
	lv_obj_set_pos(ui->screen_cont_43, 25, 93);
	lv_obj_set_size(ui->screen_cont_43, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_43, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_43, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_43, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_43, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_43, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_43, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_43, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_43, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_43, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_30
	ui->screen_label_30 = lv_label_create(ui->screen_cont_43);
	lv_label_set_text(ui->screen_label_30, "Switch 3");
	lv_label_set_long_mode(ui->screen_label_30, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_30, 87, 12);
	lv_obj_set_size(ui->screen_label_30, 120, 32);

	//Write style for screen_label_30, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_30, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_30, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_30, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_30, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_30, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_30, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_5
	ui->screen_sw_5 = lv_switch_create(ui->screen_cont_43);
	lv_obj_set_pos(ui->screen_sw_5, 35, 16);
	lv_obj_set_size(ui->screen_sw_5, 50, 25);

	//Write style for screen_sw_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_5, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_5, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_5, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_5, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_5, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_5, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_5, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_5, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_5, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_5, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_5, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_5, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_5, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_44
	ui->screen_cont_44 = lv_obj_create(ui->screen_cont_sw);
	lv_obj_set_pos(ui->screen_cont_44, 300, 93);
	lv_obj_set_size(ui->screen_cont_44, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_44, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_44, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_44, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_44, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_44, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_44, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_44, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_44, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_44, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_44, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_31
	ui->screen_label_31 = lv_label_create(ui->screen_cont_44);
	lv_label_set_text(ui->screen_label_31, "Switch 4");
	lv_label_set_long_mode(ui->screen_label_31, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_31, 87, 12);
	lv_obj_set_size(ui->screen_label_31, 120, 32);

	//Write style for screen_label_31, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_31, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_31, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_31, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_31, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_31, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_31, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_31, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_6
	ui->screen_sw_6 = lv_switch_create(ui->screen_cont_44);
	lv_obj_set_pos(ui->screen_sw_6, 35, 16);
	lv_obj_set_size(ui->screen_sw_6, 50, 25);

	//Write style for screen_sw_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_6, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_6, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_6, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_6, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_6, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_6, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_6, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_6, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_6, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_6, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_6, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_6, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_6, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_45
	ui->screen_cont_45 = lv_obj_create(ui->screen_cont_sw);
	lv_obj_set_pos(ui->screen_cont_45, 25, 165);
	lv_obj_set_size(ui->screen_cont_45, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_45, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_45, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_45, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_45, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_45, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_45, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_45, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_45, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_45, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_45, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_32
	ui->screen_label_32 = lv_label_create(ui->screen_cont_45);
	lv_label_set_text(ui->screen_label_32, "Switch 5");
	lv_label_set_long_mode(ui->screen_label_32, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_32, 87, 12);
	lv_obj_set_size(ui->screen_label_32, 120, 32);

	//Write style for screen_label_32, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_32, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_32, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_32, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_32, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_32, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_32, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_32, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_7
	ui->screen_sw_7 = lv_switch_create(ui->screen_cont_45);
	lv_obj_set_pos(ui->screen_sw_7, 35, 16);
	lv_obj_set_size(ui->screen_sw_7, 50, 25);

	//Write style for screen_sw_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_7, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_7, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_7, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_7, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_7, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_7, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_7, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_7, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_7, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_7, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_7, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_7, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_7, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_46
	ui->screen_cont_46 = lv_obj_create(ui->screen_cont_sw);
	lv_obj_set_pos(ui->screen_cont_46, 300, 165);
	lv_obj_set_size(ui->screen_cont_46, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_46, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_46, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_46, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_46, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_46, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_46, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_46, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_46, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_46, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_46, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_33
	ui->screen_label_33 = lv_label_create(ui->screen_cont_46);
	lv_label_set_text(ui->screen_label_33, "Switch 6");
	lv_label_set_long_mode(ui->screen_label_33, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_33, 87, 12);
	lv_obj_set_size(ui->screen_label_33, 120, 32);

	//Write style for screen_label_33, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_33, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_33, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_33, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_33, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_33, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_33, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_33, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_8
	ui->screen_sw_8 = lv_switch_create(ui->screen_cont_46);
	lv_obj_set_pos(ui->screen_sw_8, 35, 16);
	lv_obj_set_size(ui->screen_sw_8, 50, 25);

	//Write style for screen_sw_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_8, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_8, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_8, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_8, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_8, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_8, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_8, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_8, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_8, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_8, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_8, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_8, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_8, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_8, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_timer
	ui->screen_cont_timer = lv_obj_create(ui->screen_tileview_hub_home_control);
	lv_obj_set_pos(ui->screen_cont_timer, 204, 26);
	lv_obj_set_size(ui->screen_cont_timer, 563, 453);
	lv_obj_set_scrollbar_mode(ui->screen_cont_timer, LV_SCROLLBAR_MODE_AUTO);

	//Write style for screen_cont_timer, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_timer, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_53
	ui->screen_cont_53 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_53, 25, 20);
	lv_obj_set_size(ui->screen_cont_53, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_53, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_53, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_53, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_53, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_53, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_53, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_53, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_53, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_53, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_39
	ui->screen_label_39 = lv_label_create(ui->screen_cont_53);
	lv_label_set_text(ui->screen_label_39, "10:50-20/10");
	lv_label_set_long_mode(ui->screen_label_39, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_39, 58, 14);
	lv_obj_set_size(ui->screen_label_39, 173, 32);

	//Write style for screen_label_39, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_39, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_39, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_39, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_39, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_39, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_39, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_39, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_14
	ui->screen_imgbtn_14 = lv_imgbtn_create(ui->screen_cont_53);
	lv_obj_add_flag(ui->screen_imgbtn_14, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_14, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_14_label = lv_label_create(ui->screen_imgbtn_14);
	lv_label_set_text(ui->screen_imgbtn_14_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_14_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_14_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_14, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_14, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_14, 47, 41);

	//Write style for screen_imgbtn_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_14, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_14, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_14, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_14, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_14, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_14, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_14, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_14, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_14, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_14, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_14, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_14, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_54
	ui->screen_cont_54 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_54, 302, 20);
	lv_obj_set_size(ui->screen_cont_54, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_54, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_54, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_54, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_54, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_54, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_54, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_54, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_54, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_54, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_54, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_40
	ui->screen_label_40 = lv_label_create(ui->screen_cont_54);
	lv_label_set_text(ui->screen_label_40, "5:30-08/10");
	lv_label_set_long_mode(ui->screen_label_40, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_40, 58, 14);
	lv_obj_set_size(ui->screen_label_40, 173, 32);

	//Write style for screen_label_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_40, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_40, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_40, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_40, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_40, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_40, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_15
	ui->screen_imgbtn_15 = lv_imgbtn_create(ui->screen_cont_54);
	lv_obj_add_flag(ui->screen_imgbtn_15, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_15, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_15_label = lv_label_create(ui->screen_imgbtn_15);
	lv_label_set_text(ui->screen_imgbtn_15_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_15_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_15_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_15, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_15, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_15, 47, 41);

	//Write style for screen_imgbtn_15, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_15, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_15, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_15, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_15, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_15, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_15, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_15, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_15, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_15, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_15, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_15, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_15, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_15, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_55
	ui->screen_cont_55 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_55, 25, 95);
	lv_obj_set_size(ui->screen_cont_55, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_55, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_55, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_55, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_55, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_55, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_55, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_55, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_55, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_55, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_55, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_55, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_55, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_55, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_55, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_55, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_41
	ui->screen_label_41 = lv_label_create(ui->screen_cont_55);
	lv_label_set_text(ui->screen_label_41, "7:30-08/10");
	lv_label_set_long_mode(ui->screen_label_41, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_41, 57, 13);
	lv_obj_set_size(ui->screen_label_41, 173, 32);

	//Write style for screen_label_41, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_41, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_41, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_41, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_41, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_41, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_41, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_41, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_16
	ui->screen_imgbtn_16 = lv_imgbtn_create(ui->screen_cont_55);
	lv_obj_add_flag(ui->screen_imgbtn_16, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_16, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_16_label = lv_label_create(ui->screen_imgbtn_16);
	lv_label_set_text(ui->screen_imgbtn_16_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_16_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_16_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_16, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_16, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_16, 47, 41);

	//Write style for screen_imgbtn_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_16, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_16, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_16, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_16, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_16, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_16, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_16, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_16, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_16, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_16, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_16, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_16, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_16, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_16, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_16, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_16, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_16, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_16, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_56
	ui->screen_cont_56 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_56, 302, 95);
	lv_obj_set_size(ui->screen_cont_56, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_56, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_56, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_56, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_56, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_56, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_56, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_56, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_56, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_56, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_56, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_56, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_42
	ui->screen_label_42 = lv_label_create(ui->screen_cont_56);
	lv_label_set_text(ui->screen_label_42, "1:50-10/5");
	lv_label_set_long_mode(ui->screen_label_42, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_42, 58, 14);
	lv_obj_set_size(ui->screen_label_42, 173, 32);

	//Write style for screen_label_42, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_42, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_42, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_42, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_42, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_42, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_42, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_17
	ui->screen_imgbtn_17 = lv_imgbtn_create(ui->screen_cont_56);
	lv_obj_add_flag(ui->screen_imgbtn_17, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_17, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_17_label = lv_label_create(ui->screen_imgbtn_17);
	lv_label_set_text(ui->screen_imgbtn_17_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_17_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_17_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_17, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_17, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_17, 47, 41);

	//Write style for screen_imgbtn_17, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_17, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_17, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_17, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_17, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_17, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_17, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_17, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_17, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_17, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_17, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_17, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_17, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_17, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_17, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_17, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_17, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_17, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_17, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_57
	ui->screen_cont_57 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_57, 25, 170);
	lv_obj_set_size(ui->screen_cont_57, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_57, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_57, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_57, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_57, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_57, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_57, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_57, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_57, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_57, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_57, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_57, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_43
	ui->screen_label_43 = lv_label_create(ui->screen_cont_57);
	lv_label_set_text(ui->screen_label_43, "7:30-08/10");
	lv_label_set_long_mode(ui->screen_label_43, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_43, 57, 13);
	lv_obj_set_size(ui->screen_label_43, 173, 32);

	//Write style for screen_label_43, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_43, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_43, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_43, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_43, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_43, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_43, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_43, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_18
	ui->screen_imgbtn_18 = lv_imgbtn_create(ui->screen_cont_57);
	lv_obj_add_flag(ui->screen_imgbtn_18, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_18, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_18_label = lv_label_create(ui->screen_imgbtn_18);
	lv_label_set_text(ui->screen_imgbtn_18_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_18_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_18_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_18, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_18, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_18, 47, 41);

	//Write style for screen_imgbtn_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_18, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_18, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_18, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_18, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_18, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_18, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_18, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_18, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_18, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_18, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_18, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_18, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_18, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_58
	ui->screen_cont_58 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_58, 302, 170);
	lv_obj_set_size(ui->screen_cont_58, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_58, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_58, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_58, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_58, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_58, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_58, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_58, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_58, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_58, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_58, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_58, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_44
	ui->screen_label_44 = lv_label_create(ui->screen_cont_58);
	lv_label_set_text(ui->screen_label_44, "7:30-08/10");
	lv_label_set_long_mode(ui->screen_label_44, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_44, 57, 13);
	lv_obj_set_size(ui->screen_label_44, 173, 32);

	//Write style for screen_label_44, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_44, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_44, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_44, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_44, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_44, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_44, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_44, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_19
	ui->screen_imgbtn_19 = lv_imgbtn_create(ui->screen_cont_58);
	lv_obj_add_flag(ui->screen_imgbtn_19, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_19, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_19_label = lv_label_create(ui->screen_imgbtn_19);
	lv_label_set_text(ui->screen_imgbtn_19_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_19_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_19_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_19, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_19, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_19, 47, 41);

	//Write style for screen_imgbtn_19, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_19, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_19, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_19, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_19, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_19, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_19, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_19, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_19, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_19, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_19, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_19, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_19, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_19, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_19, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_19, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_59
	ui->screen_cont_59 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_59, 25, 241);
	lv_obj_set_size(ui->screen_cont_59, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_59, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_59, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_59, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_59, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_59, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_59, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_59, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_59, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_59, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_59, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_59, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_45
	ui->screen_label_45 = lv_label_create(ui->screen_cont_59);
	lv_label_set_text(ui->screen_label_45, "7:30-08/10");
	lv_label_set_long_mode(ui->screen_label_45, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_45, 57, 13);
	lv_obj_set_size(ui->screen_label_45, 173, 32);

	//Write style for screen_label_45, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_45, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_45, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_45, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_45, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_45, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_45, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_20
	ui->screen_imgbtn_20 = lv_imgbtn_create(ui->screen_cont_59);
	lv_obj_add_flag(ui->screen_imgbtn_20, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_20, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_20_label = lv_label_create(ui->screen_imgbtn_20);
	lv_label_set_text(ui->screen_imgbtn_20_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_20_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_20_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_20, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_20, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_20, 47, 41);

	//Write style for screen_imgbtn_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_20, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_20, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_20, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_20, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_20, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_20, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_20, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_20, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_20, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_20, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_20, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_20, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_20, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_20, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_60
	ui->screen_cont_60 = lv_obj_create(ui->screen_cont_timer);
	lv_obj_set_pos(ui->screen_cont_60, 303, 243);
	lv_obj_set_size(ui->screen_cont_60, 240, 61);
	lv_obj_set_scrollbar_mode(ui->screen_cont_60, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_60, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_60, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_60, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_60, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_60, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_60, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_60, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_60, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_60, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_60, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_46
	ui->screen_label_46 = lv_label_create(ui->screen_cont_60);
	lv_label_set_text(ui->screen_label_46, "7:30-08/10");
	lv_label_set_long_mode(ui->screen_label_46, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_46, 57, 13);
	lv_obj_set_size(ui->screen_label_46, 173, 32);

	//Write style for screen_label_46, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_46, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_46, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_46, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_46, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_46, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_46, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_21
	ui->screen_imgbtn_21 = lv_imgbtn_create(ui->screen_cont_60);
	lv_obj_add_flag(ui->screen_imgbtn_21, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_21, LV_IMGBTN_STATE_RELEASED, NULL, &_timer_alpha_47x41, NULL);
	ui->screen_imgbtn_21_label = lv_label_create(ui->screen_imgbtn_21);
	lv_label_set_text(ui->screen_imgbtn_21_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_21_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_21_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_21, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_21, 12, 7);
	lv_obj_set_size(ui->screen_imgbtn_21, 47, 41);

	//Write style for screen_imgbtn_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_21, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_21, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_21, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_21, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_21, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_21, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_21, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_21, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_21, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_21, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_21, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_21, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_21, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_21, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_21, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_21, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_21, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_21, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_win_1
	ui->screen_win_1 = lv_win_create(ui->screen_tileview_hub_home_control, 55);
	lv_obj_t * screen_win_1_title = lv_win_add_title(ui->screen_win_1, "Control device information");
	ui->screen_win_1_item0 = lv_win_add_btn(ui->screen_win_1, LV_SYMBOL_CLOSE, 40);
	lv_obj_t *screen_win_1_label = lv_label_create(lv_win_get_content(ui->screen_win_1));
	lv_obj_set_scrollbar_mode(lv_win_get_content(ui->screen_win_1), LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(screen_win_1_label, "Light 1 [on]\n\nSwitch 2 [off]");
	lv_obj_set_pos(ui->screen_win_1, 490, 0);
	lv_obj_set_size(ui->screen_win_1, 309, 480);
	lv_obj_set_scrollbar_mode(ui->screen_win_1, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_win_1, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_win_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_win_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_win_1, lv_color_hex(0xeeeef6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_win_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_outline_width(ui->screen_win_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_win_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_win_1_extra_content_main_default
	static lv_style_t style_screen_win_1_extra_content_main_default;
	ui_init_style(&style_screen_win_1_extra_content_main_default);
	
	lv_style_set_bg_opa(&style_screen_win_1_extra_content_main_default, 255);
	lv_style_set_bg_color(&style_screen_win_1_extra_content_main_default, lv_color_hex(0xeeeef6));
	lv_style_set_bg_grad_dir(&style_screen_win_1_extra_content_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_text_color(&style_screen_win_1_extra_content_main_default, lv_color_hex(0x393c41));
	lv_style_set_text_font(&style_screen_win_1_extra_content_main_default, &lv_font_montserratMedium_12);
	lv_style_set_text_opa(&style_screen_win_1_extra_content_main_default, 255);
	lv_style_set_text_letter_space(&style_screen_win_1_extra_content_main_default, 0);
	lv_style_set_text_line_space(&style_screen_win_1_extra_content_main_default, 2);
	lv_obj_add_style(lv_win_get_content(ui->screen_win_1), &style_screen_win_1_extra_content_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_win_1_extra_header_main_default
	static lv_style_t style_screen_win_1_extra_header_main_default;
	ui_init_style(&style_screen_win_1_extra_header_main_default);
	
	lv_style_set_bg_opa(&style_screen_win_1_extra_header_main_default, 255);
	lv_style_set_bg_color(&style_screen_win_1_extra_header_main_default, lv_color_hex(0xe6e6e6));
	lv_style_set_bg_grad_dir(&style_screen_win_1_extra_header_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_text_color(&style_screen_win_1_extra_header_main_default, lv_color_hex(0x393c41));
	lv_style_set_text_font(&style_screen_win_1_extra_header_main_default, &lv_font_montserratMedium_12);
	lv_style_set_text_opa(&style_screen_win_1_extra_header_main_default, 255);
	lv_style_set_text_letter_space(&style_screen_win_1_extra_header_main_default, 0);
	lv_style_set_text_line_space(&style_screen_win_1_extra_header_main_default, 2);
	lv_obj_add_style(lv_win_get_header(ui->screen_win_1), &style_screen_win_1_extra_header_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_win_1_extra_btns_main_default
	static lv_style_t style_screen_win_1_extra_btns_main_default;
	ui_init_style(&style_screen_win_1_extra_btns_main_default);
	
	lv_style_set_radius(&style_screen_win_1_extra_btns_main_default, 8);
	lv_style_set_bg_opa(&style_screen_win_1_extra_btns_main_default, 255);
	lv_style_set_bg_color(&style_screen_win_1_extra_btns_main_default, lv_color_hex(0x2195f6));
	lv_style_set_bg_grad_dir(&style_screen_win_1_extra_btns_main_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(ui->screen_win_1_item0, &style_screen_win_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);



	//Write codes screen_cont_setting_menu
	ui->screen_cont_setting_menu = lv_obj_create(ui->screen_tileview_main_settings);
	lv_obj_set_pos(ui->screen_cont_setting_menu, 0, 0);
	lv_obj_set_size(ui->screen_cont_setting_menu, 175, 480);
	lv_obj_set_scrollbar_mode(ui->screen_cont_setting_menu, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_setting_menu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_setting_menu, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_setting_menu, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_setting_menu, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_setting_menu, LV_BORDER_SIDE_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_setting_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_setting_menu, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_setting_menu, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_setting_menu, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_setting_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_setting_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_setting_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_setting_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_setting_menu, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_2
	ui->screen_cont_2 = lv_obj_create(ui->screen_cont_setting_menu);
	lv_obj_set_pos(ui->screen_cont_2, 0, 0);
	lv_obj_set_size(ui->screen_cont_2, 175, 75);
	lv_obj_set_scrollbar_mode(ui->screen_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_2, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen_cont_2);
	lv_label_set_text(ui->screen_label_1, "Settings");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_1, 17, 25);
	lv_obj_set_size(ui->screen_label_1, 143, 32);

	//Write style for screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_1, lv_color_hex(0x268cf2), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_1, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_3
	ui->screen_cont_3 = lv_obj_create(ui->screen_cont_setting_menu);
	lv_obj_set_pos(ui->screen_cont_3, 7, 101);
	lv_obj_set_size(ui->screen_cont_3, 156, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_3, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_3, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_3, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen_cont_3);
	lv_label_set_text(ui->screen_label_2, "Wifi");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_2, 5, 5);
	lv_obj_set_size(ui->screen_label_2, 56, 32);

	//Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_1
	ui->screen_imgbtn_1 = lv_imgbtn_create(ui->screen_cont_3);
	lv_obj_add_flag(ui->screen_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_38x35, NULL);
	ui->screen_imgbtn_1_label = lv_label_create(ui->screen_imgbtn_1);
	lv_label_set_text(ui->screen_imgbtn_1_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_1, 110, 4);
	lv_obj_set_size(ui->screen_imgbtn_1, 38, 35);

	//Write style for screen_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_1, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_1, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_1, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_1, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_4
	ui->screen_cont_4 = lv_obj_create(ui->screen_cont_setting_menu);
	lv_obj_set_pos(ui->screen_cont_4, 5, 157);
	lv_obj_set_size(ui->screen_cont_4, 156, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_4, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_4, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_4, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_4, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_4, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen_cont_4);
	lv_label_set_text(ui->screen_label_3, "Bluetooth");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_3, 4, 10);
	lv_obj_set_size(ui->screen_label_3, 98, 32);

	//Write style for screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_2
	ui->screen_imgbtn_2 = lv_imgbtn_create(ui->screen_cont_4);
	lv_obj_add_flag(ui->screen_imgbtn_2, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_38x35, NULL);
	ui->screen_imgbtn_2_label = lv_label_create(ui->screen_imgbtn_2);
	lv_label_set_text(ui->screen_imgbtn_2_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_2_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_2_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_2, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_2, 109, 7);
	lv_obj_set_size(ui->screen_imgbtn_2, 38, 35);

	//Write style for screen_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_2, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_2, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_2, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_2, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_5
	ui->screen_cont_5 = lv_obj_create(ui->screen_cont_setting_menu);
	lv_obj_set_pos(ui->screen_cont_5, 7, 214);
	lv_obj_set_size(ui->screen_cont_5, 156, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_5, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_5, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_5, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_5, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_5, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_5, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen_cont_5);
	lv_label_set_text(ui->screen_label_4, "Language");
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_4, 6, 7);
	lv_obj_set_size(ui->screen_label_4, 94, 32);

	//Write style for screen_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_4, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_3
	ui->screen_imgbtn_3 = lv_imgbtn_create(ui->screen_cont_5);
	lv_obj_add_flag(ui->screen_imgbtn_3, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_3, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_38x35, NULL);
	ui->screen_imgbtn_3_label = lv_label_create(ui->screen_imgbtn_3);
	lv_label_set_text(ui->screen_imgbtn_3_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_3_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_3_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_3, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_3, 109, 3);
	lv_obj_set_size(ui->screen_imgbtn_3, 38, 35);

	//Write style for screen_imgbtn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_3, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_3, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_3, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_3, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_3, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_3, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_6
	ui->screen_cont_6 = lv_obj_create(ui->screen_cont_setting_menu);
	lv_obj_set_pos(ui->screen_cont_6, 7, 272);
	lv_obj_set_size(ui->screen_cont_6, 156, 48);
	lv_obj_set_scrollbar_mode(ui->screen_cont_6, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->screen_cont_6, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->screen_cont_6, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_6, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_cont_6, lv_color_hex(0x252525), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_cont_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen_cont_6);
	lv_label_set_text(ui->screen_label_5, "Notification");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_5, 3, 7);
	lv_obj_set_size(ui->screen_label_5, 120, 32);

	//Write style for screen_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_5, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_5, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_5, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_imgbtn_4
	ui->screen_imgbtn_4 = lv_imgbtn_create(ui->screen_cont_6);
	lv_obj_add_flag(ui->screen_imgbtn_4, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->screen_imgbtn_4, LV_IMGBTN_STATE_RELEASED, NULL, &_arrow_alpha_38x35, NULL);
	ui->screen_imgbtn_4_label = lv_label_create(ui->screen_imgbtn_4);
	lv_label_set_text(ui->screen_imgbtn_4_label, "");
	lv_label_set_long_mode(ui->screen_imgbtn_4_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_imgbtn_4_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_imgbtn_4, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->screen_imgbtn_4, 113, 3);
	lv_obj_set_size(ui->screen_imgbtn_4, 38, 35);

	//Write style for screen_imgbtn_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_imgbtn_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_imgbtn_4, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_imgbtn_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_imgbtn_4, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_4, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_4, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for screen_imgbtn_4, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->screen_imgbtn_4, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_imgbtn_4, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_imgbtn_4, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_imgbtn_4, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_imgbtn_4, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->screen_imgbtn_4, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes screen_cont_wifi
	ui->screen_cont_wifi = lv_obj_create(ui->screen_tileview_main_settings);
	lv_obj_set_pos(ui->screen_cont_wifi, 207, 32);
	lv_obj_set_size(ui->screen_cont_wifi, 560, 447);
	lv_obj_set_scrollbar_mode(ui->screen_cont_wifi, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_add_flag(ui->screen_cont_wifi, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_wifi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_wifi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_8
	ui->screen_cont_8 = lv_obj_create(ui->screen_cont_wifi);
	lv_obj_set_pos(ui->screen_cont_8, 0, -1);
	lv_obj_set_size(ui->screen_cont_8, 560, 43);
	lv_obj_set_scrollbar_mode(ui->screen_cont_8, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_6
	ui->screen_label_6 = lv_label_create(ui->screen_cont_8);
	lv_label_set_text(ui->screen_label_6, "Wifi");
	lv_label_set_long_mode(ui->screen_label_6, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_6, 2, 5);
	lv_obj_set_size(ui->screen_label_6, 100, 32);

	//Write style for screen_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_6, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_6, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_1
	ui->screen_sw_1 = lv_switch_create(ui->screen_cont_8);
	lv_obj_set_pos(ui->screen_sw_1, 116, 2);
	lv_obj_set_size(ui->screen_sw_1, 40, 20);

	//Write style for screen_sw_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_1, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_1, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_1, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_1, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_1, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_1, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_1, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_1, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_9
	ui->screen_cont_9 = lv_obj_create(ui->screen_cont_wifi);
	lv_obj_set_pos(ui->screen_cont_9, -4, 70);
	lv_obj_set_size(ui->screen_cont_9, 560, 376);
	lv_obj_set_scrollbar_mode(ui->screen_cont_9, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_9, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_10
	ui->screen_cont_10 = lv_obj_create(ui->screen_cont_9);
	lv_obj_set_pos(ui->screen_cont_10, 8, 0);
	lv_obj_set_size(ui->screen_cont_10, 550, 62);
	lv_obj_set_scrollbar_mode(ui->screen_cont_10, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_10, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_10, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_7
	ui->screen_label_7 = lv_label_create(ui->screen_cont_10);
	lv_label_set_text(ui->screen_label_7, "Wifi 1");
	lv_label_set_long_mode(ui->screen_label_7, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_7, 6, 16);
	lv_obj_set_size(ui->screen_label_7, 100, 32);

	//Write style for screen_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_7, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_7, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_1
	ui->screen_btn_1 = lv_btn_create(ui->screen_cont_10);
	ui->screen_btn_1_label = lv_label_create(ui->screen_btn_1);
	lv_label_set_text(ui->screen_btn_1_label, "Connect");
	lv_label_set_long_mode(ui->screen_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_btn_1_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_btn_1, 443, 5);
	lv_obj_set_size(ui->screen_btn_1, 100, 50);

	//Write style for screen_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_1, lv_color_hex(0x5597ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_ble
	ui->screen_cont_ble = lv_obj_create(ui->screen_tileview_main_settings);
	lv_obj_set_pos(ui->screen_cont_ble, 207, 32);
	lv_obj_set_size(ui->screen_cont_ble, 560, 447);
	lv_obj_set_scrollbar_mode(ui->screen_cont_ble, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_add_flag(ui->screen_cont_ble, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_ble, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_ble, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_14
	ui->screen_cont_14 = lv_obj_create(ui->screen_cont_ble);
	lv_obj_set_pos(ui->screen_cont_14, 0, -1);
	lv_obj_set_size(ui->screen_cont_14, 560, 43);
	lv_obj_set_scrollbar_mode(ui->screen_cont_14, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_14, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_14, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_9
	ui->screen_label_9 = lv_label_create(ui->screen_cont_14);
	lv_label_set_text(ui->screen_label_9, "Bluetooth");
	lv_label_set_long_mode(ui->screen_label_9, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_9, 2, 5);
	lv_obj_set_size(ui->screen_label_9, 100, 32);

	//Write style for screen_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_9, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_9, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_2
	ui->screen_sw_2 = lv_switch_create(ui->screen_cont_14);
	lv_obj_set_pos(ui->screen_sw_2, 143, 2);
	lv_obj_set_size(ui->screen_sw_2, 40, 20);

	//Write style for screen_sw_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_2, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_2, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_sw_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_sw_2, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
	lv_obj_set_style_bg_opa(ui->screen_sw_2, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_sw_2, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_2, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_sw_2, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style for screen_sw_2, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_sw_2, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_sw_2, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_sw_2, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_sw_2, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_sw_2, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_cont_12
	ui->screen_cont_12 = lv_obj_create(ui->screen_cont_ble);
	lv_obj_set_pos(ui->screen_cont_12, -4, 70);
	lv_obj_set_size(ui->screen_cont_12, 560, 376);
	lv_obj_set_scrollbar_mode(ui->screen_cont_12, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->screen_cont_12, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_13
	ui->screen_cont_13 = lv_obj_create(ui->screen_cont_12);
	lv_obj_set_pos(ui->screen_cont_13, 8, 0);
	lv_obj_set_size(ui->screen_cont_13, 550, 62);
	lv_obj_set_scrollbar_mode(ui->screen_cont_13, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_13, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_13, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_8
	ui->screen_label_8 = lv_label_create(ui->screen_cont_13);
	lv_label_set_text(ui->screen_label_8, "Bluetooth 1");
	lv_label_set_long_mode(ui->screen_label_8, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_8, -2, 20);
	lv_obj_set_size(ui->screen_label_8, 120, 32);

	//Write style for screen_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_8, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_8, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_8, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_btn_2
	ui->screen_btn_2 = lv_btn_create(ui->screen_cont_13);
	ui->screen_btn_2_label = lv_label_create(ui->screen_btn_2);
	lv_label_set_text(ui->screen_btn_2_label, "Connect");
	lv_label_set_long_mode(ui->screen_btn_2_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->screen_btn_2_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->screen_btn_2, 0, LV_STATE_DEFAULT);
	lv_obj_set_width(ui->screen_btn_2_label, LV_PCT(100));
	lv_obj_set_pos(ui->screen_btn_2, 443, 5);
	lv_obj_set_size(ui->screen_btn_2, 100, 50);

	//Write style for screen_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->screen_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_btn_2, lv_color_hex(0x5597ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_btn_2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_lang
	ui->screen_cont_lang = lv_obj_create(ui->screen_tileview_main_settings);
	lv_obj_set_pos(ui->screen_cont_lang, 207, 32);
	lv_obj_set_size(ui->screen_cont_lang, 560, 447);
	lv_obj_set_scrollbar_mode(ui->screen_cont_lang, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_add_flag(ui->screen_cont_lang, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_lang, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_lang, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_18
	ui->screen_cont_18 = lv_obj_create(ui->screen_cont_lang);
	lv_obj_set_pos(ui->screen_cont_18, 0, -1);
	lv_obj_set_size(ui->screen_cont_18, 560, 43);
	lv_obj_set_scrollbar_mode(ui->screen_cont_18, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_18, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_18, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_11
	ui->screen_label_11 = lv_label_create(ui->screen_cont_18);
	lv_label_set_text(ui->screen_label_11, "Language");
	lv_label_set_long_mode(ui->screen_label_11, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_11, 2, 5);
	lv_obj_set_size(ui->screen_label_11, 123, 32);

	//Write style for screen_label_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_11, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_11, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_11, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_11, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_16
	ui->screen_cont_16 = lv_obj_create(ui->screen_cont_lang);
	lv_obj_set_pos(ui->screen_cont_16, -4, 70);
	lv_obj_set_size(ui->screen_cont_16, 560, 376);
	lv_obj_set_scrollbar_mode(ui->screen_cont_16, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_16, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_16, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_ddlist_1
	ui->screen_ddlist_1 = lv_dropdown_create(ui->screen_cont_16);
	lv_dropdown_set_options(ui->screen_ddlist_1, "Vietnamese\nEnglish");
	lv_obj_set_pos(ui->screen_ddlist_1, 13, 5);
	lv_obj_set_size(ui->screen_ddlist_1, 322, 30);

	//Write style for screen_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->screen_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_ddlist_1, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui->screen_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_ddlist_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_1, 126, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->screen_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_text_color(ui->screen_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->screen_ddlist_1, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_width(ui->screen_ddlist_1, 1, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_color(ui->screen_ddlist_1, lv_color_hex(0x654f4f), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_border_side(ui->screen_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_pad_top(ui->screen_ddlist_1, 8, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_pad_left(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_pad_right(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_radius(ui->screen_ddlist_1, 3, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_bg_color(ui->screen_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for screen_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_FOCUSED.
	lv_obj_set_style_text_color(ui->screen_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_text_font(ui->screen_ddlist_1, &lv_font_montserratMedium_14, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_text_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_width(ui->screen_ddlist_1, 1, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_color(ui->screen_ddlist_1, lv_color_hex(0x654f4f), LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_border_side(ui->screen_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_pad_top(ui->screen_ddlist_1, 8, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_pad_left(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_pad_right(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_radius(ui->screen_ddlist_1, 3, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_bg_color(ui->screen_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_1, 0, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style for screen_ddlist_1, Part: LV_PART_MAIN, State: LV_STATE_DISABLED.
	lv_obj_set_style_text_color(ui->screen_ddlist_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_text_font(ui->screen_ddlist_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_text_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_width(ui->screen_ddlist_1, 1, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_color(ui->screen_ddlist_1, lv_color_hex(0xe1e6ee), LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_border_side(ui->screen_ddlist_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_pad_top(ui->screen_ddlist_1, 8, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_pad_left(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_pad_right(ui->screen_ddlist_1, 6, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_radius(ui->screen_ddlist_1, 3, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_bg_opa(ui->screen_ddlist_1, 255, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_bg_color(ui->screen_ddlist_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_bg_grad_dir(ui->screen_ddlist_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_set_style_shadow_width(ui->screen_ddlist_1, 0, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for &style_screen_ddlist_1_extra_list_selected_checked
	static lv_style_t style_screen_ddlist_1_extra_list_selected_checked;
	ui_init_style(&style_screen_ddlist_1_extra_list_selected_checked);
	
	lv_style_set_text_color(&style_screen_ddlist_1_extra_list_selected_checked, lv_color_hex(0xffffff));
	lv_style_set_text_font(&style_screen_ddlist_1_extra_list_selected_checked, &lv_font_montserratMedium_14);
	lv_style_set_text_opa(&style_screen_ddlist_1_extra_list_selected_checked, 255);
	lv_style_set_border_width(&style_screen_ddlist_1_extra_list_selected_checked, 2);
	lv_style_set_border_opa(&style_screen_ddlist_1_extra_list_selected_checked, 255);
	lv_style_set_border_color(&style_screen_ddlist_1_extra_list_selected_checked, lv_color_hex(0x654f4f));
	lv_style_set_border_side(&style_screen_ddlist_1_extra_list_selected_checked, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_screen_ddlist_1_extra_list_selected_checked, 3);
	lv_style_set_bg_opa(&style_screen_ddlist_1_extra_list_selected_checked, 110);
	lv_style_set_bg_color(&style_screen_ddlist_1_extra_list_selected_checked, lv_color_hex(0x000000));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_1_extra_list_selected_checked, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_1), &style_screen_ddlist_1_extra_list_selected_checked, LV_PART_SELECTED|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for &style_screen_ddlist_1_extra_list_main_default
	static lv_style_t style_screen_ddlist_1_extra_list_main_default;
	ui_init_style(&style_screen_ddlist_1_extra_list_main_default);
	
	lv_style_set_max_height(&style_screen_ddlist_1_extra_list_main_default, 90);
	lv_style_set_text_color(&style_screen_ddlist_1_extra_list_main_default, lv_color_hex(0xffffff));
	lv_style_set_text_font(&style_screen_ddlist_1_extra_list_main_default, &lv_font_montserratMedium_14);
	lv_style_set_text_opa(&style_screen_ddlist_1_extra_list_main_default, 255);
	lv_style_set_border_width(&style_screen_ddlist_1_extra_list_main_default, 1);
	lv_style_set_border_opa(&style_screen_ddlist_1_extra_list_main_default, 255);
	lv_style_set_border_color(&style_screen_ddlist_1_extra_list_main_default, lv_color_hex(0x654f4f));
	lv_style_set_border_side(&style_screen_ddlist_1_extra_list_main_default, LV_BORDER_SIDE_FULL);
	lv_style_set_radius(&style_screen_ddlist_1_extra_list_main_default, 3);
	lv_style_set_bg_opa(&style_screen_ddlist_1_extra_list_main_default, 138);
	lv_style_set_bg_color(&style_screen_ddlist_1_extra_list_main_default, lv_color_hex(0x000000));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_1_extra_list_main_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_1), &style_screen_ddlist_1_extra_list_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_ddlist_1_extra_list_scrollbar_default
	static lv_style_t style_screen_ddlist_1_extra_list_scrollbar_default;
	ui_init_style(&style_screen_ddlist_1_extra_list_scrollbar_default);
	
	lv_style_set_radius(&style_screen_ddlist_1_extra_list_scrollbar_default, 3);
	lv_style_set_bg_opa(&style_screen_ddlist_1_extra_list_scrollbar_default, 255);
	lv_style_set_bg_color(&style_screen_ddlist_1_extra_list_scrollbar_default, lv_color_hex(0x000000));
	lv_style_set_bg_grad_dir(&style_screen_ddlist_1_extra_list_scrollbar_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(lv_dropdown_get_list(ui->screen_ddlist_1), &style_screen_ddlist_1_extra_list_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write codes screen_cont_noti
	ui->screen_cont_noti = lv_obj_create(ui->screen_tileview_main_settings);
	lv_obj_set_pos(ui->screen_cont_noti, 207, 32);
	lv_obj_set_size(ui->screen_cont_noti, 560, 447);
	lv_obj_set_scrollbar_mode(ui->screen_cont_noti, LV_SCROLLBAR_MODE_ACTIVE);
	lv_obj_add_flag(ui->screen_cont_noti, LV_OBJ_FLAG_HIDDEN);

	//Write style for screen_cont_noti, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_noti, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_21
	ui->screen_cont_21 = lv_obj_create(ui->screen_cont_noti);
	lv_obj_set_pos(ui->screen_cont_21, 0, -1);
	lv_obj_set_size(ui->screen_cont_21, 560, 40);
	lv_obj_set_scrollbar_mode(ui->screen_cont_21, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_21, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_21, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_12
	ui->screen_label_12 = lv_label_create(ui->screen_cont_21);
	lv_label_set_text(ui->screen_label_12, "Notification");
	lv_label_set_long_mode(ui->screen_label_12, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->screen_label_12, 2, 5);
	lv_obj_set_size(ui->screen_label_12, 155, 32);

	//Write style for screen_label_12, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->screen_label_12, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->screen_label_12, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->screen_label_12, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->screen_label_12, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->screen_label_12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_label_12, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_cont_20
	ui->screen_cont_20 = lv_obj_create(ui->screen_cont_noti);
	lv_obj_set_pos(ui->screen_cont_20, -4, 70);
	lv_obj_set_size(ui->screen_cont_20, 560, 376);
	lv_obj_set_scrollbar_mode(ui->screen_cont_20, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_cont_20, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->screen_cont_20, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_list_1
	ui->screen_list_1 = lv_list_create(ui->screen_cont_20);
	ui->screen_list_1_item0 = lv_list_add_btn(ui->screen_list_1, LV_SYMBOL_BATTERY_1, "Battery is running low");
	ui->screen_list_1_item1 = lv_list_add_btn(ui->screen_list_1, LV_SYMBOL_BELL, "New software is  available");
	ui->screen_list_1_item2 = lv_list_add_btn(ui->screen_list_1, LV_SYMBOL_CHARGE, "SW1 is turning on");
	lv_obj_set_pos(ui->screen_list_1, 15, -3);
	lv_obj_set_size(ui->screen_list_1, 522, 380);
	lv_obj_set_scrollbar_mode(ui->screen_list_1, LV_SCROLLBAR_MODE_ACTIVE);

	//Write style state: LV_STATE_DEFAULT for &style_screen_list_1_main_main_default
	static lv_style_t style_screen_list_1_main_main_default;
	ui_init_style(&style_screen_list_1_main_main_default);
	
	lv_style_set_pad_top(&style_screen_list_1_main_main_default, 5);
	lv_style_set_pad_left(&style_screen_list_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_list_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_list_1_main_main_default, 0);
	lv_style_set_bg_opa(&style_screen_list_1_main_main_default, 0);
	lv_style_set_border_width(&style_screen_list_1_main_main_default, 0);
	lv_style_set_radius(&style_screen_list_1_main_main_default, 7);
	lv_style_set_shadow_width(&style_screen_list_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_list_1, &style_screen_list_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_list_1_main_scrollbar_default
	static lv_style_t style_screen_list_1_main_scrollbar_default;
	ui_init_style(&style_screen_list_1_main_scrollbar_default);
	
	lv_style_set_radius(&style_screen_list_1_main_scrollbar_default, 3);
	lv_style_set_bg_opa(&style_screen_list_1_main_scrollbar_default, 255);
	lv_style_set_bg_color(&style_screen_list_1_main_scrollbar_default, lv_color_hex(0xffffff));
	lv_style_set_bg_grad_dir(&style_screen_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
	lv_obj_add_style(ui->screen_list_1, &style_screen_list_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_list_1_extra_btns_main_default
	static lv_style_t style_screen_list_1_extra_btns_main_default;
	ui_init_style(&style_screen_list_1_extra_btns_main_default);
	
	lv_style_set_pad_top(&style_screen_list_1_extra_btns_main_default, 5);
	lv_style_set_pad_left(&style_screen_list_1_extra_btns_main_default, 10);
	lv_style_set_pad_right(&style_screen_list_1_extra_btns_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_list_1_extra_btns_main_default, 10);
	lv_style_set_border_width(&style_screen_list_1_extra_btns_main_default, 1);
	lv_style_set_border_opa(&style_screen_list_1_extra_btns_main_default, 255);
	lv_style_set_border_color(&style_screen_list_1_extra_btns_main_default, lv_color_hex(0x6f6969));
	lv_style_set_border_side(&style_screen_list_1_extra_btns_main_default, LV_BORDER_SIDE_FULL);
	lv_style_set_text_color(&style_screen_list_1_extra_btns_main_default, lv_color_hex(0xfef6ea));
	lv_style_set_text_font(&style_screen_list_1_extra_btns_main_default, &lv_font_FontAwesome5_18);
	lv_style_set_text_opa(&style_screen_list_1_extra_btns_main_default, 255);
	lv_style_set_radius(&style_screen_list_1_extra_btns_main_default, 0);
	lv_style_set_bg_opa(&style_screen_list_1_extra_btns_main_default, 0);
	lv_obj_add_style(ui->screen_list_1_item2, &style_screen_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_list_1_item1, &style_screen_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_list_1_item0, &style_screen_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_list_1_extra_texts_main_default
	static lv_style_t style_screen_list_1_extra_texts_main_default;
	ui_init_style(&style_screen_list_1_extra_texts_main_default);
	
	lv_style_set_pad_top(&style_screen_list_1_extra_texts_main_default, 6);
	lv_style_set_pad_left(&style_screen_list_1_extra_texts_main_default, 5);
	lv_style_set_pad_right(&style_screen_list_1_extra_texts_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_list_1_extra_texts_main_default, 0);
	lv_style_set_border_width(&style_screen_list_1_extra_texts_main_default, 0);
	lv_style_set_text_color(&style_screen_list_1_extra_texts_main_default, lv_color_hex(0x0D3055));
	lv_style_set_text_font(&style_screen_list_1_extra_texts_main_default, &lv_font_montserratMedium_18);
	lv_style_set_text_opa(&style_screen_list_1_extra_texts_main_default, 255);
	lv_style_set_radius(&style_screen_list_1_extra_texts_main_default, 3);
	lv_style_set_bg_opa(&style_screen_list_1_extra_texts_main_default, 0);

	//The custom code of screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->screen);

	//Init events for screen.
	events_init_screen(ui);
}
