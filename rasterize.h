// rasterization of svgs, mostly for debug


#ifndef _INC_RASTERIZE

#define _INC_RASTERIZE


#include "svg.cpp"


// debug output

void printBox(vec2 Min, vec2 Max, const char end[] = "\n") {
	printf("M %.3lg %.3lg h %.3lg v %.3lg h %.3lg z%s", Min.x, Min.y, Max.x - Min.x, Max.y - Min.y, Min.x - Max.x, end);
}
void printCircle(vec2 C, double r, const char end[] = "\n") {
	printf("M %.3lg %.3lg m %.3lg 0 a %.3lg,%.3lg 0 1,0 %.3lg,0 a %.3lg,%.3lg 0 1,0 %.3lg,0%s",
		C.x, C.y, -r, r, r, 2.*r, r, r, -2.*r, end);
}
void printCross(vec2 C, double r, const char end[] = "\n") {
	printf("M%.3lg,%.3lgh%.3lgm%.3lg,%.3lgv%.3lg%s", C.x - r, C.y, 2.*r, -r, -r, 2.*r, end);
}

void splineInfo(const std::vector<spline3> &sp) {
	double A = calcArea(sp);
	vec2 p = calcCOM(sp);
	vec2 Min, Max; getBoundingBox(sp, Min, Max);

	printCircle(p, sqrt(A / PI));
	printBox(Min, Max);
	printCross(p, 3);
	printCross(.5*(Max + Min), 2);
	printf("\n");
	printf("%s\n\n", &toPath(sp)[0]);
}



// "render" it as a svg path - the original shape will be modified
void outputToScreen(std::vector<spline3> &P, int W, int H, bool print = true) {
	vec2 Min, Max; getBoundingBox(P, Min, Max);
	translateShape(P, -.5*(Min + Max));
	double SC = 0.618 * min(W / (Max.x - Min.x), H / (Max.y - Min.y));
	scaleShape(P, SC);
	translateShape(P, .5*vec2(W, H));

	if (print) {
		printBox(vec2(0.), vec2(W, H));  // border
		for (int i = 0, l = P.size(); i < l; i++) printCross(P[i].c0, 2, " "); printf("\n");  // nodes
		printf("%s\n", &toPath(P)[0]);  // shape
	}
}





// a sketchy rasterizer

// https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libraries/stb_image_write.h"

typedef uint8_t byte;
typedef uint32_t COLORREF;  // abgr
struct imageObject {
	int W, H;
	COLORREF* p;
	imageObject(int W, int H) : W(W), H(H) {
		p = new COLORREF[W*H];
		for (int i = 0, l = W * H; i < l; i++) p[i] = 0;
	}
	~imageObject() { delete p; p = 0; W = H = 0; }

	bool writePNG(const char* filename) const {
		return stbi_write_png(filename, W, H, 4, p, 4 * W);
	}
};

#define BLACK 0xFF000000
#define RED 0xFF0000FF
#define BLUE 0xFFFF0000

COLORREF rgba(double r, double g, double b, double a = 1.0) {
	COLORREF C;
	byte *c = (byte*)&C;
	c[0] = (byte)(255 * clamp(r, 0, 1));
	c[1] = (byte)(255 * clamp(g, 0, 1));
	c[2] = (byte)(255 * clamp(b, 0, 1));
	c[3] = (byte)(255 * clamp(a, 0, 1));
	return C;
}


// origin: top-left; x-axis: right; y-axis: down
void drawLine(const imageObject &Img, vec2 p, vec2 q, COLORREF col = BLACK) {
	int W = Img.W, H = Img.H;
	vec2 d = q - p;
	double slope = d.y / d.x;
	if (abs(slope) <= 1.0) {
		if (p.x > q.x) std::swap(p, q);
		int x0 = max(0, int(p.x)), x1 = min(W - 1, int(q.x)), y;
		double yf = slope * x0 + (p.y - slope * p.x);
		for (int x = x0; x <= x1; x++) {
			y = (int)yf;
			if (y >= 0 && y < H) Img.p[y*W + x] = col;
			yf += slope;
		}
	}
	else {
		slope = d.x / d.y;
		if (p.y > q.y) std::swap(p, q);
		int y0 = max(0, int(p.y)), y1 = min(H - 1, int(q.y)), x;
		double xf = slope * y0 + (p.x - slope * p.y);
		for (int y = y0; y <= y1; y++) {
			x = (int)xf;
			if (x >= 0 && x < W) Img.p[y*W + x] = col;
			xf += slope;
		}
	}
}


// render path to image
void raster_renderPath(std::vector<spline3> &v, const imageObject &P, COLORREF col = BLACK, bool transform = false) {
	if (transform) {
		// the shape will be modified with transform is set to true
		outputToScreen(v, P.W, P.H, false);
	}

	const int dif = 30;
	for (int d = 0, l = v.size(); d < l; d++) {
		//col = d & 1 ? RED : BLUE;
		vec2 a = v[d].c0, b;
		for (int i = 1; i <= dif; i++) {
			b = v[d].eval(i / double(dif));
			drawLine(P, a, b, col);
			a = b;
		}
	}

#if 0  // debug
	{
		vec2 C = calcLineCOM(v);
		double t = 1.302284134180384;
		vec2 d = vec2(sin(t), cos(t));
		int Count = 0;
		double r = splineIntersect(v, C, vec2(sin(t), cos(t)), &Count);
		drawLine(P, C, C + r * d);
	}
#endif
}

// render Fourier series to image
void raster_renderFourier(int N, const vec2* a, const vec2* b, const imageObject &P, COLORREF col = 0) {
	auto eval = [&](double t) {
		vec2 p(0.0);
		for (int k = 0; k < N; k++) p += a[k] * cos(k*t) + b[k] * sin(k*t);
		return p;
	};
	auto curvature = [&](double t) {  // use for adaptive step
		vec2 dp(0.0), ddp(0.0);
		for (int k = 1; k < N; k++) {
			double s = sin(k*t), c = cos(k*t);
			dp += k * (a[k] * -s + b[k] * c);
			ddp += k * k * (a[k] * -c + b[k] * -s);
		}
		double m = length(dp);
		return (m*m*m) / abs(det(dp, ddp));
	};

	double t = 0.0, dt = curvature(0.0);
	vec2 u = eval(0.0);
	t += clamp(dt, 0.005, 0.01);
	int seg_count = 0;
	while (t < 2.*PI) {
		//col = seg_count & 1 ? RED : BLUE;
		vec2 v = eval(t);
		double dt = curvature(t);
		//col = rgba(tanh(50. / dt), 0, 0);
		drawLine(P, u, v, col);
		t += clamp(dt, 0.005, 0.01);  // helps with rapid curvature change
		u = v;
		seg_count++;
	}
	drawLine(P, u, eval(0.0), col);
}




#endif  // _INC_RASTERIZE

