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
generate_case(atanh(2*mod(12542.569*sin(12.98*(-6:0.13:5)+78.23),1)'-1), 200, "2.4");
generate_case(1.0 ./ linspace(-2, 3, 50)' + 5, 200, "2.5");
generate_case(log(abs(sin(linspace(-3, 2, 70))))'.^2 + 1, 200, "2.6");

data = readmatrix("https://raw.githubusercontent.com/owid/covid-19-data/master/public/data/archived/who/new_cases.csv");
v = data(:, 2) - 5000;
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
generate_case(v, 200, "3.5");

data = readmatrix("https://raw.githubusercontent.com/harry7557558/engsci-2t6/master/phy180-pendulum-lab/csv/uvt-80.csv");
v = sqrt(data(:, 2).^2 + data(:, 3).^2);
v = cumsum(cumsum(v))/length(v)^2 + 4.0*data(:, 3);
generate_case(v, 210, "3.6");

data = audioread("https://www.w3schools.com/html/horse.ogg");
v = sum(data, [2]);
generate_case(v, 239, "3.7");

data = audioread("https://www.vincentsitzmann.com/siren/img/audio/gt_counting.wav");
v = cumsum(sum(data, [2]));
v = v / (mean(abs(data))*sqrt(length(data))) + 1.0;
generate_case(v, 250, "3.8");


function [] = generate_case(v, MAXN, filename)
    w = fft(v) / length(v);
    w = w(1:min(int32(length(v)/2)-1, MAXN+1));
    w = round(w, 6);
    N = length(w)-1;
    a = real(w);
    b = -imag(w);

    disp(filename + ' N=' + N);

    dir = fileparts(matlab.desktop.editor.getActiveFilename) + "/data/";
    fp = fopen(dir+filename+".in", 'w');
    fprintf(fp, "%d\n", N);
    fprintf(fp, "%s\n", strjoin(compose("%.6f", a')));
    fprintf(fp, "%s\n", strjoin(compose("%.6f", b')));
    fclose(fp);

    tic

    A = zeros(2*N);
    for i = 1:2*N-1
        A(i, i+1) = 1;
    end
    for k = 1:2*N
        j = k - 1;
        if j < N
            h = a(N-j+1) + 1i * b(N-j+1);
        elseif j > N
            h = a(j-N+1) - 1i * b(j-N+1);
        else
            h = 2.0 * a(1);
        end
        A(2*N, k) = -h / (a(N+1) - 1i*b(N+1));
    end
    lambda = eig(A);
    xr = -1i*log(lambda);

    toc

    fp = fopen(dir+filename+".out", 'w');
    xo = [real(xr'); imag(xr')]';
    for i = 1:2*N
        s = strjoin(compose("%.14f", xo(i, :)));
        fprintf(fp, "%s\n", s);
    end
    fclose(fp);
end

