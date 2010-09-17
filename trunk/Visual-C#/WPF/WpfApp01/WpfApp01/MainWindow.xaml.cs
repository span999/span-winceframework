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
using System.Net;   ///TCP
using System.Net.Sockets;   ///TCP
using System.Threading; ///TCP

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

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            ListBoxShow("button2 Click");

            if (null == myInC.Find(textBox1.Text.ToString()))
                myExC.AddLast(textBox1.Text);
            else
                myExC.Remove(myExC.Find(textBox1.Text.ToString()));


            listBox2.Items.Clear();
            int iLoop = 0;

            while (iLoop < myInC.Count)
            {
                listBox2.Items.Add("INC:" + myInC.ElementAt<string>(iLoop));
                iLoop++;
            }
            iLoop = 0;
            while (iLoop < myExC.Count)
            {
                listBox2.Items.Add("EXC:" + myExC.ElementAt<string>(iLoop));
                iLoop++;
            }

            textBox1.Clear();
        }


        private void button3_Click(object sender, RoutedEventArgs e)
        {
            ///ThreadedTcpSrvr server = new ThreadedTcpSrvr();
            ListBoxShow("button3 Click");

            if (null == myInC.Find(textBox1.Text.ToString()))
                myInC.AddLast(textBox1.Text);
            else
                myInC.Remove(myInC.Find(textBox1.Text.ToString()));


            listBox2.Items.Clear();
            int iLoop = 0;

            while(iLoop<myInC.Count)
            {
                listBox2.Items.Add("INC:" + myInC.ElementAt<string>(iLoop) );
                iLoop++;
            }
            iLoop = 0;
            while (iLoop < myExC.Count)
            {
                listBox2.Items.Add("EXC:" + myExC.ElementAt<string>(iLoop));
                iLoop++;
            }

            textBox1.Clear();
        }


        private void button4_Click(object sender, RoutedEventArgs e)
        {
            myFile rawFile;
            string sTmp;
            
            ListBoxShow("Load!!");

            rawFile = new myFile("\\mcelog.raw");

            while (!rawFile.IsEOF())
            {
                sTmp = rawFile.ReadNextLine();
                ///ListBoxShow(ReadTextLine("\\mcelog.raw"));
                if( 0 > sTmp.IndexOf(textBox1.Text.ToString()) )
                    ListBoxShow(sTmp);
            }

            rawFile.myDestroy();
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


///-------------------------------------------------------------------------------------------------------------------------------------------------
///
        private void ListBoxShow(string Msg)
        {
            listBox1.Items.Add(Msg);
            listBox1.Items.MoveCurrentToLast();
            Console.WriteLine(Msg);
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

        private string ReadTextLine(string filepath)
        {
            FileStream f = File.OpenRead(System.Environment.CurrentDirectory + filepath);
            StreamReader s = new StreamReader(f);
            string r = s.ReadLine();
            r = s.ReadLine();
            r = s.ReadLine();
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

///-------------------------------------------------------------------------------------------------------------------------------------------------
///
        private LinkedList<string> myInC = new LinkedList<string>();
        private LinkedList<string> myExC = new LinkedList<string>();

        private class myFile
        {
            private FileStream thisFile;
            private StreamReader thisStream;
            private bool bFileInited;

            public myFile(string filepath)
            { 
                thisFile = File.OpenRead(System.Environment.CurrentDirectory + filepath);
                thisStream = new StreamReader(thisFile);
                bFileInited = true;
            }
/*
            public ~myFile()
            {
                if (bFileInited)
                {
                    thisStream.Close();
                    thisFile.Close();
                    bFileInited = false;
                }
            }
*/
            public bool myDestroy()
            {
                if (bFileInited)
                {
                    thisStream.Close();
                    thisFile.Close();
                    bFileInited = false;
                }

                return true;
            }

            public bool IsInited()
            {
                bool bRet = false;

                if (bFileInited)
                    bRet = true;

                return bRet;
            }

            public bool IsEOF()
            {
                return thisStream.EndOfStream;
            }

            public string ReadNextLine()
            {
                string sRet = "";

                if (bFileInited)
                {
                    sRet = thisStream.ReadLine();
                }

                return sRet;
            }

        }

        public class ThreadedTcpSrvr
        {
            private TcpListener client;

            public ThreadedTcpSrvr()
            {
                ///client = new TcpListener(9050);
                client = new TcpListener(IPAddress.Any, 9050);
                client.Start();

                ///Console.WriteLine("Waiting for clients...");
                ///listBox1.Items.Add("tcp: " + "Waiting for clients...");
                while (true)
                {
                    while (!client.Pending())
                    {
                        Thread.Sleep(1000);
                    }

                    ConnectionThread newconnection = new ConnectionThread();
                    newconnection.threadListener = this.client;
                    Thread newthread = new Thread(new ThreadStart(newconnection.HandleConnection));
                    newthread.Start();
                }
            }
        }

        class ConnectionThread
        {
            public TcpListener threadListener;
            private static int connections = 0;

            public void HandleConnection()
            {
                int recv;
                byte[] data = new byte[1024];

                TcpClient client = threadListener.AcceptTcpClient();
                NetworkStream ns = client.GetStream();
                connections++;
                ///Console.WriteLine("New client accepted: {0} active connections", connections);
                ///listBox1.Items.Add("tcp: " + "Waiting for clients...");

                string welcome = "Welcome to my test server";
                data = Encoding.ASCII.GetBytes(welcome);
                ns.Write(data, 0, data.Length);

                while (true)
                {
                    data = new byte[1024];
                    recv = ns.Read(data, 0, data.Length);
                    if (recv == 0)
                        break;

                    ns.Write(data, 0, recv);
                }
                ns.Close();
                client.Close();
                connections--;
                ///Console.WriteLine("Client disconnected: {0} active connections", connections);
            }
        }



    }///public partial class MainWindow : Window



}
