% ====================================================================
% KHỞI TẠO THAM SỐ PBC CHO STM32
% ====================================================================
% File này sẽ chạy trước khi build model
% Các giá trị này sẽ được compile vào code STM32

%% THAM SỐ MẠCH BOOST CONVERTER
L = 0.02;           % Cuộn cảm 20mH
C = 0.000068;       % Tụ điện 68µF
R = 30;             % Tải 30Ω
E = 15;             % Điện áp đầu vào trung bình 15V
Vd = 20;            % Điện áp mong muốn 20V

%% THAM SỐ ĐIỀU KHIỂN PBC - GIÁ TRỊ MẶC ĐỊNH
a1 = 1.3;           % Tham số PBC a1
a2 = 21.7;          % Tham số PBC a2
a3 = 13;            % Tham số PBC a3

%% THAM SỐ KHÁC
Vin = 12.0;         % Điện áp đầu vào mặc định
Vout = 20.0;        % Điện áp đầu ra mặc định
iL = 0.0;           % Dòng điện mặc định
duty = 0.0;         % Duty cycle mặc định

%% SET GIÁ TRỊ MẶC ĐỊNH CHO DATA STORE MEMORY
% Các giá trị này sẽ được dùng khi STM32 khởi động
% Nếu không nhận được a1, a2, a3 từ GUI

%% HIỂN THỊ
fprintf('====================================\n');
fprintf('THAM SỐ PBC ĐÃ KHỞI TẠO:\n');
fprintf('a1 = %.3f\n', a1);
fprintf('a2 = %.3f\n', a2);
fprintf('a3 = %.3f\n', a3);
fprintf('Vd = %.2f V\n', Vd);
fprintf('Vin = %.2f V\n', Vin);
fprintf('====================================\n');
fprintf('⚠️  LƯU Ý: Giá trị này là MẶC ĐỊNH\n');
fprintf('    GUI có thể gửi giá trị mới qua UART\n');
fprintf('====================================\n');
