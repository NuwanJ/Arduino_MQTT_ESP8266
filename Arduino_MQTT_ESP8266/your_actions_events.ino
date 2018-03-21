void do_actions(const char* message) {
  //Create this function according to your actions. you will receive a message something like this

Serial.println(message);
   
StaticJsonBuffer<200> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(message);

String action = root["action"]; // "power"

JsonObject& param = root["param"]; 
String param_mac = param["mac"]; // "1902100379354041"

  if(param_mac.equals(DEVICE_SERIAL)){
    if(action == "sweep"){
        int pos = 0;    // variable to store the servo position
        Servo myservo;
        myservo.attach(4);

      for (pos = 0; pos <= 160; pos += 20) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(1000);                       // waits 15ms for the servo to reach the position
        }
      for (pos = 180; pos >= 0; pos -= 20) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(1000);                       // waits 1s for the servo to reach the position
        }
      myservo.write(0);              // tell servo to go to position in variable 'pos'

    } 
    if(action == "off"){
    analogWrite(4,0);

    } 
  }
}
//====================================================================================================================================================================
