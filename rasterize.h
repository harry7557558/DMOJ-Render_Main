// (sketchy) [faked] rasterization of svgs, mostly for debug


#ifndef _INC_RASTERIZE

#define _INC_RASTERIZE


// https://github.com/nothings/stb/blob/master/stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libraries/stb_image_write.h"

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
void outputToScreen(std::vector<spline3> &P, int W, int H) {
	vec2 Min, Max; getBoundingBox(P, Min, Max);
	translateShape(P, -.5*(Min + Max));
	double SC = 0.618 * min(W / (Max.x - Min.x), H / (Max.y - Min.y));
	scaleShape(P, SC);
	translateShape(P, .5*vec2(W, H));

	printBox(vec2(0.), vec2(W, H));  // border
	for (int i = 0, l = P.size(); i < l; i++) printCross(P[i].c0, 2, " "); printf("\n");  // nodes
	printf("%s\n", &toPath(P)[0]);  // shape
}



#endif  // _INC_RASTERIZE

