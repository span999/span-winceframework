using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Mail;  ///for mail
using System.Net.Security;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void button1_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            string temp;
            temp = "MouseLeftButtonUp";
            listBox1.Items.Add(temp);
        }

        private void button1_MouseUp(object sender, MouseButtonEventArgs e)
        {
            listBox1.Items.Add("MouseUp");
        }

        private void button1_MouseDown(object sender, MouseButtonEventArgs e)
        {
            listBox1.Items.Add("MouseDown");
        }

        private void button1_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            listBox1.Items.Add("MouseRightButtonUp");
        }

        private void listBox1_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            listBox1.Items.Clear();
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            listBox1.Items.Add("Click");
        }

        public void mailSent()
        {
            MailMessage mail = new MailMessage();
            ///NetworkCredential cred = new NetworkCredential("spanliu@gmail.com", "gmail999", "gmail");
            //收件者
            mail.To.Add("spanliu@hotmail.com");
            mail.Subject = "subject";
            //寄件者
            mail.From = new System.Net.Mail.MailAddress("spanliu@gmail.com");
            mail.IsBodyHtml = true;
            mail.Body = "message";
            //設定SMTP
            SmtpClient smtp = new SmtpClient("smtp.gmail.com");
            smtp.UseDefaultCredentials = false;
            smtp.EnableSsl = true;
            ///smtp.Credentials = cred;
            smtp.Port = 587;
            //送出Mail
            smtp.Send(mail);

        }

        private void textBox1_TextChanged(object sender, TextChangedEventArgs e)
        {
            ///listBox1.Items.Add("TextChanged");
        }

        private void textBox1_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return )
            {
                listBox1.Items.Add(textBox1.Text);
            }
        }
    }
}
