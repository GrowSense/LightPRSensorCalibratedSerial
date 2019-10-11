﻿using System;
using System.Threading;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
	public class CalibrateCommandTestHelper : GrowSenseHardwareTestHelper
	{
		public string Label;
		public string Letter;
		public int SimulatedLightPercentage = -1;
		public int RawLightValue = 0;

		public CalibrateCommandTestHelper()
		{
		}

		public void TestCalibrateCommand()
		{
			WriteTitleText("Starting calibrate " + Label + " command test");

			Console.WriteLine("Simulated light: " + SimulatedLightPercentage + "%");

			if (RawLightValue == 0)
				RawLightValue = SimulatedLightPercentage * AnalogPinMaxValue / 100;

			Console.WriteLine("Raw light value: " + RawLightValue);
			Console.WriteLine("");

			var simulatorIsNeeded = SimulatedLightPercentage > -1;

			ConnectDevices(simulatorIsNeeded);

			if (SimulatorIsEnabled)
			{
				SimulateLight(SimulatedLightPercentage);

				var values = WaitForData(4); // Wait for 4 data entries to give the simulator time to stabilise

				AssertDataValueIsWithinRange(values[values.Length - 1], "R", RawLightValue, RawValueMarginOfError);
			}

			SendCalibrationCommand();
		}

		public void SendCalibrationCommand()
		{
			var command = Letter;

			// If the simulator isn't enabled then the raw value is passed as part of the command to specify it directly
			if (!SimulatorIsEnabled)
				command = command + RawLightValue;

			SendDeviceCommand(command);

			var data = WaitForData(4); // Wait for 4 data entries to let the light simulator stabilise

			// If using the light simulator then the value needs to be within a specified range
			if (SimulatorIsEnabled)
				AssertDataValueIsWithinRange(data[data.Length - 1], Letter, RawLightValue, RawValueMarginOfError);
			else // Otherwise it needs to be exact
				AssertDataValueEquals(data[data.Length - 1], Letter, RawLightValue);
		}
	}
}