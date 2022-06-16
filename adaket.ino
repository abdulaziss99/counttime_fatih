#include <LiquidCrystal_I2C.h>
#include "Countimer.h"
Countimer tdown;
LiquidCrystal_I2C lcd(0x27,20,4);
#include <EEPROM.h>

#define bt_set    14
#define bt_up     15
#define bt_down   16
#define bt_start  17

int time_s = 0;
int time_m = 0;
int time_h = 0;

int set = 0;
int flag1=0, flag2=0;

int relay = 8;
int buzzer = A5;

void setup() {
Serial.begin (9600);

pinMode(bt_set,   INPUT_PULLUP);
pinMode(bt_up,    INPUT_PULLUP);
pinMode(bt_down,  INPUT_PULLUP);
pinMode(bt_start, INPUT_PULLUP);

pinMode(relay, OUTPUT);
pinMode(buzzer, OUTPUT);

lcd.begin(16, 2);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("   Welcome To   ");
lcd.setCursor(0,1);
lcd.print("Countdown  Timer");
tdown.setInterval(print_time, 999);
eeprom_read();
delay(1000);
lcd.clear();
}

void print_time(){
time_s = time_s-1; // detik kurang -1
if(time_s<0){time_s=59; time_m = time_m-1;} // bila menit berkurang -1 maka akan dimulai dari detik 59
if(time_m<0){time_m=59; time_h = time_h-1;} // bila jam berkurang -1 makan akan dimulai dari menit 59
}

void tdownComplete(){Serial.print("ok");}

//tdown.stop(); 

void loop(){
tdown.run();

if(digitalRead (bt_set) == 0){
if(flag1==0 && flag2==0){flag1=1; //flag adalah sebagai pengunci dan pertanda suatu algoritma
set = set+1; // jika di klik tombol set maka akan merubah menu set
if(set>3){set=0;} // set3 artinya ada menu set timer HH (hours), set time MM (minute), set timer SS (second)
delay(100); 
}
if 
}else{flag1=0;}

if(digitalRead (bt_up) == 0){
if(set==0){tdown.start(); flag2=1;}
if(set==1){time_s++;} // menambah detik
if(set==2){time_m++;} // menambah menit
if(set==3){time_h++;} // menambah jam
if(time_s>59){time_s=0;} // jika detik sudah sampai 59 maka akan mengulang dari 0 atau sama saja sudah sampai 60 dan akan menambah menit
if(time_m>59){time_m=0;} // jika menit sudah sampai 59 maka akan mengulang dari 0 maka akan menambah jam
if(time_h>99){time_h=0;} // jika jam sudah 99 maka akan mengulang dari 0. karena ardiuno bila hidup secara terus menerus lebih dari 4 hari maka akan reset
if(set>0){eeprom_write();} // tag eeprom_write adalah untuk menjalankan tugas handling karena semua sudah dikerjakan sesuai dengan set dan library
delay(200); 
}

if(digitalRead (bt_down) == 0){
if(set==0){tdown.stop(); flag2=0;}
if(set==1){time_s--;} // set 1 : set timer ss (second) bila di klik akan berkurang detik
if(set==2){time_m--;} // set 2 : set timer mm (minute) bila di klik akan berkurang menit
if(set==3){time_h--;} // set 3 : set time hh (hours) bila di klik akan berkurang jam
if(time_s<0){time_s=59;} // jika detik sudah sampai 59 maka akan mengulang dari 0 atau sama saja sudah sampai 60 dan akan menambah menit
if(time_m<0){time_m=59;} // jika menit sudah sampai 59 maka akan mengulang dari 0 maka akan menambah jam
if(time_h<0){time_h=99;} // jika jam sudah 99 maka akan mengulang dari 0. karena ardiuno bila hidup secara terus menerus lebih dari 4 hari maka akan reset
if(set>0){eeprom_write();} // tag eeprom_write adalah untuk menjalankan tugas handling karena semua sudah dikerjakan sesuai dengan set dan library
delay(200); 
}

if(digitalRead (bt_start) == 0){ flag2=1; // flag2 adalah kode binner 1 sama saja dengan benar, bila di klik button start maka akan mulai
  eeprom_read(); // handling
  digitalWrite(relay, HIGH); 
  tdown.restart(); // untuk resetrat
  tdown.start(); // mulai
}

lcd.setCursor(0,0); // tampilan saat klik tombol set
if(set==0){lcd.print("      Timer     ");}
if(set==1){lcd.print("  Set Timer SS  ");}
if(set==2){lcd.print("  Set Timer MM  ");}
if(set==3){lcd.print("  Set Timer HH  ");}

lcd.setCursor(4,1); // tampilan waktu yang berjalan
if(time_h<=9){lcd.print("0");} // jika jam sudah 99 maka akan mengulang ke 0
lcd.print(time_h);
lcd.print(":");
if(time_m<=9){lcd.print("0");} // jika menit sudah 69 maka akan mengulang ke 0
lcd.print(time_m);
lcd.print(":");
if(time_s<=9){lcd.print("0");} // jika detik sudah 99 makan akan mengulang ke 0
lcd.print(time_s);
lcd.print("   ");

if(time_s==0 && time_m==0 && time_h==0 && flag2==1){flag2=0; // untuk otomatis jika semua sudah 0 maka akan mengeksekusi relay
tdown.stop(); 
digitalWrite(relay, LOW);
digitalWrite(buzzer, HIGH);
delay(300);
digitalWrite(buzzer, LOW);
delay(200);
digitalWrite(buzzer, HIGH);
delay(300);
digitalWrite(buzzer, LOW);
delay(200);
digitalWrite(buzzer, HIGH);
delay(300);
digitalWrite(buzzer, LOW);
}

if(flag2==1){digitalWrite(relay, HIGH);}
else{digitalWrite(relay, LOW);}

delay(1);
}

void eeprom_write(){
EEPROM.write(1, time_s);  // library
EEPROM.write(2, time_m);  // library
EEPROM.write(3, time_h);  // library
}

void eeprom_read(){
time_s =  EEPROM.read(1); // membaca apa yang di program dan kesesuaian dengan library
time_m =  EEPROM.read(2); // membaca apa yang di program dan kesesuaian dengan library
time_h =  EEPROM.read(3); // membaca apa yang di program dan kesesuaian dengan library
}
