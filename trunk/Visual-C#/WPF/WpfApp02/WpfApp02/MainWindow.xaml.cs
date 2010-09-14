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
        }

//-------------------------------------------------------------------------------------
        private TcpHostThread TcpServer;
        private Thread newTcpHost;

        private void addListBoxString(ListBox TargetListBox, string InputStr)
        {
            TargetListBox.Items.Add(InputStr);
        }

        public void addListBoxString(string InputStr)
        {
            listBox1.Items.Add(InputStr);
        }

        private bool bExitthread;

        public void TcpHostMethod()
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
                ///TcpServer.SetExternalListBox(listBox1);
                ///TcpServer.extListBox = this.listBox1;
                //TcpServer.SetExternalMainWin(this);
                newTcpHost = new Thread(new ThreadStart(TcpServer.TcpHostMethod));
                newTcpHost.Start();

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
                TcpServer.StopThread();
#else
                newTcpHost.Abort();
#endif
            }
        }
    }   ///public partial class MainWindow : Window

    public class TcpHostThread
    {
        //private ListBox extListBox;
        public ListBox extListBox;
        private bool bExitthread;
        private MainWindow extMainWin;
 
        public void TcpHostMethod()
        {
            ///int result = 1;
            ///Console.WriteLine(result.ToString() + " from thread ID: " + AppDomain.GetCurrentThreadId().ToString());
            ///
            bExitthread = false;
            while (!bExitthread)
            {
                Thread.Sleep(1500);
                ///addListBoxString("In TcpHostMethod!!");
                Console.WriteLine("TcpHostThread: In TcpHostMethod!!");
            }
        }

        public void StopThread()
        {
            bExitthread = true;
        }

        public void SetExternalListBox(ListBox extLB)
        {
            extListBox = extLB;
        }

        public void SetExternalMainWin(MainWindow extMW)
        {
            extMainWin = extMW;
        }

        private void  addListBoxString(string InputStr)
        {
            ///extListBox.Items.Add(InputStr);
            ///extMainWin.listBox1.Items.Add(InputStr);
            ///extMainWin.addListBoxString();
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
}
