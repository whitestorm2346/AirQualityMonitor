#include "MQ9.h"

/*library for measuring mq9 gas values in this function
 * 
 * this library includes  function:
 *   1)inits()----> for serial begin or anything you want to describe
 *              
 *   2)calibrate()------> for calibrating sensor RL resistor inside the MQ-9 should be on for 24-48Hours
 *              
 *   3)getValue(bool printing,char gasName, double *output) ------> get values of sensor and measure gas in ppm this function have 3 input 
 *                                                                  1)bool printing: for printing values
 *
 *                                                                  2)char gasname : the gas name value which want get values 
 *                                                                      it includes: 
 *                                                                          'L': for LPG
 *                                                                          'C': for CO
 *                                                                          'H': for CH4
 *                                                                          default: for CO, LPG and CH4
 *
 *                                                                  3)double output : values return inside it should be in size of 3
 *   4)thrValue(char gasName,int threshold)  ------> this function is defined for thresholding define threshold for specific gas name and it returns true or false ****gas name are same with number 2-2
 *              
 *   5)printData(float data)  ------> this function is used for printing data it $%$ if your compiler is not arduino change as u want
 *              
 *   6)readADC(int pin)   ------> this function is used for reading adc values $%$ if your compiler is not arduino change as u want
 * 
 * 
 * 
 * the chart inside MQ-x sensors are used 
 * this chart is log chart and values are captured from them 
 * 
 * *****************Please visit below website for more information**************************
 * 
 *              https://thestempedia.com/tutorials/interfacing-mq-2-gas-sensor-with-evive/
 * ******************************************************************************************
 */

void * MQ9::inits() {

Serial.begin(9600);
adcBits = 1024;//use for defining number of bits to calculate values of MQ sensor
adcVoltage =3.3;//adc reference voltage
    


}
void * MQ9::calibrate() {
  float sensor_volt;  
  float RS_air; //  Rs in clean air 
  
  float sensorValue;
  for(int x = 0 ; x < 1000 ; x++) 
  { 
    sensorValue = sensorValue + readADC(A0); 
  }   
  sensorValue = sensorValue/1000.0;
  Serial.println(sensorValue);
  sensor_volt = (sensorValue/adcBits)*adcVoltage; 
  RS_air = (adcVoltage-sensor_volt)/sensor_volt; // Depend on RL on yor module 
  _R0 = RS_air/9.9; // According to MQ9 datasheet table 
   
 
  printData(_R0); 
  delay(1000);   
}

double * MQ9::getValue(bool printing,char gasName, double *output) {

  float sensor_volt; 
  float RS_gas; 
  float ratio; 
  float ppm;
 
  int sensorValue = readADC(A0); 
  sensor_volt = ((float)sensorValue / adcBits) * adcVoltage; 
  RS_gas = (adcVoltage - sensor_volt) / sensor_volt; // Depend on RL on yor module 
 
 
  ratio = RS_gas / _R0; // ratio = RS/R0 
  switch (gasName){
  case 'L'://lpg gas
  _M0 = -0.5157;
  _B0 = 1.56831;
  ppm = pow(10,(log(ratio)- _B0)/_M0);
  if(printing){printData(ppm);}
  output[0]=ppm;
  break;
  case 'C'://co gas
  _M0 = -0.50386;
  _B0 = 1.41558;
  ppm = pow(10,(log(ratio)- _B0)/_M0);
  if(printing){printData(ppm);}
  output[0]=ppm;
  break;
  case 'H'://CH4 gas
  _M0 = -0.372003;
  _B0 = 1.33309;
  ppm = pow(10,(log(ratio)- _B0)/_M0);
  //Serial.println(ppm);
  if(printing){printData(ppm);}
  output[0]=ppm;
  break;
  
  default://all values
  ppm = pow(10,(log(ratio)- (1.41558))/-0.50386);//CO
  output[0]=ppm;
  ppm = pow(10,(log(ratio)- (1.56831))/-0.5157);//LPG
  output[1]=ppm;
  ppm = pow(10,(log(ratio)- (1.33309))/-0.372003);//CH4
  output[2]=ppm;
 
  
  break;
}


    
}
bool  MQ9::thrValue(char gasName,float threshold){

float sensor_volt; 
  float RS_gas; 
  float ratio; 
  float ppm;
 
  int sensorValue = readADC(A0); 
  sensor_volt = ((float)sensorValue / adcBits) * adcVoltage; 
  RS_gas = (adcVoltage - sensor_volt) / sensor_volt; // Depend on RL on yor module 
 
 
  ratio = RS_gas / _R0; // ratio = RS/R0 
  switch (gasName){
  case 'C'://lpg gas
  _M0 = -0.50386;
  _B0 = 1.41558;
  ppm = pow(10,(log(ratio)- _B0)/_M0);
  
  break;
  case 'L'://lpg gas
  _M0 = -0.5157;
  _B0 = 1.56831;
  ppm = pow(10,(log(ratio)- _B0)/_M0);
  
  
  break;
  case 'H'://ch4
  _M0 = -0.372003;
  _B0 = 1.33309;
  ppm = pow(10,(log(ratio)- _B0)/_M0);
  

  break;
 
 
}


if(ppm>=threshold){
    return(true);}
  else{return(false);}


  
}
  
bool  MQ9::thrNvalue(float threshold){



if(readADC(0)>=threshold){
    return(true);}
  else{return(false);}


  
}
int MQ9::readADC(int pin){

return(analogRead(pin));


  
}
void MQ9::printData(float data){


 Serial.print("data is:");
 Serial.println(data,2);
}
