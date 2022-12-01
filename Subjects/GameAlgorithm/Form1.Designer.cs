namespace GameAlgorithm
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.elementType = new System.Windows.Forms.ComboBox();
            this.sortAlgorithm = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Count = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.DataType = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.SortType = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Time = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.label4 = new System.Windows.Forms.Label();
            this.orderType = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(612, 41);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(118, 32);
            this.label2.TabIndex = 1;
            this.label2.Text = "정렬 요소";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(906, 41);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(118, 32);
            this.label3.TabIndex = 2;
            this.label3.Text = "정렬 방식";
            // 
            // elementType
            // 
            this.elementType.FormattingEnabled = true;
            this.elementType.Items.AddRange(new object[] {
            "정수",
            "실수"});
            this.elementType.Location = new System.Drawing.Point(738, 34);
            this.elementType.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.elementType.Name = "elementType";
            this.elementType.Size = new System.Drawing.Size(156, 40);
            this.elementType.TabIndex = 3;
            this.elementType.Text = "정수";
            // 
            // sortAlgorithm
            // 
            this.sortAlgorithm.FormattingEnabled = true;
            this.sortAlgorithm.Items.AddRange(new object[] {
            "삽입 정렬",
            "퀵 정렬"});
            this.sortAlgorithm.Location = new System.Drawing.Point(1032, 34);
            this.sortAlgorithm.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.sortAlgorithm.Name = "sortAlgorithm";
            this.sortAlgorithm.Size = new System.Drawing.Size(156, 40);
            this.sortAlgorithm.TabIndex = 4;
            this.sortAlgorithm.Text = "삽입 정렬";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(228, 141);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(118, 32);
            this.label1.TabIndex = 5;
            this.label1.Text = "요소 개수";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(354, 130);
            this.textBox1.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(498, 39);
            this.textBox1.TabIndex = 6;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(864, 117);
            this.button1.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(328, 70);
            this.button1.TabIndex = 7;
            this.button1.Text = "정렬";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Count,
            this.DataType,
            this.SortType,
            this.Time});
            this.dataGridView1.Location = new System.Drawing.Point(20, 192);
            this.dataGridView1.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowHeadersWidth = 82;
            this.dataGridView1.RowTemplate.Height = 41;
            this.dataGridView1.Size = new System.Drawing.Size(1172, 826);
            this.dataGridView1.TabIndex = 8;
            // 
            // Count
            // 
            this.Count.HeaderText = "요소 개수";
            this.Count.MinimumWidth = 10;
            this.Count.Name = "Count";
            this.Count.ReadOnly = true;
            this.Count.Width = 125;
            // 
            // DataType
            // 
            this.DataType.HeaderText = "정렬 요소";
            this.DataType.MinimumWidth = 10;
            this.DataType.Name = "DataType";
            this.DataType.ReadOnly = true;
            this.DataType.Width = 115;
            // 
            // SortType
            // 
            this.SortType.HeaderText = "정렬 방식";
            this.SortType.MinimumWidth = 10;
            this.SortType.Name = "SortType";
            this.SortType.ReadOnly = true;
            this.SortType.Width = 115;
            // 
            // Time
            // 
            this.Time.HeaderText = "소요 시간";
            this.Time.MinimumWidth = 10;
            this.Time.Name = "Time";
            this.Time.ReadOnly = true;
            this.Time.Width = 140;
            // 
            // label4
            // 
            this.label4.Image = ((System.Drawing.Image)(resources.GetObject("label4.Image")));
            this.label4.Location = new System.Drawing.Point(44, 30);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(154, 149);
            this.label4.TabIndex = 9;
            // 
            // orderType
            // 
            this.orderType.FormattingEnabled = true;
            this.orderType.Items.AddRange(new object[] {
            "오름차순",
            "내림차순"});
            this.orderType.Location = new System.Drawing.Point(448, 33);
            this.orderType.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.orderType.Name = "orderType";
            this.orderType.Size = new System.Drawing.Size(156, 40);
            this.orderType.TabIndex = 11;
            this.orderType.Text = "오름차순";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(378, 36);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(62, 32);
            this.label5.TabIndex = 10;
            this.label5.Text = "방향";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(14F, 32F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1214, 1039);
            this.Controls.Add(this.orderType);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.sortAlgorithm);
            this.Controls.Add(this.elementType);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "정렬 알고리즘";
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private Label label2;
        private Label label3;
        private ComboBox elementType;
        private ComboBox sortAlgorithm;
        private Label label1;
        private TextBox textBox1;
        private Button button1;
        private DataGridView dataGridView1;
        private Label label4;
        private DataGridViewTextBoxColumn Count;
        private DataGridViewTextBoxColumn DataType;
        private DataGridViewTextBoxColumn SortType;
        private DataGridViewTextBoxColumn Time;
        private ComboBox orderType;
        private Label label5;
    }
}