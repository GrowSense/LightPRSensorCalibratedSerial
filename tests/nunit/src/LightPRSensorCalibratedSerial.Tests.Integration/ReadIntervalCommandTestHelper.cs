using System;

namespace LightPRSensorCalibratedSerial.Tests.Integration
{
  public class ReadIntervalCommandTestHelper : SerialCommandTestHelper
  {
    public int ReadingInterval = 1;

    public void TestSetReadIntervalCommand ()
    {
      Key = "I";
      Value = ReadingInterval.ToString ();
      Label = "reading interval";

      TestCommand ();
    }
  }
}
