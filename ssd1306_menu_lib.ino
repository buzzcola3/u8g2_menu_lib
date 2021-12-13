#define EEPROM_version 1 //update this if the EEPROM_store.h has been changed
#define USED_EEPROM_START 0
#define USED_EEPROM_END 255
#include "new_EEPROM_store.h"


#define SCREEN_WIDTH 16 // OLED display width, in pixels
#define SCREEN_HEIGHT 8 // OLED display height, in pixels
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

  U8X8_SSD1306_128X64_NONAME_HW_I2C display(/* reset=*/ U8X8_PIN_NONE);


unsigned long temp_flex_timer; //temp

int menu_number = 0;
short int menu_text_size = 1;


#define menu_option_lines 15 //total number of menu lines
int menu_options_section_lenght[] = {6,3,3}; //lenght of each submenu in number of lines
char main_menu_option_text[] =
{
  '9', 't', 'e', 'x', 't', ' ', 's', 'i', 'z', 'e', '[', '1', ']', //MENU_TEXT_SIZE
  '4', 'd', 'e', 'e', 'z', '[', '2', ']',
  '2', 'o', 'n', '[', '3', ']',
  '8', 's', 'e', 't', 't', 'i', 'n', 'g', 's',
  '2', 'o', 'n',
  '1', 'u',

  '4', 'b', 'e', 't', 'a', '[', '4', ']',
  '2', 'e', 'n',
  '4', 'd', 'e', 'e', 'n',

  '2', 'o', 'n',
  '1', 'u', '[', '5', ']',
  '4', 'e', 'x', 'i', 't',
};
//int selected_menu_option;

#include "menu.h"

  //default included settings--
#define MENU_TEXT_SIZE 1

void int_limits_setup(){ // ...set_int_limit(EEPROM_BANK_NUMBER, MIN, MAX)
 SSD1306_Menu Setup;
  Setup.set_int_limit(MENU_TEXT_SIZE, 1, 3); //number, min, max -saves to eeprom_int_limit[]
  Setup.set_int_limit(2, 0, 9);
  Setup.set_int_limit(3);
}


void setup()
{
  Serial.begin(115200);
  check_wipe_eeprom();

  reset_default();
  menu_endpoints(menu_option_lines, main_menu_option_text);

  display.begin();

  
  

SSD1306_Menu Setup;
Setup.Setup();
int_limits_setup(); //setup MIN/MAX value of an integer

attachInterrupt(INT0, MenuDown, FALLING);
attachInterrupt(INT1, MenuUp, FALLING);

pinMode(4, INPUT);

display.setFont(u8x8_font_chroma48medium8_r);

}

void MenuDown(){
  //Serial.println("down");
  SSD1306_Menu move;
  move.Down();
}
void MenuUp(){
  //Serial.println("up");
  SSD1306_Menu move;
  move.Up();
}
void MenuOk(){
  Serial.println("ok");
  SSD1306_Menu move;
  move.Ok();
}

void reset_default(){ //default settings
  save_to_EEPROM(MENU_TEXT_SIZE,2);
  save_to_EEPROM(2,-6997);
  save_to_EEPROM(3,1);
}

void loop()
{

 if(temp_flex_timer<=millis()-50)
 {
   if(digitalRead(4) == LOW){MenuOk();}
   //display.clearDisplay();
   //menu_text_size++;
   //if(menu_text_size>3){menu_text_size=1;}
   //Serial.print("Loop time: ");
   //Serial.println(millis()-temp_flex_timer);
   temp_flex_timer=millis();
   
 //SSD1306_Menu::eeprom menu_eeprom;
 //menu_eeprom.update_eeprom_integers(random(1,4), random(-1024,1024));

  //save_eeprom_integers();
   //dump_eeprom();

   //Serial.print("save_to_EEPROM: ");
   //menu_endpoints(menu_option_lines, main_menu_option_text);
   //eeprom_int_array
 }


//display.noInverse();

SSD1306_Menu u_menu;
menu(main_menu_option_text, menu_array, menu_options_section_lenght);
u_menu.update_integers();
u_menu.update_cursor();

//delay(10);
//u_menu.Up();


}