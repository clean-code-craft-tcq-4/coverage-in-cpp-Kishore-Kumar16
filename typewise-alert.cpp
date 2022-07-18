#define ENABLE_BREACH_STATUS

#include "typewise-alert.h"

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {

  BreachType retBreachType = NORMAL;
  if(value < lowerLimit) {
    retBreachType = TOO_LOW;
  }
  if(value > upperLimit) {
    retBreachType = TOO_HIGH;
  }
  return retBreachType;
}

void classifyTemperatureBreach(std::unordered_map <int, std::pair<int, int>> &tempBreach) {

      //std::unordered_map <std::string, std::pair<int , int> > tempBreach;
      tempBreach.insert({PASSIVE_COOLING, std::make_pair<int, int>(0, 35)});
      tempBreach.insert({HI_ACTIVE_COOLING, std::make_pair<int, int>(0, 45)});
      tempBreach.insert({MED_ACTIVE_COOLING, std::make_pair<int, int>(0, 40)});
      
/*
  int lowerLimit = 0;
  int upperLimit = 0;
  switch(coolingType) {
    case PASSIVE_COOLING:
      lowerLimit = 0;
      upperLimit = 35;
      break;
    case HI_ACTIVE_COOLING:
      lowerLimit = 0;
      upperLimit = 45;
      break;
    case MED_ACTIVE_COOLING:
      lowerLimit = 0;
      upperLimit = 40;
      break;
  }

  return inferBreach(temperatureInC, lowerLimit, upperLimit);
  */
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

    std::unordered_map <int, std::pair<int, int>> temp_Breach;
  classifyTemperatureBreach(temp_Breach);

  int lowerLimit, upperLimit;
  auto itr = temp_Breach.find(batteryChar.coolingType);
  lowerLimit = itr->second.first;
  upperLimit = itr->second.second;
  BreachType breachType = inferBreach(temperatureInC, lowerLimit, upperLimit);
  sendBatteryTempStatus(alertTarget, breachType);
/*
  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
  */
}

void sendBatteryTempStatus(AlertTarget alertTarget, BreachType breachType, std::ostream& os, const char* recepientId) {

  if (alertTarget == TO_EMAIL) {
    os << "To: " << recepientId << std::endl;
    os << "Hi, the temperature is " << breachStatus[breachType] << std::endl;
    /*
    printf("To: %s\n", recepientId);
    printf("Hi, the temperature is %s", breachStatus[breachType]);
    */
  }
  else {
    const unsigned short header = 0xfeed;
    os << header << " : " << breachType << std::endl;
    
    //printf("%x : %x\n", header, breachType);
  }
}

/*
void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  switch(breachType) {
    case TOO_LOW:
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too low\n");
      break;
    case TOO_HIGH:
      printf("To: %s\n", recepient);
      printf("Hi, the temperature is too high\n");
      break;
    case NORMAL:
      break;
  }
}
*/