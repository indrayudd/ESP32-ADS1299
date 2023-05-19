


/*Developed by Joel Murphy and Conor Russomanno (Summer 2013)
  This example uses the ADS1299 Arduino Library, a software bridge between the ADS1299 TI chip and 
  Arduino. See http://www.ti.com/product/ads1299 for more information about the device and the README
  folder in the ADS1299 directory for more information about the library.
  
  This program does the following
  It reads in all registers in verbose mode, then alters the CONFIG3 register,
  then asks for a prompt to take 500 samples at the default rate of 250sps.
  At the prompt, the START command is sent, and a mS timestamp is saved.
  A function called ADStest() proceeds to poll the DRDY pin, and call
  the updateChanelData() member funcion until all samples are counted.
  Then another timestamp is taken, duration of event is calculated and 
  printed to terminal. Then the prompt re-appears.
  The library outputs verbose feedback when verbose is true.
  
  Pi Pico - Pin Assignments
  SCK = 18
  MISO [DOUT] = 19
  MOSI [DIN] = 23
  CS = 5; 
  RESET = 16;
  DRDY = 17;
  
*/

#include "ESPADS1299.h"
#include "BluetoothSerial.h"
#include <filters.h>
//#include <FIRFilter.h> 
#include <WiFi.h>

const char* ssid = "SSID";
const char* password = "Password";
const int udpPort = 1234; // Change to desired UDP port

WiFiUDP udp;
float values[6]; // Array to store the 6 float values

//WiFiClient client;
bool isConnected = false;

ESPADS1299 ADS;                           // create an instance of ADS1299
//BluetoothSerial SerialBT;
//Notch Filter
//EMGFilters myFilter3;
//EMGFilters myFilter4;
//EMGFilters myFilter5;
//EMGFilters myFilter6;
//EMGFilters myFilter7;
//EMGFilters myFilter8;


unsigned long thisTime;                
unsigned long thatTime;
unsigned long elapsedTime;
int resetPin = 16;                      // pin 9 used to start conversion in Read Data Continuous mode
int sampleCounter = 0;                 // used to time the tesing loop
boolean testing = true;               // this flag is set in serialEvent on reciept of prompt

const float cutoff_freq   = 150.0;   //Cutoff frequency in Hz was 150
const float sampling_time = 0.004; //Sampling time in seconds.
Filter fhp(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp2(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp3(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp4(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp5(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);
Filter fhp6(cutoff_freq, sampling_time, IIR::ORDER::OD2, IIR::TYPE::HIGHPASS);


void setup() {
//  delay(5000);
  // don't put anything before the initialization routine for recommended POR  
//  myFilter3.init(sampleRate, humFreq, true, true, true);
//  myFilter4.init(sampleRate, humFreq, true, true, true);
//  myFilter5.init(sampleRate, humFreq, true, true, true);
//  myFilter6.init(sampleRate, humFreq, true, true, true);
//  myFilter7.init(sampleRate, humFreq, true, true, true);
//  myFilter8.init(sampleRate, humFreq, true, true, true);
  ADS.initialize(2,0,1, false); // (DRDY pin, RST pin, SCK frequency in MHz, isDaisy);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

//  isConnected = client.connect("192.168.69.51", 80);
//  SerialBT.begin("ESP32 Bluetooth");
  //Serial.println("ADS1299-Arduino UNO Example 2"); 
  delay(2000);             

  ADS.verbose = false;      // when verbose is true, there will be Serial feedback 
  ADS.RESET();             // send RESET command to default all registers
  ADS.SDATAC();            // exit Read Data Continuous mode to communicate with ADS
  //ADS.WREG(ID,0b00000001);
  //ADS.RREG(ID);
  ADS.WREG(CONFIG1,0b11010110);
  ADS.WREG(CONFIG2,0b11010101);
  ADS.WREG(CONFIG3,0b11001010);
  ADS.WREG(CH1SET,0b10100000);
  ADS.WREG(CH2SET,0b00100000);
  ADS.WREG(CH3SET,0b00100000);
  ADS.WREG(CH4SET,0b00100000);
  ADS.WREG(CH5SET,0b00100000);
  ADS.WREG(CH6SET,0b00100000);
  ADS.WREG(CH7SET,0b00100000);
  ADS.WREG(CH8SET,0b00100000);
  ADS.WREG(BIAS_SENSP,0b11111111);
  ADS.WREG(BIAS_SENSN,0b11111111);
  ADS.WREG(LOFF_SENSP,0x00);
  ADS.WREG(LOFF_SENSN,0x00);
  //ADS.WREG(MISC1,0b00100000);
  ADS.WREG(CONFIG4,0x02);
//  
  ADS.RREGS(0x00,0x17);     // read all registers starting at ID and ending at CONFIG4
////  ADS.WREG(CONFIG3,0xE0);  // enable internal reference buffer, for fun
////  ADS.RREG(CONFIG3);       // verify write
  ADS.RDATAC();            // enter Read Data Continuous mode
  ADS.START();                    // start sampling at the default rate
//  
//  //Serial.println("Press 'x' to begin test");    // ask for prompt
} // end of setup
unsigned long old_time;
unsigned long new_time;
void loop(){
//
////  delay(26);
//   new_time = millis();
//  
  if (testing){
    //Serial.println("entering test loop");
    while(digitalRead(2)){            // watch the DRDY pin
//      Serial.println("Checking DRDY");
      }
//    Serial.println("Data Ready!");
    ADS.updateChannelData();          // update the channelData array
//    int ads_value_1 =("Channel 1",ADS.channelData[0]);
    int ads_value_2 =("Channel 2",ADS.channelData[1]);
    int ads_value_3 =("Channel 3",ADS.channelData[2]);
    int ads_value_4 =("Channel 4",ADS.channelData[3]);
    int ads_value_5 =("Channel 5",ADS.channelData[4]);
    int ads_value_6 =("Channel 6",ADS.channelData[5]);
    int ads_value_7 =("Channel 7",ADS.channelData[6]);
    int ads_value_8 =("Channel 8",ADS.channelData[7]);
    //Serial.print("Ch1, "); 
//    Serial.print("Ch6, "); 
//    Serial.print("Ch7, "); Serial.println("Ch8");
//    float ads_value_1f = fhp.filterIn(ads_value_1)/1000;
//    float ads_value_2f = fhp6.filterIn(ads_value_2/1000);
    
    
//    float ads_value_3f = fhp2.filterIn(ads_value_3)/1000;
//    float ads_value_4f = fhp.filterIn(ads_value_4)/1000;
//    float ads_value_5f = fhp5.filterIn(ads_value_5)/1000;
//    float ads_value_6f = fhp3.filterIn(ads_value_6)/1000;
//    float ads_value_7f = fhp4.filterIn(ads_value_7)/1000;
//    float ads_value_8f = fhp6.filterIn(ads_value_8)/1000;


    values[0] = fhp2.filterIn(ads_value_3)/1000;
    values[1] = fhp.filterIn(ads_value_4)/1000;
    values[2] = fhp5.filterIn(ads_value_5)/1000;
    values[3] = fhp3.filterIn(ads_value_6)/1000;
    values[4] = fhp4.filterIn(ads_value_7)/1000;
    values[5] = fhp6.filterIn(ads_value_8)/1000;


    // Create a byte array to hold the values
  byte payload[sizeof(float) * 6];
  memcpy(payload, values, sizeof(float) * 6);

  // Send the payload to the receiver
  udp.beginPacket(IPAddress(192, 168, 69, 51), udpPort); // Change IP address to the receiver's IP
  udp.write(payload, sizeof(float) * 6);
  udp.endPacket();

//      Serial.print(-12500/1000);
//      Serial.print("\t");
      
      
//      Serial.print(ads_value_3f);
//      Serial.print("\t");
//      Serial.print(ads_value_4f);
//      Serial.print("\t");
//      Serial.print(ads_value_5f);
//      Serial.print("\t");
//      Serial.print(ads_value_6f);
//      Serial.print("\t");
//      Serial.print(ads_value_7f);
//      Serial.print("\t");
//      Serial.print(ads_value_8f);
//      Serial.print("\t");

//      Serial.print(ads_value_3f+8);
//      Serial.print("\t");
//      Serial.print(ads_value_4f+6);
//      Serial.print("\t");
//      Serial.print(ads_value_5f+4);
//      Serial.print("\t");
//      Serial.print(ads_value_6f-4);
//      Serial.print("\t");
//      Serial.print(ads_value_7f-6);
//      Serial.print("\t");
//      Serial.print(ads_value_8f-8);
//      Serial.print("\t");
//      Serial.println(12500/1000);


//      SerialBT.print(ads_value_3f+8);
//      SerialBT.print("\t");
//      SerialBT.print(ads_value_4f+6);
//      SerialBT.print("\t");
//      SerialBT.print(ads_value_5f+4);
//      SerialBT.print("\t");
//      SerialBT.print(ads_value_6f-4);
//      SerialBT.print("\t");
//      SerialBT.print(ads_value_7f-6);
//      SerialBT.print("\t");
//      SerialBT.println(ads_value_8f-8);

//if (isConnected) {
//    // Send data to the receiver ESP32.
//    String data = "12.5\t"+String(ads_value_3f+8)+"\t"+String(ads_value_4f+6)+"\t"+String(ads_value_5f+4)+"\t"+String(ads_value_6f-4)+"\t"+String(ads_value_7f-6)+"\t"+String(ads_value_8f-8)+"\t12.5"+"\n";
//    client.print(data);
//    client.flush();
//  }
//
//  if (!client.connected()) {
//    // Reconnect to the receiver if the connection is lost.
//    isConnected = client.connect("192.168.69.51", 80);  // Replace with the IP address or hostname of the receiver ESP32
//  }
////      old_time=new_time;
//
//
//
////
////      Serial.print(notched3f);
////      Serial.print("\t");
////      Serial.print(notched4f);
////      Serial.print("\t");
////      Serial.print(notched5f);
////      Serial.print("\t");
////      Serial.print(notched6f);
////      Serial.print("\t");
////      Serial.print(notched7f);
////      Serial.print("\t");
////      Serial.print(notched8f);
////      Serial.print("\t");
////      Serial.println(12500/1000);
////    }
////    Serial.print("\t");
////    Serial.println(ads_value_3);
//    //Serial.println(ads_value_3);
//    //Serial.println(ads_value_1);
////    Serial.print(",");
//////    Serial.print(ads_value_2);
////////    /**Serial.print(",");
////////    Serial.print(ads_value_3);
////////    Serial.print(",");
////////    Serial.print(ads_value_4);**/
//////    Serial.print(",");
//////    Serial.print(ads_value_5);
////    Serial.print(",");
////    Serial.print(ads_value_6);
////    Serial.print(",");
////    Serial.print(ads_value_7);
////    Serial.print(",");
////    Serial.println(ads_value_8);
//
////    ADS.updateChannelData();  
//    
//     
//    //sampleCounter++;                  // increment sample counter for next time
//    
//    //elapsedTime = thisTime - thatTime;
//    //Serial.print("Elapsed Time ");Serial.println(elapsedTime);  // benchmark
//      //Serial.print("Samples ");Serial.println(sampleCounter);   // 
//    //testing = true;                // reset testing flag
//    //sampleCounter = 0;              // reset counter
//    //Serial.println("Press 'x' to begin test");  // ask for prompt
  }// end of testing
//
////  ADS.STOP();                     // stop the sampling
//  ADS.RREGS(0x00, 0x17);
//  Serial.println();
//  delay(2000);
//  
} // end of loop


void serialEvent(){            // send an 'x' on the serial line to trigger ADStest()
  while(Serial.available()){      
    char inChar = (char)Serial.read();
    if (inChar  == 'x'){
      testing = false;
    }
    if(inChar == 'v'){
      testing = true;
    }
  }
}
/**
void ADStest(){
  while(testing){  // take only as many samples as you need
    while(digitalRead(8)){            // watch the DRDY pin
      }
    ADS.updateChannelData();          // update the channelData array
    int ads_value_1 =("Channel 1",ADS.channelData[0]);
    int ads_value_2 =("Channel 2",ADS.channelData[1]);
    int ads_value_3 =("Channel 3",ADS.channelData[2]);
    int ads_value_4 =("Channel 4",ADS.channelData[3]);
    int ads_value_5 =("Channel 5",ADS.channelData[4]);
    int ads_value_6 =("Channel 6",ADS.channelData[5]);
    int ads_value_7 =("Channel 7",ADS.channelData[6]);
    int ads_value_8 =("Channel 8",ADS.channelData[7]);
    Serial.print("Ch1, "); Serial.println("Ch5");
    Serial.print(ads_value_1);
    /**Serial.print(",");
    Serial.print(ads_value_2);
    Serial.print(",");
    Serial.print(ads_value_3);
    Serial.print(",");
    Serial.print(ads_value_4);**/
    //Serial.print(",");
    //Serial.println(ads_value_5);
    /**
    Serial.print(",");
    Serial.print(ads_value_6);
    Serial.print(",");
    Serial.print(ads_value_7);
    Serial.print(",");
    Serial.println(ads_value_8);**/
    
    /** 
    sampleCounter++;                  // increment sample counter for next time
  }
    return;
}**/
