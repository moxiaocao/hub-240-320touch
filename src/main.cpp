#include <Arduino.h>
 
/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */
 
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <../src/demos/lv_demos.h>
#include "..\..\lvgl\src\porting\lv_port_indev.h"
 
/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 as the examples and demos are now part of the main LVGL library. */
 
/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
 
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];
 
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

extern lv_indev_t * indev_touchpad;
 
#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif
 
/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
 
    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
 
    lv_disp_flush_ready( disp );
}
 

void setup()
{
    Serial.begin( 9600 ); /* prepare for possible serial debug */
    lv_init();
 
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif
 
    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */
    tft.invertDisplay(0);
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );
 
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
 
    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register( &indev_drv );

    lv_port_indev_init();
    // lv_group_t * g = lv_group_create(); // 创建组
    // lv_group_set_default(g); // 设置默认组
    // lv_indev_set_group(indev_touchpad, g);
 
#if 0
    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else
    /* Try an example from the lv_examples Arduino library
       make sure to include it as written above.
    lv_example_btn_1();
   */
 
    // uncomment one of these demos
    lv_demo_widgets();            // OK
    // lv_demo_benhmcark();          // OK
    // lv_demo_keypad_encoder();     // works, but I haven't an encoder
    // lv_demo_music();              // NOK
    // lv_demo_printer();
    // lv_demo_stress();             // seems to be OK
  //   lv_obj_t *btn = lv_btn_create(lv_scr_act());
  // lv_obj_set_pos(btn,lv_pct(50),lv_pct(50));
  // lv_obj_set_size(btn,lv_pct(25),lv_pct(25));
  
#endif
    Serial.println( "Setup done" );
}
 
void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}
 
 
 
 
 
 
 
 
 
 