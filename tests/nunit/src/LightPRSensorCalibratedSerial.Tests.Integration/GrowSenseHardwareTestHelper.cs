﻿using System;
using System.Threading;
using NUnit.Framework;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
  public class GrowSenseHardwareTestHelper : HardwareTestHelper
  {
    public int LightSimulatorPin = 9;
    public int LightSimulatorPowerPin = 3;
    public int DelayAfterTurningLightPRSensorOn = 3;
    public int RawValueMarginOfError = 36;
    public int CalibratedValueMarginOfError = 4;
    public double TimeErrorMargin = 0.2;
    public bool CalibrationIsReversedByDefault = false;
    public bool RequiresResetSettings = true;

    public GrowSenseHardwareTestHelper ()
    {
    }
    #region Enable Devices Functions
    public override void ConnectDevices (bool enableSimulator)
    {
      Console.WriteLine ("Connecting devices...");

      base.ConnectDevices (enableSimulator);

      PrepareDeviceForTest ();
    }
    #endregion
    #region Prepare Device Functions
    public virtual void PrepareDeviceForTest ()
    {
      PrepareDeviceForTest (true);
    }

    public virtual void PrepareDeviceForTest (bool consoleWriteDeviceOutput)
    {
      Console.WriteLine ("Preparing device for test...");

      if (RequiresResetSettings) {
        ResetDeviceSettings ();

        SetDeviceReadInterval (1);

        SetBrightCalibrationValue (AnalogPinMaxValue);

        SetDarkCalibrationValue (0);

        if (CalibrationIsReversedByDefault)
          ReverseDeviceCalibration ();
      }

      if (consoleWriteDeviceOutput)
        ReadFromDeviceAndOutputToConsole ();
    }
    #endregion
    #region General Device Command Settings
    public void SendDeviceCommand (string command)
    {
      WriteToDevice (command);

      WaitForMessageReceived (command);
    }

    public void WaitForMessageReceived (string message)
    {
      Console.WriteLine ("");
      Console.WriteLine ("Waiting for message: " + message);

      var output = String.Empty;
      var wasMessageReceived = false;

      Timeout.Start ();
                     
      while (!wasMessageReceived) {
        output += ReadLineFromDevice ();

        var expectedText = "Received message: " + message;
        if (output.Contains (expectedText)) {
          wasMessageReceived = true;

          Console.WriteLine ("  Message was received");

          ConsoleWriteSerialOutput (output);
        } else
          Timeout.Check (TimeoutWaitingForResponse, "Timed out waiting for text '" + expectedText + "'.");
      }
    }
    #endregion
    #region Specific Device Command Functions
    public void ResetDeviceSettings ()
    {
      var cmd = "X";

      Console.WriteLine ("");
      Console.WriteLine ("Resetting device default settings...");
      Console.WriteLine ("  Sending '" + cmd + "' command to device");
      Console.WriteLine ("");

      SendDeviceCommand (cmd);
    }

    public void SetDarkCalibrationValue (int value)
    {
      var cmd = "D" + value;

      Console.WriteLine ("");
      Console.WriteLine ("Setting dark calibration value to: " + value);
      Console.WriteLine ("  Sending '" + cmd + "' command to device");
      Console.WriteLine ("");

      SendDeviceCommand (cmd);
    }

    public void SetBrightCalibrationValue (int value)
    {
      var cmd = "B" + value;

      Console.WriteLine ("");
      Console.WriteLine ("Setting bright calibration value to: " + value);
      Console.WriteLine ("  Sending '" + cmd + "' command to device");
      Console.WriteLine ("");

      SendDeviceCommand (cmd);
    }

    public void SetDeviceReadInterval (int numberOfSeconds)
    {
      var cmd = "I" + numberOfSeconds;

      Console.WriteLine ("");
      Console.WriteLine ("Setting device read interval to " + numberOfSeconds + " second(s)...");
      Console.WriteLine ("  Sending '" + cmd + "' command to device");
      Console.WriteLine ("");

      SendDeviceCommand (cmd);
    }

    public void ReverseDeviceCalibration ()
    {
      var cmd = "R";

      Console.WriteLine ("");
      Console.WriteLine ("Reversing device calibration settings...");
      Console.WriteLine ("  Sending '" + cmd + "' command to device");
      Console.WriteLine ("");

      SendDeviceCommand (cmd);
    }
    #endregion
    #region Light light Simulator Functions
    public void SimulateLight (int lightPercentage)
    {
      Console.WriteLine ("");
      Console.WriteLine ("Simulating light percentage");
      Console.WriteLine ("  Sending analog percentage");
      Console.WriteLine ("    PWM pin: " + LightSimulatorPin);
      Console.WriteLine ("    Light light Percentage: " + lightPercentage + "%");
      Console.WriteLine ("");

      SimulatorClient.AnalogWritePercentage (LightSimulatorPin, lightPercentage);
    }
    #endregion
    #region Wait for Pin Functions
    public int WaitUntilLightPRSensorPowerPinIs (bool expectedValue)
    {
      return WaitUntilSimulatorPinIs ("light sensor power", LightSimulatorPowerPin, expectedValue);
    }

    public double WaitWhileLightPRSensorPowerPinIs (bool expectedValue)
    {
      return WaitWhileSimulatorPinIs ("light sensor power", LightSimulatorPowerPin, expectedValue);
    }
    #endregion
    #region Assert Simulator Pin Functions
    public void AssertLightPRSensorPowerPinForDuration (bool expectedValue, int durationInSeconds)
    {
      AssertSimulatorPinForDuration ("light sensor power", LightSimulatorPowerPin, expectedValue, durationInSeconds);
    }
    #endregion
  }
}
