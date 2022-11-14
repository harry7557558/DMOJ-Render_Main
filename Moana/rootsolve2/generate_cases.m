generate_case([-3 2 5 6 8 -9]', 200, "0.1");
generate_case([-3 2 5 3 4 -2 -4 5]', 200, "0.2");

generate_case([-2 2 -1 1 0.5 0]', 200, "1.1");
generate_case([-1 2 -2 1 2 10]', 200, "1.2");
generate_case([-2 -1 0 1 2 3 4 5]', 200, "1.3");
generate_case([-2 2 -1 1 0 0 1 -1 2 -2]', 200, "1.4");
generate_case([3 1 4 1 5 9 2 6 5 3 5 8 9 7]', 200, "1.5");
generate_case([1 -2 3 -4 5 -6 7 -8 9 -10 11 -12 13 14 15 16]', 200, "1.6");

generate_case(sign(-1:0.1:3)', 200, "2.1");
generate_case(exp(-abs(-2:0.1:3).^1.5)', 200, "2.2");
generate_case(sqrt(abs(sin(-10:0.15:10)))'-0.5, 200, "2.3");
generate_case(atanh(2*mod(12542.569*sin(12.98*(-6:0.09:5)+78.23),1)'-1), 200, "2.4");
generate_case(1.0 ./ (-2:0.055:3)' + 5, 200, "2.5");
generate_case(log(abs(sin(-3:0.045:2)))'.^2 + 1, 200, "2.6");

data = readmatrix("https://raw.githubusercontent.com/owid/covid-19-data/master/public/data/archived/who/new_cases.csv");
v = data(:, 2) - 5000;
v = [v' v' v']';
generate_case(v, 200, "3.1");

data = readmatrix("https://raw.githubusercontent.com/harry7557558/engsci-2t6/master/phy180-pendulum-lab/csv/uvt-20.csv");
v = atan2(data(1:200, 2)-0.5, data(1:200, 3));
generate_case(v, 200, "3.2");

data = imread("https://cdn.discordapp.com/attachments/987164635872526407/1010210504498741258/unknown.png");
v = mean(data/255, [2, 3])-0.5;
generate_case(v, 78, "3.3");

data = imread("https://cdn.discordapp.com/attachments/987164635872526407/993715549894885446/profile-ammonite-400x400.png");
v = cumsum(mean(data/255-0.5, [2, 3]));
v = v - (v(end) - v(1)) * linspace(0, 1, length(v))';
generate_case(v, 127, "3.4");

data = readmatrix("https://raw.githubusercontent.com/harry7557558/engsci-2t6/master/phy180-pendulum-lab/csv/uvt-20.csv");
v = atan2(data(:, 2)-0.5, data(:, 3));
generate_case(v, 197, "3.5");

data = readmatrix("https://raw.githubusercontent.com/harry7557558/engsci-2t6/master/phy180-pendulum-lab/csv/uvt-80.csv");
v = sqrt(data(:, 2).^2 + data(:, 3).^2);
v = cumsum(cumsum(v))/length(v)^2 + 4.0*data(:, 3);
generate_case(v, 248, "3.6");

data = audioread("https://www.w3schools.com/html/horse.ogg");
v = sum(data, [2]);
generate_case(v, 281, "3.7");

data = audioread("https://www.vincentsitzmann.com/siren/img/audio/gt_counting.wav");
v = cumsum(sum(data, [2]));
v = v / (mean(abs(data))*sqrt(length(data))) + 1.0;
generate_case(v, 300, "3.8");

data = readmatrix("https://raw.githubusercontent.com/harry7557558/engsci-2t6/master/phy180-pendulum-lab/csv/uvt-20.csv");
v = atan2(data(:, 2)-0.5, data(:, 3));
generate_case(v, 350, "4.1");

v = audioread("https://www.w3schools.com/html/horse.ogg");
generate_case(v, 400, "4.2");

data = readmatrix("https://raw.githubusercontent.com/harry7557558/engsci-2t6/master/phy180-pendulum-lab/csv/uvt-20.csv");
v = sin(exp(data(:, 2)-0.5));
generate_case(v, 500, "4.3");

data = audioread("https://www.vincentsitzmann.com/siren/img/audio/gt_counting.wav");
v = data / sqrt(mean(data.^2));
generate_case(v, 600, "4.4");

data = audioread("https://www.w3schools.com/html/horse.ogg");
v = exp(data / sqrt(mean(data.^2)));
v = v - mean(v);
generate_case(v, 700, "4.5");

data = imread("https://cdn.discordapp.com/attachments/987164635872526407/993715549894885446/profile-ammonite-400x400.png");
v = v - mean(v);
generate_case(v, 800, "4.6");

data = audioread("https://www.w3schools.com/html/horse.ogg");
v = data / sqrt(mean(data.^2));
generate_case(v, 900, "4.7");

data = audioread("https://www.vincentsitzmann.com/siren/img/audio/gt_counting.wav");
v = exp(data / sqrt(mean(data.^2)));
v = v - mean(v);
generate_case(v, 1000, "4.8");

