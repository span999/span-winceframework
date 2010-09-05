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


using System.IO;
///using System.Text;

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
            DateTime currdt = DateTime.Now;
            listBox1.Items.Add("#$" + currdt.Ticks + "#In: " + ReadTextLine());
            ///listBox1.Items.Add( ReadText() );
            ///listBox1.Items.Add(ReadTextLine());
        }

        private void mailSent()
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
            {   ///we got return, check the commend
                ///DateTime currdt = DateTime.Now;
                ///listBox1.Items.Add( currdt.Ticks + "-In: " + textBox1.Text );
                ///WriteTextLine(currdt.Ticks + "-In: " + textBox1.Text);

                listBox1.Items.Add("cmd: " + textBox1.Text);
                listBox1.Items.Add("cmd: " + CmdParser(textBox1.Text));
                textBox1.Clear();
            }
        }

        private string CmdParser(string Cmds)
        {
            ///string Ret = "Parse Done!";
            string Ret = "unknown cmd!!";

            if (Cmds == "filter")
            {
                string[] tmp = Cmds.Split(new char[] { ' ', '-' });
                listBox1.Items.Add(tmp[0]);
                Ret = "Filter Done!";
            }
            return Ret;
        }


        private string ReadText()
        {
            //FileStream fs = File.OpenRead("C:\Documents and Settings\richard.conway\My Documents\Visual Studio Projects\TextReader\bin\Debug\readtext.rtf")
            FileStream f = File.OpenRead(System.Environment.CurrentDirectory + "\\mcelog.mlg");
            StreamReader s = new StreamReader(f);
            ///int ch = f.ReadByte();
            return s.ReadToEnd();
        }

        private string ReadTextLine()
        {
            //FileStream fs = File.OpenRead("C:\Documents and Settings\richard.conway\My Documents\Visual Studio Projects\TextReader\bin\Debug\readtext.rtf")
            FileStream f = File.OpenRead(System.Environment.CurrentDirectory + "\\mcelog.mlg");
            StreamReader s = new StreamReader(f);
            string r = s.ReadLine();
            f.Close();
            return r;
        }

        private void WriteTextLine( string wIn )
        {
            //FileStream fs = File.OpenRead("C:\Documents and Settings\richard.conway\My Documents\Visual Studio Projects\TextReader\bin\Debug\readtext.rtf")
            FileStream f = File.OpenWrite(System.Environment.CurrentDirectory + "\\mcelog.mlg");
            f.Position = f.Length;
            StreamWriter w = new StreamWriter(f);
            w.WriteLine("");
            w.Flush();
            w.Write(wIn);
            w.Flush();
            f.Close();
        }



    }
}
