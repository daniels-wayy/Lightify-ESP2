#include "portal_s.h"

void buildWebInterface()
{
  GP.BUILD_BEGIN(GP_DARK);
  GP.UPDATE("ledL,sw,br,col,fx", 1500);

  GP.LABEL("STATUS");
  GP.BLOCK_BEGIN();
  GP.LABEL("Is Online:");
  GP.LED_GREEN("ledL", mqtt.isConnected());
  GP.BLOCK_END();

  GP.LABEL("SETTINGS");
  GP.BLOCK_BEGIN();
  GP.LABEL("Power:");
  GP.SWITCH("sw", cfg.power);
  GP.BREAK();
  GP.SLIDER("br", cfg.brightness, 0, 255);
  GP.SLIDER("col", cfg.colorHue, 0, 255);
  GP.SLIDER("fx", cfg.fxIndex, 0, MODE_AMOUNT);
  GP.BLOCK_END();

  GP.FORM_BEGIN("/save");

  GP.LABEL("GPIO");
  GP.BLOCK_BEGIN();
  GP.NUMBER("stripPin", "Strip pin", cfg.ledPin);
  GP.BREAK();
  GP.NUMBER("useBtn", "Use button", cfg.useBtn);
  GP.BREAK();
  GP.NUMBER("btnPin", "Button pin", cfg.btnPin);
  GP.BREAK();
  GP.BLOCK_END();

  GP.LABEL("WIFI");
  GP.BLOCK_BEGIN();
  GP.TEXT("ssid", "SSID", cfg.ssid);
  GP.BREAK();
  GP.PASS("pass", "Password", cfg.pass);
  GP.BREAK();
  GP.TEXT("apName", "Access point name", cfg.apName);
  GP.BREAK();
  GP.PASS("apPass", "Access point password", cfg.apPass);
  GP.BLOCK_END();

  GP.LABEL("MQTT");
  GP.BLOCK_BEGIN();
  GP.TEXT("local", "Local Name", cfg.local);
  GP.BREAK();
  GP.TEXT("remote", "Remote Name", cfg.remote);
  GP.BREAK();
  GP.TEXT("host", "Host", cfg.host);
  GP.BREAK();
  GP.NUMBER("port", "Port", cfg.port);
  GP.BREAK();
  GP.NUMBER("stripC", "Strip Current", cfg.stripCurrent);
  GP.BREAK();
  GP.NUMBER("ledCount", "Led Count", cfg.ledCount);
  GP.BREAK();
  GP.NUMBER("gmt", "Time Offset", cfg.GMT);
  GP.BLOCK_END();
  GP.SUBMIT("Save");
  GP.FORM_END();
  GP.BUILD_END();
}

void startLocalPortal(IPAddress ip)
{
  DEBUG(F("Create AP: "));
  DEBUGLN(cfg.apName);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(cfg.apName, cfg.apPass);

  portal.start(); // launch portal

  while (portal.tick())
  {
    effects.loadingEffect(CRGB::Blue);
    if (cfg.useBtn)
    {
      btn.tick();
      // if button is clicked -> restart ESP
      if (btn.isClick())
        ESP.reset();
    }
  }
}

void checkPortal()
{
  if (portal.click())
  {
    if (portal.click("br"))
    {
      stopWorkflowProcess();
      cfg.brightness = portal.getInt("br");
    }
    if (portal.click("sw"))
    {
      stopWorkflowProcess();
      cfg.power = portal.getCheck("sw");
    }
    if (portal.click("col"))
    {
      cfg.colorHue = portal.getInt("col");
    }
    if (portal.click("fx"))
    {
      cfg.fxIndex = portal.getInt("fx");
    }
    if (portal.click())
    {
      EE_updateCfg();
    }
  }

  if (portal.update())
  {
    if (portal.update("ledL"))
      portal.answer(mqtt.isConnected());
    if (portal.update("br"))
      portal.answer(cfg.brightness);
    if (portal.update("sw"))
      portal.answer(cfg.power);
    if (portal.update("col"))
      portal.answer(cfg.colorHue);
    if (portal.update("fx"))
      portal.answer(cfg.fxIndex);
  }

  if (portal.form())
  {
    if (portal.form("/save"))
    {
      portal.copyStr("ssid", cfg.ssid);
      portal.copyStr("pass", cfg.pass);
      portal.copyStr("local", cfg.local);
      portal.copyStr("remote", cfg.remote);
      portal.copyStr("host", cfg.host);
      portal.copyStr("apName", cfg.apName);
      portal.copyStr("apPass", cfg.apPass);
      cfg.port = portal.getInt("port");
      cfg.stripCurrent = portal.getInt("stripC");
      cfg.ledCount = portal.getInt("ledCount");
      cfg.GMT = portal.getInt("gmt");
      cfg.ledPin = portal.getInt("stripPin");
      cfg.useBtn = constrain(portal.getInt("useBtn"), 0, 1);
      cfg.btnPin = portal.getInt("btnPin");
      led.clear();
      led.update();
      // yield();
      EE_updateCfgRst();
    }
  }
}