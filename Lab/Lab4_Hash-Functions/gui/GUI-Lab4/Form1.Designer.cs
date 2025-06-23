namespace GUI_Lab4
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.hashBtn = new System.Windows.Forms.Button();
            this.saveBtn = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.inputBox = new System.Windows.Forms.TextBox();
            this.digestLengthBox = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.modeBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.chooseFileBtn = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.outputBox = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.button2 = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(534, 481);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.hashBtn);
            this.tabPage1.Controls.Add(this.saveBtn);
            this.tabPage1.Controls.Add(this.label6);
            this.tabPage1.Controls.Add(this.inputBox);
            this.tabPage1.Controls.Add(this.digestLengthBox);
            this.tabPage1.Controls.Add(this.label5);
            this.tabPage1.Controls.Add(this.label4);
            this.tabPage1.Controls.Add(this.modeBox);
            this.tabPage1.Controls.Add(this.label3);
            this.tabPage1.Controls.Add(this.chooseFileBtn);
            this.tabPage1.Controls.Add(this.label2);
            this.tabPage1.Controls.Add(this.outputBox);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(526, 452);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Hash";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // hashBtn
            // 
            this.hashBtn.Location = new System.Drawing.Point(199, 252);
            this.hashBtn.Name = "hashBtn";
            this.hashBtn.Size = new System.Drawing.Size(119, 35);
            this.hashBtn.TabIndex = 12;
            this.hashBtn.Text = "Hash";
            this.hashBtn.UseVisualStyleBackColor = true;
            this.hashBtn.Click += new System.EventHandler(this.hashBtn_Click);
            // 
            // saveBtn
            // 
            this.saveBtn.Location = new System.Drawing.Point(431, 401);
            this.saveBtn.Name = "saveBtn";
            this.saveBtn.Size = new System.Drawing.Size(75, 23);
            this.saveBtn.TabIndex = 11;
            this.saveBtn.Text = "Save";
            this.saveBtn.UseVisualStyleBackColor = true;
            this.saveBtn.Click += new System.EventHandler(this.saveBtn_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(181, 29);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(152, 32);
            this.label6.TabIndex = 1;
            this.label6.Text = "Hash Tool";
            // 
            // inputBox
            // 
            this.inputBox.Location = new System.Drawing.Point(15, 207);
            this.inputBox.Name = "inputBox";
            this.inputBox.Size = new System.Drawing.Size(344, 22);
            this.inputBox.TabIndex = 10;
            // 
            // digestLengthBox
            // 
            this.digestLengthBox.Location = new System.Drawing.Point(212, 129);
            this.digestLengthBox.Name = "digestLengthBox";
            this.digestLengthBox.Size = new System.Drawing.Size(294, 22);
            this.digestLengthBox.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(215, 99);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(291, 16);
            this.label5.TabIndex = 8;
            this.label5.Text = "Digest output length (for SHAKE128, SHAKE256)";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 99);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(159, 16);
            this.label4.TabIndex = 7;
            this.label4.Text = "Choose hashing functions";
            // 
            // modeBox
            // 
            this.modeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.modeBox.FormattingEnabled = true;
            this.modeBox.Items.AddRange(new object[] {
            "SHA224",
            "SHA256",
            "SHA384",
            "SHA512",
            "SHA3-224",
            "SHA3-256",
            "SHA3-384",
            "SHA3-512",
            "SHAKE128",
            "SHAKE256"});
            this.modeBox.Location = new System.Drawing.Point(15, 127);
            this.modeBox.Name = "modeBox";
            this.modeBox.Size = new System.Drawing.Size(156, 24);
            this.modeBox.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(393, 178);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(70, 16);
            this.label3.TabIndex = 5;
            this.label3.Text = "Or from file";
            // 
            // chooseFileBtn
            // 
            this.chooseFileBtn.Location = new System.Drawing.Point(387, 206);
            this.chooseFileBtn.Name = "chooseFileBtn";
            this.chooseFileBtn.Size = new System.Drawing.Size(119, 23);
            this.chooseFileBtn.TabIndex = 4;
            this.chooseFileBtn.Text = "Choose file";
            this.chooseFileBtn.UseVisualStyleBackColor = true;
            this.chooseFileBtn.Click += new System.EventHandler(this.chooseFileBtn_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 295);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(45, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "Output";
            // 
            // outputBox
            // 
            this.outputBox.Location = new System.Drawing.Point(20, 326);
            this.outputBox.Name = "outputBox";
            this.outputBox.Size = new System.Drawing.Size(390, 98);
            this.outputBox.TabIndex = 2;
            this.outputBox.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 178);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "Input text";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.button3);
            this.tabPage2.Controls.Add(this.label8);
            this.tabPage2.Controls.Add(this.button2);
            this.tabPage2.Controls.Add(this.richTextBox1);
            this.tabPage2.Controls.Add(this.button1);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(526, 452);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Certificate";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(8, 73);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(167, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Choose certificate";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(8, 102);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(512, 309);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(380, 417);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(140, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Save public key";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(135, 29);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(242, 32);
            this.label8.TabIndex = 4;
            this.label8.Text = "X.509 Certificate";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(8, 417);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(138, 23);
            this.button3.TabIndex = 5;
            this.button3.Text = "Check validate";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(537, 483);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.RichTextBox outputBox;
        private System.Windows.Forms.ComboBox modeBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button chooseFileBtn;
        private System.Windows.Forms.TextBox digestLengthBox;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox inputBox;
        private System.Windows.Forms.Button saveBtn;
        private System.Windows.Forms.Button hashBtn;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button3;
    }
}

