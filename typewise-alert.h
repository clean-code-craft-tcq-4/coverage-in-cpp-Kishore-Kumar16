#ifndef TYPEWISE_ALERT_H_
#define TYPEWISE_ALERT_H_

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <unordered_map>

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

#ifdef ENABLE_BREACH_STATUS
const char* breachStatus[] = {"normal", "too low", "too high"};
#endif

BreachType inferBreach(double value, double lowerLimit, double upperLimit);
void classifyTemperatureBreach(std::unordered_map <int, std::pair<int, int>> &tempBreach);

void checkAndAlert(
  AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

void sendBatteryTempStatus(AlertTarget alertTarget, BreachType breachType, std::ostream& os = std::cout, const char* recepientId = "a.b@c.com");

/*
void sendToController(BreachType breachType);
void sendToEmail(BreachType breachType);
*/

#endif //TYPEWISE_ALERT_H_