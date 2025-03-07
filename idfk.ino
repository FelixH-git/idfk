
#include "SD_Card.h"
#include "Display_ST7789.h"
#include "ui.h"
#include "LVGL_Driver.h"
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>

//ui_Image3 = lv_img_create(ui_Screen1);

bool clear = false;
void setup()
{       

  LCD_Init();
  Lvgl_Init();
  SD_Init();
  ui_init();
  BLEDevice::init("Merlin");

}

void create_new_star(int rssi_zoom){
    lv_obj_t * new_star;

    new_star = lv_img_create(ui_Screen1);
    int x = random(22, 172 - 22);
    int y = random(22, 160);

    lv_img_set_src(new_star, &ui_img_greenstar_png);
    lv_obj_set_width(new_star, LV_SIZE_CONTENT);   /// 22
    lv_obj_set_height(new_star, LV_SIZE_CONTENT);    /// 22
    lv_obj_set_x(new_star, x);
    lv_obj_set_y(new_star, y);
    lv_obj_set_align(new_star, LV_ALIGN_TOP_LEFT);
    lv_obj_add_flag(new_star, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(new_star, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(new_star, rssi_zoom);
}


void loop()
{
  Timer_Loop();
  randomSeed(0);
  
  BLEScan *scan = BLEDevice::getScan();
  scan->setActiveScan(true);

  BLEScanResults* results = scan->start(1);

  int count = results->getCount();
  lv_obj_t* stars[count];

  for(int i = 0; i < count; i++){

    if(clear == true){
      lv_obj_del(stars[i]);
      
      if(i == (count - 1)){
        clear = false;
        memset(stars, 0, sizeof(stars));
      }
    }
    else{
      BLEAdvertisedDevice device = results->getDevice(i);
          
        int rssi = device.getRSSI();
        int zoom_value = 800 - ((rssi * -1) * 7);
        if(zoom_value < 0){
          zoom_value = 0;
        }
        create_new_star(zoom_value);

        if(i == (count - 1)){
          clear = true;
        }
    }

    
  }
  
  
  // if(otherway == false){
  //   lv_img_set_zoom(ui_Image3, i);
  //   i++;

  //   if(i > 600){
  //     otherway = true;
  //   }

  // }

  // if(otherway == true){
  //   lv_img_set_zoom(ui_Image3, i);
  //   i--;

  //   if(i < 200){
  //     otherway = false;
  //   }
  // }
  

  delay(10);
  scan->clearResults();
}
