#define USETCPHOSTTHREAD

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

using System.Threading;     ///for thread
using System.Diagnostics;   ///for trace
using System.IO;            ///for TextWrite
using System.Net;           ///for network
using System.Net.Sockets;   ///for winsock




namespace WpfApp02
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public MainWindow()
        {
            InitializeComponent();
            ///set Disconnect button default OFF
            button3.IsEnabled = false;
            iRunType = 0;   ///0 for not run, 1 for server run, 2 for client run
        }

//-------------------------------------------------------------------------------------
        private TcpHostThread TcpServer;
        private TcpClientThread TcpClient;
        private Thread newTcpHost;
        private Thread newTcpClient;
        private int iRunType;

        private void addListBoxString(ListBox TargetListBox, string InputStr)
        {
            TargetListBox.Items.Add(InputStr);
        }

        public void addListBoxString(string InputStr)
        {
            listBox1.Items.Add(InputStr);
        }

        private bool bExitthread;

        private void TcpHostMethod()
        {
            ///int result = 1;
            ///Console.WriteLine(result.ToString() + " from thread ID: " + AppDomain.GetCurrentThreadId().ToString());
            ///
            bExitthread = false;
            while (!bExitthread)
            {
                Thread.Sleep(1500);
                ///addListBoxString("In TcpHostMethod!!");
                Trace.WriteLine("In TcpHostMethod!!");
                Console.WriteLine("In TcpHostMethod!!");

            }
        }
//-------------------------------------------------------------------------------------
        private void button1_Click(object sender, RoutedEventArgs e)
        {   ///Tcp Host control,
            ///
            if (true == button1.IsEnabled)
            {
                addListBoxString(listBox1, "TCP host Up");
                button1.IsEnabled = false;
                button2.IsEnabled = false;
                button3.IsEnabled = true;
#if USETCPHOSTTHREAD
                TcpServer = new TcpHostThread();
                newTcpHost = new Thread(new ThreadStart(TcpServer.TcpHostMethod));
                newTcpHost.Start();
                iRunType = 1;   ///0 for not run, 1 for server run, 2 for client run
#else
                newTcpHost = new Thread(new ThreadStart(this.TcpHostMethod));
                newTcpHost.Start();
#endif
            }
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            if (true == button1.IsEnabled)
            {
                addListBoxString(listBox1, "TCP client Up");
                button1.IsEnabled = false;
                button2.IsEnabled = false;
                button3.IsEnabled = true;

                TcpClient = new TcpClientThread();
                newTcpClient = new Thread(new ThreadStart(TcpClient.TcpClientMethod));
                newTcpClient.Start();
                iRunType = 2;   ///0 for not run, 1 for server run, 2 for client run
            }
        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {   ///Tcp disconnect
            ///
            if (true == button3.IsEnabled)
            {
                addListBoxString(listBox1, "TCP disconnect");
                button1.IsEnabled = true;
                button2.IsEnabled = true;
                button3.IsEnabled = false;
#if USETCPHOSTTHREAD
                if( 1 == iRunType )
                    TcpServer.StopThread();
                if( 2 == iRunType )
                    TcpClient.StopThread();
#else
                newTcpHost.Abort();
#endif
                iRunType = 0;   ///0 for not run, 1 for server run, 2 for client run
            }
        }
    }   ///public partial class MainWindow : Window



    public class TcpHostThread
    {
        private bool bExitthread;
        private TcpListener HostListen;
        private ConnectionThread newconnection;
        private Thread newthread;
        private Int32 port = 13000;
        private IPAddress localAddr = IPAddress.Parse("127.0.0.1");

 
        public void TcpHostMethod()
        {
            ///int result = 1;
            ///Console.WriteLine(result.ToString() + " from thread ID: " + AppDomain.GetCurrentThreadId().ToString());

            ///HostListen = new TcpListener(9050);
            HostListen = new TcpListener(localAddr, port);
            HostListen.Start();
            Console.WriteLine("TcpHostThread: In TcpHostMethod:" + localAddr.ToString() + " " + port.ToString() );

            Console.WriteLine("Waiting for clients...");
            bExitthread = false;
            while (!bExitthread)
            {
                while (!HostListen.Pending() && !bExitthread)
                {
                    Thread.Sleep(1000);
                    Console.WriteLine("TcpHostThread: In HostListen Pending!!");
                }
                ///Thread.Sleep(1500);
                Console.WriteLine("TcpHostThread: In TcpHostMethod!!");

                if (!bExitthread)
                {
                    Console.WriteLine("TcpHostThread: Client found!!");

                    newconnection = new ConnectionThread();
                    newconnection.threadListener = this.HostListen;
                    newthread = new Thread(new ThreadStart(newconnection.HandleConnection));
                    newthread.Start();
                }

            }
            Console.WriteLine("TcpHostThread: Quit TcpHostMethod!!");
            if (bExitthread)
            {
                Console.WriteLine("TcpHostThread: Abort newthread!!");
                newthread.Abort();
            }
        }

        public void StopThread()
        {
            bExitthread = true;
        }

/*
        public static void Main()
        {
            SimpleThread simpleThread = new SimpleThread();
            simpleThread.SimpleMethod();

            ThreadStart ts = new ThreadStart(simpleThread.SimpleMethod);
            Thread t = new Thread(ts);
            t.Start();
        }
*/ 
    }


    public class TcpClientThread
    {
        ///private IPAddress localAddr = IPAddress.Parse("127.0.0.1");
        private TcpClient server;
        private Int32 port = 13000;
        private bool bExitthread;

        public void TcpClientMethod()
        {

            byte[] data = new byte[1024];
            string input, stringData;

            try
            {
                server = new TcpClient("127.0.0.1", port);
            }
            catch (SocketException)
            {
                Console.WriteLine("Unable to connect to server");
                return;
            }
            NetworkStream ns = server.GetStream();

            bExitthread = false;
            while (!bExitthread)
            {
                //input = Console.ReadLine();
                //if (input == "exit")
                //    break;
                input = "This client message!!";
                ns.Write(Encoding.ASCII.GetBytes(input), 0, input.Length);
                ns.Flush();

                data = new byte[1024];
                int recv = ns.Read(data, 0, data.Length);
                stringData = Encoding.ASCII.GetString(data, 0, recv);
                Console.WriteLine("Msg from server: " + stringData);

                Thread.Sleep(1900);
            }
            Console.WriteLine("Disconnecting from server...");
            ns.Close();
            server.Close();
        }

        public void StopThread()
        {
            bExitthread = true;
        }
    }


    public class ConnectionThread
    {
        public TcpListener threadListener;
        private static int connections = 0;

        public void HandleConnection()
        {
            int recv;
            byte[] data = new byte[1024];
            string stringData;

            TcpClient client = threadListener.AcceptTcpClient();
            NetworkStream ns = client.GetStream();
            connections++;
            Console.WriteLine("New client accepted: {0} active connections", connections);

            string welcome = "Welcome to my test server";
            data = Encoding.ASCII.GetBytes(welcome);
            ns.Write(data, 0, data.Length);

            while (true)
            {
                data = new byte[1024];
#if true
                try
                {
                    recv = ns.Read(data, 0, data.Length);
                }
                catch (SocketException)
                {
                    Console.WriteLine("Unable to read from client");
                    recv = 0;
                }
#else
                recv = ns.Read(data, 0, data.Length);
#endif
                if (recv == 0)
                    break;

                stringData = Encoding.ASCII.GetString(data, 0, recv);
                Console.WriteLine("Msg from clinet: " + stringData);
                ns.Write(data, 0, recv);
            }
            ns.Close();
            client.Close();
            connections--;
            Console.WriteLine("Client disconnected: {0} active connections", connections);
        }
    }


}
