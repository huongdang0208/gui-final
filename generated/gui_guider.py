# Copyright 2024 NXP
# NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

import SDL
import utime as time
import usys as sys
import lvgl as lv
import lodepng as png
import ustruct
import fs_driver

lv.init()
SDL.init(w=800,h=480)

# Register SDL display driver.
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytearray(800*10)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = SDL.monitor_flush
disp_drv.hor_res = 800
disp_drv.ver_res = 480
disp_drv.register()

# Regsiter SDL mouse driver
indev_drv = lv.indev_drv_t()
indev_drv.init()
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = SDL.mouse_read
indev_drv.register()

fs_drv = lv.fs_drv_t()
fs_driver.fs_register(fs_drv, 'Z')

# Below: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

COLOR_SIZE = lv.color_t.__SIZE__
COLOR_IS_SWAPPED = hasattr(lv.color_t().ch,'green_h')

class lodepng_error(RuntimeError):
    def __init__(self, err):
        if type(err) is int:
            super().__init__(png.error_text(err))
        else:
            super().__init__(err)

# Parse PNG file header
# Taken from https://github.com/shibukawa/imagesize_py/blob/ffef30c1a4715c5acf90e8945ceb77f4a2ed2d45/imagesize.py#L63-L85

def get_png_info(decoder, src, header):
    # Only handle variable image types

    if lv.img.src_get_type(src) != lv.img.SRC.VARIABLE:
        return lv.RES.INV

    data = lv.img_dsc_t.__cast__(src).data
    if data == None:
        return lv.RES.INV

    png_header = bytes(data.__dereference__(24))

    if png_header.startswith(b'\211PNG\r\n\032\n'):
        if png_header[12:16] == b'IHDR':
            start = 16
        # Maybe this is for an older PNG version.
        else:
            start = 8
        try:
            width, height = ustruct.unpack(">LL", png_header[start:start+8])
        except ustruct.error:
            return lv.RES.INV
    else:
        return lv.RES.INV

    header.always_zero = 0
    header.w = width
    header.h = height
    header.cf = lv.img.CF.TRUE_COLOR_ALPHA

    return lv.RES.OK

def convert_rgba8888_to_bgra8888(img_view):
    for i in range(0, len(img_view), lv.color_t.__SIZE__):
        ch = lv.color_t.__cast__(img_view[i:i]).ch
        ch.red, ch.blue = ch.blue, ch.red

# Read and parse PNG file

def open_png(decoder, dsc):
    img_dsc = lv.img_dsc_t.__cast__(dsc.src)
    png_data = img_dsc.data
    png_size = img_dsc.data_size
    png_decoded = png.C_Pointer()
    png_width = png.C_Pointer()
    png_height = png.C_Pointer()
    error = png.decode32(png_decoded, png_width, png_height, png_data, png_size)
    if error:
        raise lodepng_error(error)
    img_size = png_width.int_val * png_height.int_val * 4
    img_data = png_decoded.ptr_val
    img_view = img_data.__dereference__(img_size)

    if COLOR_SIZE == 4:
        convert_rgba8888_to_bgra8888(img_view)
    else:
        raise lodepng_error("Error: Color mode not supported yet!")

    dsc.img_data = img_data
    return lv.RES.OK

# Above: Taken from https://github.com/lvgl/lv_binding_micropython/blob/master/driver/js/imagetools.py#L22-L94

decoder = lv.img.decoder_create()
decoder.info_cb = get_png_info
decoder.open_cb = open_png

def anim_x_cb(obj, v):
    obj.set_x(v)

def anim_y_cb(obj, v):
    obj.set_y(v)

def anim_width_cb(obj, v):
    obj.set_width(v)

def anim_height_cb(obj, v):
    obj.set_height(v)

def anim_img_zoom_cb(obj, v):
    obj.set_zoom(v)

def anim_img_rotate_cb(obj, v):
    obj.set_angle(v)

global_font_cache = {}
def test_font(font_family, font_size):
    global global_font_cache
    if font_family + str(font_size) in global_font_cache:
        return global_font_cache[font_family + str(font_size)]
    if font_size % 2:
        candidates = [
            (font_family, font_size),
            (font_family, font_size-font_size%2),
            (font_family, font_size+font_size%2),
            ("montserrat", font_size-font_size%2),
            ("montserrat", font_size+font_size%2),
            ("montserrat", 16)
        ]
    else:
        candidates = [
            (font_family, font_size),
            ("montserrat", font_size),
            ("montserrat", 16)
        ]
    for (family, size) in candidates:
        try:
            if eval(f'lv.font_{family}_{size}'):
                global_font_cache[font_family + str(font_size)] = eval(f'lv.font_{family}_{size}')
                if family != font_family or size != font_size:
                    print(f'WARNING: lv.font_{family}_{size} is used!')
                return eval(f'lv.font_{family}_{size}')
        except AttributeError:
            try:
                load_font = lv.font_load(f"Z:MicroPython/lv_font_{family}_{size}.fnt")
                global_font_cache[font_family + str(font_size)] = load_font
                return load_font
            except:
                if family == font_family and size == font_size:
                    print(f'WARNING: lv.font_{family}_{size} is NOT supported!')

global_image_cache = {}
def load_image(file):
    global global_image_cache
    if file in global_image_cache:
        return global_image_cache[file]
    try:
        with open(file,'rb') as f:
            data = f.read()
    except:
        print(f'Could not open {file}')
        sys.exit()

    img = lv.img_dsc_t({
        'data_size': len(data),
        'data': data
    })
    global_image_cache[file] = img
    return img

def calendar_event_handler(e,obj):
    code = e.get_code()

    if code == lv.EVENT.VALUE_CHANGED:
        source = e.get_current_target()
        date = lv.calendar_date_t()
        if source.get_pressed_date(date) == lv.RES.OK:
            source.set_highlighted_dates([date], 1)

def spinbox_increment_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.increment()
def spinbox_decrement_event_cb(e, obj):
    code = e.get_code()
    if code == lv.EVENT.SHORT_CLICKED or code == lv.EVENT.LONG_PRESSED_REPEAT:
        obj.decrement()

def digital_clock_cb(timer, obj, current_time, show_second, use_ampm):
    hour = int(current_time[0])
    minute = int(current_time[1])
    second = int(current_time[2])
    ampm = current_time[3]
    second = second + 1
    if second == 60:
        second = 0
        minute = minute + 1
        if minute == 60:
            minute = 0
            hour = hour + 1
            if use_ampm:
                if hour == 12:
                    if ampm == 'AM':
                        ampm = 'PM'
                    elif ampm == 'PM':
                        ampm = 'AM'
                if hour > 12:
                    hour = hour % 12
    hour = hour % 24
    if use_ampm:
        if show_second:
            obj.set_text("%d:%02d:%02d %s" %(hour, minute, second, ampm))
        else:
            obj.set_text("%d:%02d %s" %(hour, minute, ampm))
    else:
        if show_second:
            obj.set_text("%d:%02d:%02d" %(hour, minute, second))
        else:
            obj.set_text("%d:%02d" %(hour, minute))
    current_time[0] = hour
    current_time[1] = minute
    current_time[2] = second
    current_time[3] = ampm

def analog_clock_cb(timer, obj):
    datetime = time.localtime()
    hour = datetime[3]
    if hour >= 12: hour = hour - 12
    obj.set_time(hour, datetime[4], datetime[5])

def datetext_event_handler(e, obj):
    code = e.get_code()
    target = e.get_target()
    if code == lv.EVENT.FOCUSED:
        if obj is None:
            bg = lv.layer_top()
            bg.add_flag(lv.obj.FLAG.CLICKABLE)
            obj = lv.calendar(bg)
            scr = target.get_screen()
            scr_height = scr.get_height()
            scr_width = scr.get_width()
            obj.set_size(int(scr_width * 0.8), int(scr_height * 0.8))
            datestring = target.get_text()
            year = int(datestring.split('/')[0])
            month = int(datestring.split('/')[1])
            day = int(datestring.split('/')[2])
            obj.set_showed_date(year, month)
            highlighted_days=[lv.calendar_date_t({'year':year, 'month':month, 'day':day})]
            obj.set_highlighted_dates(highlighted_days, 1)
            obj.align(lv.ALIGN.CENTER, 0, 0)
            lv.calendar_header_arrow(obj)
            obj.add_event_cb(lambda e: datetext_calendar_event_handler(e, target), lv.EVENT.ALL, None)
            scr.update_layout()

def datetext_calendar_event_handler(e, obj):
    code = e.get_code()
    target = e.get_current_target()
    if code == lv.EVENT.VALUE_CHANGED:
        date = lv.calendar_date_t()
        if target.get_pressed_date(date) == lv.RES.OK:
            obj.set_text(f"{date.year}/{date.month}/{date.day}")
            bg = lv.layer_top()
            bg.clear_flag(lv.obj.FLAG.CLICKABLE)
            bg.set_style_bg_opa(lv.OPA.TRANSP, 0)
            target.delete()

# Create screen
screen = lv.obj()
screen.set_size(800, 480)
screen.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_tileview_main
screen_tileview_main = lv.tileview(screen)
screen_tileview_main_hub_screen = screen_tileview_main.add_tile(0, 0, lv.DIR.BOTTOM)
screen_tileview_main_settings = screen_tileview_main.add_tile(0, 1, lv.DIR.TOP)
screen_tileview_main.set_pos(0, 0)
screen_tileview_main.set_size(800, 480)
screen_tileview_main.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_tileview_main, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_tileview_main.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_tileview_main.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_tileview_main.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_tileview_main.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_tileview_main.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_tileview_main, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
screen_tileview_main.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_tileview_main.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_tileview_main.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_tileview_main.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create screen_tileview_hub
screen_tileview_hub = lv.tileview(screen_tileview_main_hub_screen)
screen_tileview_hub_home = screen_tileview_hub.add_tile(0, 0, lv.DIR.RIGHT)
screen_tileview_hub_home_control = screen_tileview_hub.add_tile(1, 0, lv.DIR.LEFT)
screen_tileview_hub.set_pos(0, 0)
screen_tileview_hub.set_size(800, 480)
screen_tileview_hub.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_tileview_hub, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_tileview_hub.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_tileview_hub.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_tileview_hub.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_tileview_hub, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
screen_tileview_hub.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_tileview_hub.set_style_bg_color(lv.color_hex(0xeaeff3), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_tileview_hub.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_tileview_hub.set_style_radius(0, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create screen_imgbtn_22
screen_imgbtn_22 = lv.imgbtn(screen_tileview_hub_home)
screen_imgbtn_22.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_22.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_background_alpha_800x480.bin", None)
screen_imgbtn_22.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_22_label = lv.label(screen_imgbtn_22)
screen_imgbtn_22_label.set_text("")
screen_imgbtn_22_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_22_label.set_width(lv.pct(100))
screen_imgbtn_22_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_22.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_22.set_pos(0, 0)
screen_imgbtn_22.set_size(800, 480)
# Set style for screen_imgbtn_22, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_22.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_22.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_22.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_22, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_22.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_22.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_22.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_22, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_22.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_22.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_22.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_22, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_22.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_date
screen_cont_date = lv.obj(screen_tileview_hub_home)
screen_cont_date.set_pos(472, 280)
screen_cont_date.set_size(326, 200)
screen_cont_date.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_date, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_date.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_date.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_47
screen_label_47 = lv.label(screen_cont_date)
screen_label_47.set_text("11 May, 2024")
screen_label_47.set_long_mode(lv.label.LONG.WRAP)
screen_label_47.set_width(lv.pct(100))
screen_label_47.set_pos(127, 91)
screen_label_47.set_size(183, 32)
# Set style for screen_label_47, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_47.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_text_font(test_font("montserratMedium", 24), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_47.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_label_48
screen_label_48 = lv.label(screen_cont_date)
screen_label_48.set_text("9:00 AM")
screen_label_48.set_long_mode(lv.label.LONG.WRAP)
screen_label_48.set_width(lv.pct(100))
screen_label_48.set_pos(139, 137)
screen_label_48.set_size(183, 32)
# Set style for screen_label_48, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_48.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_text_font(test_font("Antonio_Regular", 32), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_48.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_sensor
screen_cont_sensor = lv.obj(screen_tileview_hub_home)
screen_cont_sensor.set_pos(455, -1)
screen_cont_sensor.set_size(344, 60)
screen_cont_sensor.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_sensor, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_sensor.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sensor.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_imgbtn_23
screen_imgbtn_23 = lv.imgbtn(screen_cont_sensor)
screen_imgbtn_23.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_23.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_itemperature_alpha_40x30.bin", None)
screen_imgbtn_23.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_23_label = lv.label(screen_imgbtn_23)
screen_imgbtn_23_label.set_text("")
screen_imgbtn_23_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_23_label.set_width(lv.pct(100))
screen_imgbtn_23_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_23.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_23.set_pos(113, 3)
screen_imgbtn_23.set_size(40, 30)
# Set style for screen_imgbtn_23, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_23.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_23.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_23.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_23.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_23.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_23, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_23.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_23.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_23.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_23.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_23.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_23, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_23.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_23.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_23.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_23.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_23.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_23, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_23.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_label_49
screen_label_49 = lv.label(screen_cont_sensor)
screen_label_49.set_text("25°C")
screen_label_49.set_long_mode(lv.label.LONG.WRAP)
screen_label_49.set_width(lv.pct(100))
screen_label_49.set_pos(149, 6)
screen_label_49.set_size(63, 24)
# Set style for screen_label_49, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_49.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_text_font(test_font("Alatsi_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_pad_top(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_49.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_24
screen_imgbtn_24 = lv.imgbtn(screen_cont_sensor)
screen_imgbtn_24.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_24.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_ihumidity_alpha_31x28.bin", None)
screen_imgbtn_24.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_24_label = lv.label(screen_imgbtn_24)
screen_imgbtn_24_label.set_text("")
screen_imgbtn_24_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_24_label.set_width(lv.pct(100))
screen_imgbtn_24_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_24.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_24.set_pos(233, 4)
screen_imgbtn_24.set_size(31, 28)
# Set style for screen_imgbtn_24, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_24.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_24.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_24.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_24.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_24, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_24.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_24.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_24.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_24.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_24, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_24.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_24.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_24.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_24.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_24, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_24.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_label_50
screen_label_50 = lv.label(screen_cont_sensor)
screen_label_50.set_text("70%")
screen_label_50.set_long_mode(lv.label.LONG.WRAP)
screen_label_50.set_width(lv.pct(100))
screen_label_50.set_pos(272, 6)
screen_label_50.set_size(63, 24)
# Set style for screen_label_50, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_50.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_text_font(test_font("Alatsi_Regular", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_pad_top(3, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_50.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_hub_menu
screen_cont_hub_menu = lv.obj(screen_tileview_hub_home_control)
screen_cont_hub_menu.set_pos(0, 0)
screen_cont_hub_menu.set_size(175, 480)
screen_cont_hub_menu.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_hub_menu, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_hub_menu.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_border_side(lv.BORDER_SIDE.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_bg_opa(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_hub_menu.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_13
screen_label_13 = lv.label(screen_cont_hub_menu)
screen_label_13.set_text("Home")
screen_label_13.set_long_mode(lv.label.LONG.WRAP)
screen_label_13.set_width(lv.pct(100))
screen_label_13.set_pos(0, 0)
screen_label_13.set_size(175, 57)
# Set style for screen_label_13, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_13.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_text_font(test_font("Acme_Regular", 25), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_text_letter_space(1, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_pad_top(13, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_24
screen_cont_24 = lv.obj(screen_cont_hub_menu)
screen_cont_24.set_pos(7, 76)
screen_cont_24.set_size(161, 48)
screen_cont_24.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_24, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_24.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_24.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_14
screen_label_14 = lv.label(screen_cont_24)
screen_label_14.set_text("Lights")
screen_label_14.set_long_mode(lv.label.LONG.WRAP)
screen_label_14.set_width(lv.pct(100))
screen_label_14.set_pos(27, 5)
screen_label_14.set_size(120, 32)
# Set style for screen_label_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_14.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_5
screen_imgbtn_5 = lv.imgbtn(screen_cont_24)
screen_imgbtn_5.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_5.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_onlight_alpha_38x35.bin", None)
screen_imgbtn_5.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_5_label = lv.label(screen_imgbtn_5)
screen_imgbtn_5_label.set_text("")
screen_imgbtn_5_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_5_label.set_width(lv.pct(100))
screen_imgbtn_5_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_5.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_5.set_pos(0, 0)
screen_imgbtn_5.set_size(38, 35)
# Set style for screen_imgbtn_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_5.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_5.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_5, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_5.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_5.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_5.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_5, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_5.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_5.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_5.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_5, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_5.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_25
screen_cont_25 = lv.obj(screen_cont_hub_menu)
screen_cont_25.set_pos(8, 133)
screen_cont_25.set_size(161, 48)
screen_cont_25.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_25, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_25.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_25.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_15
screen_label_15 = lv.label(screen_cont_25)
screen_label_15.set_text("Switches")
screen_label_15.set_long_mode(lv.label.LONG.WRAP)
screen_label_15.set_width(lv.pct(100))
screen_label_15.set_pos(34, 5)
screen_label_15.set_size(120, 32)
# Set style for screen_label_15, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_15.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_6
screen_imgbtn_6 = lv.imgbtn(screen_cont_25)
screen_imgbtn_6.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_6.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_switch_alpha_37x33.bin", None)
screen_imgbtn_6.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_6_label = lv.label(screen_imgbtn_6)
screen_imgbtn_6_label.set_text("")
screen_imgbtn_6_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_6_label.set_width(lv.pct(100))
screen_imgbtn_6_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_6.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_6.set_pos(3, 5)
screen_imgbtn_6.set_size(37, 33)
# Set style for screen_imgbtn_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_6.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_6.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_6, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_6.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_6.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_6.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_6, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_6.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_6.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_6.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_6, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_6.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_26
screen_cont_26 = lv.obj(screen_cont_hub_menu)
screen_cont_26.set_pos(8, 188)
screen_cont_26.set_size(160, 48)
screen_cont_26.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_26, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_26.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_26.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_16
screen_label_16 = lv.label(screen_cont_26)
screen_label_16.set_text("Set timer")
screen_label_16.set_long_mode(lv.label.LONG.WRAP)
screen_label_16.set_width(lv.pct(100))
screen_label_16.set_pos(27, 5)
screen_label_16.set_size(120, 32)
# Set style for screen_label_16, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_16.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_16.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_7
screen_imgbtn_7 = lv.imgbtn(screen_cont_26)
screen_imgbtn_7.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_7.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_clock_alpha_32x32.bin", None)
screen_imgbtn_7.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_7_label = lv.label(screen_imgbtn_7)
screen_imgbtn_7_label.set_text("")
screen_imgbtn_7_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_7_label.set_width(lv.pct(100))
screen_imgbtn_7_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_7.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_7.set_pos(6, 5)
screen_imgbtn_7.set_size(32, 32)
# Set style for screen_imgbtn_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_7.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_7.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_7.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_7, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_7.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_7.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_7.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_7, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_7.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_7.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_7.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_7, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_7.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_led
screen_cont_led = lv.obj(screen_tileview_hub_home_control)
screen_cont_led.set_pos(204, 26)
screen_cont_led.set_size(563, 453)
screen_cont_led.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_led.set_scrollbar_mode(lv.SCROLLBAR_MODE.AUTO)
# Set style for screen_cont_led, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_led.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_led.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_29
screen_cont_29 = lv.obj(screen_cont_led)
screen_cont_29.set_pos(25, 20)
screen_cont_29.set_size(240, 61)
screen_cont_29.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_29, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_29.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_29.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_17
screen_label_17 = lv.label(screen_cont_29)
screen_label_17.set_text("Light 1")
screen_label_17.set_long_mode(lv.label.LONG.WRAP)
screen_label_17.set_width(lv.pct(100))
screen_label_17.set_pos(85, 13)
screen_label_17.set_size(120, 32)
# Set style for screen_label_17, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_17.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_17.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_8
screen_imgbtn_8 = lv.imgbtn(screen_cont_29)
screen_imgbtn_8.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_8.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_54x52.bin", None)
screen_imgbtn_8.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_led_alpha_54x52.bin", None)
screen_imgbtn_8.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_8_label = lv.label(screen_imgbtn_8)
screen_imgbtn_8_label.set_text("")
screen_imgbtn_8_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_8_label.set_width(lv.pct(100))
screen_imgbtn_8_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_8.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_8.set_pos(25, 1)
screen_imgbtn_8.set_size(54, 52)
# Set style for screen_imgbtn_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_8.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_8.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_8.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_8, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_8.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_8.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_8.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_8, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_8.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_8.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_8.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_8, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_8.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_30
screen_cont_30 = lv.obj(screen_cont_led)
screen_cont_30.set_pos(297, 21)
screen_cont_30.set_size(240, 61)
screen_cont_30.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_30, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_30.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_30.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_18
screen_label_18 = lv.label(screen_cont_30)
screen_label_18.set_text("Light 2")
screen_label_18.set_long_mode(lv.label.LONG.WRAP)
screen_label_18.set_width(lv.pct(100))
screen_label_18.set_pos(85, 13)
screen_label_18.set_size(120, 32)
# Set style for screen_label_18, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_18.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_9
screen_imgbtn_9 = lv.imgbtn(screen_cont_30)
screen_imgbtn_9.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_9.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_54x52.bin", None)
screen_imgbtn_9.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_led_alpha_54x52.bin", None)
screen_imgbtn_9.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_9_label = lv.label(screen_imgbtn_9)
screen_imgbtn_9_label.set_text("")
screen_imgbtn_9_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_9_label.set_width(lv.pct(100))
screen_imgbtn_9_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_9.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_9.set_pos(25, 1)
screen_imgbtn_9.set_size(54, 52)
# Set style for screen_imgbtn_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_9.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_9.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_9.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_9, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_9.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_9.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_9.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_9, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_9.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_9.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_9.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_9, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_9.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_31
screen_cont_31 = lv.obj(screen_cont_led)
screen_cont_31.set_pos(25, 96)
screen_cont_31.set_size(240, 61)
screen_cont_31.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_31.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_31, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_31.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_31.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_19
screen_label_19 = lv.label(screen_cont_31)
screen_label_19.set_text("Light 3")
screen_label_19.set_long_mode(lv.label.LONG.WRAP)
screen_label_19.set_width(lv.pct(100))
screen_label_19.set_pos(85, 13)
screen_label_19.set_size(120, 32)
# Set style for screen_label_19, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_19.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_10
screen_imgbtn_10 = lv.imgbtn(screen_cont_31)
screen_imgbtn_10.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_10.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_54x52.bin", None)
screen_imgbtn_10.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_led_alpha_54x52.bin", None)
screen_imgbtn_10.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_10_label = lv.label(screen_imgbtn_10)
screen_imgbtn_10_label.set_text("")
screen_imgbtn_10_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_10_label.set_width(lv.pct(100))
screen_imgbtn_10_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_10.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_10.set_pos(25, 1)
screen_imgbtn_10.set_size(54, 52)
# Set style for screen_imgbtn_10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_10.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_10.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_10.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_10, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_10.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_10.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_10.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_10, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_10.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_10.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_10.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_10.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_10, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_10.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_32
screen_cont_32 = lv.obj(screen_cont_led)
screen_cont_32.set_pos(297, 96)
screen_cont_32.set_size(240, 61)
screen_cont_32.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_32.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_32, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_32.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_32.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_20
screen_label_20 = lv.label(screen_cont_32)
screen_label_20.set_text("Light 4")
screen_label_20.set_long_mode(lv.label.LONG.WRAP)
screen_label_20.set_width(lv.pct(100))
screen_label_20.set_pos(86, 13)
screen_label_20.set_size(120, 32)
# Set style for screen_label_20, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_20.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_11
screen_imgbtn_11 = lv.imgbtn(screen_cont_32)
screen_imgbtn_11.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_11.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_54x52.bin", None)
screen_imgbtn_11.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_led_alpha_54x52.bin", None)
screen_imgbtn_11.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_11_label = lv.label(screen_imgbtn_11)
screen_imgbtn_11_label.set_text("")
screen_imgbtn_11_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_11_label.set_width(lv.pct(100))
screen_imgbtn_11_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_11.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_11.set_pos(25, 1)
screen_imgbtn_11.set_size(54, 52)
# Set style for screen_imgbtn_11, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_11.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_11.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_11.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_11, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_11.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_11.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_11.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_11, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_11.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_11.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_11.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_11, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_11.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_33
screen_cont_33 = lv.obj(screen_cont_led)
screen_cont_33.set_pos(25, 169)
screen_cont_33.set_size(240, 61)
screen_cont_33.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_33.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_33, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_33.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_33.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_21
screen_label_21 = lv.label(screen_cont_33)
screen_label_21.set_text("Light 5")
screen_label_21.set_long_mode(lv.label.LONG.WRAP)
screen_label_21.set_width(lv.pct(100))
screen_label_21.set_pos(85, 13)
screen_label_21.set_size(120, 32)
# Set style for screen_label_21, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_21.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_12
screen_imgbtn_12 = lv.imgbtn(screen_cont_33)
screen_imgbtn_12.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_12.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_54x52.bin", None)
screen_imgbtn_12.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_led_alpha_54x52.bin", None)
screen_imgbtn_12.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_12_label = lv.label(screen_imgbtn_12)
screen_imgbtn_12_label.set_text("")
screen_imgbtn_12_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_12_label.set_width(lv.pct(100))
screen_imgbtn_12_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_12.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_12.set_pos(25, 1)
screen_imgbtn_12.set_size(54, 52)
# Set style for screen_imgbtn_12, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_12.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_12.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_12.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_12, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_12.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_12.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_12.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_12, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_12.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_12.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_12.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_12, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_12.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_34
screen_cont_34 = lv.obj(screen_cont_led)
screen_cont_34.set_pos(296, 169)
screen_cont_34.set_size(240, 61)
screen_cont_34.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_34.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_34, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_34.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_34.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_22
screen_label_22 = lv.label(screen_cont_34)
screen_label_22.set_text("Light 6")
screen_label_22.set_long_mode(lv.label.LONG.WRAP)
screen_label_22.set_width(lv.pct(100))
screen_label_22.set_pos(85, 12)
screen_label_22.set_size(120, 32)
# Set style for screen_label_22, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_22.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_22.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_13
screen_imgbtn_13 = lv.imgbtn(screen_cont_34)
screen_imgbtn_13.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_13.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_offlight_alpha_54x52.bin", None)
screen_imgbtn_13.set_src(lv.imgbtn.STATE.CHECKED_RELEASED, None, "B:MicroPython/_led_alpha_54x52.bin", None)
screen_imgbtn_13.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_13_label = lv.label(screen_imgbtn_13)
screen_imgbtn_13_label.set_text("")
screen_imgbtn_13_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_13_label.set_width(lv.pct(100))
screen_imgbtn_13_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_13.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_13.set_pos(25, 1)
screen_imgbtn_13.set_size(54, 52)
# Set style for screen_imgbtn_13, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_13.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_13.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_13.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_13, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_13.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_13.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_13.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_13, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_13.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_13.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_13.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_13.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_13, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_13.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_sw
screen_cont_sw = lv.obj(screen_tileview_hub_home_control)
screen_cont_sw.set_pos(204, 26)
screen_cont_sw.set_size(563, 453)
screen_cont_sw.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_sw.set_scrollbar_mode(lv.SCROLLBAR_MODE.AUTO)
# Set style for screen_cont_sw, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_sw.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_sw.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_41
screen_cont_41 = lv.obj(screen_cont_sw)
screen_cont_41.set_pos(25, 20)
screen_cont_41.set_size(240, 61)
screen_cont_41.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_41, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_41.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_41.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_28
screen_label_28 = lv.label(screen_cont_41)
screen_label_28.set_text("Switch 1")
screen_label_28.set_long_mode(lv.label.LONG.WRAP)
screen_label_28.set_width(lv.pct(100))
screen_label_28.set_pos(88, 13)
screen_label_28.set_size(120, 32)
# Set style for screen_label_28, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_28.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_28.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_3
screen_sw_3 = lv.switch(screen_cont_41)
screen_sw_3.set_pos(35, 16)
screen_sw_3.set_size(50, 25)
# Set style for screen_sw_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_3.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_3.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_3, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_3.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_3.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_3.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_3, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_3.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_3.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_3.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_3.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_42
screen_cont_42 = lv.obj(screen_cont_sw)
screen_cont_42.set_pos(300, 20)
screen_cont_42.set_size(240, 61)
screen_cont_42.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_42, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_42.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_42.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_29
screen_label_29 = lv.label(screen_cont_42)
screen_label_29.set_text("Switch 2")
screen_label_29.set_long_mode(lv.label.LONG.WRAP)
screen_label_29.set_width(lv.pct(100))
screen_label_29.set_pos(88, 13)
screen_label_29.set_size(120, 32)
# Set style for screen_label_29, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_29.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_29.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_4
screen_sw_4 = lv.switch(screen_cont_42)
screen_sw_4.set_pos(35, 16)
screen_sw_4.set_size(50, 25)
# Set style for screen_sw_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_4.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_4.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_4.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_4, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_4.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_4.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_4.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_4, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_4.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_4.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_4.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_4.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_43
screen_cont_43 = lv.obj(screen_cont_sw)
screen_cont_43.set_pos(25, 93)
screen_cont_43.set_size(240, 61)
screen_cont_43.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_43, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_43.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_43.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_30
screen_label_30 = lv.label(screen_cont_43)
screen_label_30.set_text("Switch 3")
screen_label_30.set_long_mode(lv.label.LONG.WRAP)
screen_label_30.set_width(lv.pct(100))
screen_label_30.set_pos(87, 12)
screen_label_30.set_size(120, 32)
# Set style for screen_label_30, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_30.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_30.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_5
screen_sw_5 = lv.switch(screen_cont_43)
screen_sw_5.set_pos(35, 16)
screen_sw_5.set_size(50, 25)
# Set style for screen_sw_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_5.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_5.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_5.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_5, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_5.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_5.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_5.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_5, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_5.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_5.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_5.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_5.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_44
screen_cont_44 = lv.obj(screen_cont_sw)
screen_cont_44.set_pos(300, 93)
screen_cont_44.set_size(240, 61)
screen_cont_44.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_44.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_44, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_44.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_44.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_31
screen_label_31 = lv.label(screen_cont_44)
screen_label_31.set_text("Switch 4")
screen_label_31.set_long_mode(lv.label.LONG.WRAP)
screen_label_31.set_width(lv.pct(100))
screen_label_31.set_pos(87, 12)
screen_label_31.set_size(120, 32)
# Set style for screen_label_31, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_31.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_31.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_6
screen_sw_6 = lv.switch(screen_cont_44)
screen_sw_6.set_pos(35, 16)
screen_sw_6.set_size(50, 25)
# Set style for screen_sw_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_6.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_6.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_6.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_6, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_6.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_6.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_6.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_6, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_6.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_6.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_6.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_6.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_45
screen_cont_45 = lv.obj(screen_cont_sw)
screen_cont_45.set_pos(25, 165)
screen_cont_45.set_size(240, 61)
screen_cont_45.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_45.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_45, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_45.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_45.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_32
screen_label_32 = lv.label(screen_cont_45)
screen_label_32.set_text("Switch 5")
screen_label_32.set_long_mode(lv.label.LONG.WRAP)
screen_label_32.set_width(lv.pct(100))
screen_label_32.set_pos(87, 12)
screen_label_32.set_size(120, 32)
# Set style for screen_label_32, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_32.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_32.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_7
screen_sw_7 = lv.switch(screen_cont_45)
screen_sw_7.set_pos(35, 16)
screen_sw_7.set_size(50, 25)
# Set style for screen_sw_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_7.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_7.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_7.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_7, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_7.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_7.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_7.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_7, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_7.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_7.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_7.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_7.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_7.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_46
screen_cont_46 = lv.obj(screen_cont_sw)
screen_cont_46.set_pos(300, 165)
screen_cont_46.set_size(240, 61)
screen_cont_46.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_46.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_46, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_46.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_46.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_33
screen_label_33 = lv.label(screen_cont_46)
screen_label_33.set_text("Switch 6")
screen_label_33.set_long_mode(lv.label.LONG.WRAP)
screen_label_33.set_width(lv.pct(100))
screen_label_33.set_pos(87, 12)
screen_label_33.set_size(120, 32)
# Set style for screen_label_33, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_33.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_33.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_8
screen_sw_8 = lv.switch(screen_cont_46)
screen_sw_8.set_pos(35, 16)
screen_sw_8.set_size(50, 25)
# Set style for screen_sw_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_8.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_8.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_8.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_8.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_8, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_8.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_8.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_8.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_8.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_8, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_8.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_8.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_8.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_8.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_8.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_timer
screen_cont_timer = lv.obj(screen_tileview_hub_home_control)
screen_cont_timer.set_pos(204, 26)
screen_cont_timer.set_size(563, 453)
screen_cont_timer.set_scrollbar_mode(lv.SCROLLBAR_MODE.AUTO)
# Set style for screen_cont_timer, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_timer.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_timer.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_53
screen_cont_53 = lv.obj(screen_cont_timer)
screen_cont_53.set_pos(25, 20)
screen_cont_53.set_size(240, 61)
screen_cont_53.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_53, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_53.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_53.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_39
screen_label_39 = lv.label(screen_cont_53)
screen_label_39.set_text("10:50-20/10")
screen_label_39.set_long_mode(lv.label.LONG.WRAP)
screen_label_39.set_width(lv.pct(100))
screen_label_39.set_pos(58, 14)
screen_label_39.set_size(173, 32)
# Set style for screen_label_39, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_39.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_39.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_14
screen_imgbtn_14 = lv.imgbtn(screen_cont_53)
screen_imgbtn_14.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_14.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_14.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_14_label = lv.label(screen_imgbtn_14)
screen_imgbtn_14_label.set_text("")
screen_imgbtn_14_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_14_label.set_width(lv.pct(100))
screen_imgbtn_14_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_14.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_14.set_pos(12, 7)
screen_imgbtn_14.set_size(47, 41)
# Set style for screen_imgbtn_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_14.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_14.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_14.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_14, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_14.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_14.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_14.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_14, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_14.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_14.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_14.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_14.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_14, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_14.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_54
screen_cont_54 = lv.obj(screen_cont_timer)
screen_cont_54.set_pos(302, 20)
screen_cont_54.set_size(240, 61)
screen_cont_54.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_54, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_54.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_54.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_40
screen_label_40 = lv.label(screen_cont_54)
screen_label_40.set_text("5:30-08/10")
screen_label_40.set_long_mode(lv.label.LONG.WRAP)
screen_label_40.set_width(lv.pct(100))
screen_label_40.set_pos(58, 14)
screen_label_40.set_size(173, 32)
# Set style for screen_label_40, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_40.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_40.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_15
screen_imgbtn_15 = lv.imgbtn(screen_cont_54)
screen_imgbtn_15.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_15.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_15.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_15_label = lv.label(screen_imgbtn_15)
screen_imgbtn_15_label.set_text("")
screen_imgbtn_15_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_15_label.set_width(lv.pct(100))
screen_imgbtn_15_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_15.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_15.set_pos(12, 7)
screen_imgbtn_15.set_size(47, 41)
# Set style for screen_imgbtn_15, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_15.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_15.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_15.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_15, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_15.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_15.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_15.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_15, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_15.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_15.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_15.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_15.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_15.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_15, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_15.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_55
screen_cont_55 = lv.obj(screen_cont_timer)
screen_cont_55.set_pos(25, 95)
screen_cont_55.set_size(240, 61)
screen_cont_55.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_55, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_55.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_55.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_41
screen_label_41 = lv.label(screen_cont_55)
screen_label_41.set_text("7:30-08/10")
screen_label_41.set_long_mode(lv.label.LONG.WRAP)
screen_label_41.set_width(lv.pct(100))
screen_label_41.set_pos(57, 13)
screen_label_41.set_size(173, 32)
# Set style for screen_label_41, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_41.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_41.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_16
screen_imgbtn_16 = lv.imgbtn(screen_cont_55)
screen_imgbtn_16.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_16.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_16.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_16_label = lv.label(screen_imgbtn_16)
screen_imgbtn_16_label.set_text("")
screen_imgbtn_16_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_16_label.set_width(lv.pct(100))
screen_imgbtn_16_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_16.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_16.set_pos(12, 7)
screen_imgbtn_16.set_size(47, 41)
# Set style for screen_imgbtn_16, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_16.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_16.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_16.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_16.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_16.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_16, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_16.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_16.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_16.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_16.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_16.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_16, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_16.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_16.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_16.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_16.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_16.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_16, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_16.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_56
screen_cont_56 = lv.obj(screen_cont_timer)
screen_cont_56.set_pos(302, 95)
screen_cont_56.set_size(240, 61)
screen_cont_56.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_56.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_56, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_56.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_56.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_42
screen_label_42 = lv.label(screen_cont_56)
screen_label_42.set_text("1:50-10/5")
screen_label_42.set_long_mode(lv.label.LONG.WRAP)
screen_label_42.set_width(lv.pct(100))
screen_label_42.set_pos(58, 14)
screen_label_42.set_size(173, 32)
# Set style for screen_label_42, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_42.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_42.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_17
screen_imgbtn_17 = lv.imgbtn(screen_cont_56)
screen_imgbtn_17.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_17.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_17.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_17_label = lv.label(screen_imgbtn_17)
screen_imgbtn_17_label.set_text("")
screen_imgbtn_17_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_17_label.set_width(lv.pct(100))
screen_imgbtn_17_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_17.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_17.set_pos(12, 7)
screen_imgbtn_17.set_size(47, 41)
# Set style for screen_imgbtn_17, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_17.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_17.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_17.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_17.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_17.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_17, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_17.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_17.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_17.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_17.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_17.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_17, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_17.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_17.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_17.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_17.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_17.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_17, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_17.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_57
screen_cont_57 = lv.obj(screen_cont_timer)
screen_cont_57.set_pos(25, 170)
screen_cont_57.set_size(240, 61)
screen_cont_57.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_57.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_57, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_57.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_57.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_43
screen_label_43 = lv.label(screen_cont_57)
screen_label_43.set_text("7:30-08/10")
screen_label_43.set_long_mode(lv.label.LONG.WRAP)
screen_label_43.set_width(lv.pct(100))
screen_label_43.set_pos(57, 13)
screen_label_43.set_size(173, 32)
# Set style for screen_label_43, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_43.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_43.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_18
screen_imgbtn_18 = lv.imgbtn(screen_cont_57)
screen_imgbtn_18.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_18.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_18.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_18_label = lv.label(screen_imgbtn_18)
screen_imgbtn_18_label.set_text("")
screen_imgbtn_18_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_18_label.set_width(lv.pct(100))
screen_imgbtn_18_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_18.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_18.set_pos(12, 7)
screen_imgbtn_18.set_size(47, 41)
# Set style for screen_imgbtn_18, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_18.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_18.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_18.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_18, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_18.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_18.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_18.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_18, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_18.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_18.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_18.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_18.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_18, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_18.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_58
screen_cont_58 = lv.obj(screen_cont_timer)
screen_cont_58.set_pos(302, 170)
screen_cont_58.set_size(240, 61)
screen_cont_58.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_58.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_58, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_58.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_58.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_44
screen_label_44 = lv.label(screen_cont_58)
screen_label_44.set_text("7:30-08/10")
screen_label_44.set_long_mode(lv.label.LONG.WRAP)
screen_label_44.set_width(lv.pct(100))
screen_label_44.set_pos(57, 13)
screen_label_44.set_size(173, 32)
# Set style for screen_label_44, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_44.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_44.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_19
screen_imgbtn_19 = lv.imgbtn(screen_cont_58)
screen_imgbtn_19.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_19.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_19.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_19_label = lv.label(screen_imgbtn_19)
screen_imgbtn_19_label.set_text("")
screen_imgbtn_19_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_19_label.set_width(lv.pct(100))
screen_imgbtn_19_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_19.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_19.set_pos(12, 7)
screen_imgbtn_19.set_size(47, 41)
# Set style for screen_imgbtn_19, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_19.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_19.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_19.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_19, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_19.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_19.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_19.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_19, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_19.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_19.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_19.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_19.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_19.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_19, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_19.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_59
screen_cont_59 = lv.obj(screen_cont_timer)
screen_cont_59.set_pos(25, 241)
screen_cont_59.set_size(240, 61)
screen_cont_59.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_59.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_59, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_59.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_59.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_45
screen_label_45 = lv.label(screen_cont_59)
screen_label_45.set_text("7:30-08/10")
screen_label_45.set_long_mode(lv.label.LONG.WRAP)
screen_label_45.set_width(lv.pct(100))
screen_label_45.set_pos(57, 13)
screen_label_45.set_size(173, 32)
# Set style for screen_label_45, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_45.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_45.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_20
screen_imgbtn_20 = lv.imgbtn(screen_cont_59)
screen_imgbtn_20.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_20.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_20.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_20_label = lv.label(screen_imgbtn_20)
screen_imgbtn_20_label.set_text("")
screen_imgbtn_20_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_20_label.set_width(lv.pct(100))
screen_imgbtn_20_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_20.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_20.set_pos(12, 7)
screen_imgbtn_20.set_size(47, 41)
# Set style for screen_imgbtn_20, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_20.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_20.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_20.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_20, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_20.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_20.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_20.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_20, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_20.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_20.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_20.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_20.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_20, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_20.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_60
screen_cont_60 = lv.obj(screen_cont_timer)
screen_cont_60.set_pos(303, 243)
screen_cont_60.set_size(240, 61)
screen_cont_60.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_60.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_60, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_60.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_60.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_46
screen_label_46 = lv.label(screen_cont_60)
screen_label_46.set_text("7:30-08/10")
screen_label_46.set_long_mode(lv.label.LONG.WRAP)
screen_label_46.set_width(lv.pct(100))
screen_label_46.set_pos(57, 13)
screen_label_46.set_size(173, 32)
# Set style for screen_label_46, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_46.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_text_font(test_font("montserratMedium", 18), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_46.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_21
screen_imgbtn_21 = lv.imgbtn(screen_cont_60)
screen_imgbtn_21.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_21.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_timer_alpha_47x41.bin", None)
screen_imgbtn_21.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_21_label = lv.label(screen_imgbtn_21)
screen_imgbtn_21_label.set_text("")
screen_imgbtn_21_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_21_label.set_width(lv.pct(100))
screen_imgbtn_21_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_21.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_21.set_pos(12, 7)
screen_imgbtn_21.set_size(47, 41)
# Set style for screen_imgbtn_21, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_21.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_21.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_21.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_21, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_21.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_21.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_21.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_21, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_21.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_21.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_21.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_21.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_21, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_21.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_win_1
screen_win_1 = lv.win(screen_tileview_hub_home_control, 55)
screen_win_1.add_title("Control device information")
screen_win_1_item0 = screen_win_1.add_btn(lv.SYMBOL.CLOSE, 40)
screen_win_1_label = lv.label(screen_win_1.get_content())
screen_win_1.get_content().set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
screen_win_1_label.set_text("Light 1 [on]\n\nSwitch 2 [off]")
screen_win_1.set_pos(490, 0)
screen_win_1.set_size(309, 480)
screen_win_1.add_flag(lv.obj.FLAG.HIDDEN)
screen_win_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_win_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_win_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_win_1.set_style_bg_color(lv.color_hex(0xeeeef6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_win_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_win_1.set_style_outline_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_win_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_win_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_win_1_extra_content_main_default = lv.style_t()
style_screen_win_1_extra_content_main_default.init()
style_screen_win_1_extra_content_main_default.set_bg_opa(255)
style_screen_win_1_extra_content_main_default.set_bg_color(lv.color_hex(0xeeeef6))
style_screen_win_1_extra_content_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_win_1_extra_content_main_default.set_text_color(lv.color_hex(0x393c41))
style_screen_win_1_extra_content_main_default.set_text_font(test_font("montserratMedium", 12))
style_screen_win_1_extra_content_main_default.set_text_opa(255)
style_screen_win_1_extra_content_main_default.set_text_letter_space(0)
style_screen_win_1_extra_content_main_default.set_text_line_space(2)
screen_win_1.get_content().add_style(style_screen_win_1_extra_content_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_win_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_win_1_extra_header_main_default = lv.style_t()
style_screen_win_1_extra_header_main_default.init()
style_screen_win_1_extra_header_main_default.set_bg_opa(255)
style_screen_win_1_extra_header_main_default.set_bg_color(lv.color_hex(0xe6e6e6))
style_screen_win_1_extra_header_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
style_screen_win_1_extra_header_main_default.set_text_color(lv.color_hex(0x393c41))
style_screen_win_1_extra_header_main_default.set_text_font(test_font("montserratMedium", 12))
style_screen_win_1_extra_header_main_default.set_text_opa(255)
style_screen_win_1_extra_header_main_default.set_text_letter_space(0)
style_screen_win_1_extra_header_main_default.set_text_line_space(2)
screen_win_1.get_header().add_style(style_screen_win_1_extra_header_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_win_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_win_1_extra_btns_main_default = lv.style_t()
style_screen_win_1_extra_btns_main_default.init()
style_screen_win_1_extra_btns_main_default.set_radius(8)
style_screen_win_1_extra_btns_main_default.set_bg_opa(255)
style_screen_win_1_extra_btns_main_default.set_bg_color(lv.color_hex(0x2195f6))
style_screen_win_1_extra_btns_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_win_1_item0.add_style(style_screen_win_1_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_setting_menu
screen_cont_setting_menu = lv.obj(screen_tileview_main_settings)
screen_cont_setting_menu.set_pos(0, 0)
screen_cont_setting_menu.set_size(175, 480)
screen_cont_setting_menu.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_setting_menu, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_setting_menu.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_border_side(lv.BORDER_SIDE.RIGHT, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_setting_menu.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_2
screen_cont_2 = lv.obj(screen_cont_setting_menu)
screen_cont_2.set_pos(0, 0)
screen_cont_2.set_size(175, 75)
screen_cont_2.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_2.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_border_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_border_side(lv.BORDER_SIDE.BOTTOM, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_1
screen_label_1 = lv.label(screen_cont_2)
screen_label_1.set_text("Settings")
screen_label_1.set_long_mode(lv.label.LONG.WRAP)
screen_label_1.set_width(lv.pct(100))
screen_label_1.set_pos(17, 25)
screen_label_1.set_size(143, 32)
# Set style for screen_label_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_color(lv.color_hex(0x268cf2), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_font(test_font("montserratMedium", 20), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_3
screen_cont_3 = lv.obj(screen_cont_setting_menu)
screen_cont_3.set_pos(7, 101)
screen_cont_3.set_size(156, 48)
screen_cont_3.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_3.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_2
screen_label_2 = lv.label(screen_cont_3)
screen_label_2.set_text("Wifi")
screen_label_2.set_long_mode(lv.label.LONG.WRAP)
screen_label_2.set_width(lv.pct(100))
screen_label_2.set_pos(5, 5)
screen_label_2.set_size(56, 32)
# Set style for screen_label_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_1
screen_imgbtn_1 = lv.imgbtn(screen_cont_3)
screen_imgbtn_1.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_1.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_38x35.bin", None)
screen_imgbtn_1.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_1_label = lv.label(screen_imgbtn_1)
screen_imgbtn_1_label.set_text("")
screen_imgbtn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_1_label.set_width(lv.pct(100))
screen_imgbtn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_1.set_pos(110, 4)
screen_imgbtn_1.set_size(38, 35)
# Set style for screen_imgbtn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_1.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_1, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_1.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_1.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_1, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_1.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_1.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_1, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_1.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_4
screen_cont_4 = lv.obj(screen_cont_setting_menu)
screen_cont_4.set_pos(5, 157)
screen_cont_4.set_size(156, 48)
screen_cont_4.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_4.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_3
screen_label_3 = lv.label(screen_cont_4)
screen_label_3.set_text("Bluetooth")
screen_label_3.set_long_mode(lv.label.LONG.WRAP)
screen_label_3.set_width(lv.pct(100))
screen_label_3.set_pos(4, 10)
screen_label_3.set_size(98, 32)
# Set style for screen_label_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_3.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_2
screen_imgbtn_2 = lv.imgbtn(screen_cont_4)
screen_imgbtn_2.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_2.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_38x35.bin", None)
screen_imgbtn_2.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_2_label = lv.label(screen_imgbtn_2)
screen_imgbtn_2_label.set_text("")
screen_imgbtn_2_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_2_label.set_width(lv.pct(100))
screen_imgbtn_2_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_2.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_2.set_pos(109, 7)
screen_imgbtn_2.set_size(38, 35)
# Set style for screen_imgbtn_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_2.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_2, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_2.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_2.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_2, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_2.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_2.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_2.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_2, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_2.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_5
screen_cont_5 = lv.obj(screen_cont_setting_menu)
screen_cont_5.set_pos(7, 214)
screen_cont_5.set_size(156, 48)
screen_cont_5.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_5.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_4
screen_label_4 = lv.label(screen_cont_5)
screen_label_4.set_text("Language")
screen_label_4.set_long_mode(lv.label.LONG.WRAP)
screen_label_4.set_width(lv.pct(100))
screen_label_4.set_pos(6, 7)
screen_label_4.set_size(94, 32)
# Set style for screen_label_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_4.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_3
screen_imgbtn_3 = lv.imgbtn(screen_cont_5)
screen_imgbtn_3.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_3.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_38x35.bin", None)
screen_imgbtn_3.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_3_label = lv.label(screen_imgbtn_3)
screen_imgbtn_3_label.set_text("")
screen_imgbtn_3_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_3_label.set_width(lv.pct(100))
screen_imgbtn_3_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_3.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_3.set_pos(109, 3)
screen_imgbtn_3.set_size(38, 35)
# Set style for screen_imgbtn_3, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_3.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_3.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_3.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_3, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_3.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_3.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_3.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_3, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_3.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_3.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_3.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_3.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_3.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_3, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_3.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_6
screen_cont_6 = lv.obj(screen_cont_setting_menu)
screen_cont_6.set_pos(7, 272)
screen_cont_6.set_size(156, 48)
screen_cont_6.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_6.set_style_border_width(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_border_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_border_color(lv.color_hex(0x2195f6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_bg_color(lv.color_hex(0x252525), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_5
screen_label_5 = lv.label(screen_cont_6)
screen_label_5.set_text("Notification")
screen_label_5.set_long_mode(lv.label.LONG.WRAP)
screen_label_5.set_width(lv.pct(100))
screen_label_5.set_pos(3, 7)
screen_label_5.set_size(120, 32)
# Set style for screen_label_5, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_5.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_pad_top(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_5.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_imgbtn_4
screen_imgbtn_4 = lv.imgbtn(screen_cont_6)
screen_imgbtn_4.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_4.set_src(lv.imgbtn.STATE.RELEASED, None, "B:MicroPython/_arrow_alpha_38x35.bin", None)
screen_imgbtn_4.add_flag(lv.obj.FLAG.CHECKABLE)
screen_imgbtn_4_label = lv.label(screen_imgbtn_4)
screen_imgbtn_4_label.set_text("")
screen_imgbtn_4_label.set_long_mode(lv.label.LONG.WRAP)
screen_imgbtn_4_label.set_width(lv.pct(100))
screen_imgbtn_4_label.align(lv.ALIGN.CENTER, 0, 0)
screen_imgbtn_4.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_imgbtn_4.set_pos(113, 3)
screen_imgbtn_4.set_size(38, 35)
# Set style for screen_imgbtn_4, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_imgbtn_4.set_style_text_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_4.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_4.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_imgbtn_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_imgbtn_4, Part: lv.PART.MAIN, State: lv.STATE.PRESSED.
screen_imgbtn_4.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_4.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_4.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.PRESSED)
screen_imgbtn_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.PRESSED)
# Set style for screen_imgbtn_4, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_imgbtn_4.set_style_img_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_4.set_style_text_color(lv.color_hex(0xFF33FF), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_4.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_4.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_imgbtn_4.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_imgbtn_4, Part: lv.PART.MAIN, State: LV_IMGBTN_STATE_RELEASED.
screen_imgbtn_4.set_style_img_opa(255, lv.PART.MAIN|lv.imgbtn.STATE.RELEASED)

# Create screen_cont_wifi
screen_cont_wifi = lv.obj(screen_tileview_main_settings)
screen_cont_wifi.set_pos(207, 32)
screen_cont_wifi.set_size(560, 447)
screen_cont_wifi.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_wifi.set_scrollbar_mode(lv.SCROLLBAR_MODE.ACTIVE)
# Set style for screen_cont_wifi, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_wifi.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_wifi.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_8
screen_cont_8 = lv.obj(screen_cont_wifi)
screen_cont_8.set_pos(0, -1)
screen_cont_8.set_size(560, 43)
screen_cont_8.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_6
screen_label_6 = lv.label(screen_cont_8)
screen_label_6.set_text("Wifi")
screen_label_6.set_long_mode(lv.label.LONG.WRAP)
screen_label_6.set_width(lv.pct(100))
screen_label_6.set_pos(2, 5)
screen_label_6.set_size(100, 32)
# Set style for screen_label_6, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_6.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_6.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_1
screen_sw_1 = lv.switch(screen_cont_8)
screen_sw_1.set_pos(116, 2)
screen_sw_1.set_size(40, 20)
# Set style for screen_sw_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_1.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_1.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_1, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_1.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_1.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_1.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_1, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_1.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_1.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_1.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_9
screen_cont_9 = lv.obj(screen_cont_wifi)
screen_cont_9.set_pos(-4, 70)
screen_cont_9.set_size(560, 376)
screen_cont_9.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_9.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_9.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_10
screen_cont_10 = lv.obj(screen_cont_9)
screen_cont_10.set_pos(8, 0)
screen_cont_10.set_size(550, 62)
screen_cont_10.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_10, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_10.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_10.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_7
screen_label_7 = lv.label(screen_cont_10)
screen_label_7.set_text("Wifi 1")
screen_label_7.set_long_mode(lv.label.LONG.WRAP)
screen_label_7.set_width(lv.pct(100))
screen_label_7.set_pos(6, 16)
screen_label_7.set_size(100, 32)
# Set style for screen_label_7, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_7.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_7.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_btn_1
screen_btn_1 = lv.btn(screen_cont_10)
screen_btn_1_label = lv.label(screen_btn_1)
screen_btn_1_label.set_text("Connect")
screen_btn_1_label.set_long_mode(lv.label.LONG.WRAP)
screen_btn_1_label.set_width(lv.pct(100))
screen_btn_1_label.align(lv.ALIGN.CENTER, 0, 0)
screen_btn_1.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_btn_1.set_pos(443, 5)
screen_btn_1.set_size(100, 50)
# Set style for screen_btn_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_btn_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_color(lv.color_hex(0x5597ff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_1.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_ble
screen_cont_ble = lv.obj(screen_tileview_main_settings)
screen_cont_ble.set_pos(207, 32)
screen_cont_ble.set_size(560, 447)
screen_cont_ble.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_ble.set_scrollbar_mode(lv.SCROLLBAR_MODE.ACTIVE)
# Set style for screen_cont_ble, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_ble.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_ble.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_14
screen_cont_14 = lv.obj(screen_cont_ble)
screen_cont_14.set_pos(0, -1)
screen_cont_14.set_size(560, 43)
screen_cont_14.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_14, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_14.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_14.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_9
screen_label_9 = lv.label(screen_cont_14)
screen_label_9.set_text("Bluetooth")
screen_label_9.set_long_mode(lv.label.LONG.WRAP)
screen_label_9.set_width(lv.pct(100))
screen_label_9.set_pos(2, 5)
screen_label_9.set_size(100, 32)
# Set style for screen_label_9, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_9.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_9.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_sw_2
screen_sw_2 = lv.switch(screen_cont_14)
screen_sw_2.set_pos(143, 2)
screen_sw_2.set_size(40, 20)
# Set style for screen_sw_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_sw_2.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_2.set_style_bg_color(lv.color_hex(0xe6e2e6), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_2.set_style_radius(10, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_sw_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_sw_2, Part: lv.PART.INDICATOR, State: lv.STATE.CHECKED.
screen_sw_2.set_style_bg_opa(255, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_2.set_style_bg_color(lv.color_hex(0x2195f6), lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.INDICATOR|lv.STATE.CHECKED)
screen_sw_2.set_style_border_width(0, lv.PART.INDICATOR|lv.STATE.CHECKED)

# Set style for screen_sw_2, Part: lv.PART.KNOB, State: lv.STATE.DEFAULT.
screen_sw_2.set_style_bg_opa(255, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_2.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_2.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_2.set_style_border_width(0, lv.PART.KNOB|lv.STATE.DEFAULT)
screen_sw_2.set_style_radius(10, lv.PART.KNOB|lv.STATE.DEFAULT)

# Create screen_cont_12
screen_cont_12 = lv.obj(screen_cont_ble)
screen_cont_12.set_pos(-4, 70)
screen_cont_12.set_size(560, 376)
screen_cont_12.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_12.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_12, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_12.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_13
screen_cont_13 = lv.obj(screen_cont_12)
screen_cont_13.set_pos(8, 0)
screen_cont_13.set_size(550, 62)
screen_cont_13.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_13, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_13.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_13.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_8
screen_label_8 = lv.label(screen_cont_13)
screen_label_8.set_text("Bluetooth 1")
screen_label_8.set_long_mode(lv.label.LONG.WRAP)
screen_label_8.set_width(lv.pct(100))
screen_label_8.set_pos(-2, 20)
screen_label_8.set_size(120, 32)
# Set style for screen_label_8, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_8.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_8.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_btn_2
screen_btn_2 = lv.btn(screen_cont_13)
screen_btn_2_label = lv.label(screen_btn_2)
screen_btn_2_label.set_text("Connect")
screen_btn_2_label.set_long_mode(lv.label.LONG.WRAP)
screen_btn_2_label.set_width(lv.pct(100))
screen_btn_2_label.align(lv.ALIGN.CENTER, 0, 0)
screen_btn_2.set_style_pad_all(0, lv.STATE.DEFAULT)
screen_btn_2.set_pos(443, 5)
screen_btn_2.set_size(100, 50)
# Set style for screen_btn_2, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_btn_2.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_radius(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_color(lv.color_hex(0x5597ff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_btn_2.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_lang
screen_cont_lang = lv.obj(screen_tileview_main_settings)
screen_cont_lang.set_pos(207, 32)
screen_cont_lang.set_size(560, 447)
screen_cont_lang.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_lang.set_scrollbar_mode(lv.SCROLLBAR_MODE.ACTIVE)
# Set style for screen_cont_lang, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_lang.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_lang.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_18
screen_cont_18 = lv.obj(screen_cont_lang)
screen_cont_18.set_pos(0, -1)
screen_cont_18.set_size(560, 43)
screen_cont_18.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_18, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_18.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_18.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_11
screen_label_11 = lv.label(screen_cont_18)
screen_label_11.set_text("Language")
screen_label_11.set_long_mode(lv.label.LONG.WRAP)
screen_label_11.set_width(lv.pct(100))
screen_label_11.set_pos(2, 5)
screen_label_11.set_size(123, 32)
# Set style for screen_label_11, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_11.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_11.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_16
screen_cont_16 = lv.obj(screen_cont_lang)
screen_cont_16.set_pos(-4, 70)
screen_cont_16.set_size(560, 376)
screen_cont_16.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_16, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_16.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_16.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_ddlist_1
screen_ddlist_1 = lv.dropdown(screen_cont_16)
screen_ddlist_1.set_options("Vietnamese\nEnglish")
screen_ddlist_1.set_pos(13, 5)
screen_ddlist_1.set_size(322, 30)
# Set style for screen_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_ddlist_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_radius(4, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_bg_opa(126, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.CHECKED.
screen_ddlist_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_border_color(lv.color_hex(0x654f4f), lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_radius(3, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.CHECKED)
screen_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.CHECKED)
# Set style for screen_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.FOCUSED.
screen_ddlist_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_text_font(test_font("montserratMedium", 14), lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_border_color(lv.color_hex(0x654f4f), lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_radius(3, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.FOCUSED)
screen_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.FOCUSED)
# Set style for screen_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DISABLED.
screen_ddlist_1.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_text_font(test_font("montserratMedium", 12), lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_border_width(1, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_border_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_border_color(lv.color_hex(0xe1e6ee), lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_border_side(lv.BORDER_SIDE.FULL, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_pad_top(8, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_pad_left(6, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_pad_right(6, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_radius(3, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_bg_opa(255, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_bg_color(lv.color_hex(0x000000), lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.MAIN|lv.STATE.DISABLED)
screen_ddlist_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DISABLED)
# Set style for screen_ddlist_1, Part: lv.PART.SELECTED, State: lv.STATE.CHECKED.
style_screen_ddlist_1_extra_list_selected_checked = lv.style_t()
style_screen_ddlist_1_extra_list_selected_checked.init()
style_screen_ddlist_1_extra_list_selected_checked.set_text_color(lv.color_hex(0xffffff))
style_screen_ddlist_1_extra_list_selected_checked.set_text_font(test_font("montserratMedium", 14))
style_screen_ddlist_1_extra_list_selected_checked.set_text_opa(255)
style_screen_ddlist_1_extra_list_selected_checked.set_border_width(2)
style_screen_ddlist_1_extra_list_selected_checked.set_border_opa(255)
style_screen_ddlist_1_extra_list_selected_checked.set_border_color(lv.color_hex(0x654f4f))
style_screen_ddlist_1_extra_list_selected_checked.set_border_side(lv.BORDER_SIDE.FULL)
style_screen_ddlist_1_extra_list_selected_checked.set_radius(3)
style_screen_ddlist_1_extra_list_selected_checked.set_bg_opa(110)
style_screen_ddlist_1_extra_list_selected_checked.set_bg_color(lv.color_hex(0x000000))
style_screen_ddlist_1_extra_list_selected_checked.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_ddlist_1.get_list().add_style(style_screen_ddlist_1_extra_list_selected_checked, lv.PART.SELECTED|lv.STATE.CHECKED)
# Set style for screen_ddlist_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_ddlist_1_extra_list_main_default = lv.style_t()
style_screen_ddlist_1_extra_list_main_default.init()
style_screen_ddlist_1_extra_list_main_default.set_max_height(90)
style_screen_ddlist_1_extra_list_main_default.set_text_color(lv.color_hex(0xffffff))
style_screen_ddlist_1_extra_list_main_default.set_text_font(test_font("montserratMedium", 14))
style_screen_ddlist_1_extra_list_main_default.set_text_opa(255)
style_screen_ddlist_1_extra_list_main_default.set_border_width(1)
style_screen_ddlist_1_extra_list_main_default.set_border_opa(255)
style_screen_ddlist_1_extra_list_main_default.set_border_color(lv.color_hex(0x654f4f))
style_screen_ddlist_1_extra_list_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_screen_ddlist_1_extra_list_main_default.set_radius(3)
style_screen_ddlist_1_extra_list_main_default.set_bg_opa(138)
style_screen_ddlist_1_extra_list_main_default.set_bg_color(lv.color_hex(0x000000))
style_screen_ddlist_1_extra_list_main_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_ddlist_1.get_list().add_style(style_screen_ddlist_1_extra_list_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
# Set style for screen_ddlist_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
style_screen_ddlist_1_extra_list_scrollbar_default = lv.style_t()
style_screen_ddlist_1_extra_list_scrollbar_default.init()
style_screen_ddlist_1_extra_list_scrollbar_default.set_radius(3)
style_screen_ddlist_1_extra_list_scrollbar_default.set_bg_opa(255)
style_screen_ddlist_1_extra_list_scrollbar_default.set_bg_color(lv.color_hex(0x000000))
style_screen_ddlist_1_extra_list_scrollbar_default.set_bg_grad_dir(lv.GRAD_DIR.NONE)
screen_ddlist_1.get_list().add_style(style_screen_ddlist_1_extra_list_scrollbar_default, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)

# Create screen_cont_noti
screen_cont_noti = lv.obj(screen_tileview_main_settings)
screen_cont_noti.set_pos(207, 32)
screen_cont_noti.set_size(560, 447)
screen_cont_noti.add_flag(lv.obj.FLAG.HIDDEN)
screen_cont_noti.set_scrollbar_mode(lv.SCROLLBAR_MODE.ACTIVE)
# Set style for screen_cont_noti, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_noti.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_noti.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_cont_21
screen_cont_21 = lv.obj(screen_cont_noti)
screen_cont_21.set_pos(0, -1)
screen_cont_21.set_size(560, 40)
screen_cont_21.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_21, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_21.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_21.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_label_12
screen_label_12 = lv.label(screen_cont_21)
screen_label_12.set_text("Notification")
screen_label_12.set_long_mode(lv.label.LONG.WRAP)
screen_label_12.set_width(lv.pct(100))
screen_label_12.set_pos(2, 5)
screen_label_12.set_size(155, 32)
# Set style for screen_label_12, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_label_12.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_text_color(lv.color_hex(0xffffff), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_text_font(test_font("montserratMedium", 16), lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_text_opa(255, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_text_letter_space(2, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_text_line_space(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_text_align(lv.TEXT_ALIGN.CENTER, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_label_12.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Create screen_cont_20
screen_cont_20 = lv.obj(screen_cont_noti)
screen_cont_20.set_pos(-4, 70)
screen_cont_20.set_size(560, 376)
screen_cont_20.set_scrollbar_mode(lv.SCROLLBAR_MODE.OFF)
# Set style for screen_cont_20, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_cont_20.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_radius(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_pad_top(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_cont_20.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
# Create screen_list_1
screen_list_1 = lv.list(screen_cont_20)
screen_list_1_item0 = screen_list_1.add_btn(lv.SYMBOL.BATTERY_1, "Battery is running low")
screen_list_1_item1 = screen_list_1.add_btn(lv.SYMBOL.BELL, "New software is  available")
screen_list_1_item2 = screen_list_1.add_btn(lv.SYMBOL.CHARGE, "SW1 is turning on")
screen_list_1.set_pos(15, -3)
screen_list_1.set_size(522, 380)
screen_list_1.set_scrollbar_mode(lv.SCROLLBAR_MODE.ACTIVE)
# Set style for screen_list_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
screen_list_1.set_style_pad_top(5, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_pad_left(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_pad_right(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_pad_bottom(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_bg_opa(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_border_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_radius(7, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1.set_style_shadow_width(0, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_list_1, Part: lv.PART.SCROLLBAR, State: lv.STATE.DEFAULT.
screen_list_1.set_style_radius(3, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_list_1.set_style_bg_opa(255, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_list_1.set_style_bg_color(lv.color_hex(0xffffff), lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
screen_list_1.set_style_bg_grad_dir(lv.GRAD_DIR.NONE, lv.PART.SCROLLBAR|lv.STATE.DEFAULT)
# Set style for screen_list_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_list_1_extra_btns_main_default = lv.style_t()
style_screen_list_1_extra_btns_main_default.init()
style_screen_list_1_extra_btns_main_default.set_pad_top(5)
style_screen_list_1_extra_btns_main_default.set_pad_left(10)
style_screen_list_1_extra_btns_main_default.set_pad_right(0)
style_screen_list_1_extra_btns_main_default.set_pad_bottom(10)
style_screen_list_1_extra_btns_main_default.set_border_width(1)
style_screen_list_1_extra_btns_main_default.set_border_opa(255)
style_screen_list_1_extra_btns_main_default.set_border_color(lv.color_hex(0x6f6969))
style_screen_list_1_extra_btns_main_default.set_border_side(lv.BORDER_SIDE.FULL)
style_screen_list_1_extra_btns_main_default.set_text_color(lv.color_hex(0xfef6ea))
style_screen_list_1_extra_btns_main_default.set_text_font(test_font("FontAwesome5", 18))
style_screen_list_1_extra_btns_main_default.set_text_opa(255)
style_screen_list_1_extra_btns_main_default.set_radius(0)
style_screen_list_1_extra_btns_main_default.set_bg_opa(0)
screen_list_1_item2.add_style(style_screen_list_1_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1_item1.add_style(style_screen_list_1_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)
screen_list_1_item0.add_style(style_screen_list_1_extra_btns_main_default, lv.PART.MAIN|lv.STATE.DEFAULT)

# Set style for screen_list_1, Part: lv.PART.MAIN, State: lv.STATE.DEFAULT.
style_screen_list_1_extra_texts_main_default = lv.style_t()
style_screen_list_1_extra_texts_main_default.init()
style_screen_list_1_extra_texts_main_default.set_pad_top(6)
style_screen_list_1_extra_texts_main_default.set_pad_left(5)
style_screen_list_1_extra_texts_main_default.set_pad_right(0)
style_screen_list_1_extra_texts_main_default.set_pad_bottom(0)
style_screen_list_1_extra_texts_main_default.set_border_width(0)
style_screen_list_1_extra_texts_main_default.set_text_color(lv.color_hex(0x0D3055))
style_screen_list_1_extra_texts_main_default.set_text_font(test_font("montserratMedium", 18))
style_screen_list_1_extra_texts_main_default.set_text_opa(255)
style_screen_list_1_extra_texts_main_default.set_radius(3)
style_screen_list_1_extra_texts_main_default.set_bg_opa(0)

screen.update_layout()

def screen_label_14_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_sw.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_timer.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_led.clear_flag(lv.obj.FLAG.HIDDEN)

screen_label_14.add_event_cb(lambda e: screen_label_14_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_5_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_timer.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_sw.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_led.clear_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_5.add_event_cb(lambda e: screen_imgbtn_5_event_handler(e), lv.EVENT.ALL, None)

def screen_label_15_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_timer.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_sw.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_led.add_flag(lv.obj.FLAG.HIDDEN)

screen_label_15.add_event_cb(lambda e: screen_label_15_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_6_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_timer.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_sw.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_led.add_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_6.add_event_cb(lambda e: screen_imgbtn_6_event_handler(e), lv.EVENT.ALL, None)

def screen_label_16_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_timer.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_sw.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_led.add_flag(lv.obj.FLAG.HIDDEN)

screen_label_16.add_event_cb(lambda e: screen_label_16_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_7_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_timer.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_sw.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_led.add_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_7.add_event_cb(lambda e: screen_imgbtn_7_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_14_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_win_1.clear_flag(lv.obj.FLAG.HIDDEN);#Write animation: screen_win_1 move_x
        screen_win_1_anim_move_x = lv.anim_t()
        screen_win_1_anim_move_x.init()
        screen_win_1_anim_move_x.set_var(screen_win_1)
        screen_win_1_anim_move_x.set_time(1000)
        screen_win_1_anim_move_x.set_delay(0)
        screen_win_1_anim_move_x.set_custom_exec_cb(lambda e,val: anim_x_cb(screen_win_1,val))
        screen_win_1_anim_move_x.set_values(screen_win_1.get_x(), 490)
        screen_win_1_anim_move_x.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_x.set_repeat_count(0)
        screen_win_1_anim_move_x.set_repeat_delay(0)
        screen_win_1_anim_move_x.set_playback_time(0)
        screen_win_1_anim_move_x.set_playback_delay(0)
        screen_win_1_anim_move_x.start()
        #Write animation: screen_win_1 move_y
        screen_win_1_anim_move_y = lv.anim_t()
        screen_win_1_anim_move_y.init()
        screen_win_1_anim_move_y.set_var(screen_win_1)
        screen_win_1_anim_move_y.set_time(1000)
        screen_win_1_anim_move_y.set_delay(0)
        screen_win_1_anim_move_y.set_custom_exec_cb(lambda e,val: anim_y_cb(screen_win_1,val))
        screen_win_1_anim_move_y.set_values(screen_win_1.get_y(), 0)
        screen_win_1_anim_move_y.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_y.set_repeat_count(0)
        screen_win_1_anim_move_y.set_repeat_delay(0)
        screen_win_1_anim_move_y.set_playback_time(0)
        screen_win_1_anim_move_y.set_playback_delay(0)
        screen_win_1_anim_move_y.start()

screen_imgbtn_14.add_event_cb(lambda e: screen_imgbtn_14_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_15_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_win_1.clear_flag(lv.obj.FLAG.HIDDEN);#Write animation: screen_win_1 move_x
        screen_win_1_anim_move_x = lv.anim_t()
        screen_win_1_anim_move_x.init()
        screen_win_1_anim_move_x.set_var(screen_win_1)
        screen_win_1_anim_move_x.set_time(1000)
        screen_win_1_anim_move_x.set_delay(0)
        screen_win_1_anim_move_x.set_custom_exec_cb(lambda e,val: anim_x_cb(screen_win_1,val))
        screen_win_1_anim_move_x.set_values(screen_win_1.get_x(), 490)
        screen_win_1_anim_move_x.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_x.set_repeat_count(0)
        screen_win_1_anim_move_x.set_repeat_delay(0)
        screen_win_1_anim_move_x.set_playback_time(0)
        screen_win_1_anim_move_x.set_playback_delay(0)
        screen_win_1_anim_move_x.start()
        #Write animation: screen_win_1 move_y
        screen_win_1_anim_move_y = lv.anim_t()
        screen_win_1_anim_move_y.init()
        screen_win_1_anim_move_y.set_var(screen_win_1)
        screen_win_1_anim_move_y.set_time(1000)
        screen_win_1_anim_move_y.set_delay(0)
        screen_win_1_anim_move_y.set_custom_exec_cb(lambda e,val: anim_y_cb(screen_win_1,val))
        screen_win_1_anim_move_y.set_values(screen_win_1.get_y(), 0)
        screen_win_1_anim_move_y.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_y.set_repeat_count(0)
        screen_win_1_anim_move_y.set_repeat_delay(0)
        screen_win_1_anim_move_y.set_playback_time(0)
        screen_win_1_anim_move_y.set_playback_delay(0)
        screen_win_1_anim_move_y.start()

screen_imgbtn_15.add_event_cb(lambda e: screen_imgbtn_15_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_16_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_win_1.clear_flag(lv.obj.FLAG.HIDDEN);#Write animation: screen_win_1 move_x
        screen_win_1_anim_move_x = lv.anim_t()
        screen_win_1_anim_move_x.init()
        screen_win_1_anim_move_x.set_var(screen_win_1)
        screen_win_1_anim_move_x.set_time(1000)
        screen_win_1_anim_move_x.set_delay(0)
        screen_win_1_anim_move_x.set_custom_exec_cb(lambda e,val: anim_x_cb(screen_win_1,val))
        screen_win_1_anim_move_x.set_values(screen_win_1.get_x(), 490)
        screen_win_1_anim_move_x.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_x.set_repeat_count(0)
        screen_win_1_anim_move_x.set_repeat_delay(0)
        screen_win_1_anim_move_x.set_playback_time(0)
        screen_win_1_anim_move_x.set_playback_delay(0)
        screen_win_1_anim_move_x.start()
        #Write animation: screen_win_1 move_y
        screen_win_1_anim_move_y = lv.anim_t()
        screen_win_1_anim_move_y.init()
        screen_win_1_anim_move_y.set_var(screen_win_1)
        screen_win_1_anim_move_y.set_time(1000)
        screen_win_1_anim_move_y.set_delay(0)
        screen_win_1_anim_move_y.set_custom_exec_cb(lambda e,val: anim_y_cb(screen_win_1,val))
        screen_win_1_anim_move_y.set_values(screen_win_1.get_y(), 0)
        screen_win_1_anim_move_y.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_y.set_repeat_count(0)
        screen_win_1_anim_move_y.set_repeat_delay(0)
        screen_win_1_anim_move_y.set_playback_time(0)
        screen_win_1_anim_move_y.set_playback_delay(0)
        screen_win_1_anim_move_y.start()

screen_imgbtn_16.add_event_cb(lambda e: screen_imgbtn_16_event_handler(e), lv.EVENT.ALL, None)

def screen_win_1_event_handler(e):
    code = e.get_code()
def screen_win_1_item0_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_win_1.add_flag(lv.obj.FLAG.HIDDEN);#Write animation: screen_win_1 move_x
        screen_win_1_anim_move_x = lv.anim_t()
        screen_win_1_anim_move_x.init()
        screen_win_1_anim_move_x.set_var(screen_win_1)
        screen_win_1_anim_move_x.set_time(1000)
        screen_win_1_anim_move_x.set_delay(0)
        screen_win_1_anim_move_x.set_custom_exec_cb(lambda e,val: anim_x_cb(screen_win_1,val))
        screen_win_1_anim_move_x.set_values(screen_win_1.get_x(), 810)
        screen_win_1_anim_move_x.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_x.set_repeat_count(0)
        screen_win_1_anim_move_x.set_repeat_delay(0)
        screen_win_1_anim_move_x.set_playback_time(0)
        screen_win_1_anim_move_x.set_playback_delay(0)
        screen_win_1_anim_move_x.start()
        #Write animation: screen_win_1 move_y
        screen_win_1_anim_move_y = lv.anim_t()
        screen_win_1_anim_move_y.init()
        screen_win_1_anim_move_y.set_var(screen_win_1)
        screen_win_1_anim_move_y.set_time(1000)
        screen_win_1_anim_move_y.set_delay(0)
        screen_win_1_anim_move_y.set_custom_exec_cb(lambda e,val: anim_y_cb(screen_win_1,val))
        screen_win_1_anim_move_y.set_values(screen_win_1.get_y(), 0)
        screen_win_1_anim_move_y.set_path_cb(lv.anim_t.path_linear)
        screen_win_1_anim_move_y.set_repeat_count(0)
        screen_win_1_anim_move_y.set_repeat_delay(0)
        screen_win_1_anim_move_y.set_playback_time(0)
        screen_win_1_anim_move_y.set_playback_delay(0)
        screen_win_1_anim_move_y.start()

screen_win_1_item0.add_event_cb(lambda e: screen_win_1_item0_event_handler(e), lv.EVENT.ALL, None)

screen_win_1.add_event_cb(lambda e: screen_win_1_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_wifi.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_ble.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_lang.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_noti.add_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_1.add_event_cb(lambda e: screen_imgbtn_1_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_2_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_wifi.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_ble.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_lang.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_noti.add_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_2.add_event_cb(lambda e: screen_imgbtn_2_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_3_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_noti.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_lang.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_ble.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_wifi.add_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_3.add_event_cb(lambda e: screen_imgbtn_3_event_handler(e), lv.EVENT.ALL, None)

def screen_imgbtn_4_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.CLICKED):
        screen_cont_noti.clear_flag(lv.obj.FLAG.HIDDEN);screen_cont_lang.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_ble.add_flag(lv.obj.FLAG.HIDDEN);screen_cont_wifi.add_flag(lv.obj.FLAG.HIDDEN)

screen_imgbtn_4.add_event_cb(lambda e: screen_imgbtn_4_event_handler(e), lv.EVENT.ALL, None)

def screen_sw_1_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED and screen_sw_1.has_state(lv.STATE.CHECKED)):
        screen_cont_9.clear_flag(lv.obj.FLAG.HIDDEN)
    if (code == lv.EVENT.VALUE_CHANGED and not screen_sw_1.has_state(lv.STATE.CHECKED)):
        screen_cont_9.add_flag(lv.obj.FLAG.HIDDEN)

screen_sw_1.add_event_cb(lambda e: screen_sw_1_event_handler(e), lv.EVENT.ALL, None)

def screen_sw_2_event_handler(e):
    code = e.get_code()
    if (code == lv.EVENT.VALUE_CHANGED and screen_sw_2.has_state(lv.STATE.CHECKED)):
        screen_cont_12.clear_flag(lv.obj.FLAG.HIDDEN)
    if (code == lv.EVENT.VALUE_CHANGED and not screen_sw_2.has_state(lv.STATE.CHECKED)):
        screen_cont_12.add_flag(lv.obj.FLAG.HIDDEN)

screen_sw_2.add_event_cb(lambda e: screen_sw_2_event_handler(e), lv.EVENT.ALL, None)

# content from custom.py

# Load the default screen
lv.scr_load(screen)

while SDL.check():
    time.sleep_ms(5)

