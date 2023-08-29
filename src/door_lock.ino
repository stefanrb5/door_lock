#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27,16,2);  

const byte ROWS = 4; 
const byte COLS = 4;


char password[5] ={"1234"} ;
char new_password[5];
char check[5];

char start,choose;
int choose_mode;
int check_old=0;
int RED = 11, GREEN = 10, BUZZER = 12;
int i,j,password_good=0;
int counter_check=0;
int counter_np;
char customKey;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {

  lcd.init();
  lcd.clear();         
  lcd.backlight();

  Serial.begin(9600);
 
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
    
}

void loop() 
{
  Serial.println(password);
  menu();
  if(choose_mode == 1)
  {
    set_password();
  }
  else if(choose_mode == 2)
  {
    check_f();
  }
}

void buzzer_wrong() //function used when password is wrong to activate the buzzer and red led
{
  int i;
  for(i=0; i<3;i++)
  {
    tone(BUZZER, 1000); 
    digitalWrite(RED,HIGH);
    delay(500);
    digitalWrite(RED,LOW);       
    noTone(BUZZER);
    delay(500);    
  }
}

void check_f() //function used for checking if the password is good
{
  lcd.print("Password:");
  do
  {  
    check[counter_check]=customKeypad.getKey();
    if(check[counter_check])
      {
        lcd.setCursor(counter_check,1);
        lcd.print('*');
        delay(100);
 	      counter_check++;
      }
  }while(counter_check < 4);
  
   counter_check=0;
   lcd.clear();
  
   for(i=0;i<4;i++)
   {
    for(j=0;j<4;j++)
    {
      if(password[i]!=check[j])
      {
        password_good=1;
        check_old=0;
      }
      else
      {
        password_good = 0;
        check_old = 1;
      }   
    }
   }
   if(password_good == 1)
   {
    lcd.setCursor(0,1);
    lcd.print("Wrong password"); 
    buzzer_wrong();
    lcd.clear();
   }
   else if(password_good==0)
   {
    buzzer_correct();
    lcd.clear();
  	password_good=0;
   }
}

void buzzer_correct() // function used when password is good to activate green led
{
  lcd.print("Welcome");
  digitalWrite(GREEN, HIGH);
  delay(3000);
  digitalWrite(10, LOW);
}

void menu() // function used to generate menu
{
  do
  {
    lcd.setCursor(0,0);
    lcd.print("Press 1 for menu");
    start=customKeypad.getKey();
  }while(start !='1');
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1.set password");
  lcd.setCursor(0,1);
  lcd.print("2.unlock");
  
  do
  {
    choose=customKeypad.getKey();
      if(choose == '1')
      {
        choose_mode = 1; 
      }
      else if(choose == '2')
      {
        choose_mode = 2;
      }
  }while(choose!='1' && choose!='2');
 
  lcd.clear();
  Serial.println(choose_mode);
}

void set_password() // function used for setting the new password
{
  lcd.clear();
  check_f();

  if(check_old == 1)
  {
  lcd.print("Length must be 4");

    do
    {  
      new_password[counter_np]=customKeypad.getKey();
      if(new_password[counter_np])
      {
        lcd.setCursor(counter_np,1);
        lcd.print(new_password[counter_np]);
        delay(100);
 	      counter_np++;
      }
    }while(counter_np < 4);

  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
       password[i] = new_password[j];
    }
  }

   counter_np=0;
   lcd.clear();
   lcd.print("Password set");
   lcd.setCursor(0, 1);
   lcd.print("successfully");
   delay(2000);
   lcd.clear();

  }
}
