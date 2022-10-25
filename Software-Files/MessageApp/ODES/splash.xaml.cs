using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ODES
{
    /// <summary>
    /// Interaction logic for splash.xaml
    /// </summary>
    public partial class splash : Window
    {
        public splash()
        {
            InitializeComponent();
        }
        [STAThread]
        private void initializeStuff()
        {
            Application.Current.Dispatcher.Invoke((Action)delegate
               {
                   try
                   {
                       criticalSection.connectSerial();
                       Thread.Sleep(3000);
                       LoginForm loginscreen = new LoginForm();
                       loginscreen.Show();
                   }
                   catch (Exception ex)
                   {
                       MessageBox.Show(ex.Message);
                   }
                   finally
                   {
                       this.Close();
                   }
               });
        }
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                Thread t = new Thread(initializeStuff);
                t.IsBackground = true;
                t.Start();
            }
            catch (Exception ex)
            {

                MessageBox.Show(ex.Message);
            }
        }
    }
}
