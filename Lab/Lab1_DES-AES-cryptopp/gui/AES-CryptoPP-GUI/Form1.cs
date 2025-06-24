using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace AES_CryptoPP_GUI
{
    public partial class Form1: Form
    {
        [DllImport("aes_cryptopp.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "GenerateKey")]
        public static extern void GenerateKey(string algo, int mode, byte[] key, byte[] iv, string filename);
        [DllImport("aes_cryptopp.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "AESEncrypt")]
        public static extern void AESEncrypt(int mode, string plaintext, bool saveToFile, string output, byte[] key, byte[] iv);
        [DllImport("aes_cryptopp.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "AESDecrypt")]
        public static extern void AESDecrypt(int mode, byte[] ciphertext, int size, bool saveToFile, string output, byte[] key, byte[] iv);
        [DllImport("aes_cryptopp.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "DESEncrypt")]
        public static extern void DESEncrypt(int mode, string plaintext, bool saveToFile, string output, byte[] key, byte[] iv);
        [DllImport("aes_cryptopp.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "DESDecrypt")]
        public static extern void DESDecrypt(int mode, byte[] ciphertext, int size, bool saveToFile, string output, byte[] key, byte[] iv);
        [DllImport("aes_cryptopp.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "LoadKeyFromFile")]
        public static extern void LoadKeyFromFile(string algo, int mode, string filename, byte[] key, byte[] iv);

        public Form1()
        {
            InitializeComponent();
        }

        private void generateBtn_Click(object sender, EventArgs e)
        {
            if (keygenAlgoBox.Text == "")
            {
                MessageBox.Show("Please choose an algorithm");
                return;
            }
            if (keygenModeBox.Text == "")
            {
                MessageBox.Show("Please choose a mode");
                return;
            }
            byte[] key = new byte[32];
            byte[] iv = new byte[16];
            GenerateKey(keygenAlgoBox.Text, keygenModeBox.SelectedIndex, key, iv, keygenFilePath.Text);
            keygenOutputBox.Text += "Key and IV saved to: " + keygenFilePath.Text + "\r\n";
        }

        private void keygenChooseFileBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Choose where to save key file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    keygenFilePath.Text = openFileDialog.FileName;
                }
            }
        }

        private void encryptBtn_Click(object sender, EventArgs e)
        {
            if (encryptAlgoBox.Text == "")
            {
                MessageBox.Show("Please choose an algorithm");
                return;
            }
            if (encryptModeBox.Text == "")
            {
                MessageBox.Show("Please choose a mode");
                return;
            }
            if (encryptDestBox.Text == "")
            {
                MessageBox.Show("Please choose a destination file");
                return;
            }
            if (encryptFormatBox.Text == "")
            {
                MessageBox.Show("Please choose an output format");
                return;
            }
            string keyfile = "";
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Choose where to load key file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    keyfile = openFileDialog.FileName;
                }
                else
                {
                    MessageBox.Show("Please choose a key file");
                    return;
                }
            }
            bool saveToFile = (encryptFormatBox.Text == "File");
            string outputPath = "temp";
            if (saveToFile)
            {
                using (OpenFileDialog openFileDialog = new OpenFileDialog())
                {
                    openFileDialog.Filter = "All files (*.*)|*.*";
                    openFileDialog.Title = "Choose where to save output";

                    if (openFileDialog.ShowDialog() == DialogResult.OK)
                    {
                        keyfile = openFileDialog.FileName;
                    }
                    else
                    {
                        MessageBox.Show("Please choose an ouput file");
                        return;
                    }
                }
            }
            if (encryptAlgoBox.Text == "AES")
            {
                byte[] key = new byte[32];
                byte[] iv = new byte[16];
                LoadKeyFromFile(encryptAlgoBox.Text, encryptModeBox.SelectedIndex, keyfile, key, iv);
                AESEncrypt(encryptModeBox.SelectedIndex + 1, encryptPlaintextBox.Text, true, outputPath, key, iv);
            }
            else if (encryptAlgoBox.Text == "DES")
            {
                byte[] key = new byte[8];
                byte[] iv = new byte[8];
                LoadKeyFromFile(encryptAlgoBox.Text, encryptModeBox.SelectedIndex, keyfile, key, iv);
                DESEncrypt(encryptModeBox.SelectedIndex + 1, encryptPlaintextBox.Text, true, outputPath, key, iv);
            }
            if (saveToFile)
            {
                encryptOutputBox.Text += "Encryption completed. Output saved to: " + outputPath + "\r\n";
            }
            else
            {
                byte[] ciphertext = File.ReadAllBytes(outputPath);
                if (encryptFormatBox.Text == "Hex")
                {
                    encryptOutputBox.Text += "Encryption completed.\r\nCiphertext: " + BitConverter.ToString(ciphertext).Replace("-", "") + "\r\n";
                }
                else // Base 64
                {
                    encryptOutputBox.Text += "Encryption completed.\r\nCiphertext: " + Convert.ToBase64String(ciphertext) + "\r\n";
                }
            }
        }

        private void encryptFileBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Text file (*.txt)|*.txt|All files (*.*)|*.*";
                openFileDialog.Title = "Open file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        byte[] bytes = File.ReadAllBytes(openFileDialog.FileName);
                        string text = Encoding.UTF8.GetString(bytes);
                        encryptPlaintextBox.Text = text;
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Error: " + ex.Message);
                    }
                }
            }
        }
    }
}
