using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.IO;

namespace GUI_Lab4
{
    public partial class Form1 : Form
    {
        [DllImport("D:\\Repository\\Crypto\\Lab\\Lab4\\hash_tool-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "hash_char")]
        public static extern IntPtr hash_char(int mode, IntPtr message, int shakeLen);
        public Form1()
        {
            InitializeComponent();
        }

        private void chooseFileBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Open file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        byte[] bytes = File.ReadAllBytes(openFileDialog.FileName);
                        string text = Encoding.UTF8.GetString(bytes);
                        inputBox.Text = text;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error: " + ex.Message);
                    }
                }
            }
        }

        private void hashBtn_Click(object sender, EventArgs e)
        {
            int mode = 1;
            string algo = modeBox.Text.Trim();

            if (algo == "SHA224") mode = 1;
            else if (algo == "SHA256") mode = 2;
            else if (algo == "SHA384") mode = 3;
            else if (algo == "SHA512") mode = 4;
            else if (algo == "SHA3-224") mode = 5;
            else if (algo == "SHA3-256") mode = 6;
            else if (algo == "SHA3-384") mode = 7;
            else if (algo == "SHA3-512") mode = 8;
            else if (algo == "SHAKE128") mode = 9;
            else if (algo == "SHAKE256") mode = 10;
            else
            {
                MessageBox.Show("Unknown hash algorithm selected.");
                return;
            }
            int d = 32;
            if (digestLengthBox.Text.Length > 0) {
                bool res = Int32.TryParse(digestLengthBox.Text, out d);
                if (!res) d = 32;
            }

            string originalText = inputBox.Text;

            byte[] utf8Bytes = Encoding.UTF8.GetBytes(originalText);

            IntPtr ptr = Marshal.AllocHGlobal(utf8Bytes.Length);
            Marshal.Copy(utf8Bytes, 0, ptr, utf8Bytes.Length);

            //string decodedText = Encoding.UTF8.GetString(utf8Bytes);
            IntPtr resultPtr = hash_char(mode, ptr, d);
            string result = Marshal.PtrToStringAnsi(resultPtr);
            
            outputBox.Text = result;

        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                saveFileDialog.Title = "Save your file";
                saveFileDialog.FileName = "digest.txt";

                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        File.WriteAllText(saveFileDialog.FileName, outputBox.Text);
                        MessageBox.Show("File saved successfully.", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error saving file:\n" + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }
    }
}
