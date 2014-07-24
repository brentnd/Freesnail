clf;
clc;
clear all;
close all;

s = serial('COM5');

set(s, 'BaudRate',115200);
set(s, 'InputBufferSize',1024);
set(s, 'Terminator',0);
set(s, 'Timeout',100);
fopen(s);

range = 10;
line_hist = zeros(1,range);
direction_hist = zeros(1,range);
speed_hist = zeros(1,range);
ramp_hist = zeros(1,range);

line_plot = plot(1:range,line_hist,'b','LineWidth',10);
axis([0 range -100 100]);
hold on;
direction_plot = plot(1:range,direction_hist,'r','LineWidth',10);
speed_plot = plot(1:range,speed_hist,'g','LineWidth',10);
ramp_plot = plot(1:range,ramp_hist,'c','LineWidth',10);
legend('Line Position','Turn direction','Speed','Ramp');
drawnow;

while 1
    B = fread(s,4,'uint8');
    line = B(1)-100;
    direction = B(2)-100;
    speed = B(3);
    ramp = B(4)-1;
    
    %Update history
    line_hist = [line_hist(2:end) line];
    direction_hist = [direction_hist(2:end) direction];
    speed_hist = [speed_hist(2:end) speed];
    ramp_hist = [ramp_hist(2:end) ramp];
    
    %Update plots
    set(line_plot,'YData',line_hist);
    set(direction_plot,'YData',direction_hist);
    set(speed_plot,'YData',speed_hist);
    set(ramp_plot,'YData',ramp_hist*50);
    
    drawnow;
end


fclose(s);
delete(s);
clear s;