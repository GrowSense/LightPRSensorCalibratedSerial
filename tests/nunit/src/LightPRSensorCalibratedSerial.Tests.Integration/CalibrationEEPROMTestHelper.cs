﻿using System;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
	public class CalibrationEEPROMTestHelper : GrowSenseHardwareTestHelper
	{
		public string Label;
		public string Letter;
		public int RawLightValue = 0;

		public void TestCalibrationEEPROM()
		{
			WriteTitleText("Starting calibration command test");

			Console.WriteLine("Label: " + Label);
			Console.WriteLine("Letter: " + Letter);
			Console.WriteLine("");

			ConnectDevices();

			ResetDeviceSettings ();

			SendCalibrationCommand();

			ResetDeviceViaPin ();

			var dataEntry = WaitForDataEntry ();

			AssertDataValueEquals(dataEntry, Letter, RawLightValue);
		}

		public void SendCalibrationCommand()
		{
			var command = Letter + RawLightValue;

			WriteParagraphTitleText("Sending calibration command...");

			SendDeviceCommand(command);

			var dataEntry = WaitForDataEntry();

			WriteParagraphTitleText("Checking calibration value...");

			AssertDataValueEquals(dataEntry, Letter, RawLightValue);
		}
	}
}
