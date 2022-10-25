using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
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
    public partial class BlankForm : MetroWindow
    {
        public BlankForm()
        {
            InitializeComponent();
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
    }
}
