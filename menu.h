//e
int menu_array[(menu_option_lines*2)+1];
int EEPROM_int_text_offset[menu_option_lines*2]; //stores eeprom location and the lenght of the int in characters
int eeprom_current_integers[menu_option_lines*2]; //loads the integers from eeprom, and then is edited while running to be saved later
int eeprom_integer_min_max[menu_option_lines*2]; //stores min/max values for integers

bool display_update_cursor = false;
bool display_update_integers = false;
bool display_update_menu_text = false;

boolean in_navigate_state;

int cursor_selected_menu_option = 1;

class SSD1306_Menu
{
  public:

    class eeprom
  {
      public:
      
    void update_eeprom_integers(int bank_number, int new_int_value){ //used when editing an integer
     
      int n = 0;
      while(eeprom_current_integers[n] != bank_number)
       {
        if(n > menu_option_lines*2){break;}
        n=n+2; //check the next line
       }
      eeprom_current_integers[n+1] = new_int_value;
    }


    void save_eeprom_integers(){
      int n = 0;
      while(n < menu_option_lines*2)
       {
        save_to_EEPROM(eeprom_current_integers[n],eeprom_current_integers[n+1]);
        n=n+2;
       }
     }
  };

void Setup(){ //set variables to defailt
    in_navigate_state = true;
    //EEPROM_int_text_offset[(cursor_selected_menu_option*2)+1] = '0';

    display_update_cursor = true;
    display_update_integers = true;
    display_update_menu_text = true;
}

void update_cursor(){

  if(display_update_cursor == false){return;}

  int cursor_range = (8/menu_text_size);
  int last_line;
  int n = 0;
  int c = 0;
  

 if(menu_number != 0){ //move selected_line to the correct first line
  do{
    if(menu_number == 0){break;};
    n=n+menu_options_section_lenght[c];
    c++;
    
  }while(c!=menu_number);
  };

 last_line = n+menu_options_section_lenght[c];
 int line_selected = n*2;

 if(cursor_selected_menu_option-1 < n){cursor_selected_menu_option = n+1;} //dont allow cursor_selected_menu_option to go outside the current submenu
 if(cursor_selected_menu_option > last_line){cursor_selected_menu_option = last_line;} //dont allow cursor_selected_menu_option to go outside the current submenu

 c = 0;
 do{
  display.setCursor(0,c);
  if(line_selected/2 == cursor_selected_menu_option-1){display.print('>');}
  else(display.print(' '));
  
  c = c + 1;
  line_selected=line_selected+2;
 }while(line_selected/2 < n+menu_options_section_lenght[menu_number]);

 display_update_cursor = false;
}

void update_integers(){

  if(display_update_integers == false){return;}

 int n;
 int c = 0;

 if(menu_number != 0){ //move selected_line to the correct first line
 do{n=n+menu_options_section_lenght[c]; c++;}while(c!=menu_number);
 };

 int line_selected = n*2;

 do
  {
    c = c + 1;
    line_selected=line_selected+2;

 if(eeprom_current_integers[line_selected]!=0) //if there is a EEPROM stored variable
 {
    //EEPROM_int_text_offset[line_selected+1] = 'E'; //E is EEPROM type of current selected line

    //fix the offset if the character was changed

    int r = eeprom_current_integers[line_selected+1];// --> loaded integer
    //EEPROM_int_text_offset[b/2] -- size of it in characters
    //if size not correct --> update size

      if(r>=0)
      {
       if (r>=0 && r<10){EEPROM_int_text_offset[line_selected]=1;}
       if (r>=10 && r<100){EEPROM_int_text_offset[line_selected]=2;}
       if (r>=100 && r<1000){EEPROM_int_text_offset[line_selected]=3;}
       if (r>=1000 && r<10000){EEPROM_int_text_offset[line_selected]=4;}
      }
      if (r<0)
      {
       if (r<=-0 && r>-10){EEPROM_int_text_offset[line_selected]=2;}
       if (r<=-10 && r>-100){EEPROM_int_text_offset[line_selected]=3;}
       if (r<=-100 && r>-1000){EEPROM_int_text_offset[line_selected]=4;}
       if (r<=-1000 && r>-10000){EEPROM_int_text_offset[line_selected]=5;}
      }
      //}while();





  int f = (SCREEN_WIDTH-(menu_text_size*1)-1) - (EEPROM_int_text_offset[line_selected]*(menu_text_size)); //EEPROM_int_text_offset[b/2] is the number of characters that have to he shifted for int to not go off the screen; 104 is the offset of the last character (]); 12 is the width of each character
  

  //display.setCursor(f, c-16);
  display.setCursor(f, c-1);

 //Serial.println(c-(8*menu_text_size));

  display.print('[');
  display.print(eeprom_current_integers[line_selected+1]);
  display.print(']');
 ;}

  }while(line_selected/2 < n+menu_options_section_lenght[menu_number]);

  display_update_integers = false;

}

int get_option_type(){
 
  return EEPROM_int_text_offset[(cursor_selected_menu_option*2)+1];
}

void Up(){ //if in_navigate_state == ture, move cursor
    if(in_navigate_state == true){
      cursor_selected_menu_option++;
      display_update_cursor = true;
      return;
    }

    if(in_navigate_state == false){
      Serial.println(eeprom_integer_min_max[cursor_selected_menu_option*2+1]);
      if(eeprom_integer_min_max[(cursor_selected_menu_option*2)+1] > eeprom_current_integers[(cursor_selected_menu_option*2)+1]){
        eeprom_current_integers[(cursor_selected_menu_option*2)+1]++;
        //Serial.println((cursor_selected_menu_option*2)+1);
      if(eeprom_current_integers[(cursor_selected_menu_option*2)+1] < eeprom_integer_min_max[(cursor_selected_menu_option*2)+0]){ //if its larger than maximum allowed value, reset to maximum allowed value
          eeprom_current_integers[(cursor_selected_menu_option*2)+1] = eeprom_integer_min_max[(cursor_selected_menu_option*2)+0];
      }
      }
      display_update_integers = true;
      return;

    }
}

void Down(){ //if in_navigate_state == true, move cursor
    if(in_navigate_state == true){
      cursor_selected_menu_option--;
      display_update_cursor = true;
      return;
    }
    
    if(in_navigate_state == false){
      //Serial.println(eeprom_current_integers[cursor_selected_menu_option*2]);
      if(eeprom_integer_min_max[(cursor_selected_menu_option*2)+0] < eeprom_current_integers[(cursor_selected_menu_option*2)+1]){
        eeprom_current_integers[(cursor_selected_menu_option*2)+1]--;
        //Serial.println((cursor_selected_menu_option*2)+1);
        if(eeprom_current_integers[(cursor_selected_menu_option*2)+1] > eeprom_integer_min_max[(cursor_selected_menu_option*2)+1]){ //if its larger than maximum allowed value, reset to maximum allowed value
          eeprom_current_integers[(cursor_selected_menu_option*2)+1] = eeprom_integer_min_max[(cursor_selected_menu_option*2)+1];
        }
      }
      display_update_integers = true;
      return;
    }
}

void Ok(){ //get state of variables

    if(in_navigate_state == false){
      //Serial.println("e");
      in_navigate_state = true;
      return;
    }

    Serial.print("state "); Serial.println(in_navigate_state);
    Serial.print("type" ); Serial.println(get_option_type());

    if(in_navigate_state == true &&  get_option_type() == 'E'){ //E is EEPROM type of current selected line
      
      in_navigate_state = false;
      return;
    }
}

void set_int_limit(int integer_ID, int MIN = -32768, int MAX = 32767){ //assigned eeprom number, min, max -saves to eeprom_integer_min_max[]
    //; //eeprom_current_integers[1] gives integer_ID line by line
    //; //eeprom_integer_min_max[1,2] gets saved based on on what line integer_ID relative on the eeprom_current_integers
    int line_selected = 0;
    while (eeprom_current_integers[line_selected] != integer_ID){
      line_selected = line_selected+2;
    }
    eeprom_integer_min_max[line_selected] = MIN;
    eeprom_integer_min_max[line_selected+1] = MAX;
    Serial.println("assigned MIN, MAX to line: ");Serial.println(line_selected/2);
}

};





void menu_endpoints(int menu_options, char menu_option_text[])
{
  int menu_options_lenght[menu_options];
  int i = 0;
  int a = 0;
  int n = 0;
  int e = 0;
  //int x = 0;
  //int k = -1;

  do
  {
    a = menu_option_text[i] - 47;
    
 //---------------   
    if(menu_option_text[i]=='[') //if there is eeprom read mark
    {
      e = i;
      int k = 0;
      int x = 0;
      int r = 0;

      //Serial.println(menu_option_text[e+1]);
      do{e++;}while(menu_option_text[e]!=']');
    //Serial.println((e+1)-i);
    
    //Serial.print("menu_option_text[e]:");
    //Serial.println(menu_option_text[e]);
    do{
      k++; //Serial.println(menu_option_text[e-(k)]);
      int v = menu_option_text[e-(k)]-48; r = 1; while(r!=k){v=v*10, r++;} x=x+v;
      //Serial.print("r:"); Serial.println(r);
      }
    while(menu_option_text[(e-k)-1]!='[');
    
    eeprom_current_integers[n] = x;
    eeprom_current_integers[n+1] = load_from_eeprom(x);
    //eeprom_current_integers[n+3] = n;

    EEPROM_int_text_offset[n+1] = 'E';
    Serial.print("E:");Serial.println(n);

    Serial.print("x:"); Serial.println(x); //save int location in eeprom 
    Serial.print("l:"); Serial.println(eeprom_current_integers[n+1]); //loaded value from eeprom
    //Serial.print("r:"); Serial.println(r);

a = 0;

//--------------
    //Serial.print("dd:"); Serial.println((menu_option_text[i+1]-48));

      //Serial.println(menu_option_text[i+1]-48);
      i=i+(e+1)-i; //skip the "[**]" (eeprom readmark)
      a = menu_option_text[i] - 47;
    }

    
     //main_menu_start_read_positions[n] = a;
    //menu_options_lenght[n] = i;

 
    
   
    //if(menu_option_text[a] == 0){break;};
    
    
    
    //Serial.print("spacing:"); Serial.println(i);
    menu_array[n] = i;
    if(a<0){break;};
    //Serial.print("lenght:"); Serial.println(a);
    menu_array[n+1] = a;
    
    
    //Serial.print("n:"); Serial.println(n);
     

    i = i + a;
    n = n + 2;

    

  }
  while (a > 0);
  
}

void menu(char text_array[], int start_positions[], int menu_lengts[])
{
  if(display_update_menu_text == false){return;}
  //display.clearDisplay();

   //temp 
 //int i=0;
 //do{
   //Serial.print("A:"); Serial.println(start_positions[i]);
   //Serial.print("I:"); Serial.println(start_positions[i+1]);
 //  i=i+2;
 //  }
 //while(i!=24);


 int a=1; int b=0; int c=0; int n=0;
 
//b=start_positions[0] //menu_lengts[0]; //0>menu

//b>>0 if menu_number == 0
//b>>18 if menu_number == 1 (9*2) menu_number

if(menu_number != 0)
{
do{n=n+menu_lengts[c]; c++;}while(c!=menu_number);
//Serial.print("n:"); Serial.println(n);
};

c=0; b=n*2;

  do
  {
     display.setCursor(menu_text_size, c);
     //c = c + 16;
     c = c + (menu_text_size);
     do{display.print(text_array[start_positions[b]+a]);
     a=a+1;
     if(a>=start_positions[b+1]){break;};
     }while(a!=(start_positions[(b+2)])-(start_positions[(b)]));
     //display.println(); //new line
     a=1;
     b=b+2; 

//if(b/2 == (n+menu_lengts[menu_number])){break;}

//Serial.print("b:"); Serial.println(b/2);
//Serial.println(start_positions[b-1]);





  }
  while (b/2 < n+menu_options_section_lenght[menu_number]); //0>menu

//Serial.println(n+menu_lengts[menu_number]);
display_update_menu_text = false;

Serial.println(EEPROM_int_text_offset[3]);
}