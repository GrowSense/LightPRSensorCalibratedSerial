﻿using System;
using NUnit.Framework;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
  [TestFixture (Category = "Integration")]
  public class CalibrateToCurrentCommandTestFixture : BaseTestFixture
  {
    [Test]
    public void Test_CalibrateDarkToCurrentSoilMoistureValueCommand_20Percent ()
    {
      using (var helper = new CalibrateToCurrentCommandTestHelper ()) {
        helper.Label = "dark";
        helper.Letter = "D";
        helper.SimulatedLightPercentage = 20;

        helper.DevicePort = GetDevicePort ();
        helper.DeviceBaudRate = GetDeviceSerialBaudRate ();

        helper.SimulatorPort = GetSimulatorPort ();
        helper.SimulatorBaudRate = GetSimulatorSerialBaudRate ();

        helper.TestCalibrateCommand ();
      }
    }

    [Test]
    public void Test_CalibrateDarkToCurrentSoilMoistureValueCommand_30Percent ()
    {
      using (var helper = new CalibrateToCurrentCommandTestHelper ()) {
        helper.Label = "dark";
        helper.Letter = "D";
        helper.SimulatedLightPercentage = 30;

        helper.DevicePort = GetDevicePort ();
        helper.DeviceBaudRate = GetDeviceSerialBaudRate ();

        helper.SimulatorPort = GetSimulatorPort ();
        helper.SimulatorBaudRate = GetSimulatorSerialBaudRate ();

        helper.TestCalibrateCommand ();
      }
    }

    [Test]
    public void Test_CalibrateBrightToCurrentSoilMoistureValueCommand_80Percent ()
    {
      using (var helper = new CalibrateToCurrentCommandTestHelper ()) {
        helper.Label = "bright";
        helper.Letter = "B";
        helper.SimulatedLightPercentage = 80;

        helper.DevicePort = GetDevicePort ();
        helper.DeviceBaudRate = GetDeviceSerialBaudRate ();

        helper.SimulatorPort = GetSimulatorPort ();
        helper.SimulatorBaudRate = GetSimulatorSerialBaudRate ();

        helper.TestCalibrateCommand ();
      }
    }

    [Test]
    public void Test_CalibrateBrightToCurrentSoilMoistureValueCommand_90Percent ()
    {
      using (var helper = new CalibrateToCurrentCommandTestHelper ()) {
        helper.Label = "bright";
        helper.Letter = "B";
        helper.SimulatedLightPercentage = 90;

        helper.DevicePort = GetDevicePort ();
        helper.DeviceBaudRate = GetDeviceSerialBaudRate ();

        helper.SimulatorPort = GetSimulatorPort ();
        helper.SimulatorBaudRate = GetSimulatorSerialBaudRate ();

        helper.TestCalibrateCommand ();
      }
    }
  }
}

