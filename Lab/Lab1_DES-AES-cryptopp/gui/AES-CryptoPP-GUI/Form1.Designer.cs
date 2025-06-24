namespace AES_CryptoPP_GUI
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.decryptFormatBox = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.decryptDestBox = new System.Windows.Forms.ComboBox();
            this.label13 = new System.Windows.Forms.Label();
            this.richTextBox2 = new System.Windows.Forms.RichTextBox();
            this.decryptCiphertextBox = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.decryptBtn = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.decryptFileBtn = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.encryptFormatBox = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.encryptAlgoBox = new System.Windows.Forms.ComboBox();
            this.encryptOutputBox = new System.Windows.Forms.RichTextBox();
            this.encryptPlaintextBox = new System.Windows.Forms.TextBox();
            this.encryptDestBox = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.encryptBtn = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.encryptFileBtn = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.encryptModeBox = new System.Windows.Forms.ComboBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.keygenChooseFileBtn = new System.Windows.Forms.Button();
            this.keygenFilePath = new System.Windows.Forms.TextBox();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.keygenAlgoBox = new System.Windows.Forms.ComboBox();
            this.generateBtn = new System.Windows.Forms.Button();
            this.keygenModeBox = new System.Windows.Forms.ComboBox();
            this.label20 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.keygenOutputBox = new System.Windows.Forms.RichTextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.decryptAlgoBox = new System.Windows.Forms.ComboBox();
            this.decryptModeBox = new System.Windows.Forms.ComboBox();
            this.label22 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.tabPage2.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(274, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(138, 29);
            this.label1.TabIndex = 0;
            this.label1.Text = "AES - DES";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label22);
            this.tabPage2.Controls.Add(this.label18);
            this.tabPage2.Controls.Add(this.label19);
            this.tabPage2.Controls.Add(this.decryptAlgoBox);
            this.tabPage2.Controls.Add(this.decryptModeBox);
            this.tabPage2.Controls.Add(this.decryptFormatBox);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Controls.Add(this.label12);
            this.tabPage2.Controls.Add(this.decryptDestBox);
            this.tabPage2.Controls.Add(this.label13);
            this.tabPage2.Controls.Add(this.richTextBox2);
            this.tabPage2.Controls.Add(this.decryptCiphertextBox);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.decryptBtn);
            this.tabPage2.Controls.Add(this.label8);
            this.tabPage2.Controls.Add(this.decryptFileBtn);
            this.tabPage2.Controls.Add(this.label9);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(731, 594);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Decryption";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // decryptFormatBox
            // 
            this.decryptFormatBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.decryptFormatBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptFormatBox.FormattingEnabled = true;
            this.decryptFormatBox.Items.AddRange(new object[] {
            "UTF-8",
            "Hex",
            "Binary"});
            this.decryptFormatBox.Location = new System.Drawing.Point(569, 178);
            this.decryptFormatBox.Name = "decryptFormatBox";
            this.decryptFormatBox.Size = new System.Drawing.Size(146, 28);
            this.decryptFormatBox.TabIndex = 27;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(418, 181);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(62, 20);
            this.label6.TabIndex = 26;
            this.label6.Text = "Format";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(418, 139);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(128, 20);
            this.label12.TabIndex = 25;
            this.label12.Text = "Where to output";
            // 
            // decryptDestBox
            // 
            this.decryptDestBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.decryptDestBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptDestBox.FormattingEnabled = true;
            this.decryptDestBox.Items.AddRange(new object[] {
            "Console",
            "File"});
            this.decryptDestBox.Location = new System.Drawing.Point(569, 135);
            this.decryptDestBox.Name = "decryptDestBox";
            this.decryptDestBox.Size = new System.Drawing.Size(146, 28);
            this.decryptDestBox.TabIndex = 24;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(542, 100);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(59, 20);
            this.label13.TabIndex = 23;
            this.label13.Text = "Output";
            // 
            // richTextBox2
            // 
            this.richTextBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.richTextBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.richTextBox2.Location = new System.Drawing.Point(21, 272);
            this.richTextBox2.Name = "richTextBox2";
            this.richTextBox2.Size = new System.Drawing.Size(678, 307);
            this.richTextBox2.TabIndex = 21;
            this.richTextBox2.Text = "";
            // 
            // decryptCiphertextBox
            // 
            this.decryptCiphertextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.decryptCiphertextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptCiphertextBox.Location = new System.Drawing.Point(160, 136);
            this.decryptCiphertextBox.Name = "decryptCiphertextBox";
            this.decryptCiphertextBox.Size = new System.Drawing.Size(217, 27);
            this.decryptCiphertextBox.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(133, 100);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(45, 20);
            this.label7.TabIndex = 18;
            this.label7.Text = "Input";
            // 
            // decryptBtn
            // 
            this.decryptBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptBtn.Location = new System.Drawing.Point(257, 217);
            this.decryptBtn.Name = "decryptBtn";
            this.decryptBtn.Size = new System.Drawing.Size(130, 33);
            this.decryptBtn.TabIndex = 17;
            this.decryptBtn.Text = "Decrypt";
            this.decryptBtn.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(13, 181);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(94, 20);
            this.label8.TabIndex = 16;
            this.label8.Text = "Or from file";
            // 
            // decryptFileBtn
            // 
            this.decryptFileBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptFileBtn.Location = new System.Drawing.Point(160, 178);
            this.decryptFileBtn.Name = "decryptFileBtn";
            this.decryptFileBtn.Size = new System.Drawing.Size(109, 27);
            this.decryptFileBtn.TabIndex = 15;
            this.decryptFileBtn.Text = "Choose file";
            this.decryptFileBtn.UseVisualStyleBackColor = true;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(13, 139);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(128, 20);
            this.label9.TabIndex = 14;
            this.label9.Text = "Ciphertext (hex)";
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label23);
            this.tabPage1.Controls.Add(this.label20);
            this.tabPage1.Controls.Add(this.label21);
            this.tabPage1.Controls.Add(this.encryptFormatBox);
            this.tabPage1.Controls.Add(this.label11);
            this.tabPage1.Controls.Add(this.label10);
            this.tabPage1.Controls.Add(this.encryptAlgoBox);
            this.tabPage1.Controls.Add(this.encryptOutputBox);
            this.tabPage1.Controls.Add(this.encryptPlaintextBox);
            this.tabPage1.Controls.Add(this.encryptDestBox);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.encryptBtn);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.encryptFileBtn);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.encryptModeBox);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(731, 594);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Encryption";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // encryptFormatBox
            // 
            this.encryptFormatBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.encryptFormatBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptFormatBox.FormattingEnabled = true;
            this.encryptFormatBox.Items.AddRange(new object[] {
            "Hex",
            "Base64"});
            this.encryptFormatBox.Location = new System.Drawing.Point(551, 169);
            this.encryptFormatBox.Name = "encryptFormatBox";
            this.encryptFormatBox.Size = new System.Drawing.Size(145, 28);
            this.encryptFormatBox.TabIndex = 14;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(391, 172);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(62, 20);
            this.label11.TabIndex = 13;
            this.label11.Text = "Format";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(391, 130);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(128, 20);
            this.label10.TabIndex = 12;
            this.label10.Text = "Where to output";
            // 
            // encryptAlgoBox
            // 
            this.encryptAlgoBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.encryptAlgoBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptAlgoBox.FormattingEnabled = true;
            this.encryptAlgoBox.Items.AddRange(new object[] {
            "AES",
            "DES"});
            this.encryptAlgoBox.Location = new System.Drawing.Point(212, 51);
            this.encryptAlgoBox.Name = "encryptAlgoBox";
            this.encryptAlgoBox.Size = new System.Drawing.Size(160, 28);
            this.encryptAlgoBox.TabIndex = 11;
            // 
            // encryptOutputBox
            // 
            this.encryptOutputBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.encryptOutputBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptOutputBox.Location = new System.Drawing.Point(20, 257);
            this.encryptOutputBox.Name = "encryptOutputBox";
            this.encryptOutputBox.Size = new System.Drawing.Size(678, 315);
            this.encryptOutputBox.TabIndex = 10;
            this.encryptOutputBox.Text = "";
            // 
            // encryptPlaintextBox
            // 
            this.encryptPlaintextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.encryptPlaintextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptPlaintextBox.Location = new System.Drawing.Point(128, 128);
            this.encryptPlaintextBox.Name = "encryptPlaintextBox";
            this.encryptPlaintextBox.Size = new System.Drawing.Size(243, 27);
            this.encryptPlaintextBox.TabIndex = 2;
            // 
            // encryptDestBox
            // 
            this.encryptDestBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.encryptDestBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptDestBox.FormattingEnabled = true;
            this.encryptDestBox.Items.AddRange(new object[] {
            "Console",
            "File"});
            this.encryptDestBox.Location = new System.Drawing.Point(551, 126);
            this.encryptDestBox.Name = "encryptDestBox";
            this.encryptDestBox.Size = new System.Drawing.Size(145, 28);
            this.encryptDestBox.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(510, 91);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 20);
            this.label5.TabIndex = 8;
            this.label5.Text = "Output";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(148, 92);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(45, 20);
            this.label4.TabIndex = 7;
            this.label4.Text = "Input";
            // 
            // encryptBtn
            // 
            this.encryptBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptBtn.Location = new System.Drawing.Point(272, 204);
            this.encryptBtn.Name = "encryptBtn";
            this.encryptBtn.Size = new System.Drawing.Size(109, 33);
            this.encryptBtn.TabIndex = 6;
            this.encryptBtn.Text = "Encrypt";
            this.encryptBtn.UseVisualStyleBackColor = true;
            this.encryptBtn.Click += new System.EventHandler(this.encryptBtn_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(28, 173);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(94, 20);
            this.label3.TabIndex = 5;
            this.label3.Text = "Or from file";
            // 
            // encryptFileBtn
            // 
            this.encryptFileBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptFileBtn.Location = new System.Drawing.Point(128, 170);
            this.encryptFileBtn.Name = "encryptFileBtn";
            this.encryptFileBtn.Size = new System.Drawing.Size(109, 27);
            this.encryptFileBtn.TabIndex = 4;
            this.encryptFileBtn.Text = "Choose file";
            this.encryptFileBtn.UseVisualStyleBackColor = true;
            this.encryptFileBtn.Click += new System.EventHandler(this.encryptFileBtn_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(28, 131);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(73, 20);
            this.label2.TabIndex = 3;
            this.label2.Text = "Plaintext";
            // 
            // encryptModeBox
            // 
            this.encryptModeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.encryptModeBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.encryptModeBox.FormattingEnabled = true;
            this.encryptModeBox.Items.AddRange(new object[] {
            "ECB",
            "CBC",
            "OFB",
            "CFB",
            "CTR",
            "XTS",
            "CCM",
            "GCM"});
            this.encryptModeBox.Location = new System.Drawing.Point(449, 51);
            this.encryptModeBox.Name = "encryptModeBox";
            this.encryptModeBox.Size = new System.Drawing.Size(147, 28);
            this.encryptModeBox.TabIndex = 1;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(3, 40);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(739, 623);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.keygenChooseFileBtn);
            this.tabPage3.Controls.Add(this.keygenFilePath);
            this.tabPage3.Controls.Add(this.label17);
            this.tabPage3.Controls.Add(this.label16);
            this.tabPage3.Controls.Add(this.label15);
            this.tabPage3.Controls.Add(this.keygenOutputBox);
            this.tabPage3.Controls.Add(this.label14);
            this.tabPage3.Controls.Add(this.keygenAlgoBox);
            this.tabPage3.Controls.Add(this.generateBtn);
            this.tabPage3.Controls.Add(this.keygenModeBox);
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(731, 594);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Key generation";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // keygenChooseFileBtn
            // 
            this.keygenChooseFileBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.keygenChooseFileBtn.Location = new System.Drawing.Point(551, 109);
            this.keygenChooseFileBtn.Name = "keygenChooseFileBtn";
            this.keygenChooseFileBtn.Size = new System.Drawing.Size(115, 33);
            this.keygenChooseFileBtn.TabIndex = 31;
            this.keygenChooseFileBtn.Text = "Choose file";
            this.keygenChooseFileBtn.UseVisualStyleBackColor = true;
            this.keygenChooseFileBtn.Click += new System.EventHandler(this.keygenChooseFileBtn_Click);
            // 
            // keygenFilePath
            // 
            this.keygenFilePath.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.keygenFilePath.Location = new System.Drawing.Point(117, 112);
            this.keygenFilePath.Name = "keygenFilePath";
            this.keygenFilePath.Size = new System.Drawing.Size(403, 27);
            this.keygenFilePath.TabIndex = 30;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label17.Location = new System.Drawing.Point(22, 115);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(64, 20);
            this.label17.TabIndex = 29;
            this.label17.Text = "Key file";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label16.Location = new System.Drawing.Point(317, 74);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(50, 20);
            this.label16.TabIndex = 28;
            this.label16.Text = "Mode";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(22, 74);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(80, 20);
            this.label15.TabIndex = 27;
            this.label15.Text = "Algorithm";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(236, 23);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(205, 25);
            this.label14.TabIndex = 5;
            this.label14.Text = "Generate key and iv";
            // 
            // keygenAlgoBox
            // 
            this.keygenAlgoBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.keygenAlgoBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.keygenAlgoBox.FormattingEnabled = true;
            this.keygenAlgoBox.Items.AddRange(new object[] {
            "AES",
            "DES"});
            this.keygenAlgoBox.Location = new System.Drawing.Point(117, 71);
            this.keygenAlgoBox.Name = "keygenAlgoBox";
            this.keygenAlgoBox.Size = new System.Drawing.Size(174, 28);
            this.keygenAlgoBox.TabIndex = 25;
            // 
            // generateBtn
            // 
            this.generateBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.generateBtn.Location = new System.Drawing.Point(272, 176);
            this.generateBtn.Name = "generateBtn";
            this.generateBtn.Size = new System.Drawing.Size(115, 33);
            this.generateBtn.TabIndex = 24;
            this.generateBtn.Text = "Generate";
            this.generateBtn.UseVisualStyleBackColor = true;
            this.generateBtn.Click += new System.EventHandler(this.generateBtn_Click);
            // 
            // keygenModeBox
            // 
            this.keygenModeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.keygenModeBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.keygenModeBox.FormattingEnabled = true;
            this.keygenModeBox.Items.AddRange(new object[] {
            "ECB",
            "CBC",
            "OFB",
            "CFB",
            "CTR",
            "XTS",
            "CCM",
            "GCM"});
            this.keygenModeBox.Location = new System.Drawing.Point(373, 71);
            this.keygenModeBox.Name = "keygenModeBox";
            this.keygenModeBox.Size = new System.Drawing.Size(147, 28);
            this.keygenModeBox.TabIndex = 23;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label20.Location = new System.Drawing.Point(393, 54);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(50, 20);
            this.label20.TabIndex = 32;
            this.label20.Text = "Mode";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label21.Location = new System.Drawing.Point(112, 54);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(80, 20);
            this.label21.TabIndex = 31;
            this.label21.Text = "Algorithm";
            // 
            // keygenOutputBox
            // 
            this.keygenOutputBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.keygenOutputBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.keygenOutputBox.Location = new System.Drawing.Point(17, 231);
            this.keygenOutputBox.Name = "keygenOutputBox";
            this.keygenOutputBox.Size = new System.Drawing.Size(678, 348);
            this.keygenOutputBox.TabIndex = 26;
            this.keygenOutputBox.Text = "";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label18.Location = new System.Drawing.Point(424, 67);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(50, 20);
            this.label18.TabIndex = 34;
            this.label18.Text = "Mode";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label19.Location = new System.Drawing.Point(117, 67);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(80, 20);
            this.label19.TabIndex = 33;
            this.label19.Text = "Algorithm";
            // 
            // decryptAlgoBox
            // 
            this.decryptAlgoBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.decryptAlgoBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptAlgoBox.FormattingEnabled = true;
            this.decryptAlgoBox.Items.AddRange(new object[] {
            "AES",
            "DES"});
            this.decryptAlgoBox.Location = new System.Drawing.Point(203, 64);
            this.decryptAlgoBox.Name = "decryptAlgoBox";
            this.decryptAlgoBox.Size = new System.Drawing.Size(174, 28);
            this.decryptAlgoBox.TabIndex = 32;
            // 
            // decryptModeBox
            // 
            this.decryptModeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.decryptModeBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.decryptModeBox.FormattingEnabled = true;
            this.decryptModeBox.Items.AddRange(new object[] {
            "ECB",
            "CBC",
            "OFB",
            "CFB",
            "CTR",
            "XTS",
            "CCM",
            "GCM"});
            this.decryptModeBox.Location = new System.Drawing.Point(480, 64);
            this.decryptModeBox.Name = "decryptModeBox";
            this.decryptModeBox.Size = new System.Drawing.Size(147, 28);
            this.decryptModeBox.TabIndex = 31;
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label22.Location = new System.Drawing.Point(272, 19);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(115, 25);
            this.label22.TabIndex = 35;
            this.label22.Text = "Decryption";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label23.Location = new System.Drawing.Point(277, 13);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(114, 25);
            this.label23.TabIndex = 36;
            this.label23.Text = "Encryption";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(748, 666);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.ComboBox decryptFormatBox;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox decryptDestBox;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.RichTextBox richTextBox2;
        private System.Windows.Forms.TextBox decryptCiphertextBox;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button decryptBtn;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button decryptFileBtn;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.ComboBox encryptFormatBox;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox encryptAlgoBox;
        private System.Windows.Forms.RichTextBox encryptOutputBox;
        private System.Windows.Forms.TextBox encryptPlaintextBox;
        private System.Windows.Forms.ComboBox encryptDestBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button encryptBtn;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button encryptFileBtn;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox encryptModeBox;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox keygenAlgoBox;
        private System.Windows.Forms.Button generateBtn;
        private System.Windows.Forms.ComboBox keygenModeBox;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox keygenFilePath;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Button keygenChooseFileBtn;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.RichTextBox keygenOutputBox;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.ComboBox decryptAlgoBox;
        private System.Windows.Forms.ComboBox decryptModeBox;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label23;
    }
}

