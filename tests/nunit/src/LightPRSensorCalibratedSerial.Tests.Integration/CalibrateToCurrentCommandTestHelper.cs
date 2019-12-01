using System;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
  public class CalibrateToCurrentCommandTestHelper : GrowSenseHardwareTestHelper
  {
    public string Label;
    public string Letter;
    public int SimulatedLightPercentage = -1;
    public int RawLightValue = 0;

    public CalibrateToCurrentCommandTestHelper ()
    {
    }

    public void TestCalibrateCommand ()
    {
      WriteTitleText ("Starting calibrate " + Label + " command test");


      Console.WriteLine ("Simulated light: " + SimulatedLightPercentage + "%");

      if (RawLightValue == 0)
        RawLightValue = SimulatedLightPercentage * AnalogPinMaxValue / 100;

      Console.WriteLine ("Raw light value: " + RawLightValue);
      Console.WriteLine ("");

      var simulatorIsNeeded = SimulatedLightPercentage > -1;

      ConnectDevices (simulatorIsNeeded);

      if (SimulatorIsEnabled) {
        SimulateLight (SimulatedLightPercentage);

        // Skip the first entries to give the value time to stabilise
        WaitForData (2);

        var dataEntry = WaitForDataEntry ();

        AssertDataValueIsWithinRange (dataEntry, "R", RawLightValue, RawValueMarginOfError);
      }

      SendCalibrationCommand ();
    }

    public void SendCalibrationCommand ()
    {
      var command = Letter + RawLightValue;

      SendDeviceCommand (command);

      // Skip the first X entries to give the value time to update
      WaitForData (2);

      var dataEntry = WaitForDataEntry ();

      AssertDataValueIsWithinRange (dataEntry, Letter, RawLightValue, RawValueMarginOfError);
    }
  }
}

