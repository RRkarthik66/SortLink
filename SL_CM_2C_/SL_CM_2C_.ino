//#include "esp_system.h"
#include <WiFi.h>
//#include <PubSubClient.h>f
#include <EEPROM.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

//sensor input variables
//const int button         = 0;         //gpio to use to trigger delay
const int beltDetection    = 32;
const int fullCopDetection = 33;
const int yarn1            = 34;
const int color1           = 35;
const int obj1             = 36;
//const int yarn2            = 37;
const int color2           = 37;
const int obj2             = 38;
const int crate1           = 39;
const int crate2           = 40;
const int pas              = 41;

//sensor output variable
//const int hzMotor       = 3;
const int vcMotor       = 2;
const int ccb           = 3;
const int s1            = 4;
const int s2            = 5;
const int s3            = 6;
const int copStorage1   = 7; //orientation box 1
const int copStorage2   = 8; //orientation box 2
const int crateEjector1 = 9; //bin change 1
const int crateEjector2 = 10; //bin change 2
const int binLock1      = 11; 
const int binLock2      = 12;
const int binUp1        = 14; //bin down 1
const int binUp2        = 15;  //bin down 2
//const int alarmLightred = 36;
const int alarmLightyellow = 16;
//const int alarmLightgreen = 39;

//ccb
bool loaderState = false;
unsigned long timerccb = 0;

/*shutter1*/
//1 yarn
unsigned long yarn2simulatorPreviousPulse = 0;
//to adjust yarn2 pulse delay
int yarn2pulsedelay = 16;
bool yarn2simulatorflag = false;
bool yarn1flag =  false;
int yarn2index = 0;
int yarn2indexcurrent = 0;
//int yarn2currentOutput = 0;
int yarn2simTrainarray[100] ;
unsigned long timeryarn1 = 0;
bool yarn1detected = false;
//color1
bool color1flag = false;
bool s2on = false;
bool color1identified = false;
//object1
bool Object1flag1 = false;
bool Object1Detected = false;
unsigned long Object1DetectTmr = 0;
bool Shutter2OnReady = false;
bool Shutter2Onflag1 = false;
bool Shutter2Onflag2 = false;
bool C1CountFlag2 = false;
bool C1CountFlag1 = false;
short s2OnDelay = 2; //was2
short s2OffDelay=6;//was 6
unsigned long color1Cnt = 0;
unsigned long color1Cntflag = 0;

//copstorage1
bool flag1CopStorage1 = false;
bool flag2CopStorage1 = false;
unsigned long timer1CopStorage1 =  0;
unsigned long timer2CopStorage1 =  0;
bool copstorage1highflag = false;
bool copstorage1lowflag = false;
short copStorage1Ondelay=20;
short copStorage1Offdelay=10;
bool copstorage1Shutterflag1 = false;
bool copstorage1Shutterflag2 = false;
unsigned long copStorage1Count = 0 ;
unsigned long copStorage1Countflag = 0;
short numCopStorage1 = 4;


//crate1error 
bool tray1check = false;
bool tray1checkflag = false;
unsigned long crate1alert = 0;
short crate1alertondelay=200;
bool crate1error = false;
unsigned long  binup1inerrortime = 0;
unsigned long  binup1inerrortime2 = 0;
bool binup1inerrorok1 = false;
bool binup1inerrorok2 = false;
bool crate1alertcheck = false;

//crate1ejection
bool flag1CrateEjection1 = false;
bool flag2CrateEjection1 = false;
unsigned long timer1CrateEjection1 = 0 ;
unsigned long timer2CrateEjection1 = 0 ;
bool flag3CrateEjection1 = false;
short crate1RemoveOndelay=40;
bool crate1Complete = false;

//crate1replacement
bool crate1ProcessFlag = false;
bool flag1binUp1 =  false;
unsigned long timer1binUp1 = 0 ;


/*shutter3*/
//yarn2
int yarn2currentOutput = 0;
bool yarn2flag =  false;
bool yarn2detected =false;
//color2
bool color2flag = false;
bool s3on = false;
bool color2identified = false;
//object2
bool Object2flag1 = false;
bool Object2Detected = false;
unsigned long Object2DetectTmr = 0;
bool Shutter3OnReady = false;
bool Shutter3Onflag1 = false;
bool Shutter3Onflag2 = false;
bool C2CountFlag2 = false;
bool C2CountFlag1 = false;
short s3OnDelay = 2; //was2
short s3OffDelay=6;//was 6
unsigned long color2Cnt = 0;
unsigned long color2Cntflag = 0;

//copstorage2
bool flag1CopStorage2 = false;
bool flag2CopStorage2 = false;
unsigned long timer1CopStorage2 =  0;
unsigned long timer2CopStorage2 =  0;
bool copstorage2highflag = false;
bool copstorage2lowflag = false;
short copStorage2Ondelay=20;
short copStorage2Offdelay=10;
bool copstorage2Shutterflag1 = false;
bool copstorage2Shutterflag2 = false;
unsigned long copStorage2Count = 0 ;
unsigned long copStorage2Countflag = 0;
short numCopStorage2 = 4;


//crate2error 
bool tray2check = false;
bool tray2checkflag = false;
unsigned long crate2alert = 0;
short crate2alertondelay=200;
bool crate2error = false;
unsigned long  binup2inerrortime = 0;
unsigned long  binup2inerrortime2 = 0;
bool binup2inerrorok1 = false;
bool binup2inerrorok2 = false;
bool crate2alertcheck = false;

//crate2ejection
bool flag1CrateEjection2 = false;
bool flag2CrateEjection2 = false;
unsigned long timer1CrateEjection2 = 0 ;
unsigned long timer2CrateEjection2 = 0 ;
bool flag3CrateEjection2 = false;
short crate2RemoveOndelay=40;
bool crate2Complete = false;

//crate2replacement
bool crate2ProcessFlag = false;
bool flag1binUp2 =  false;
unsigned long timer1binUp2 = 0 ;

//krichicount
bool krichi1 = false;
bool krichi2 = false;
unsigned long krichi1Cnt = 0;
unsigned long krichi2Cnt = 0;


//errors2
bool error=false;
bool errorflag=false;
bool erroroccured=false;
bool erroroccured1=false;
bool erroroccuredoff=false;
unsigned long errorontmr=0;
unsigned long errorofftmr=0;
//obj1
bool objerrflag=false;
bool objerrflag1=false;
unsigned long objerrtimer=0;
unsigned long objerrtimerondelay=100;
//color1
bool colorerrflag=false;
bool colorerrflag1=false;
unsigned long colorerrtimer=0;
unsigned long colorerrtimerondelay=100;
//yarn1
bool yarnerrflag=false;
bool yarnerrflag1=false;
unsigned long yarnerrtimer=0;
unsigned long yarnerrtimerondelay=100;


//errors3
bool errors3=false;
bool errors3flag=false;
bool errors3occured=false;
bool errors3occured1=false;
bool errors3occuredoff=false;
unsigned long errors3ontmr=0;
unsigned long errors3offtmr=0;
//obj2
bool objerrflag2=false;
bool objerrflag12=false;
unsigned long objerrtimer2=0;
unsigned long objerrtimerondelay2=100;
//color2
bool colorerrflag2=false;
bool colorerrflag12=false;
unsigned long colorerrtimer2=0;
unsigned long colorerrtimerondelay2=100;


//colorcount reset
unsigned long c1Cntreset = 5;
unsigned long c2Cntreset = 5;
bool krichiCntFlag = false;
unsigned long  a=0;

//belt
bool toggle = false;
bool toggle1 = false;
unsigned long beltonDetection = 0;
unsigned long beltoffDetection= 0;
unsigned long timeblock=100;


//belt error
bool errorB=false;
//bool errorBflag=false;
bool errorBoccured=false;
//bool errorBoccured1=false;
//bool errorBoccuredoff=false;
//unsigned long errorBontmr=0;
//unsigned long errorBofftmr=0;

//full bobbin error
bool errorFC = false;
bool fberrflag=false;
bool fberrflag1=false;
unsigned long fberrtimer=0;
unsigned long fberrtimerondelay=100;
//full cop
bool togglefc=false;
unsigned long fullcopBeltSeperationTimerStart=0;
unsigned long fullcopBeltSeperationTimerEnd=0;
unsigned long timefc=0;
unsigned long fullcopinterval=0;
short s1OnDelay=1;
short s1OffDelay=4;
unsigned long FCCnt = 0;
unsigned long FCCountFlag = 0;



void setup() {
  
  Serial.begin(9600);//bits per second
  // initialize the LCD
  lcd.init(); 
  // Turn on the blacklight and print a message.
  lcd.backlight();
  pinMode(beltDetection, INPUT);
  pinMode(fullCopDetection, INPUT);
  pinMode(color1, INPUT);
  pinMode(color2, INPUT);
  pinMode(obj1, INPUT);
  pinMode(obj2, INPUT);
  pinMode(yarn1, INPUT);
  pinMode(crate1, INPUT);
  pinMode(crate2, INPUT);
  pinMode(pas, INPUT);

  pinMode(ccb,OUTPUT);
  pinMode(vcMotor,OUTPUT);
  //pinMode(s1,OUTPUT);
  pinMode(binLock2,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(copStorage1,OUTPUT);
  pinMode(copStorage2,OUTPUT);
  pinMode(crateEjector1,OUTPUT);
  pinMode(crateEjector2,OUTPUT);
  pinMode(binUp1,OUTPUT);
  pinMode(binUp2,OUTPUT);
  pinMode(binLock1,OUTPUT);
  
  //pinMode(alarmLightred,OUTPUT);
  pinMode(alarmLightyellow,OUTPUT);
 // pinMode(alarmLightgreen,OUTPUT);

lcd.setCursor(0,0);
lcd.print("C1=");
lcd.setCursor(7,0);
lcd.print("C2=");
lcd.setCursor(0,1);
lcd.print("K=");
lcd.setCursor(7,1);
lcd.print("FC=");


 
}



void loop(){
  //this is both horizontal and vertical
  digitalWrite(vcMotor,HIGH);
    //Cop Collecting Box
  if (((millis() - timerccb) > 2000) /*and error == false*/)
  {
    loaderState = !loaderState;
    digitalWrite(ccb, loaderState);
    timerccb = millis();
    //Serial.println(loaderOn);
  }

/*********************belt movement detection*************/ 
if (digitalRead(beltDetection) == true and toggle == false )
  {
    beltonDetection = millis();
    toggle = true;
    toggle1 = false;
  }
  
if (digitalRead(beltDetection) == false and toggle1 == false )
  {
    toggle = false;
    toggle1 = true;
    beltoffDetection = millis();
  }
  
  
if ( (millis() - beltonDetection > (timeblock * 50)) or (millis() - beltoffDetection > (timeblock * 50)))
  {
    lcd.setCursor(0,2);
    lcd.print("BELT ERROR");
    Serial.println("Vertical belt error occured");
    errorB = true;
  }

/*error for belt*/

if (errorB == true and errorflag == false)
  {
    errorBoccured = true;
    errorflag = true;
    //digitalWrite(alarmLightred,HIGH);
  }
 
  
if (errorB == true and errorBoccured == true)
  {
    digitalWrite(alarmLightyellow,HIGH);
    //errorBontmr = millis();
    errorBoccured = false;
    //errorBoccured1 = true;
    
  }

/*full bobin ejection*/

//**************Full bobbin Sensor error**************
if (digitalRead(fullCopDetection) == true and fberrflag == false)
  {
    fberrtimer = millis();
    fberrflag1 = true;
    fberrflag = true;
    
  }

if (digitalRead(fullCopDetection) == false )
  {
    fberrflag = false;
    fberrflag1 = false;
  }
   
if ((millis() - fberrtimer > (fberrtimerondelay * 50) and fberrflag1 == true) )
  { 
    Serial.println("Full bobbin error occured");
    errorFC = true;
  }




//Full Cop detection
  if (digitalRead(fullCopDetection) == true and togglefc == false)
  {
    fullcopBeltSeperationTimerStart= millis();
    togglefc = true;
  }
  
  if (digitalRead(fullCopDetection) == false and togglefc == true )
  { 
    fullcopBeltSeperationTimerEnd = millis();
    
    fullcopinterval = fullcopBeltSeperationTimerEnd - fullcopBeltSeperationTimerStart;
    if (fullcopinterval > 200)
      {
        Serial.println("start of Full cop"+String(fullcopBeltSeperationTimerStart));
        Serial.println("End of Full cop"+ String(fullcopBeltSeperationTimerEnd));
        Serial.println("belt cut time exceeded threshold: " + String(fullcopBeltSeperationTimerEnd - fullcopBeltSeperationTimerStart));
        Serial.println("setting timefc");
        timefc = millis();
      }
    togglefc = false;
    
  }
  if ((millis() - timefc > s1OnDelay*50) and (millis() - timefc < (s1OnDelay+s1OffDelay)*50) )
  { 
    digitalWrite(s1,HIGH);
    FCCountFlag = true;
}
  else
  {
    digitalWrite(s1,LOW);
    FCCountFlag = false;
  }

if (FCCountFlag == true)
{
FCCountFlag = false;
FCCnt++;
Serial.print("FC count=");
Serial.println(FCCnt);
lcd.setCursor(10,1);
lcd.print(FCCnt);
 
}  


/*shutter2 start*/
/*s2error */
if (error == true and errorflag == false)
  {
    
    erroroccured = true;
    errorflag = true;
  }
 
  
if (error == true and erroroccured == true)
  {
    errorontmr = millis();
    erroroccured = false;
    erroroccured1 = true;
    
  }

if (millis() - errorontmr > 1000 and erroroccured1 == true)
  {
    digitalWrite(alarmLightyellow,HIGH);
    errorofftmr = millis();
    erroroccuredoff = true;
    erroroccured1 = false;
  }
if (millis() - errorofftmr > 1000 and erroroccuredoff == true)
  {
    digitalWrite(alarmLightyellow,LOW);
    erroroccuredoff = false;
  errorflag = false;
  }

//object 1 error
if (digitalRead(obj1) == true and objerrflag == false)
  {
    objerrtimer = millis();
    objerrflag1 = true;
    objerrflag = true;
    //objerrtimer1 = millis();
    
  }

if (digitalRead(obj1) == false )
  {
    objerrflag = false;
    objerrflag1 = false;
  }
   
if ((millis() - objerrtimer > (objerrtimerondelay * 50) and objerrflag1 == true) )
  { 
    lcd.setCursor(0,2);
    lcd.print("OBJ1 ERROR");
    Serial.println("Object 1 sensor error occured");
    error = true;
   
   // errorCode = "Object Sensor error";
  }



//color1 error
  if (digitalRead(color1) == false and colorerrflag == false)
    {
      colorerrtimer = millis();
      colorerrflag1 = true;
      colorerrflag = true;
      //objerrtimer1 = millis();
    //Serial.println("Color error test error occured"); 
    }

  if (digitalRead(color1) == true )
    {
      colorerrflag = false;
      colorerrflag1 = false;
    }
   
  
  if ((millis() - colorerrtimer > (colorerrtimerondelay * 50) and colorerrflag1 == true) )
    { 
      lcd.setCursor(0,2);
      lcd.print("CLR1 ERROR");
      Serial.println("color 1 sensor error occured");
      error = true;
      
     // errorCode = "Color Sensor error";
    }


  
//yarn1 error
  if (digitalRead(yarn1) == false and yarnerrflag == false)
  {
    yarnerrtimer = millis();
    yarnerrflag = true;
    yarnerrflag1 = true;
  }

  if (digitalRead(yarn1) == true )
    {
      yarnerrflag = false;
      yarnerrflag1 = false;
    }
    
  if ((millis() - yarnerrtimer > (yarnerrtimerondelay * 50) and yarnerrflag1 == true) )
  { 
    lcd.setCursor(0,2);
    lcd.print("YARN ERROR");
    Serial.println("Yarn sensor error occured");
    error = true;
   
    //errorCode = "Yarn Sensor error";
  }

/*s2error ends*/
// ****************************Yarn1 identification********************************//
    if(digitalRead(yarn1) == false and yarn1flag == false  )
    {
    timeryarn1 = millis();
    yarn1flag = true;
    Serial.println("yarn1 identified");
    yarn2simulatorflag = true;
    }

    if( (millis() - yarn2simulatorPreviousPulse) >50 )
    {
      if( yarn2simulatorflag == true)
      {
       yarn2simTrainarray[yarn2index] = 1;
      }
      if( yarn2simulatorflag == false)
      {
       yarn2simTrainarray[yarn2index] = 0;
      }
      
      yarn2indexcurrent = yarn2index - yarn2pulsedelay;
      if (yarn2indexcurrent < 0)
      {
        yarn2indexcurrent = yarn2indexcurrent + 100 ;
      }

      yarn2currentOutput = yarn2simTrainarray[yarn2indexcurrent] ;
      if (yarn2indexcurrent < 99 and yarn2indexcurrent > 0 )
      {
      yarn2currentOutput = yarn2simTrainarray[yarn2indexcurrent] +
                            yarn2simTrainarray[yarn2indexcurrent - 1] +
                            yarn2simTrainarray[yarn2indexcurrent + 1];
      }
     
//      Serial.println("yarn2index " + String(yarn2index) +"current" + String(yarn2indexcurrent) );
      yarn2index = (yarn2index+1) % 100 ;
      yarn2simulatorPreviousPulse = millis();
      yarn2simulatorflag = false;
      
    }
// ****************************Color 1 identification********************************//

 if(digitalRead(color1) == false and  color1flag == false) //clr1false
 {
  color1flag = true;
  color1identified = true;
  Serial.println("colour1 identified");
  s2on = true;
}   
// ****************************Object 1 identification********************************//

  if(digitalRead(obj1) == true  and Object1flag1 == false)
  {
    Object1Detected = true;//only used inside loop
    Object1flag1 = true;
    Shutter2OnReady = false;
    Serial.println("Object1 identified");
    yarn1flag = false;
    color1flag = false;
    color1identified = false;
  }
//TO TRIGGER SHUTTER2 for color1COP
  if (Object1flag1 == true and digitalRead(obj1) == false and color1identified == true and yarn1flag == false)
      {
      Shutter2Onflag1 = true;
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      Serial.println("Color1 Empty Rested");
      }
  
  //Passing Shutter2 - cop must be 2nd or 3rd color    
     if (Object1flag1 == true and digitalRead(obj1) == false and color1identified == false and yarn1flag == false) 

     {
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false; 
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      Serial.println("Color2 empty Check");
    }
 //To find Yarn at Object1 sensor and use the information for shutter3 and shutter4     
        if   (Object1flag1 == true and digitalRead(obj1) == false and color1identified == false and yarn1flag == true)
    {
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      Serial.println("Color2 yarn Check");
    }
//Color 1 COP with Yarn (color 1 kirchi)
    if   (Object1flag1 == true and digitalRead(obj1) == false and color1identified == true and yarn1flag == true)
    {
      Object1DetectTmr = millis();
      yarn1flag = false;
      yarn1detected = false;
      color1flag = false;
      color1identified = false;
      s2on = false;
      Object1Detected = false;
      Object1flag1 = false;
      krichi1 =  true;
      Serial.println("Color1 yarn Rested");
if (krichi1==true)
{
krichi1Cnt++;
Serial.print("krichi 1=");
Serial.println(krichi1Cnt);
//lcd.setCursor(3,1);
//lcd.print(krichi1Cnt);
krichiCntFlag=true;
krichi1==false;
}
}   
//Shutter2 ON flag
    if (Shutter2Onflag1 == true and Shutter2Onflag2 == false)
        {
          Shutter2OnReady = true;
          Shutter2Onflag2 = true;
          Shutter2Onflag1 = false;   
        }

     if (C1CountFlag1 == false)
     {
      Shutter2Onflag2 = false;
      //Serial.println("Shutter2 off ");
     }

// ****************************Color 1 Ejection********************************//
if( (millis()-Object1DetectTmr) > (s2OnDelay*1) and (millis()-Object1DetectTmr) < (s2OnDelay+s2OffDelay)*50 and Shutter2OnReady == true)//was*50
  
  {
  digitalWrite(s2,HIGH);
  //Serial.println("Shutter 2 On");
  C1CountFlag1 = true;
  }
else{
  digitalWrite(s2,LOW);
  C1CountFlag1 = false;
}



if (C1CountFlag1 == true and C1CountFlag2 == false)
{
  C1CountFlag2 = true;
  color1Cnt++;
  color1Cntflag++;
  Serial.print("color 1 count=");
  Serial.println(color1Cnt);
  lcd.setCursor(3,0);
  lcd.print(color1Cnt);
  
}

if (C1CountFlag1 == false)
{
  C1CountFlag2 = false;
}

//if (color1Cnt >= 4 and C1CountFlag1 == false)//40 //color 1 count resetted
//{
//  color1Cnt = 0;
//}

if (color1Cntflag >=c1Cntreset and C1CountFlag1 == false)//40 //color 1 count resetted
{
  color1Cntflag = 0;
}




/************************************CopStorage 1*******************************/

//if (color1Cnt >= c1Cntreset  and flag1CopStorage1 == false)//40
//{
//  color1Cntflag = color1Cnt;
//  Serial.print("color1Cntflag=");
//  Serial.println(color1Cntflag);
//  flag1CopStorage1 = true;
//}
if(color1Cntflag >= c1Cntreset and flag1CopStorage1 == false /*flag2CopStorage1 == false*/)//40
{
  timer1CopStorage1 = millis(); 
  flag1CopStorage1 = true;
//  flag2CopStorage1 = true;
  copstorage1highflag = true;
  Serial.println("cop storage ready to high ");
  
}

if (millis() - timer1CopStorage1 > copStorage1Ondelay*50  and copstorage1highflag == true)
{
  digitalWrite(copStorage1,HIGH); 
  
  timer2CopStorage1 = millis();
  copstorage1Shutterflag1 = true;
  Serial.println("cop storage  high ");
  copstorage1highflag = false;
  copstorage1lowflag = true;
}
if( millis() - timer2CopStorage1 > copStorage1Offdelay*50 and copstorage1lowflag==true){
     digitalWrite(copStorage1,LOW);
     copstorage1Shutterflag1 = false;
     color1Cntflag = 0;
      flag2CopStorage1 = false;//false
      flag1CopStorage1 = false;//false
      copstorage1lowflag = false;
      //color1Cntflag = 0;
      //Serial.println("cop storage low ");
      //copStorage1Count++ ;
  }




if (copstorage1Shutterflag1 == true and copstorage1Shutterflag2 == false)
{
  copstorage1Shutterflag2 = true;
  copStorage1Count++ ;
  Serial.print("copStorage1Count=");
  Serial.println(copStorage1Count);
}



if (copstorage1Shutterflag1 == false)
{
  copstorage1Shutterflag2 = false;
}




if (copStorage1Count >= 4 and copstorage1Shutterflag1 == false)
{
  copStorage1Count = 0;
}

   /**************************************************crate error check********************************************/

if (digitalRead(crate1) == true and tray1check == false)
  {
    crate1alert = millis();
    tray1check = true;
    tray1checkflag = true;
    Serial.println("no tray");
  }

  if (digitalRead(crate1) == false )
  {
    tray1check = false;
    tray1checkflag = false;
  }
if (millis() - crate1alert > crate1alertondelay*50 and tray1checkflag == true)
{
  crate1alertcheck = true;
  Serial.println("tray alert");
}
if (digitalRead(crate1) == true and crate1alertcheck == true)
{
  crate1error = true;
  //error = true;
  digitalWrite(binLock1,HIGH);
  Serial.println("binlock high");
}

if (crate1error == true and digitalRead(crate1) == false) 
{
  digitalWrite(binUp1,HIGH);
  binup1inerrortime = millis();
  binup1inerrorok1 = true;
  crate1alertcheck = false;
  crate1error = false;
//  error = false;
  Serial.println("binup high");
  
 
}

if (binup1inerrorok1 == true and millis() - binup1inerrortime > 4000)
  {
    digitalWrite(binUp1,LOW);
    binup1inerrorok1 = false;
    binup1inerrortime2 = millis();
    binup1inerrorok2 = true;
    //Serial.println("binup low");
  }
    

  if (binup1inerrorok2 == true and millis() - binup1inerrortime2 > 500)//was400
  {
    digitalWrite(binLock1,LOW);
      
    binup1inerrorok2 = false;
    //Serial.println("binlock low"); 
    if 
      (digitalRead(crate1) == false)
      {
       tray1check = false;
       tray1checkflag = false;
    }
  }





/*************************************************CrateEjection********************************************/

if( copStorage1Count >= 4 and crate1error == false and digitalRead(crate1) == false)
{
  copStorage1Countflag = copStorage1Count;
}
{
  if( copStorage1Countflag == numCopStorage1 and flag1CrateEjection1 ==false){
  timer1CrateEjection1 = millis();
  flag1CrateEjection1 = true;
  flag2CrateEjection1 = true;
  
  }

  if  (millis() - timer1CrateEjection1 > 2000 and flag2CrateEjection1 == true)
  {
  digitalWrite(crateEjector1,HIGH);
  Serial.println("crate eject high");
  timer2CrateEjection1 = millis();
  flag3CrateEjection1 = true;
  flag2CrateEjection1 = false;
 }

if( millis() - timer2CrateEjection1 > (crate1RemoveOndelay * 50) and flag3CrateEjection1 == true){
      digitalWrite(crateEjector1,LOW);
      copStorage1Countflag =0;
      flag1CrateEjection1 = false;
      flag3CrateEjection1 = false;
     // Serial.println("crate eject low");
      crate1Complete = true;
  }

/*****************************************************CrateReplacement************************************************************/
if( flag3CrateEjection1 == true and millis() - timer2CrateEjection1 > 1700 and crate1ProcessFlag == false){
  if( digitalRead(crate1) == false){
  //  error = true;
   // errorCode="crateBlock";
    crate1ProcessFlag =true;
  }
}

if( crate1Complete == true  and crate1ProcessFlag == false and flag1binUp1 ==false){
  digitalWrite(binUp1,HIGH);
  timer1binUp1 = millis();
  flag1binUp1 = true;
}

if(millis() - timer1binUp1 > 1000  and flag1binUp1 == true){
  digitalWrite(binLock1,HIGH);
 if( millis() - timer1binUp1 > 1000 and millis() - timer1binUp1 < 2000 and digitalRead(crate1) == false){
//    error = true;
//    errorCode="BinUpError";
  }
}

if(millis() - timer1binUp1 > 2400 and flag1binUp1 == true){
  digitalWrite(binLock1,LOW);
 
}
if(millis() - timer1binUp1 > 2000 and flag1binUp1 == true){
  digitalWrite(binUp1,LOW);
  
}
if(millis() - timer1binUp1 > 4000 and flag1binUp1 == true){
 if( digitalRead(crate1) == true){
 //   errorCode="NoCrate";
  }

  crate1Complete =false;
  crate1ProcessFlag =false;
  flag1binUp1 = false;
  

}
}

/*shutter2 end*/
/*shutter3 start*/

/*s3 error*/

if (errors3 == true and errorflag == false)
  {
    errors3occured = true;
    errorflag = true;
    //digitalWrite(alarmLightred,HIGH);
  }
 
  
if (errors3 == true and errors3occured == true)
  {
    errors3ontmr = millis();
    errors3occured = false;
    errors3occured1 = true;
    
  }

if (millis() - errors3ontmr > 1000 and errors3occured1 == true)
  {
    digitalWrite(alarmLightyellow,HIGH);
    errors3offtmr = millis();
    errors3occuredoff = true;
    errors3occured1 = false;
  }
if (millis() - errors3offtmr > 200 and errors3occuredoff == true)
  {
    digitalWrite(alarmLightyellow,LOW);
    errors3occuredoff = false;
  errorflag = false;

  }

//Object2 Sensor Error
if (digitalRead(obj2) == true and objerrflag2 == false)
  {
    objerrtimer2 = millis();
    objerrflag12 = true;
    objerrflag2 = true;  
  }

if (digitalRead(obj2) == false)
  {
    objerrflag2 = false;
    objerrflag12 = false;
  }
   
  
if ((millis() - objerrtimer2 > (objerrtimerondelay2 * 50) and objerrflag12 == true) )
  { 
    lcd.setCursor(0,2);
    lcd.print("OBJ2 ERROR");
    Serial.println("Object2 error occured");
    errors3 = true;
    
   // errorCode = "Object2 Sensor error";
  }




  //Color2 Sensor Error
  if (digitalRead(color2) == false and colorerrflag2 == false)
    {
      colorerrtimer2 = millis();
      colorerrflag12 = true;
      colorerrflag2 = true;
      //objerrtimer1 = millis();
//    Serial.println("Color2 error test error occured"); 
    }

  if (digitalRead(color2) == true )
    {
      colorerrflag2 = false;
      colorerrflag12 = false;
    }
   
  
  if ((millis() - colorerrtimer2 > (colorerrtimerondelay2 * 50) and colorerrflag12 == true) )
    { 
      lcd.setCursor(0,2);
      lcd.print("CLR2 ERROR");
      Serial.println("Color 2 error occured");
      errors3 = true;
     
     // errorCode = "Color 2 Sensor error";
    }
/*s3error ends*/


// ****************************Yarn  2 identification********************************//
    if( yarn2currentOutput >= 1 and yarn2flag == false and digitalRead(obj2) == true )
    {
    yarn2flag = true;
    Serial.println("yarn2 identified");
    }


// ****************************Color 2 identification********************************//

 if(digitalRead(color2) == false and  color2flag == false) //clr1false
 {
  color2flag = true;
  color2identified = true;
  Serial.println("colour2 identified");
  s3on = true;
}


// ****************************Object 2 identification********************************//

  if(digitalRead(obj2) == true  and Object2flag1 == false)
  {
    Object2Detected = true;//only used inside loop
    Object2flag1 = true;
    Shutter3OnReady = false;
    Serial.println("Object2 identified");
    yarn2flag = false;
    color2flag = false;
    color2identified = false;
  }
//TO TRIGGER SHUTTER3 for color2COP
  if (Object2flag1 == true and digitalRead(obj2) == false and color2identified == true and yarn2flag == false)
      {
      Shutter3Onflag1 = true;
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color2 identified");
      }
  
  //Passing Shutter3 - cop must be 3rd color    
     if (Object2flag1 == true and digitalRead(obj2) == false and color2identified == false and yarn2flag == false) 

     {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color3u empty Check");
    }
 //To find Yarn at Object2 sensor and use the information for shutter4     
        if   (Object2flag1 == true and digitalRead(obj2) == false and color2identified == false and yarn2flag == true)
    {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
      Serial.println("Color3 yarn Check");
    }
//Color 2 COP with Yarn (color 2 kirchi)
    if   (Object2flag1 == true and digitalRead(obj2) == false and color2identified == true and yarn2flag == true)
    {
      Object2DetectTmr = millis();
      yarn2flag = false;
      yarn2detected = false;
      color2flag = false;
      color2identified = false;
      s3on = false;
      Object2Detected = false;
      Object2flag1 = false;
           krichi2 =  true;
      Serial.println("Color2 yarn Rested");
if (krichi2==true)
{
krichi2Cnt++;
Serial.print("krichi 2=");
Serial.println(krichi2Cnt);
krichiCntFlag=true;
//lcd.setCursor(10,1);
//lcd.print(krichi2Cnt);
krichi2==false;
}
      
}   
//Shutter3 ON flag
    if (Shutter3Onflag1 == true and Shutter3Onflag2 == false)
        {
          Shutter3OnReady = true;
          Shutter3Onflag2 = true;
          Shutter3Onflag1 = false;   
        }

     if (C2CountFlag1 == false)
     {
      Shutter3Onflag2 = false;
      //Serial.println("Shutter2 off ");
     }







// ****************************Color 2 Ejection********************************//
if( (millis()-Object2DetectTmr) > (s3OnDelay*1) and (millis()-Object2DetectTmr) < (s3OnDelay+s3OffDelay)*50 and Shutter3OnReady == true)//was*50
  
  {
  digitalWrite(s3,HIGH);
  //Serial.println("Shutter 2 On");
  C2CountFlag1 = true;
  }
else{
  digitalWrite(s3,LOW);
  C2CountFlag1 = false;
}



if (C2CountFlag1 == true and C2CountFlag2 == false)
{
  C2CountFlag2 = true;
  color2Cnt++;
  color2Cntflag++;
  Serial.print("color 2 count=");
  Serial.println(color2Cnt);
  lcd.setCursor(10,0);
  lcd.print(color2Cnt);
  
}

if (C2CountFlag1 == false)
{
  C2CountFlag2 = false;
}

//if (color2Cnt >= 4 and C2CountFlag1 == false)//40 //color 1 count resetted
//{
//  color2Cnt = 0;
//}

if (color2Cntflag >=c1Cntreset and C2CountFlag1 == false)//40 //color 1 count resetted
{
  color2Cntflag = 0;
}




/************************************CopStorage 2*******************************/

//if (color2Cnt >= 4  and flag1CopStorage2 == false)//40
//{
//  color2Cntflag = color2Cnt;
//  Serial.print("color2Cntflag=");
//  Serial.println(color2Cntflag);
//  flag1CopStorage2 = true;
//}

if(color2Cntflag >= c1Cntreset and flag1CopStorage2 == false /*flag2CopStorage2 == false*/)//40
{
  timer1CopStorage2 = millis(); 
//  flag2CopStorage2 = true;
  flag1CopStorage2 = true;
  copstorage2highflag = true;
  Serial.println("cop storage 2 ready to high ");
  
}

if (millis() - timer1CopStorage2 > copStorage2Ondelay*50  and copstorage2highflag == true)
{
  digitalWrite(copStorage2,HIGH); 
  
  timer2CopStorage2 = millis();
  copstorage2Shutterflag1 = true;
  Serial.println("cop storage  high ");
  copstorage2highflag = false;
  copstorage2lowflag = true;
}
if( millis() - timer2CopStorage2 > copStorage2Offdelay*50 and copstorage2lowflag==true){
     digitalWrite(copStorage2,LOW);
     copstorage2Shutterflag1 = false;
     color2Cntflag = 0;
      flag2CopStorage2 = false;//false
      flag1CopStorage2 = false;//false
      copstorage2lowflag = false;
      //color2Cntflag = 0;
      //Serial.println("cop storage2 low ");
      //copStorage2Count++ ;
  }




if (copstorage2Shutterflag1 == true and copstorage2Shutterflag2 == false)
{
  copstorage2Shutterflag2 = true;
  copStorage2Count++ ;
  Serial.print("copStorage2Count=");
  Serial.println(copStorage2Count);
}



if (copstorage2Shutterflag1 == false)
{
  copstorage2Shutterflag2 = false;
}




if (copStorage2Count >= 4 and copstorage2Shutterflag1 == false)
{
  copStorage2Count = 0;
}

   /**************************************************crate 2 error check********************************************/

if (digitalRead(crate2) == true and tray2check == false)
  {
    crate2alert = millis();
    tray2check = true;
    tray2checkflag = true;
    Serial.println("no tray2");
  }

  if (digitalRead(crate2) == false )
  {
    tray2check = false;
    tray2checkflag = false;
  }
if (millis() - crate2alert > crate2alertondelay*50 and tray2checkflag == true)
{
  crate2alertcheck = true;
  Serial.println("tray2 alert");
}
if (digitalRead(crate2) == true and crate2alertcheck == true)
{
  crate2error = true;
  //errors3 = true;
  digitalWrite(binLock2,HIGH);
  Serial.println("binlock 2 high");
}

if (crate2error == true and digitalRead(crate2) == false) 
{
  digitalWrite(binUp2,HIGH);
  binup2inerrortime = millis();
  binup2inerrorok1 = true;
  crate2alertcheck = false;
  crate2error = false;
//  errors3 = false;
  Serial.println("binup2 high");
  
 
}

if (binup2inerrorok1 == true and millis() - binup2inerrortime > 4000)
  {
    digitalWrite(binUp2,LOW);
    binup2inerrorok1 = false;
    binup2inerrortime2 = millis();
    binup2inerrorok2 = true;
    //Serial.println("binup2 low");
  }
    

  if (binup2inerrorok2 == true and millis() - binup2inerrortime2 > 500)//was400
  {
    digitalWrite(binLock2,LOW);
      
    binup2inerrorok2 = false;
    //Serial.println("binlock 2low"); 
    if 
      (digitalRead(crate2) == false)
      {
       tray2check = false;
       tray2checkflag = false;
    }
  }




/*************************************************Crate2Ejection********************************************/

if( copStorage2Count >= 4 and crate2error == false and digitalRead(crate2) == false)
{
  copStorage2Countflag = copStorage2Count;
}
{
  if( copStorage2Countflag == numCopStorage2 and flag1CrateEjection2 ==false){
  timer1CrateEjection2 = millis();
  flag1CrateEjection2 = true;
  flag2CrateEjection2 = true;
  
  }

  if  (millis() - timer1CrateEjection2 > 2000 and flag2CrateEjection2 == true)
  {
  digitalWrite(crateEjector2,HIGH);
  Serial.println("crate eject 2 high");
  timer2CrateEjection2 = millis();
  flag3CrateEjection2 = true;
  flag2CrateEjection2 = false;
 }

if( millis() - timer2CrateEjection2 > (crate2RemoveOndelay * 50) and flag3CrateEjection2 == true){
      digitalWrite(crateEjector2,LOW);
      copStorage2Countflag =0;
      flag1CrateEjection2 = false;
      flag3CrateEjection2 = false;
     // Serial.println("crate eject2 low");
      crate2Complete = true;
  }


/*****************************************************Crate2Replacement************************************************************/
if( flag3CrateEjection2 == true and millis() - timer2CrateEjection2 > 1700 and crate2ProcessFlag == false){
  if( digitalRead(crate2) == false){
  //  errors3 = true;
   // errorCode="crateBlock";
    crate2ProcessFlag =true;
  }
}

if( crate2Complete == true  and crate2ProcessFlag == false and flag1binUp2 ==false){
  digitalWrite(binUp2,HIGH);
  timer1binUp2 = millis();
  flag1binUp2 = true;
}

if(millis() - timer1binUp2 > 1000  and flag1binUp2 == true){
  digitalWrite(binLock2,HIGH);
 if( millis() - timer1binUp2 > 1000 and millis() - timer1binUp2 < 2000 and digitalRead(crate2) == false){
//    errors3 = true;
//    errorCode="BinUpError";
  }
}

if(millis() - timer1binUp2 > 2400 and flag1binUp2 == true){
  digitalWrite(binLock2,LOW);
 
}
if(millis() - timer1binUp2 > 2000 and flag1binUp2 == true){
  digitalWrite(binUp2,LOW);
  
}
if(millis() - timer1binUp2 > 4000 and flag1binUp2 == true){
 if( digitalRead(crate2) == true){
 //   errorCode="NoCrate";
  }

  crate2Complete =false;
  crate2ProcessFlag =false;
  flag1binUp2 = false;
  

}
}

/*shutter2 end*/
/*error reset*/

//reset object error
if (digitalRead(pas) == false and ((error == true)or(errors3 == true)))
  { 
    error = false;
    errors3 = false;
    errorB = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    objerrflag = false;
    objerrflag1 = false;
    objerrflag2 = false;
    objerrflag12 = false;
    lcd.setCursor(0,2);
    lcd.print("          ");
    Serial.println("Object error reseted");    
  }

 // color error resetting
if (digitalRead(pas) == false and ((error == true)or(errors3 == true) or (errorFC == true)))
  { 
    error = false;
    errors3 = false;
    errorB = false;
    errorFC = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    colorerrflag1 = false;
    colorerrflag = false;
    colorerrflag12 = false;
    colorerrflag2 = false;
    lcd.setCursor(0,2);
    lcd.print("          ");
    Serial.println("Color error reseted");
    }
//yarn error reset
 if (digitalRead(pas) == false and ((error == true)or(errors3 == true) or (errorFC == true)))
  { 
    error = false;
    errors3 = false;
    errorFC = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    yarnerrflag1 = false;
    yarnerrflag = false;
    lcd.setCursor(0,2);
    lcd.print("          ");
    Serial.println("Yarn error reseted");
  }


// VC error resetting
if (digitalRead(pas) == false and ((error == true) or (errorB == true) or (errors3 = true) or (errorFC == true)))
  { 
    error = false;
    errors3 = false;
    errorB = false;
    errorFC = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    objerrflag = false;
    objerrflag1 = false;
    objerrflag2 = false;
    objerrflag12 = false;
    lcd.setCursor(0,2);
    lcd.print("          ");
    Serial.println("belt error reseted");    
  }
  

/************reset fb error reset*/
if (digitalRead(pas) == false and ((error == true) or (errorB == true) or (errors3 = true) or (errorFC == true)))
  { 
    error = false;
    errors3 = false;
    errorB = false;
    errorFC = false;
    digitalWrite(alarmLightyellow,LOW);
    errorflag =false;
    errors3flag =false;
    objerrflag = false;
    objerrflag1 = false;
    objerrflag2 = false;
    objerrflag12 = false;
    lcd.setCursor(0,2);
    lcd.print("          ");
    Serial.println("FC error reseted");    
  }
  




/*error reset ends*/

/*error reset ends*/
if(krichiCntFlag  ==  true)
{
a=  krichi1Cnt + krichi2Cnt;
lcd.setCursor(3,1);
lcd.print(a);
krichiCntFlag=false;
}




}
