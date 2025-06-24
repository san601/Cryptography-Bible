namespace GUI_Lab5
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
            this.tab = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.parameterBox = new System.Windows.Forms.TextBox();
            this.parameterLabel = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.keygenFormatBox = new System.Windows.Forms.ComboBox();
            this.keygenPrivkeyBox = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.keygenPubkeyBox = new System.Windows.Forms.TextBox();
            this.label12 = new System.Windows.Forms.Label();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.keygenAlgoBox = new System.Windows.Forms.ComboBox();
            this.generateBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label16 = new System.Windows.Forms.Label();
            this.signFormatBox = new System.Windows.Forms.ComboBox();
            this.richTextBox2 = new System.Windows.Forms.RichTextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.signAlgoBox = new System.Windows.Forms.ComboBox();
            this.signPrivkeyBox = new System.Windows.Forms.TextBox();
            this.signPrivkeyBtn = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.signFilePathBox = new System.Windows.Forms.TextBox();
            this.signBtn = new System.Windows.Forms.Button();
            this.signTextBox = new System.Windows.Forms.TextBox();
            this.textFileBtn = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.label17 = new System.Windows.Forms.Label();
            this.verifyFormatBox = new System.Windows.Forms.ComboBox();
            this.button8 = new System.Windows.Forms.Button();
            this.label15 = new System.Windows.Forms.Label();
            this.verifyAlgoBox = new System.Windows.Forms.ComboBox();
            this.outputBox = new System.Windows.Forms.RichTextBox();
            this.verifySignatureBox = new System.Windows.Forms.TextBox();
            this.verifySigBtn = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.verifyPubkeyBox = new System.Windows.Forms.TextBox();
            this.verifyPubkeyBtn = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.verifyFilePathBox = new System.Windows.Forms.TextBox();
            this.verifyFileBtn = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.signSignatureBox = new System.Windows.Forms.TextBox();
            this.signSignatureBtn = new System.Windows.Forms.Button();
            this.label18 = new System.Windows.Forms.Label();
            this.tab.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tab
            // 
            this.tab.Controls.Add(this.tabPage1);
            this.tab.Controls.Add(this.tabPage2);
            this.tab.Controls.Add(this.tabPage3);
            this.tab.Location = new System.Drawing.Point(0, 0);
            this.tab.Name = "tab";
            this.tab.SelectedIndex = 0;
            this.tab.Size = new System.Drawing.Size(553, 451);
            this.tab.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.parameterBox);
            this.tabPage1.Controls.Add(this.parameterLabel);
            this.tabPage1.Controls.Add(this.label14);
            this.tabPage1.Controls.Add(this.keygenFormatBox);
            this.tabPage1.Controls.Add(this.keygenPrivkeyBox);
            this.tabPage1.Controls.Add(this.label13);
            this.tabPage1.Controls.Add(this.keygenPubkeyBox);
            this.tabPage1.Controls.Add(this.label12);
            this.tabPage1.Controls.Add(this.richTextBox1);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.keygenAlgoBox);
            this.tabPage1.Controls.Add(this.generateBtn);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(545, 422);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Key Generation";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // parameterBox
            // 
            this.parameterBox.Location = new System.Drawing.Point(152, 112);
            this.parameterBox.Name = "parameterBox";
            this.parameterBox.Size = new System.Drawing.Size(165, 22);
            this.parameterBox.TabIndex = 24;
            // 
            // parameterLabel
            // 
            this.parameterLabel.AutoSize = true;
            this.parameterLabel.Location = new System.Drawing.Point(25, 115);
            this.parameterLabel.Name = "parameterLabel";
            this.parameterLabel.Size = new System.Drawing.Size(79, 16);
            this.parameterLabel.TabIndex = 23;
            this.parameterLabel.Text = "Curve name";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(25, 201);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(49, 16);
            this.label14.TabIndex = 22;
            this.label14.Text = "Format";
            // 
            // keygenFormatBox
            // 
            this.keygenFormatBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.keygenFormatBox.FormattingEnabled = true;
            this.keygenFormatBox.Items.AddRange(new object[] {
            "der",
            "pem"});
            this.keygenFormatBox.Location = new System.Drawing.Point(152, 198);
            this.keygenFormatBox.Name = "keygenFormatBox";
            this.keygenFormatBox.Size = new System.Drawing.Size(165, 24);
            this.keygenFormatBox.TabIndex = 21;
            // 
            // keygenPrivkeyBox
            // 
            this.keygenPrivkeyBox.Location = new System.Drawing.Point(152, 170);
            this.keygenPrivkeyBox.Name = "keygenPrivkeyBox";
            this.keygenPrivkeyBox.Size = new System.Drawing.Size(339, 22);
            this.keygenPrivkeyBox.TabIndex = 20;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(25, 173);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(111, 16);
            this.label13.TabIndex = 19;
            this.label13.Text = "Private key name";
            // 
            // keygenPubkeyBox
            // 
            this.keygenPubkeyBox.Location = new System.Drawing.Point(152, 142);
            this.keygenPubkeyBox.Name = "keygenPubkeyBox";
            this.keygenPubkeyBox.Size = new System.Drawing.Size(339, 22);
            this.keygenPubkeyBox.TabIndex = 18;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(25, 145);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(106, 16);
            this.label12.TabIndex = 17;
            this.label12.Text = "Public key name";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(34, 279);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(457, 123);
            this.richTextBox1.TabIndex = 9;
            this.richTextBox1.Text = "";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(25, 85);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(63, 16);
            this.label2.TabIndex = 8;
            this.label2.Text = "Algorithm";
            // 
            // keygenAlgoBox
            // 
            this.keygenAlgoBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.keygenAlgoBox.FormattingEnabled = true;
            this.keygenAlgoBox.Items.AddRange(new object[] {
            "ECDSA",
            "RSAPSS"});
            this.keygenAlgoBox.Location = new System.Drawing.Point(152, 82);
            this.keygenAlgoBox.Name = "keygenAlgoBox";
            this.keygenAlgoBox.Size = new System.Drawing.Size(165, 24);
            this.keygenAlgoBox.TabIndex = 7;
            this.keygenAlgoBox.SelectedIndexChanged += new System.EventHandler(this.keygenAlgoBox_SelectedIndexChanged);
            // 
            // generateBtn
            // 
            this.generateBtn.Location = new System.Drawing.Point(187, 239);
            this.generateBtn.Name = "generateBtn";
            this.generateBtn.Size = new System.Drawing.Size(130, 23);
            this.generateBtn.TabIndex = 5;
            this.generateBtn.Text = "Generate";
            this.generateBtn.UseVisualStyleBackColor = true;
            this.generateBtn.Click += new System.EventHandler(this.generateBtn_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(146, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(226, 32);
            this.label1.TabIndex = 6;
            this.label1.Text = "Key Generation";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.signSignatureBox);
            this.tabPage2.Controls.Add(this.signSignatureBtn);
            this.tabPage2.Controls.Add(this.label18);
            this.tabPage2.Controls.Add(this.label16);
            this.tabPage2.Controls.Add(this.signFormatBox);
            this.tabPage2.Controls.Add(this.richTextBox2);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.signAlgoBox);
            this.tabPage2.Controls.Add(this.signPrivkeyBox);
            this.tabPage2.Controls.Add(this.signPrivkeyBtn);
            this.tabPage2.Controls.Add(this.label11);
            this.tabPage2.Controls.Add(this.signFilePathBox);
            this.tabPage2.Controls.Add(this.signBtn);
            this.tabPage2.Controls.Add(this.signTextBox);
            this.tabPage2.Controls.Add(this.textFileBtn);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Controls.Add(this.label5);
            this.tabPage2.Controls.Add(this.label3);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(545, 422);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Sign";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(19, 208);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(49, 16);
            this.label16.TabIndex = 24;
            this.label16.Text = "Format";
            // 
            // signFormatBox
            // 
            this.signFormatBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.signFormatBox.FormattingEnabled = true;
            this.signFormatBox.Items.AddRange(new object[] {
            "der",
            "pem"});
            this.signFormatBox.Location = new System.Drawing.Point(146, 205);
            this.signFormatBox.Name = "signFormatBox";
            this.signFormatBox.Size = new System.Drawing.Size(165, 24);
            this.signFormatBox.TabIndex = 23;
            // 
            // richTextBox2
            // 
            this.richTextBox2.Location = new System.Drawing.Point(21, 307);
            this.richTextBox2.Name = "richTextBox2";
            this.richTextBox2.Size = new System.Drawing.Size(503, 96);
            this.richTextBox2.TabIndex = 19;
            this.richTextBox2.Text = "";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(19, 66);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(63, 16);
            this.label7.TabIndex = 18;
            this.label7.Text = "Algorithm";
            // 
            // signAlgoBox
            // 
            this.signAlgoBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.signAlgoBox.FormattingEnabled = true;
            this.signAlgoBox.Items.AddRange(new object[] {
            "ECDSA",
            "RSAPSS"});
            this.signAlgoBox.Location = new System.Drawing.Point(146, 63);
            this.signAlgoBox.Name = "signAlgoBox";
            this.signAlgoBox.Size = new System.Drawing.Size(165, 24);
            this.signAlgoBox.TabIndex = 17;
            // 
            // signPrivkeyBox
            // 
            this.signPrivkeyBox.Location = new System.Drawing.Point(146, 149);
            this.signPrivkeyBox.Name = "signPrivkeyBox";
            this.signPrivkeyBox.Size = new System.Drawing.Size(250, 22);
            this.signPrivkeyBox.TabIndex = 16;
            // 
            // signPrivkeyBtn
            // 
            this.signPrivkeyBtn.Location = new System.Drawing.Point(402, 149);
            this.signPrivkeyBtn.Name = "signPrivkeyBtn";
            this.signPrivkeyBtn.Size = new System.Drawing.Size(123, 23);
            this.signPrivkeyBtn.TabIndex = 15;
            this.signPrivkeyBtn.Text = "Choose file";
            this.signPrivkeyBtn.UseVisualStyleBackColor = true;
            this.signPrivkeyBtn.Click += new System.EventHandler(this.signPrivkeyBtn_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(19, 152);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(74, 16);
            this.label11.TabIndex = 14;
            this.label11.Text = "Private key";
            // 
            // signFilePathBox
            // 
            this.signFilePathBox.Location = new System.Drawing.Point(146, 121);
            this.signFilePathBox.Name = "signFilePathBox";
            this.signFilePathBox.Size = new System.Drawing.Size(250, 22);
            this.signFilePathBox.TabIndex = 13;
            // 
            // signBtn
            // 
            this.signBtn.Location = new System.Drawing.Point(151, 252);
            this.signBtn.Name = "signBtn";
            this.signBtn.Size = new System.Drawing.Size(160, 29);
            this.signBtn.TabIndex = 12;
            this.signBtn.Text = "Sign";
            this.signBtn.UseVisualStyleBackColor = true;
            this.signBtn.Click += new System.EventHandler(this.signBtn_Click);
            // 
            // signTextBox
            // 
            this.signTextBox.Location = new System.Drawing.Point(146, 93);
            this.signTextBox.Name = "signTextBox";
            this.signTextBox.Size = new System.Drawing.Size(379, 22);
            this.signTextBox.TabIndex = 11;
            this.signTextBox.TextChanged += new System.EventHandler(this.signTextBox_TextChanged);
            // 
            // textFileBtn
            // 
            this.textFileBtn.Location = new System.Drawing.Point(402, 121);
            this.textFileBtn.Name = "textFileBtn";
            this.textFileBtn.Size = new System.Drawing.Size(123, 23);
            this.textFileBtn.TabIndex = 10;
            this.textFileBtn.Text = "Choose file";
            this.textFileBtn.UseVisualStyleBackColor = true;
            this.textFileBtn.Click += new System.EventHandler(this.textFileBtn_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(19, 124);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(70, 16);
            this.label6.TabIndex = 9;
            this.label6.Text = "Or from file";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(19, 96);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 16);
            this.label5.TabIndex = 8;
            this.label5.Text = "Text to be signed";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(197, 18);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(76, 32);
            this.label3.TabIndex = 7;
            this.label3.Text = "Sign";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.label17);
            this.tabPage3.Controls.Add(this.verifyFormatBox);
            this.tabPage3.Controls.Add(this.button8);
            this.tabPage3.Controls.Add(this.label15);
            this.tabPage3.Controls.Add(this.verifyAlgoBox);
            this.tabPage3.Controls.Add(this.outputBox);
            this.tabPage3.Controls.Add(this.verifySignatureBox);
            this.tabPage3.Controls.Add(this.verifySigBtn);
            this.tabPage3.Controls.Add(this.label10);
            this.tabPage3.Controls.Add(this.verifyPubkeyBox);
            this.tabPage3.Controls.Add(this.verifyPubkeyBtn);
            this.tabPage3.Controls.Add(this.label9);
            this.tabPage3.Controls.Add(this.verifyFilePathBox);
            this.tabPage3.Controls.Add(this.verifyFileBtn);
            this.tabPage3.Controls.Add(this.label8);
            this.tabPage3.Controls.Add(this.label4);
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(545, 422);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Verify";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(21, 202);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(49, 16);
            this.label17.TabIndex = 26;
            this.label17.Text = "Format";
            // 
            // verifyFormatBox
            // 
            this.verifyFormatBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.verifyFormatBox.FormattingEnabled = true;
            this.verifyFormatBox.Items.AddRange(new object[] {
            "der",
            "pem"});
            this.verifyFormatBox.Location = new System.Drawing.Point(116, 199);
            this.verifyFormatBox.Name = "verifyFormatBox";
            this.verifyFormatBox.Size = new System.Drawing.Size(165, 24);
            this.verifyFormatBox.TabIndex = 25;
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(171, 250);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(160, 29);
            this.button8.TabIndex = 23;
            this.button8.Text = "Verify";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(21, 88);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(63, 16);
            this.label15.TabIndex = 22;
            this.label15.Text = "Algorithm";
            // 
            // verifyAlgoBox
            // 
            this.verifyAlgoBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.verifyAlgoBox.FormattingEnabled = true;
            this.verifyAlgoBox.Items.AddRange(new object[] {
            "ECDSA",
            "RSAPSS"});
            this.verifyAlgoBox.Location = new System.Drawing.Point(116, 85);
            this.verifyAlgoBox.Name = "verifyAlgoBox";
            this.verifyAlgoBox.Size = new System.Drawing.Size(165, 24);
            this.verifyAlgoBox.TabIndex = 21;
            // 
            // outputBox
            // 
            this.outputBox.Location = new System.Drawing.Point(23, 309);
            this.outputBox.Name = "outputBox";
            this.outputBox.Size = new System.Drawing.Size(499, 104);
            this.outputBox.TabIndex = 20;
            this.outputBox.Text = "";
            // 
            // verifySignatureBox
            // 
            this.verifySignatureBox.Location = new System.Drawing.Point(116, 171);
            this.verifySignatureBox.Name = "verifySignatureBox";
            this.verifySignatureBox.Size = new System.Drawing.Size(282, 22);
            this.verifySignatureBox.TabIndex = 19;
            // 
            // verifySigBtn
            // 
            this.verifySigBtn.Location = new System.Drawing.Point(404, 171);
            this.verifySigBtn.Name = "verifySigBtn";
            this.verifySigBtn.Size = new System.Drawing.Size(118, 23);
            this.verifySigBtn.TabIndex = 18;
            this.verifySigBtn.Text = "Choose file";
            this.verifySigBtn.UseVisualStyleBackColor = true;
            this.verifySigBtn.Click += new System.EventHandler(this.verifySigBtn_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(20, 174);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(84, 16);
            this.label10.TabIndex = 17;
            this.label10.Text = "Signature file";
            // 
            // verifyPubkeyBox
            // 
            this.verifyPubkeyBox.Location = new System.Drawing.Point(116, 143);
            this.verifyPubkeyBox.Name = "verifyPubkeyBox";
            this.verifyPubkeyBox.Size = new System.Drawing.Size(282, 22);
            this.verifyPubkeyBox.TabIndex = 16;
            // 
            // verifyPubkeyBtn
            // 
            this.verifyPubkeyBtn.Location = new System.Drawing.Point(404, 143);
            this.verifyPubkeyBtn.Name = "verifyPubkeyBtn";
            this.verifyPubkeyBtn.Size = new System.Drawing.Size(118, 23);
            this.verifyPubkeyBtn.TabIndex = 15;
            this.verifyPubkeyBtn.Text = "Choose file";
            this.verifyPubkeyBtn.UseVisualStyleBackColor = true;
            this.verifyPubkeyBtn.Click += new System.EventHandler(this.verifyPubkeyBtn_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(20, 146);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(89, 16);
            this.label9.TabIndex = 14;
            this.label9.Text = "Public key file";
            // 
            // verifyFilePathBox
            // 
            this.verifyFilePathBox.Location = new System.Drawing.Point(116, 115);
            this.verifyFilePathBox.Name = "verifyFilePathBox";
            this.verifyFilePathBox.Size = new System.Drawing.Size(282, 22);
            this.verifyFilePathBox.TabIndex = 13;
            // 
            // verifyFileBtn
            // 
            this.verifyFileBtn.Location = new System.Drawing.Point(404, 115);
            this.verifyFileBtn.Name = "verifyFileBtn";
            this.verifyFileBtn.Size = new System.Drawing.Size(118, 23);
            this.verifyFileBtn.TabIndex = 12;
            this.verifyFileBtn.Text = "Choose file";
            this.verifyFileBtn.UseVisualStyleBackColor = true;
            this.verifyFileBtn.Click += new System.EventHandler(this.verifyFileBtn_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(20, 118);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(61, 16);
            this.label8.TabIndex = 11;
            this.label8.Text = "Verify file";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(208, 27);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(93, 32);
            this.label4.TabIndex = 7;
            this.label4.Text = "Verify";
            // 
            // signSignatureBox
            // 
            this.signSignatureBox.Location = new System.Drawing.Point(146, 177);
            this.signSignatureBox.Name = "signSignatureBox";
            this.signSignatureBox.Size = new System.Drawing.Size(250, 22);
            this.signSignatureBox.TabIndex = 27;
            // 
            // signSignatureBtn
            // 
            this.signSignatureBtn.Location = new System.Drawing.Point(402, 177);
            this.signSignatureBtn.Name = "signSignatureBtn";
            this.signSignatureBtn.Size = new System.Drawing.Size(123, 23);
            this.signSignatureBtn.TabIndex = 26;
            this.signSignatureBtn.Text = "Choose file";
            this.signSignatureBtn.UseVisualStyleBackColor = true;
            this.signSignatureBtn.Click += new System.EventHandler(this.signSignatureBtn_Click);
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(19, 180);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(84, 16);
            this.label18.TabIndex = 25;
            this.label18.Text = "Signature file";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(562, 450);
            this.Controls.Add(this.tab);
            this.Name = "Form1";
            this.Text = "Lab 5";
            this.tab.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tab;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox keygenAlgoBox;
        private System.Windows.Forms.Button generateBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button signBtn;
        private System.Windows.Forms.TextBox signTextBox;
        private System.Windows.Forms.Button textFileBtn;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox verifySignatureBox;
        private System.Windows.Forms.Button verifySigBtn;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox verifyPubkeyBox;
        private System.Windows.Forms.Button verifyPubkeyBtn;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox verifyFilePathBox;
        private System.Windows.Forms.Button verifyFileBtn;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.RichTextBox outputBox;
        private System.Windows.Forms.TextBox signPrivkeyBox;
        private System.Windows.Forms.Button signPrivkeyBtn;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox keygenPubkeyBox;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.ComboBox keygenFormatBox;
        private System.Windows.Forms.TextBox keygenPrivkeyBox;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox signAlgoBox;
        private System.Windows.Forms.RichTextBox richTextBox2;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.ComboBox verifyAlgoBox;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.ComboBox signFormatBox;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.ComboBox verifyFormatBox;
        private System.Windows.Forms.TextBox parameterBox;
        private System.Windows.Forms.Label parameterLabel;
        private System.Windows.Forms.TextBox signFilePathBox;
        private System.Windows.Forms.TextBox signSignatureBox;
        private System.Windows.Forms.Button signSignatureBtn;
        private System.Windows.Forms.Label label18;
    }
}

