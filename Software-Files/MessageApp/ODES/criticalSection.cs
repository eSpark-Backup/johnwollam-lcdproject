using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ODES
{
    public static class criticalSection
    {
        public static SerialPort serialPort { get; set; }

        public static void connectSerial()
        {
            try
            {
                // Get a list of serial port names.
                string[] ports = SerialPort.GetPortNames();
                SerialPort serialPort;
                bool isConnected = false;
                foreach (string port in ports)
                {
                    try
                    {
                        serialPort = new SerialPort(port, 9600);
                        serialPort.DtrEnable = true;
                        serialPort.ReadTimeout = 1000;
                        serialPort.WriteTimeout = 1000;
                        serialPort.Open();
                        char[] a = new char[1];
                        a[0] = '!';
                        serialPort.Write(a, 0, 1);
                        char[] temp = new char[1];

                        serialPort.Read(temp, 0, 1);
                        string received = temp[0].ToString();
                        serialPort.Read(temp, 0, 1);
                        received += temp[0].ToString();
                        if (received == "$#") 
                        {
                            criticalSection.serialPort = serialPort;
                            isConnected = true;
                            serialPort.Close();
                            break;
                        }
                        if (serialPort.IsOpen)
                        {
                            serialPort.Close();
                        }
                    }
                    catch (IOException)
                    {
                    }
                    catch (UnauthorizedAccessException)
                    {
                    }
                    catch (TimeoutException)
                    {
                    }
                    catch (Exception ex)
                    {
                        throw ex;
                    }
                }
                if (isConnected == false)
                {
                    throw new Exception("Hardware not found");
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
    }
}
