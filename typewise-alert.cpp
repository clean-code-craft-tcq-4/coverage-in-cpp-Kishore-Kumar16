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

      tempBreach.insert({PASSIVE_COOLING, std::make_pair<int, int>(0, 35)});
      tempBreach.insert({HI_ACTIVE_COOLING, std::make_pair<int, int>(0, 45)});
      tempBreach.insert({MED_ACTIVE_COOLING, std::make_pair<int, int>(0, 40)});
      
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

    std::unordered_map <int, std::pair<int, int>> temp_Breach;
  classifyTemperatureBreach(temp_Breach);

  int lowerLimit, upperLimit;

  // find cooling type and update lower&upper limit
  auto itr = temp_Breach.find(batteryChar.coolingType);
  lowerLimit = itr->second.first;
  upperLimit = itr->second.second;
  BreachType breachType = inferBreach(temperatureInC, lowerLimit, upperLimit);
  sendBatteryTempStatus(alertTarget, breachType);

}

void sendBatteryTempStatus(AlertTarget alertTarget, BreachType breachType, std::ostream& os, const char* recepientId) {

  if (alertTarget == TO_EMAIL) {
    os << "To: " << recepientId << std::endl;
    os << "Hi, the temperature is " << breachStatus[breachType] << std::endl;
  }
  else {
    const unsigned short header = 0xfeed;
    os << header << " : " << breachType << std::endl;
  }
}
