YY = XX1(:,1);
UU = XX1(:,2);
YY1=XX1(:,3);
TT = XX1(:,4);
E = XX2(:,1);
R = XX2(:,2);
Vd = XX2(:,3);
T2 = XX2(:,4);
%subplot(211), plot(TT,YY,'k','LineWidth',2), axis([0  0.02  -0.05  1.2]),xlabel('time(sec)'),ylabel('i(A)')
%subplot(212), plot(TT,UU,'k','LineWidth',2), axis([0  0.02  -0.1  1.1]),xlabel('time(sec)'),ylabel('muy')
subplot(211), plot(TT,YY1,'k','LineWidth',2), axis([0 0.02  -1  22]),xlabel('time(sec)'),ylabel('v(V)')
subplot(212), plot(T2, E,'k','LineWidth',2), axis([0 0.02  -1  25]),xlabel('time(sec)'),ylabel('E(V)')
