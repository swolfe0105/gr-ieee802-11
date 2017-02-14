%%
clear all;

directory = '/home/stephen/wireless/python-adaptive_pursuit/';

NORX = -999;

d           = csvread(strcat(directory,'ap.log'));
t           = d(:,1);   % extract timestamp
state       = d(:,2);   % extract snr (dBm)
snr         = d(:,3);
quality     = d(:,4);
optimal_arm = d(:,5);

valid_reading = (snr ~= NORX);

states = unique(state);

figure(1)

subplot(2,2,1)
boxplot(snr(valid_reading), state(valid_reading)+1)
title('SNR')
xlabel('state')
ylabel('SNR')

subplot(2,2,2)
rx_count = hist(state(valid_reading), states);
rx_ratio = rx_count./hist(state,states);
bar(rx_ratio)
title('Probability of Receiving Frame')
xlabel('State')
ylabel('Probability of Successful RX')

subplot(2,2,3)

boxplot(quality, state+1);
title('Quality')
xlabel('State')
ylabel('Quality')

subplot(2,2,4)
opt_count = hist(optimal_arm,states);
opt_ratio = opt_count/numel(t);
bar(opt_ratio)
title('Optimal Operator Histogram')
xlabel('time')