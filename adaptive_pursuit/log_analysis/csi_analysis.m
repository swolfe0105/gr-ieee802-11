% flight1.log contains SNR CSI measurements. 
% Each row contains observations in the format:
%  [timestamp, SNR, subcarrier1_I, subcarrier1_Q, subcarrier2_I, subcarrier2_Q, ... ]

d_omni   = csvread('../../csi_omni.log',1);
t_omni   = d_omni(:,1);       % extract timestamp
d_snr_omni    = d_omni(:,2);   % extract snr (dBm) 
snr_omni = (10.^(d_snr_omni/10));

d_ap   = csvread('../../csi_ap.log',1);
t_ap     = d_ap(:,1);       % extract timestamp
d_snr_ap = d_ap(:,2);   % extract snr (dBm) 
snr_ap   = (10.^(d_snr_ap/10));

% build CSI matrix by combining the I and Q components for each subcarrier
% into a single complex value.
csi_omni = zeros(size(d_omni,1),(size(d_omni,2)-2)/2);
for j=1:size(csi_omni,2)
    csi_omni(:,j) = d_omni(:,2*j+1)+1i*d_omni(:,2*j+2); 
end

% build CSI matrix by combining the I and Q components for each subcarrier
% into a single complex value.

csi_ap = zeros(size(d_ap,1),(size(d_ap,2)-2)/2);
for j=1:size(csi_ap,2)
    csi_ap(:,j) = d_ap(:,2*j+1)+1i*d_ap(:,2*j+2); 
end

% Determine which measurements were obtained in flight.
% all samples between ind_start and ind_stop occured during flight.
t_start = 10;               % time flight started
t_stop = 35;                % time flight ended
[~, ind_start_omni] = min(abs( t_omni - t_start ));
[~, ind_stop_omni]  = min(abs( t_omni - t_stop ));
[~, ind_start_ap]   = min(abs( t_ap - t_start ));
[~, ind_stop_ap]    = min(abs( t_ap - t_stop ));

% plot snr vs. time for entire log
figure(1)       
%plot(t_omni,smooth(d_snr_omni),t_ap,smooth(d_snr_ap))
plot(t_omni,d_snr_omni,'*',t_ap,d_snr_ap,'*')
title('SNR vs. Time')
xlabel('time (s)')
ylabel('SNR (dB)')
legend('omni','ap')
grid on;


%% Time ACFs

max_lag = 400;
Rx_csi = zeros(max_lag,size(csi_omni,2));

for j=1:size(csi_omni,2)
    Rx_csi(:,j) = acf(abs(csi_omni(ind_start_omni:ind_stop_omni,j)), max_lag, 0);
end
figure(2);
subplot(2,1,1)
bar(mean(Rx_csi'))
xlabel('lag')
title('Omni Mean CSI ACF')

subplot(2,1,2)
bar(acf(abs(snr_omni(ind_start_omni:ind_stop_omni)), max_lag, 0))
xlabel('lag')
title('Omni SNR Mean ACF')

%% Freq ACF

figure(4);
subplot(2,1,1)
bar([mean(abs(csi_omni)) ; mean(abs(csi_ap)) ]')
xlabel('Subcarrier')
ylabel('Mean CSI')
title('Mean CSI Magnitude per Subcarrier')
legend('omni','ap')

max_lag = 15;
cbw_omni= zeros(max_lag, size(csi_omni,2));
cbw_ap  = zeros(max_lag, size(csi_ap,2));

for j=ind_start_omni:ind_stop_omni
    cbw_omni(:,j) = acf(abs(csi_omni(j,:)'), max_lag, 0);
end

for j=ind_start_ap:ind_stop_ap
    cbw_ap(:,j) = acf(abs(csi_ap(j,:)'), max_lag, 0);
end

subplot(2,1,2)
bar([mean(cbw_omni'); mean(cbw_ap')]')
xlabel('subcarrier distance')
title('Subcarrier CSI ACF')
legend('Omni','ap')

%%
figure(5)
sc = 1:48;
%sc = [2 16 28 42];
plot(abs(Rx_csi(:,sc)))
legend(strread(num2str(sc),'%s'))