using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
           
        }

        private void listBox1_MouseLeave(object sender, EventArgs e)
        {
            richTextBox1.Text = "Mouse Move"; 
        }

        private void button1_Click(object sender, EventArgs e)
        {

            if (true == richTextBox1.Visible )
            {
                richTextBox1.Hide();
                listBox1.Show();
            }
            else
            {
                richTextBox1.Text = "123";
                richTextBox1.Text = "456";
                listBox1.Text = "567";

                richTextBox1.Show();
                listBox1.Hide();

            }
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
