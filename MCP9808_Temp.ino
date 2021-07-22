/*
 * MCP9808
 * Description:  Seeed Studio MCP9808 Temp Sensor
 * Author:  Patrick Bolton 
 * Date:  05/27/21
 */

#include "Particle.h"
#include "math.h"
#include "MCP9808.h"

SYSTEM_THREAD(ENABLED);

MCP9808 mcp = MCP9808();

#define dbSerial Serial

SerialLogHandler logHandler;

#define UPDATE_INTERVAL 5000  //1 sec = 1000 millis

unsigned long UpdateInterval;
int min_last, min_time;

void setup() 
{
  dbSerial.begin(9600);

  // Wait for the sensor to come up
	while(! mcp.begin()){
	    Log.info("MCP9808 not found");
	    delay(500);
	}

	// Set resolution to the maximum (slowest conversion)
	mcp.setResolution(MCP9808_SLOWEST);

	Log.info("MCP9808 Begun");

  UpdateInterval = millis();
  min_last=-1;
}

void loop()
{
  Time.zone(-7);
  if(Particle.disconnected()){return;}

  if ((millis() - UpdateInterval) > UPDATE_INTERVAL)
  {
    float MCP_T=mcp.getTemperature();  
    Log.info("MCP9808_Temp: %f", MCP_T);
    min_time=Time.minute();
    if((min_time!=min_last)&&(min_time==0||min_time==15||min_time==30||min_time==45))
    {
      //createEventPayload1();
      min_last = min_time;    
      Log.info("Last Update: %d", min_last);
      Log.info(Time.timeStr());
    }
    Log.info("loop");
    Log.info(Time.timeStr());
    UpdateInterval = millis();
  }
}