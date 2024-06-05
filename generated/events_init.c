/*
* Copyright 2024 NXP
* NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void screen_label_14_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_cont_sw, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_timer, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_cont_led, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_5_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_cont_timer, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_sw, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_cont_led, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_label_15_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_cont_timer, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_cont_sw, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_led, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_6_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_cont_timer, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_cont_sw, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_led, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_label_16_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_cont_timer, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_sw, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_led, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_7_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_cont_timer, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_sw, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_led, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_14_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_win_1, LV_OBJ_FLAG_HIDDEN);
		ui_move_animation(guider_ui.screen_win_1, 1000, 0, 490, 0, &lv_anim_path_linear, 0, 0, 0, 0, NULL, NULL, NULL);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_15_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_win_1, LV_OBJ_FLAG_HIDDEN);
		ui_move_animation(guider_ui.screen_win_1, 1000, 0, 490, 0, &lv_anim_path_linear, 0, 0, 0, 0, NULL, NULL, NULL);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_16_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_win_1, LV_OBJ_FLAG_HIDDEN);
		ui_move_animation(guider_ui.screen_win_1, 1000, 0, 490, 0, &lv_anim_path_linear, 0, 0, 0, 0, NULL, NULL, NULL);
		break;
	}
	default:
		break;
	}
}
static void screen_win_1_item0_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_win_1, LV_OBJ_FLAG_HIDDEN);
		ui_move_animation(guider_ui.screen_win_1, 1000, 0, 810, 0, &lv_anim_path_linear, 0, 0, 0, 0, NULL, NULL, NULL);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_cont_wifi, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_ble, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_lang, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_noti, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_2_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_cont_wifi, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_cont_ble, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_lang, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_noti, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_3_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_add_flag(guider_ui.screen_cont_noti, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(guider_ui.screen_cont_lang, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_ble, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_wifi, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_imgbtn_4_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_CLICKED:
	{
		lv_obj_clear_flag(guider_ui.screen_cont_noti, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_lang, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_ble, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(guider_ui.screen_cont_wifi, LV_OBJ_FLAG_HIDDEN);
		break;
	}
	default:
		break;
	}
}
static void screen_sw_1_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		lv_obj_t * status_obj = lv_event_get_target(e);
		int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? 1 : 0;
		switch(status) {
		case 0:
		{
			lv_obj_add_flag(guider_ui.screen_cont_9, LV_OBJ_FLAG_HIDDEN);
			break;
		}
		case 1:
		{
			lv_obj_clear_flag(guider_ui.screen_cont_9, LV_OBJ_FLAG_HIDDEN);
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
static void screen_sw_2_event_handler (lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);

	switch (code) {
	case LV_EVENT_VALUE_CHANGED:
	{
		lv_obj_t * status_obj = lv_event_get_target(e);
		int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? 1 : 0;
		switch(status) {
		case 0:
		{
			lv_obj_add_flag(guider_ui.screen_cont_12, LV_OBJ_FLAG_HIDDEN);
			break;
		}
		case 1:
		{
			lv_obj_clear_flag(guider_ui.screen_cont_12, LV_OBJ_FLAG_HIDDEN);
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
void events_init_screen(lv_ui *ui)
{
	lv_obj_add_event_cb(ui->screen_label_14, screen_label_14_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_5, screen_imgbtn_5_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_label_15, screen_label_15_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_6, screen_imgbtn_6_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_label_16, screen_label_16_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_7, screen_imgbtn_7_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_14, screen_imgbtn_14_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_15, screen_imgbtn_15_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_16, screen_imgbtn_16_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_win_1_item0, screen_win_1_item0_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_1, screen_imgbtn_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_2, screen_imgbtn_2_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_3, screen_imgbtn_3_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_imgbtn_4, screen_imgbtn_4_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_sw_1, screen_sw_1_event_handler, LV_EVENT_ALL, ui);
	lv_obj_add_event_cb(ui->screen_sw_2, screen_sw_2_event_handler, LV_EVENT_ALL, ui);
}

void events_init(lv_ui *ui)
{

}
