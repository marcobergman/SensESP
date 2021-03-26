#include <Arduino.h>
#include "vedirect_helper.h"

#include "sensesp_app.h"
#include "signalk/signalk_output.h"

#include "sensesp.h"
#include "sensesp_app_builder.h"

VEDirectInput::VEDirectInput(Stream* rx_stream, uint8_t chargernumber)
    : Sensor() {
  rx_stream_ = rx_stream;

  vedirect_parser_.add_sentence_parser(new SentenceParserV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserVPV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserPPV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserI(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserIL(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH19(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH20(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH21(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH22(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH23(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserERR(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserCS(&vedirect_data_));

}

void VEDirectInput::enable() {
  // enable reading the serial port
  app.onAvailable(*rx_stream_, [this]() {
    while (rx_stream_->available()) {
      vedirect_parser_.handle(rx_stream_->read());
    }
  });
}

//SKMetadata* metadata = new SKMetadata("W", "Panel Power", "Panel Power", "Pnl Pow");

VEDirectInput* setup_vedirect(Stream* rx_stream, uint8_t chargernumber) {
  VEDirectInput* victronDevice = new VEDirectInput(rx_stream, chargernumber);
  victronDevice->vedirect_data_.batteryVoltage.connect_to(
      new SKOutputNumber("electrical.batteries.houseBattery.solar" + String(chargernumber) + ".voltage", "/ve.direct." + String(chargernumber) + "/batteryVoltagePath"));
  victronDevice->vedirect_data_.panelVoltage.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".panelVoltage", "/ve.direct." + String(chargernumber) + "/panelVoltagePath"));
  victronDevice->vedirect_data_.panelPower.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".power", "/ve.direct." + String(chargernumber) + "/panelPowerPath", 
        new SKMetadata("W", "Panel Power", "Panel Power", "Pnl Pow")));
  victronDevice->vedirect_data_.batteryCurrent.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".current", "/ve.direct." + String(chargernumber) + "/chargeCurrentPath"));
  victronDevice->vedirect_data_.loadCurrent.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".loadCurrent", "/ve.direct." + String(chargernumber) + "/loadCurrentPath"));
  victronDevice->vedirect_data_.yieldTotal.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".yieldTotal", "/ve.direct." + String(chargernumber) + "/yieldTotalPath", 
        new SKMetadata("kWh", "Yield Total", "Yield Total", "Yield Total")));
  victronDevice->vedirect_data_.yieldToday.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".yieldToday", "/ve.direct." + String(chargernumber) + "/yieldTodayPath", 
        new SKMetadata("kWh", "Yield Today", "Yield Today", "Yield Today")));
  victronDevice->vedirect_data_.maximumPowerToday.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".maximumPowerToday", "/ve.direct." + String(chargernumber) + "/maximumPowerTodayPath", 
        new SKMetadata("W", "Maximum Power Today", "Maximum Power Today", "Max Power Today")));
  victronDevice->vedirect_data_.yieldYesterday.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".yieldYesterday", "/ve.direct." + String(chargernumber) + "/yieldYesterdayPath", 
        new SKMetadata("kWh", "Yield Yesterday", "Yield Yesterday", "Yield Yesterday")));
  victronDevice->vedirect_data_.maximumPowerYesterday.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".maximumPowerYesterday", "/ve.direct." + String(chargernumber) + "/maximumPowerYesterdayPath", 
        new SKMetadata("W", "Maximum Power Yesterday", "Maximum Power Yesterday", "Max Power Yesterday")));
  victronDevice->vedirect_data_.errorCode.connect_to(
      new SKOutputNumber("electrical.solar.charger" + String(chargernumber) + ".errorCode", "/ve.direct." + String(chargernumber) + "/errorCodePath"));
  victronDevice->vedirect_data_.stateOfOperation.connect_to(
      new SKOutputString("electrical.solar.charger" + String(chargernumber) + ".stateOfOperation", "/ve.direct." + String(chargernumber) + "/stateOfOperationPath"));

  return victronDevice;
}

