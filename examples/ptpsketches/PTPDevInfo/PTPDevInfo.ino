#include <usbhub.h>

#include <ptp.h>
#include <ptpdebug.h>
#include "devinfoparser.h"

class CamStateHandlers : public PTPStateHandlers
{
      enum CamStates { stInitial, stDisconnected, stConnected };
      CamStates stateConnected;

public:
      CamStateHandlers() : stateConnected(stInitial) {};

      virtual void OnDeviceDisconnectedState(PTP *ptp);
      virtual void OnDeviceInitializedState(PTP *ptp);
} CamStates;

USB      Usb;
// USBHub   Hub1(&Usb);
//PTP      Ptp(&Usb, &CamStates);
// PTP camera(&Usb, &CamStates);

PTP ptp_instance(&Usb, &CamStates);

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp
    __attribute__((unused)))
{
    if (stateConnected == stConnected || stateConnected == stInitial)
    {
        stateConnected = stDisconnected;
     		E_Notify(PSTR("Camera disconnected\r\n"), 0x80);
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp /* __attribute__((unused)) */)
{
    if (stateConnected == stDisconnected || stateConnected == stInitial)
    {
        stateConnected = stConnected;
       E_Notify(PSTR("Camera connected\r\n"), 0x80);
        {
        	HexDump          dmp;
        	ptp_instance.GetDeviceInfo(&dmp);
                E_Notify(PSTR("\n"), 0x80);
        }

        {
                DevInfoParser    prs;
        	ptp_instance.GetDeviceInfo(&prs);
        }
    }
}

void setup()
{
    Serial.begin( 115200 );
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay( 200 );
}

void loop()
{
    Usb.Task();
}

