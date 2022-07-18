#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
}

TEST_CASE("Check temperature classification") {
  std::unordered_map <int ,std::pair<int, int>> testTempClassify;
  classifyTemperatureBreach(testTempClassify);
  
  auto itr = testTempClassify.find(PASSIVE_COOLING);
  REQUIRE((itr->second.first) == 0);
  REQUIRE((itr->second.second) == 35);

  itr = testTempClassify.find(HI_ACTIVE_COOLING);
  REQUIRE((itr->second.first) == 0);
  REQUIRE((itr->second.second) == 45);

  itr = testTempClassify.find(MED_ACTIVE_COOLING);
  REQUIRE((itr->second.first) == 0);
  REQUIRE((itr->second.second) == 40);
}

TEST_CASE("Send alert") {
  std::ostringstream lowToEmail;
  sendBatteryTempStatus(TO_EMAIL, TOO_LOW, lowToEmail, "krish@gmail.com");
  REQUIRE(lowToEmail.str() == "To: krish@gmail.com\nHi, the temperature is too low\n");

  std::ostringstream highToEmail;
  sendBatteryTempStatus(TO_EMAIL, TOO_HIGH, highToEmail, "krish@gmail.com");
  REQUIRE(highToEmail.str() == "To: krish@gmail.com\nHi, the temperature is too high\n");

  std::ostringstream lowToController;
  sendBatteryTempStatus(TO_CONTROLLER, TOO_LOW, lowToController);
  REQUIRE(lowToController.str() == "65261 : 1\n");

  std::ostringstream highToController;
  sendBatteryTempStatus(TO_CONTROLLER, TOO_HIGH, highToController);
  REQUIRE(highToController.str() == "65261 : 2\n");
}

TEST_CASE("Test checkAndAlert(to email)") {

  BatteryCharacter batterych = {
    .coolingType = HI_ACTIVE_COOLING
  };

  std::ostringstream toEmail;
  std::streambuf* streambuf_1 = std::cout.rdbuf();
  std::cout.rdbuf(toEmail.rdbuf());

  checkAndAlert(TO_EMAIL, batterych, 20);
  
  //restore out stream
  std::cout.rdbuf(streambuf_1);
  REQUIRE(toEmail.str() == "To: a.b@c.com\nHi, the temperature is normal\n");

}

TEST_CASE("Test checkAndAlert(to controller)") {

  BatteryCharacter batterych = {
    .coolingType = HI_ACTIVE_COOLING
  };

  std::ostringstream toController;
  std::streambuf* streambuf_1 = std::cout.rdbuf();
  std::cout.rdbuf(toController.rdbuf());

  checkAndAlert(TO_CONTROLLER, batterych, 50);
  
  //restore out stream
  std::cout.rdbuf(streambuf_1);
  REQUIRE(toController.str() == "65261 : 2\n");

}