using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ODES
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class Home : MetroWindow
    {
        public Home()
        {
            InitializeComponent();
            appendLog("Hardware found on " + criticalSection.serialPort.PortName);
            selectedComportTxt.Text = criticalSection.serialPort.PortName;
        }
        private void ThemeSelector_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (ThemeSelector.SelectedIndex)
            {
                case 0:
                    {
                        ODES.Properties.Settings.Default.Theme = "/Resources/bg4.jpg";
                        this.Background = new ImageBrush(new BitmapImage(new Uri(BaseUriHelper.GetBaseUri(this), ODES.Properties.Settings.Default.Theme)));
                        ODES.Properties.Settings.Default.Save();

                    } break;
                case 1:
                    {
                        ODES.Properties.Settings.Default.Theme = "/Resources/bg.jpg";
                        this.Background = new ImageBrush(new BitmapImage(new Uri(BaseUriHelper.GetBaseUri(this), ODES.Properties.Settings.Default.Theme)));
                        ODES.Properties.Settings.Default.Save();
                    } break;
                case 2:
                    {
                        ODES.Properties.Settings.Default.Theme = "/Resources/bg2.jpg";
                        this.Background = new ImageBrush(new BitmapImage(new Uri(BaseUriHelper.GetBaseUri(this), ODES.Properties.Settings.Default.Theme)));
                        ODES.Properties.Settings.Default.Save();
                    } break;
                case 3:
                    {
                        ODES.Properties.Settings.Default.Theme = "/Resources/tabBg.jpg";
                        this.Background = new ImageBrush(new BitmapImage(new Uri(BaseUriHelper.GetBaseUri(this), ODES.Properties.Settings.Default.Theme)));
                        ODES.Properties.Settings.Default.Save();
                    } break;
            }
        }

        private void LogoutBtn_Click(object sender, RoutedEventArgs e)
        {
            LoginForm loginForm = new LoginForm();
            loginForm.Show();
            this.Close();
        }

        private void uploadMsg1_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(0, msg_1_Txt.Text);
        }
        private void uploadMsg3_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(2, msg_3_Txt.Text);
        }
        private void uploadMsg5_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(4, msg_5_Txt.Text);
        }
        private void uploadMsg7_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(6, msg_7_Txt.Text);
        }
        private void uploadMsg9_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(8, msg_9_Txt.Text);
        }
        private void uploadMsg2_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(1, msg_2_Txt.Text);
        }
        private void uploadMsg4_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(3, msg_4_Txt.Text);
        }
        private void uploadMsg6_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(5, msg_6_Txt.Text);
        }
        private void uploadMsg8_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(7, msg_8_Txt.Text);
        }
        private void uploadMsg10_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(9, msg_10_Txt.Text);
        }

        private string sanitize_text(string text)
        {
            if (text.Length < 64)
            {
                for (int i = text.Length; i < 64; i++)
                {
                    text += ".";
                }

            }
            return text;
        }

        private void sendMessage(int index, string message, bool isSplash = false)
        {
            int L = message.Length;
            string length;

            if (L < 10)
            {
                length = "0" + L.ToString();
            }
            else
            {
                length = L.ToString();
            }

            try
            {
                //checking for emptry string 
                if (message == string.Empty)
                {
                    appendLog("Cannot upload empty string");
                    return;
                }
                if (message.Length > 64)
                {
                    appendLog("Cannot send more than 64 characters ");
                    return;
                }

                //message = sanitize_text(message);
                criticalSection.serialPort.Open();
                if (isSplash == true)
                {
                    criticalSection.serialPort.Write("$#" + length + message + "%");
                }
                else
                {
                    criticalSection.serialPort.Write("$" + index + length + message + "%");
                }
                criticalSection.serialPort.Close();
                appendLog("Text:" + message + " has been uploaded to " + criticalSection.serialPort.PortName);
            }
            catch (Exception ex)
            {
               appendLog("error in sendMessage function \n" + ex.Message);
            }
        }

        private void appendLog(string message)
        {
            MainLogTxt.Text += "==> "+ DateTime.Now+ ": " + message +"\n\n";
        }

        private void clearBtn_Click(object sender, RoutedEventArgs e)
        {
            MainLogTxt.Text = string.Empty;
        }

        private void uploadAllBtn_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(0, msg_1_Txt.Text);
            sendMessage(1, msg_2_Txt.Text);
            sendMessage(2, msg_3_Txt.Text);
            sendMessage(3, msg_4_Txt.Text);
            sendMessage(4, msg_5_Txt.Text);
            sendMessage(5, msg_6_Txt.Text);
            sendMessage(6, msg_7_Txt.Text);
            sendMessage(7, msg_8_Txt.Text);
            sendMessage(8, msg_9_Txt.Text); 
            sendMessage(9, msg_10_Txt.Text);
        }

        private void reconnectBtn_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                criticalSection.connectSerial();
                selectedComportTxt.Text = criticalSection.serialPort.PortName;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void uploadSplashBtn_Click(object sender, RoutedEventArgs e)
        {
            sendMessage(-1,splashMsgTxt.Text,true);
        }

        private void RichTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
