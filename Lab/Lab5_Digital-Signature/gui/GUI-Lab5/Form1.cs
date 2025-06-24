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
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.IO;


namespace GUI_Lab5
{
    public partial class Form1 : Form
    {
        [DllImport("digital_signature-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "generate_ecdsa_keypair")]
        public static extern bool generate_ecdsa_keypair(string curve_name, string private_key_path, string public_key_path, string format);
        [DllImport("digital_signature-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "generate_rsa_keypair")]
        public static extern bool generate_rsa_keypair(int bits, string private_key_path, string public_key_path, string format);
        [DllImport("digital_signature-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "sign_ecdsa")]
        public static extern bool sign_ecdsa(string private_key_path, string message_path, string signature_path, string format);
        [DllImport("digital_signature-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "sign_rsapss")]
        public static extern bool sign_rsapss(string private_key_path, string message_path, string signature_path, string format);
        [DllImport("digital_signature-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "verify_ecdsa")]
        public static extern bool verify_ecdsa(string public_key_path, string message_path, string signature_path, string format);
        [DllImport("digital_signature-msvc.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "verify_rsapss")]
        public static extern bool verify_rsapss(string public_key_path, string message_path, string signature_path, string format);

        public Form1()
        {
            InitializeComponent();
        }

        private void generateBtn_Click(object sender, EventArgs e)
        {
            bool result = false;
            if (keygenFormatBox.Text == "")
            {
                MessageBox.Show("Please choose key format");
                return;
            }
            if (parameterBox.Text == "")
            {
                MessageBox.Show("Please provide key parameters");
                return;
            }
            if (keygenPubkeyBox.Text == "")
            {
                MessageBox.Show("Please provide public key file name");
                return;
            }
            if (keygenPrivkeyBox.Text == "")
            {
                MessageBox.Show("Please provide private key file name");
                return;
            }
            else
            {
                try
                {
                    if (keygenAlgoBox.Text == "ECDSA")
                    {
                        result = generate_ecdsa_keypair(parameterBox.Text, keygenPrivkeyBox.Text, keygenPubkeyBox.Text, keygenFormatBox.Text);
                        if (result)
                        {
                            richTextBox1.Text += "Key created: " + keygenPrivkeyBox.Text + ", " + keygenPubkeyBox.Text + "\r\n";
                        }
                        else richTextBox1.Text += "Failed to create key\r\n";
                    }
                    else
                    {
                        int bit = 3072;
                        if (!Int32.TryParse(parameterBox.Text, out bit))
                        {
                            richTextBox1.Text += "Invalid key length\r\n";
                            return;
                        }
                        result = generate_rsa_keypair(bit, keygenPrivkeyBox.Text, keygenPubkeyBox.Text, keygenFormatBox.Text);
                        if (result)
                        {
                            richTextBox1.Text += "Key created: " + keygenPrivkeyBox.Text + ", " + keygenPubkeyBox.Text + "\r\n";
                        }
                        else richTextBox1.Text += "Failed to create key\r\n";
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void keygenAlgoBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (keygenAlgoBox.Text == "ECDSA")
            {
                parameterLabel.Text = "Curve name";
            }
            else parameterLabel.Text = "Key length (bit)";
        }

        private void textFileBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Open file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    signFilePathBox.Text = openFileDialog.FileName;
                }
            }
        }

        private void signPrivkeyBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Open file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    signPrivkeyBox.Text = openFileDialog.FileName;
                }
            }
        }

        private void signBtn_Click(object sender, EventArgs e)
        {
            if (signFormatBox.Text == "")
            {
                MessageBox.Show("Please choose key format");
                return;
            }
            if (signAlgoBox.Text == "")
            {
                MessageBox.Show("Please choose algorithm (ECDSA, RSAPSS)");
                return;
            }
            if (signPrivkeyBox.Text == "") 
            {
                MessageBox.Show("Please choose secret key file");
                return;
            }
            if (signSignatureBox.Text == "")
            {
                MessageBox.Show("Please choose where to save signature");
                return;
            }
            string signedFilePath = "";
            if (signTextBox.Text != "")
            {
                using (SaveFileDialog sfd = new SaveFileDialog())
                {
                    sfd.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                    sfd.Title = "Save text to file";

                    if (sfd.ShowDialog() == DialogResult.OK)
                    {
                        try
                        {
                            File.WriteAllText(sfd.FileName, signTextBox.Text);
                            MessageBox.Show("File saved successfully!", "Success",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
                            signedFilePath = sfd.FileName;
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("Error saving file: " + ex.Message, "Error",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                        }
                    }
                }
                bool result = false;
                if (signAlgoBox.Text == "ECDSA")
                {
                    result = sign_ecdsa(signPrivkeyBox.Text, signedFilePath, signSignatureBox.Text, signFormatBox.Text);
                }
                else
                {
                    result = sign_rsapss(signPrivkeyBox.Text, signedFilePath, signSignatureBox.Text, signFormatBox.Text);
                }
                if (result)
                {
                    richTextBox2.Text += "Message signed successfully\r\n";
                }
                else richTextBox2.Text += "Failed to sign message\r\n";
            }
        }

        private void signTextBox_TextChanged(object sender, EventArgs e)
        {
            if (signTextBox.Text != "")
            {
                textFileBtn.Enabled = false;
                signFilePathBox.Enabled = false;
            }
            else
            {
                textFileBtn.Enabled = true;
                signFilePathBox.Enabled = true;
            }
        }

        private void signSignatureBtn_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog sfd = new SaveFileDialog())
            {
                sfd.Filter = "All files (*.*)|*.*";
                sfd.Title = "Save signature to file";

                if (sfd.ShowDialog() == DialogResult.OK)
                {
                    signSignatureBox.Text = sfd.FileName;
                }
            }
        }

        private void verifyFileBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                openFileDialog.Title = "Text file to verify with";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    verifyFilePathBox.Text = openFileDialog.FileName;
                }
            }
        }

        private void verifyPubkeyBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Public key file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    verifyPubkeyBox.Text = openFileDialog.FileName;
                }
            }
        }

        private void verifySigBtn_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "All files (*.*)|*.*";
                openFileDialog.Title = "Signature file";

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    verifySignatureBox.Text = openFileDialog.FileName;
                }
            }
        }

        private void button8_Click(object sender, EventArgs e)
        {
            if (verifyFormatBox.Text == "")
            {
                MessageBox.Show("Please choose key format");
                return;
            }
            if (verifyAlgoBox.Text == "") 
            {
                MessageBox.Show("Please choose algorithm (ECDSA, RSAPSS)");
                return;
            }
            if (verifyPubkeyBox.Text == "") 
            {
                MessageBox.Show("Please choose public key file");
                return;
            }

            if (verifySignatureBox.Text == "")
            {
                MessageBox.Show("Please choose where the signature is");
                return;
            }
            bool result = false;
            if (verifyAlgoBox.Text == "ECDSA")
            {
                result = verify_ecdsa(verifyPubkeyBox.Text, verifyFilePathBox.Text, verifySignatureBox.Text, verifyFormatBox.Text);
            }
            else
            {
                result = verify_rsapss(verifyPubkeyBox.Text, verifyFilePathBox.Text, verifySignatureBox.Text, verifyFormatBox.Text);
            }
            if (result)
            {
                outputBox.Text += "Message verify successfully\r\n";
            }
            else outputBox.Text += "Failed to sign message\r\n";
        }
    }
}
