# SỬA MODEL PBC - CHỈ DÙNG CẢM BIẾN VOUT

## MỤC TIÊU
- Giữ nguyên thuật toán PBC trong Simulink
- Chỉ đo Vout từ PCB (PA0 → J3-1)
- Vin và IL dùng giá trị cố định (fake values)

---

## 📋 CÁC BƯỚC THỰC HIỆN

### **BƯỚC 1: MỞ FILE SIMULINK**

```matlab
cd 'D:\DO AN TOT NGIEP\code_simulink'
open_system('boostconverter_nap.slx')
```

---

### **BƯỚC 2: SỬA VIN SCALE** ⚠️ (QUAN TRỌNG NHẤT!)

**Vấn đề:**
- Code đang dùng Gain = `0.008864447` cho CẢ Vin và Vout
- Nhưng mạch Vin dùng R4=18kΩ, R5=3.3kΩ (tỉ số 6.4545)
- Mạch Vout dùng R2=10kΩ, R3=1kΩ (tỉ số 11)
- → Vin đang đọc SAI 1.72 lần!

**Cách sửa:**
1. Vào **Function-Call Subsystem** → **PBC** subsystem
2. Tìm khối **Gain** đo **Vin** (sau ADC kênh PA1)
3. Double-click vào Gain
4. Đổi giá trị:
   ```
   TỪ:  0.008864447
   THÀNH: 0.005129580
   ```
5. Click **OK**

**Giải thích:**
```
Tỉ số cầu chia Vin = (R4 + R5) / R5 = (18 + 3.3) / 3.3 = 6.4545
Gain_Vin = 3.3V / (3.3V × 6.4545) = 0.00512958
```

---

### **BƯỚC 3: GIỚI HẠN DUTY** 🛡️

**Vấn đề:**
- Saturation đang để Upper = 1 (duty 100%)
- Với gate 3.3V yếu, duty cao → MOSFET nóng cháy
- Boost 12V→20V chỉ cần duty ≈ 40%

**Cách sửa:**
1. Trong **PBC subsystem**
2. Tìm khối **Saturation** (trước Gain 100, trước PWM)
3. Double-click vào Saturation
4. Đổi:
   ```
   Upper limit: 1 → 0.5  (giới hạn 50%)
   Lower limit: 0  (giữ nguyên)
   ```
5. Click **OK**

**Công thức duty lý tưởng:**
```
duty = (Vout - Vin) / Vout = (20 - 12) / 20 = 0.4 (40%)
```

---

### **BƯỚC 4: GIẢM TẦN SỐ PWM** ❄️ (Giảm nhiệt)

**Vấn đề:**
- PWM 20kHz = 20,000 lần chuyển mạch/giây
- Với gate 3.3V, mỗi lần chuyển mạch chậm → tổn hao lớn
- 20kHz × 1mJ = 20W tổn hao!

**Cách sửa:**
1. Tìm khối **Basic PWM** (Timer 4, CH1 (B6))
2. Double-click vào Basic PWM
3. Tìm parameter **"Period (sec)"** hoặc **"Polarity, Active High, Period (sec)"**
4. Đổi giá trị:
   ```
   TỪ:  5e-5   (20 kHz)
   THÀNH: 1e-4   (10 kHz)  ← Khuyến nghị
   HOẶC:  2e-4   (5 kHz)   ← Mát hơn
   ```
5. Click **OK**

**Lợi ích:**
- 10kHz: tổn hao giảm 50% → MOSFET mát hơn rõ rệt
- 5kHz: tổn hao giảm 75% → MOSFET chỉ ấm

---

### **BƯỚC 5: BUILD LẠI CODE** 🔨

**Trong Simulink:**
- Nhấn **Ctrl+B**
- Hoặc: **Tools → Run on Target Hardware → Build**

**Hoặc dùng MATLAB Command Window:**
```matlab
cd 'D:\DO AN TOT NGIEP\code_simulink'
slbuild('boostconverter_nap')
```

**Chờ build xong, file output:**
```
D:\DO AN TOT NGIEP\code_simulink\boostconverter_nap_stm32f4\boostconverter_nap.bin
```

---

### **BƯỚC 6: FLASH VÀO STM32** 📥

**Dùng STM32CubeProgrammer:**
1. Connect ST-Link
2. **Open file:** `boostconverter_nap.bin`
3. **Start address:** `0x08000000`
4. Click **Download**
5. Đợi "File download complete"
6. Click **Disconnect**

**Hoặc dùng ST-Link Utility:**
```
Target → Program & Verify
File: boostconverter_nap.bin
Start: 0x08000000
```

---

## 🧪 **HƯỚNG DẪN TEST SAU KHI FLASH**

### **Test 1: HỞ VÒNG (không tải)** 

**Mục tiêu:** Chứng minh mạch lên 20V được

1. **Vô hiệu hóa PBC tạm thời:**
   - Trong Simulink, comment out khối PBC
   - Đặt duty = **Constant** (0.15 → 0.25 → 0.35 → 0.45)
   
2. **Test từng bước:**
   ```
   Duty 15% → đo Vout = ?V
   Duty 25% → đo Vout = ?V
   Duty 35% → đo Vout = ?V
   Duty 40% → đo Vout = ?V (mục tiêu ~20V)
   ```

3. **Quan sát MOSFET:**
   - Nếu ấm (40-50°C): OK
   - Nếu nóng (>70°C): giảm duty hoặc PWM xuống 5kHz

---

### **Test 2: KÍN VÒNG (tải nhẹ)** 

**Sau khi test hở vòng OK:**

1. **Kết nối tải:** Điện trở ≥ 200Ω (dòng <1A)
2. **Bật PBC:** Uncomment khối điều khiển
3. **Đặt Vd_cmd = 20V** trong GUI
4. **Quan sát trên GUI:**
   - Vout → 20V (từ từ)
   - duty → 0.35-0.45
   - iL → 1-2A
5. **Kiểm tra MOSFET:** Không được quá 70°C

---

## 📊 **KẾT QUẢ KỲ VỌNG**

### ✅ **SAU KHI SỬA:**
- Vin đọc chính xác (12.0V thay vì ~20V)
- Duty ổn định 35-45% (không vọt lên 80-100%)
- MOSFET chỉ ấm 45-60°C (không nóng 80-90°C)
- Vout lên 20V ổn định với tải ≤3A

### ⚠️ **GIỚI HẠN VỚI MẠCH HIỆN TẠI:**

| Tham số | Giá trị an toàn | Giá trị tối đa |
|---------|-----------------|----------------|
| Dòng tải | ≤ 2A | ~3A (có tản nhiệt) |
| Nhiệt độ MOSFET | 50-60°C | 70°C |
| Hiệu suất | ~75-80% | (do Rds(on) cao) |
| Duty | 35-45% | 50% (đã giới hạn) |

---

## 🔧 **TROUBLESHOOTING**

### ❌ **MOSFET VẪN NÓNG (>70°C):**
**Giải pháp:**
1. Giảm Saturation Upper xuống **0.4** (40%)
2. Giảm PWM xuống **5kHz** (Period = 2e-4)
3. Giảm tải (tăng R tải)
4. Gắn tản nhiệt TO-220

### ❌ **VOUT KHÔNG LÊN 20V:**
**Kiểm tra:**
1. Vin scale đã sửa đúng chưa? (0.00512958)
2. ADC mapping đúng chân không?
   - PA0 → Vout
   - PA1 → Vin
   - PA4 → iL
3. Test hở vòng duty 40% xem Vout bao nhiêu?

### ❌ **DUTY VẪN STUCK Ở 0.250:**
**Nguyên nhân:** TIM5 ARR=-1 (lỗi cũ)
**Giải pháp:** Đảm bảo đã flash code MỚI (build sau khi sửa)

---

## 🎯 **NÂNG CẤP TIẾP THEO (nếu cần tải lớn)**

### **Cách 1: Thêm Gate Driver** (khuyến nghị)
- **IC:** TC4420, MCP1407, IR2110
- **Sơ đồ:**
  ```
  STM32 PB6 (3.3V) → TC4420 Input
  TC4420 VDD = 12V (từ Vout)
  TC4420 Output → Gate MOSFET (12V PWM)
  ```
- **Lợi ích:** Rds(on) giảm từ 0.3Ω → 0.044Ω (gấp 7 lần)
- **Dòng tải:** Có thể lên 10-15A

### **Cách 2: Đổi MOSFET Logic-Level**
- **IC:** IRLZ44N, IRLB8743PBF
- **Đặc điểm:** Rds(on) thấp ngay cả với Vgs=3.3V
- **Không cần gate driver**

---

## 📝 **CHECKLIST TRƯỚC KHI TEST**

- [ ] Đã sửa Vin Gain = 0.005129580
- [ ] Đã giới hạn Saturation Upper = 0.5
- [ ] Đã giảm PWM Period = 1e-4 (10kHz)
- [ ] Đã build lại code (Ctrl+B)
- [ ] Đã flash file .bin mới
- [ ] Có tản nhiệt gắn vào MOSFET
- [ ] Test hở vòng duty 15-40% trước
- [ ] Dùng tải ≥200Ω (dòng nhỏ) để test

---

**Cập nhật:** 12/12/2025  
**Mục đích:** Sửa 3 lỗi chính để Boost lên 20V an toàn với mạch hiện tại
- **GIỮ NGUYÊN** giá trị gain hiện tại
- HOẶC đổi thành **0.01081081** (để fix lỗi calibration)

---

## BƯỚC 4: CẤU TRÚC MODEL SAU KHI SỬA

```
[Constant: 12] ────────┐
                       │
[ADC PA0] → [Gain] ────┤→ [PBC Controller] → [Saturation] → [PWM PB6]
                       │
[Constant: 0.5] ───────┘
```

**Giải thích:**
- Constant 12 → thay thế cho Vin sensor
- ADC PA0 → đọc Vout thật từ mạch
- Constant 0.5 → thay thế cho IL sensor
- PBC controller vẫn chạy bình thường với 3 đầu vào

---

## BƯỚC 5: CHỈNH REFERENCE VOLTAGE

Tìm block "Reference" hoặc "Vref":
- Đổi giá trị thành **400** (vì Vout_ref = 20V → 20²=400)
- Hoặc giữ nguyên nếu đã đúng

---

## BƯỚC 6: BUILD VÀ NẠP CODE

### 6.1. Cấu hình Hardware
```matlab
% Trong Simulink Model Configuration:
% Hardware Implementation → Hardware board → STM32F4xx
% Solver → Fixed-step, ode4 (Runge-Kutta)
% Step size: 5e-5 (20kHz)
```

### 6.2. Build
- Ctrl+B trong Simulink
- Hoặc: Tools → Embedded Coder → Build Model
- Chờ build xong (2-3 phút)

### 6.3. Nạp vào STM32
```matlab
% Tự động nạp nếu đã config ST-Link
% Hoặc dùng STM32CubeProgrammer:
% File: test2_stm32f4\test2.hex
```

---

## BƯỚC 7: KẾT NỐI PCB VỚI STM32

**CHỈ CẦN 3 DÂY:**

```
[STM32]          [PCB]
PB6 (PWM)   →    J2-1  (PWM IN)
PA0 (ADC)   ←    J3-1  (Vout feedback)
GND         →    J2-2, J3-2  (GND chung)
```

**KHÔNG CẦN NỐI:**
- ~~PA4 (Vin sensor)~~ → không dùng nữa
- ~~PA1 (IL sensor)~~ → không dùng nữa

---

## BƯỚC 8: KIỂM TRA UART (NẾU DÙNG GUI)

Nếu bạn vẫn muốn xem dữ liệu trên GUI:

### 8.1. Sửa UART transmission trong model
Tìm block "UART Transmit":
- Byte 2-5: Duty cycle (giữ nguyên)
- Byte 6-9: Vc (giữ nguyên)
- Byte 10-13: **Constant 12** (thay vì ADC_Vin)
- Byte 14-17: **Constant 0.5** (thay vì ADC_IL)
- Byte 18-21: Sum2 (giữ nguyên)
- Byte 22-25: Tick (giữ nguyên)

### 8.2. Sửa GUI validation
File: `d:\DO AN TOT NGIEP\giaodien\BoostPBC_GUI\Program.cs`

Dòng 335-339, sửa validation:
```csharp
// Chấp nhận Vin = 12 cố định
if (vin < 11.5 || vin > 12.5) continue; // Cho phép 12±0.5V

// Chấp nhận IL = 0.5 cố định  
if (current < 0 || current > 2) continue; // Cho phép 0-2A
```

---

## BƯỚC 9: TEST

### 9.1. Không dùng GUI (test trực tiếp)
1. Nạp code vào STM32
2. Cấp nguồn 12V vào J1 của PCB
3. Nối 3 dây: PB6→J2-1, PA0←J3-1, GND
4. Đo Vout tại J4 bằng đồng hồ vạn năng
5. **Kết quả mong đợi: Vout ≈ 20V**

### 9.2. Dùng GUI
1. Nối thêm UART: PA2(TX)→RX, PA3(RX)→TX của USB-TTL
2. Mở GUI `BoostPBC_GUI.exe`
3. Chọn COM port → Connect
4. **Kết quả mong đợi:**
   - Vin hiển thị ≈ 12V (constant)
   - Vout hiển thị ≈ 20V (đo thật từ PCB)
   - Current hiển thị ≈ 0.5A (constant)
   - Duty cycle ≈ 0.4-0.5

---

## LƯU Ý AN TOÀN

⚠️ **TRƯỚC KHI NỐI STM32 VÀO PCB:**

### Kiểm tra điện áp J3-1 (Vout feedback)
```
1. Cấp nguồn 12V vào J1 của PCB (chưa nối STM32)
2. Không nối PWM (J2-1 để hở)
3. Đo J3-1 với multimeter:
   - Phải đo được 0V (vì chưa có PWM)
   - Nếu > 3.3V → NGUY HIỂM! Kiểm tra R_TOP, R_BOT
```

### Sau khi nối PWM, kiểm tra lại J3-1
```
1. Nối PB6 → J2-1
2. Đo J3-1:
   - Khi Vout = 20V → J3-1 phải ≤ 3.3V
   - Nếu > 3.3V → TẮT NGUỒN NGAY, sửa voltage divider
```

---

## TÓM TẮT

**TRƯỚC KHI SỬA:**
- 3 cảm biến: Vin (PA4), Vout (PA0), IL (PA1)
- 5 dây kết nối PCB ↔ STM32

**SAU KHI SỬA:**
- 1 cảm biến: Vout (PA0)
- 3 dây kết nối PCB ↔ STM32
- Vin = 12V (constant trong code)
- IL = 0.5A (constant trong code)
- PBC vẫn chạy bình thường

**ƯU ĐIỂM:**
✅ Không cần sửa PCB
✅ Không cần sửa thuật toán PBC
✅ Chỉ cần đo Vout
✅ Đơn giản hơn, ít dây hơn

**NHƯỢC ĐIỂM:**
⚠️ Không đáp ứng được với thay đổi Vin (nếu Vin ≠ 12V)
⚠️ Không bảo vệ quá dòng (vì IL fake)
⚠️ PBC không tối ưu như khi có đầy đủ cảm biến
