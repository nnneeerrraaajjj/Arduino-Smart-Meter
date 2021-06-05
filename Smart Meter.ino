#include<LiquidCrystal.h>
#include<SoftwareSerial.h>
int tlimit=40;
float usage;
char pno[]="7769092556";
int flag=0;
float vsample1=0.0; // read the Vout from voltage divider
float voltage=0.0;
float csample2=0.0;
float val=0.0;
float actualval=0.0;
float amps=0.0;
float totamps=0.0;
float avgamps=0.0;
float amphr=0.0;
float watt=0.0;
float energy=0.0;
int on;
int relay=7;
int count;
int dflag=1;
int cflag=0;
byte b[8]=
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
  };
  byte d[8]=
{
  B11111,
  B11111,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111
  };
  byte e[8]=
{
  B11111,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
  };
  byte f[8]=
{
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B11111,
  B11111
  };
byte g[8]=
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B11111
  };

byte c[8]=
{
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111
  };
byte h[] = {
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B10101,
  B11111,
  B10101
};
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(9, 10);
void setup()
{
  
  lcd.begin(16,2);
  lcd.createChar(1,b);
  lcd.createChar(2,c);
  lcd.createChar(3,d);
  lcd.createChar(4,e);
  lcd.createChar(5,f);
  lcd.createChar(6,g);
  lcd.createChar(7,h);
  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.write(7);
  lcd.print("Friendometer");
  lcd.write(7);
  lcd.write(2);
  
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(3);
  lcd.write(1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(5);
  lcd.write(1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(3);
  lcd.write(1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(5);
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
  delay(3000);
}
void loop()
{
  long milisec = millis();
  long time=milisec/1000;
  
 
  for(int i=0;i<150;i++)
  {
    vsample1+=analogRead(A2);
    csample2+=analogRead(A3);
    delay(2);
    
  }
  

  
 vsample1=vsample1/150; 
 voltage=4.669*2*vsample1/1000;

 csample2=csample2/150;
 val =(5.0*csample2)/1024.0;
 actualval =val-2.5; 
 amps =actualval*10;

totamps=totamps+amps; 
avgamps=totamps/time; 
amphr=(avgamps*time)/3600; 
watt =voltage*amps;
energy=(watt*time)/3600;
count=tlimit-time;
usage=((energy*2.5)+(time/10))/100;
if (usage<0){
  usage=0;
  }

if (count<=0){
  count=0;
  }

if(dflag==1){   
lcd.clear();
lcd.setCursor(1,0);
lcd.print(watt);
lcd.print("W ");
lcd.print(voltage);
lcd.print("V");
lcd.setCursor(1,1); 
lcd.print(energy);
lcd.print("WH ");
lcd.print(amps);
lcd.print("A");
delay(3000);
//idk...
lcd.clear();
lcd.setCursor(0,2);
lcd.print(" Usage: Rs.");
lcd.print(usage);
lcd.setCursor(1,0);
milisec = millis();
time=milisec/1000;
count=tlimit-time;
if(count>=0){
lcd.print(count);
lcd.print(" Seconds left");
}
else{
  lcd.print("Timed out");
  }
delay(1000);
lcd.clear();
lcd.setCursor(0,2);
lcd.print(" Usage: Rs.");
lcd.print(usage);
lcd.setCursor(1,0);
milisec = millis();
time=milisec/1000;
count=tlimit-time;
if(count>0){
lcd.print(count);
lcd.print(" Seconds left");
}
else{
  lcd.print("Timed out");
  }
delay(1000);
lcd.clear();
lcd.setCursor(0,2);
lcd.print(" Usage: Rs.");
lcd.print(usage);
lcd.setCursor(1,0);
milisec = millis();
time=milisec/1000;
count=tlimit-time;
if(count>0){
lcd.print(count);
lcd.print(" Seconds left");
}
else{
  lcd.print("Timed out");
  }
  delay(1000);
}
milisec = millis();
time=milisec/1000;
if(time>tlimit){
  if(flag==0 || flag==1){
lcd.clear();
lcd.setCursor(1,0);
lcd.print("Turning off");
digitalWrite(relay,LOW);
if(cflag==0){
  lcd.setCursor(0,1);
  lcd.print(" Calling...");
  mySerial.println("AT");
  mySerial.print("ATD");
  mySerial.print(pno);
  mySerial.println(";");
  delay(5000);
  mySerial.println("ATH");
  lcd.clear();
  cflag=1;
    }
dflag=0;
delay(3000);
flag=1;
  }
}
if(flag==1){
  on=0;
  Serial.println("Enter Code...");
  delay(3000);
  }

  if(Serial.available()>0){
    if(Serial.peek()== 'c'){
      Serial.read();
      on=Serial.parseInt();  
    }
       
  }
  if(on>=1){
  if(flag==1){
    lcd.clear();
    lcd.setCursor(1,0); 
    lcd.print("Turning on");
    lcd.setCursor(0,1);
    lcd.print(" For ");
    lcd.print(on);
    lcd.print(" Seconds");
    delay(2000);
    digitalWrite(relay,HIGH);
    flag=0;
    dflag=1;
    cflag=0;
    milisec = millis();
    time=milisec/1000;
    tlimit=time+on;
    }
  } 

}
