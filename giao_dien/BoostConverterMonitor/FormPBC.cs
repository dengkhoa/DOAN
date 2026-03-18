using System;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections.Generic;
using ScottPlot.WinForms;
using WinColor = System.Drawing.Color;
using WinLabel = System.Windows.Forms.Label;
using WinFont = System.Drawing.Font;
using WinFontStyle = System.Drawing.FontStyle;

namespace BoostConverterMonitor
{
    public partial class FormPBC : Form
    {
        // Serial Port
        private SerialPort? serialPort;
        private ComboBox cbPorts = null!;
        private ComboBox cbBaud = null!;
        private Button btnRefresh = null!;
        private Button btnConnect = null!;
        private Button btnDisconnect = null!;
        private Label lblStatus = null!;
        
        // PBC Parameters
        private NumericUpDown numVd = null!;
        private NumericUpDown numA1 = null!;
        private NumericUpDown numA2 = null!;
        private NumericUpDown numA3 = null!;
        private CheckBox chkEnable = null!;
        private Button btnSendOnce = null!;
        private CheckBox chkAutoSend = null!;
        
        // Telemetry Display
        private Label lblVc = null!;
        private Label lblVin = null!;
        private Label lblIL = null!;
        private Label lblDuty = null!;
        
        // Chart
        private FormsPlot formsPlot = null!;
        private List<double> xData = new List<double>();
        private List<double> yData = new List<double>();
        private int chartXCounter = 0;
        
        // Communication
        private System.Windows.Forms.Timer? autoSendTimer;
        private StringBuilder rxBuffer = new StringBuilder();
        
        // Telemetry data
        private float lastVc = 0;
        private float lastVin = 0;
        private float lastIL = 0;
        private float lastDuty = 0;
        
        public FormPBC()
        {
            InitializeUI();
            RefreshPorts();
        }
        
        private void InitializeUI()
        {
            // Form settings
            this.Text = "Boost Converter PBC Monitor - NGUYEN DANG KHOA 21045251";
            this.Width = 1100;
            this.Height = 650;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.BackColor = Color.White;
            
            // Left Panel - Controls
            Panel leftPanel = new Panel
            {
                Left = 10,
                Top = 10,
                Width = 380,
                Height = 570,
                BorderStyle = BorderStyle.FixedSingle
            };
            
            // COM Port selection
            Label lblBaud = new Label { Left = 10, Top = 10, Width = 80, Text = "Baud Rate" };
            cbBaud = new ComboBox 
            { 
                Left = 100, 
                Top = 8, 
                Width = 120, 
                DropDownStyle = ComboBoxStyle.DropDownList 
            };
            cbBaud.Items.AddRange(new object[] { 9600, 19200, 38400, 57600, 115200 });
            cbBaud.SelectedIndex = 0; // 9600
            
            Label lblPort = new Label { Left = 10, Top = 40, Width = 80, Text = "COM Port" };
            cbPorts = new ComboBox 
            { 
                Left = 100, 
                Top = 38, 
                Width = 120, 
                DropDownStyle = ComboBoxStyle.DropDownList 
            };
            
            btnRefresh = new Button { Left = 230, Top = 36, Width = 80, Height = 28, Text = "Refresh" };
            btnConnect = new Button { Left = 100, Top = 70, Width = 100, Height = 30, Text = "Connect" };
            btnDisconnect = new Button { Left = 210, Top = 70, Width = 100, Height = 30, Text = "Disconnect" };
            
            lblStatus = new Label 
            { 
                Left = 10, 
                Top = 110, 
                Width = 360, 
                Height = 25,
                Text = "trạng thái         điện áp",
                BackColor = Color.LightGray,
                TextAlign = ContentAlignment.MiddleLeft,
                Font = new Font("Arial", 10, FontStyle.Bold)
            };
            
            // PBC Parameters
            Label lblVdCmd = new Label { Left = 10, Top = 150, Width = 80, Text = "Vd_cmd [V]:" };
            numVd = new NumericUpDown 
            { 
                Left = 100, 
                Top = 148, 
                Width = 180,
                DecimalPlaces = 3,
                Minimum = 15,
                Maximum = 25,
                Value = 20.000M,
                Increment = 0.1M
            };
            
            Label lblA1 = new Label { Left = 10, Top = 185, Width = 80, Text = "a1:" };
            numA1 = new NumericUpDown 
            { 
                Left = 100, 
                Top = 183, 
                Width = 180,
                DecimalPlaces = 3,
                Minimum = 0,
                Maximum = 100,
                Value = 1.300M,
                Increment = 0.1M
            };
            
            Label lblA2 = new Label { Left = 10, Top = 220, Width = 80, Text = "a2:" };
            numA2 = new NumericUpDown 
            { 
                Left = 100, 
                Top = 218, 
                Width = 180,
                DecimalPlaces = 3,
                Minimum = 0,
                Maximum = 50,
                Value = 21.700M,
                Increment = 0.1M
            };
            
            Label lblA3 = new Label { Left = 10, Top = 255, Width = 80, Text = "a3:" };
            numA3 = new NumericUpDown 
            { 
                Left = 100, 
                Top = 253, 
                Width = 180,
                DecimalPlaces = 3,
                Minimum = 0,
                Maximum = 20,
                Value = 13.000M,
                Increment = 0.1M
            };
            
            chkEnable = new CheckBox { Left = 100, Top = 295, Width = 100, Text = "Enable", Checked = true };
            btnSendOnce = new Button { Left = 100, Top = 325, Width = 180, Height = 35, Text = "Send once" };
            
            // Telemetry section
            Label lblTelemetry = new Label 
            { 
                Left = 10, 
                Top = 375, 
                Width = 360, 
                Text = "---- Telemetry ----",
                Font = new Font("Arial", 10, FontStyle.Bold)
            };
            
            chkAutoSend = new CheckBox { Left = 100, Top = 405, Width = 180, Text = "Auto-send 20 Hz" };
            
            Label lblVcLabel = new Label { Left = 10, Top = 435, Width = 40, Text = "Vc:" };
            lblVc = new Label 
            { 
                Left = 100, 
                Top = 435, 
                Width = 180, 
                Text = "Vc: 20.00 V (Vout)",
                Font = new Font("Arial", 10, FontStyle.Regular)
            };
            
            Label lblVinLabel = new Label { Left = 10, Top = 460, Width = 40, Text = "Vin:" };
            lblVin = new Label 
            { 
                Left = 100, 
                Top = 460, 
                Width = 180, 
                Text = "Vin: 12.00 V",
                Font = new Font("Arial", 10, FontStyle.Regular)
            };
            
            Label lblILLabel = new Label { Left = 10, Top = 485, Width = 40, Text = "iL:" };
            lblIL = new Label 
            { 
                Left = 100, 
                Top = 485, 
                Width = 180, 
                Text = "iL: 9.556 A",
                Font = new Font("Arial", 10, FontStyle.Regular)
            };
            
            lblDuty = new Label 
            { 
                Left = 10, 
                Top = 510, 
                Width = 360, 
                Text = "duty: 0.854",
                Font = new Font("Arial", 10, FontStyle.Regular)
            };
            
            // Add controls to left panel
            leftPanel.Controls.AddRange(new Control[] 
            {
                lblBaud, cbBaud, lblPort, cbPorts, btnRefresh, 
                btnConnect, btnDisconnect, lblStatus,
                lblVdCmd, numVd, lblA1, numA1, lblA2, numA2, lblA3, numA3,
                chkEnable, btnSendOnce, lblTelemetry, chkAutoSend,
                lblVcLabel, lblVc, lblVinLabel, lblVin, lblILLabel, lblIL, lblDuty
            });
            
            this.Controls.Add(leftPanel);
            
            // Right Panel - Chart
            Panel rightPanel = new Panel
            {
                Left = 400,
                Top = 10,
                Width = 670,
                Height = 520,
                BorderStyle = BorderStyle.FixedSingle
            };
            
            formsPlot = new FormsPlot
            {
                Left = 10,
                Top = 10,
                Width = 650,
                Height = 500
            };
            
            // Setup chart
            formsPlot.Plot.Title("HIEN THI MACH TANG AP (BOOST PBC) Vin:12-15V → Vout:20V");
            formsPlot.Plot.XLabel("Thoi gian cot X");
            formsPlot.Plot.YLabel("Dong dien(V)");
            formsPlot.Plot.Axes.SetLimits(35, 60, 14, 21);
            
            // Add horizontal reference line at 20V (blue)
            double[] xRef = new double[51];
            double[] yRef = new double[51];
            for (int i = 0; i <= 50; i++)
            {
                xRef[i] = 10 + i;
                yRef[i] = 20.0;
            }
            var refLine = formsPlot.Plot.Add.Scatter(xRef, yRef);
            refLine.Color = ScottPlot.Color.FromColor(WinColor.Blue);
            refLine.LineWidth = 2;
            refLine.LegendText = "Duong 2: Dien ap dat";
            formsPlot.Plot.ShowLegend();
            formsPlot.Refresh();
            
            rightPanel.Controls.Add(formsPlot);
            this.Controls.Add(rightPanel);
            
            // Event handlers
            btnRefresh.Click += (s, e) => RefreshPorts();
            btnConnect.Click += (s, e) => Connect();
            btnDisconnect.Click += (s, e) => Disconnect();
            btnSendOnce.Click += (s, e) => SendPBCCommand();
            chkAutoSend.CheckedChanged += (s, e) => ToggleAutoSend();
            
            this.FormClosing += (s, e) => { Disconnect(); };
        }
        
        private void RefreshPorts()
        {
            cbPorts.Items.Clear();
            var ports = SerialPort.GetPortNames().OrderBy(p => p).ToArray();
            cbPorts.Items.AddRange(ports);
            if (ports.Length > 0)
                cbPorts.SelectedIndex = 0;
        }
        
        private void Connect()
        {
            try
            {
                if (cbPorts.SelectedItem == null)
                {
                    MessageBox.Show("Please select a COM port.");
                    return;
                }
                
                // Force cleanup any existing connection
                if (serialPort != null)
                {
                    try
                    {
                        if (serialPort.IsOpen)
                            serialPort.Close();
                        serialPort.Dispose();
                    }
                    catch { }
                    serialPort = null;
                }
                
                // Wait a moment for port to be released
                System.Threading.Thread.Sleep(200);
                
                serialPort = new SerialPort
                {
                    PortName = cbPorts.SelectedItem.ToString()!,
                    BaudRate = int.Parse(cbBaud.SelectedItem!.ToString()!),
                    Parity = Parity.None,
                    StopBits = StopBits.One,
                    DataBits = 8,
                    Handshake = Handshake.None,
                    ReadTimeout = 1000,
                    WriteTimeout = 1000
                };
                
                serialPort.DataReceived += SerialPort_DataReceived;
                
                try
                {
                    serialPort.Open();
                }
                catch (UnauthorizedAccessException)
                {
                    MessageBox.Show($"COM port '{serialPort.PortName}' đang bị sử dụng!\n\n" +
                                  "Hãy:\n" +
                                  "1. Đóng tất cả chương trình đang dùng COM port\n" +
                                  "2. Rút và cắm lại CP2102\n" +
                                  "3. Thử lại", 
                                  "Lỗi truy cập COM port", 
                                  MessageBoxButtons.OK, 
                                  MessageBoxIcon.Warning);
                    return;
                }
                
                lblStatus.Text = $"Connecting         điện áp {DateTime.Now:HH:mm:ss}";
                lblStatus.BackColor = Color.LightGreen;
                
                // Update status after delay
                System.Threading.Tasks.Task.Delay(500).ContinueWith(_ =>
                {
                    if (this.InvokeRequired)
                        this.Invoke(new Action(() =>
                        {
                            lblStatus.Text = $"TX OK: Vd={numVd.Value}V a1={numA1.Value:F2}";
                        }));
                });
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Connection failed: {ex.Message}\n\nChi tiết: {ex.GetType().Name}");
            }
        }
        
        private void Disconnect()
        {
            try
            {
                if (autoSendTimer != null)
                {
                    autoSendTimer.Stop();
                    autoSendTimer.Dispose();
                    autoSendTimer = null;
                }
                
                if (serialPort != null)
                {
                    try
                    {
                        if (serialPort.IsOpen)
                        {
                            serialPort.Close();
                        }
                    }
                    catch { }
                    
                    try
                    {
                        serialPort.Dispose();
                    }
                    catch { }
                    
                    serialPort = null;
                }
                
                // Force garbage collection to release port
                System.GC.Collect();
                System.GC.WaitForPendingFinalizers();
                
                lblStatus.Text = "trạng thái         điện áp";
                lblStatus.BackColor = Color.LightGray;
            }
            catch { }
        }
        
        private void SendPBCCommand()
        {
            if (serialPort == null || !serialPort.IsOpen)
            {
                MessageBox.Show("Not connected!");
                return;
            }

            try
            {
                // Send ASCII: S,<enable>,<Vd_cmd>,<a1>,<a2>,<a3>\r\n
                int enable = chkEnable.Checked ? 1 : 0;
                string packet = string.Format(System.Globalization.CultureInfo.InvariantCulture,
                    "S,{0},{1:F3},{2:F3},{3:F3},{4:F3}\r\n",
                    enable, numVd.Value, numA1.Value, numA2.Value, numA3.Value);
                serialPort.Write(packet);

                lblStatus.Text = $"TX OK: S,{enable},{numVd.Value:F3},{numA1.Value:F3},{numA2.Value:F3},{numA3.Value:F3}";
                lblStatus.BackColor = Color.LightGreen;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Send failed: {ex.Message}");
            }
        }
        
        private void ToggleAutoSend()
        {
            if (chkAutoSend.Checked)
            {
                autoSendTimer = new System.Windows.Forms.Timer();
                autoSendTimer.Interval = 50; // 20 Hz
                autoSendTimer.Tick += (s, e) => SendPBCCommand();
                autoSendTimer.Start();
            }
            else
            {
                if (autoSendTimer != null)
                {
                    autoSendTimer.Stop();
                    autoSendTimer.Dispose();
                    autoSendTimer = null;
                }
            }
        }
        
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string data = serialPort!.ReadExisting();
                if (string.IsNullOrEmpty(data))
                    return;

                rxBuffer.Append(data);

                string bufferStr = rxBuffer.ToString();
                string[] lines = bufferStr.Split(new[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);

                // Keep last incomplete line in buffer
                if (bufferStr.EndsWith("\r\n"))
                {
                    rxBuffer.Clear();
                }
                else if (lines.Length > 0)
                {
                    rxBuffer.Clear();
                    rxBuffer.Append(lines[lines.Length - 1]);
                    Array.Resize(ref lines, lines.Length - 1);
                }

                var inv = System.Globalization.CultureInfo.InvariantCulture;

                foreach (string lineRaw in lines)
                {
                    string line = lineRaw.Trim();
                    if (line.StartsWith("vi=", StringComparison.OrdinalIgnoreCase))
                    {
                        if (float.TryParse(line.Substring(3), System.Globalization.NumberStyles.Float, inv, out float val))
                            lastVin = val;
                    }
                    else if (line.StartsWith("vl=", StringComparison.OrdinalIgnoreCase))
                    {
                        if (float.TryParse(line.Substring(3), System.Globalization.NumberStyles.Float, inv, out float val))
                            lastVc = val;
                    }
                    else if (line.StartsWith("d=", StringComparison.OrdinalIgnoreCase))
                    {
                        if (float.TryParse(line.Substring(2), System.Globalization.NumberStyles.Float, inv, out float val))
                            lastDuty = val;
                    }
                    else if (line.StartsWith("i=", StringComparison.OrdinalIgnoreCase))
                    {
                        if (float.TryParse(line.Substring(2), System.Globalization.NumberStyles.Float, inv, out float val))
                            lastIL = val;
                    }
                    else if (line.StartsWith("il=", StringComparison.OrdinalIgnoreCase))
                    {
                        if (float.TryParse(line.Substring(3), System.Globalization.NumberStyles.Float, inv, out float val))
                            lastIL = val;
                    }
                }

                if (this.InvokeRequired)
                    this.BeginInvoke(new Action(UpdateTelemetryDisplay));
                else
                    UpdateTelemetryDisplay();
            }
            catch
            {
                // ignore
            }
        }
        
        private void UpdateTelemetryDisplay()
        {
            lblVc.Text = $"Vc: {lastVc:F2} V";
            lblVin.Text = $"Vin: {lastVin:F2} V";
            lblIL.Text = $"iL: {lastIL:F3} A";
            lblDuty.Text = $"duty: {lastDuty:F3}";
            
            // Update chart - bắt đầu từ X=35
            if (chartXCounter < 35)
                chartXCounter = 35;
            
            // Add new data point
            xData.Add(chartXCounter);
            yData.Add(lastVc);
            chartXCounter++;
            
            // Keep only last 25 points
            if (xData.Count > 25)
            {
                xData.RemoveAt(0);
                yData.RemoveAt(0);
            }
            
            // Clear and redraw
            formsPlot.Plot.Clear();
            
            // Add horizontal reference line at 20V (blue)
            double[] xRef = new double[51];
            double[] yRef = new double[51];
            for (int i = 0; i <= 50; i++)
            {
                xRef[i] = 10 + i;
                yRef[i] = 20.0;
            }
            var refLine = formsPlot.Plot.Add.Scatter(xRef, yRef);
            refLine.Color = ScottPlot.Color.FromColor(WinColor.Blue);
            refLine.LineWidth = 2;
            refLine.LegendText = "Duong 2: Dien ap dat";
            
            // Add measured voltage line (red)
            if (xData.Count > 0)
            {
                var dataLine = formsPlot.Plot.Add.Scatter(xData.ToArray(), yData.ToArray());
                dataLine.Color = ScottPlot.Color.FromColor(WinColor.Red);
                dataLine.LineWidth = 2;
                dataLine.LegendText = "Duong 1: Dien ap do duoc";
            }
            
            // Auto-scroll X axis
            double xMin, xMax;
            if (chartXCounter > 60)
            {
                xMin = chartXCounter - 25;
                xMax = chartXCounter;
            }
            else
            {
                xMin = 35;
                xMax = 60;
            }

            // Y axis follows measured Vout (lastVc), but always shows the 20V reference line
            double yCenter = lastVc;
            double yMin = Math.Max(0, yCenter - 5);
            double yMax = yCenter + 5;

            yMin = Math.Min(yMin, 19.0);
            yMax = Math.Max(yMax, 21.0);

            formsPlot.Plot.Axes.SetLimits(xMin, xMax, yMin, yMax);

            formsPlot.Plot.ShowLegend();
            formsPlot.Refresh();
}
    }
}
