using System;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
  public class FullScaleMonitorTestHelper : GrowSenseHardwareTestHelper
  {
    public FullScaleMonitorTestHelper ()
    {
    }

    public void RunFullScaleTest ()
    {
      WriteTitleText ("Starting full scale test");

      ConnectDevices ();

      int step = 25;

      for (int i = 100; i >= 0; i -= step) {
        RunFullScaleTestSegment (i);
      }

      for (int i = 0; i <= 100; i += step) {
        RunFullScaleTestSegment (i);
      }
    }

    public void RunFullScaleTestSegment (int lightPercentage)
    {
      WriteSubTitleText ("Starting full scale test segment");

      Console.WriteLine ("Light: " + lightPercentage + "%");
      Console.WriteLine ("");

      SimulateLight (lightPercentage);

      var data = WaitForData (3); // Wait for 3 data entries to give the simulator time to stabilise

      Console.WriteLine ("");
      Console.WriteLine ("Checking calibrated value");
      Console.WriteLine ("");
      
      var dataEntry = WaitForDataEntry ();

      AssertDataValueIsWithinRange (dataEntry, "L", lightPercentage, CalibratedValueMarginOfError);

      Console.WriteLine ("");
      Console.WriteLine ("Checking raw value");
      Console.WriteLine ("");

      var expectedRawValue = lightPercentage * AnalogPinMaxValue / 100;

      AssertDataValueIsWithinRange (data [data.Length - 1], "R", expectedRawValue, RawValueMarginOfError);
    }
  }
}