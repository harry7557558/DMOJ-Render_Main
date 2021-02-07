const int M = 14;

const char* MNIST_TRAIN_FILES[10] = {
	"mnist/mnist_train0.jpg",
	"mnist/mnist_train1.jpg",
	"mnist/mnist_train2.jpg",
	"mnist/mnist_train3.jpg",
	"mnist/mnist_train4.jpg",
	"mnist/mnist_train5.jpg",
	"mnist/mnist_train6.jpg",
	"mnist/mnist_train7.jpg",
	"mnist/mnist_train8.jpg",
	"mnist/mnist_train9.jpg",
};
const char* MNIST_TEST_FILES[10] = {
	"mnist/mnist_test0.jpg",
	"mnist/mnist_test1.jpg",
	"mnist/mnist_test2.jpg",
	"mnist/mnist_test3.jpg",
	"mnist/mnist_test4.jpg",
	"mnist/mnist_test5.jpg",
	"mnist/mnist_test6.jpg",
	"mnist/mnist_test7.jpg",
	"mnist/mnist_test8.jpg",
	"mnist/mnist_test9.jpg",
};

struct mnist_img {
	double data[M][M];
	double* operator[](int d) { return &data[d][0]; }
	const double* operator[](int d) const { return &data[d][0]; }
};

#define STB_IMAGE_IMPLEMENTATION
#include <libraries/stb_image.h>
int loadData(const char filename[], std::vector<mnist_img> &v) {
	int W, H;
	typedef unsigned char byte;
	byte* p = (byte*)stbi_load(filename, &W, &H, nullptr, 1);
	if (!p) return 0;

	int WN = W / 28, HN = H / 28;
	int count = 0;
	for (int Wi = 0; Wi < WN; Wi++) {
		for (int Hi = 0; Hi < HN; Hi++) {
			double data[28][28];
			for (int v = 0; v < 28; v++) {
				for (int u = 0; u < 28; u++) {
					data[v][u] = p[(Hi * 28 + v)*W + (Wi * 28 + u)] * (1. / 255.);
				}
			}
			double avr = 0;
			for (int u = 0; u < 28; u++) for (int v = 0; v < 28; v++) avr += data[u][v];
			avr /= 28 * 28;
			if (!(avr > 1. / 255)) continue;

			mnist_img temp;
			if (M == 28) {
				for (int v = 0; v < 28; v++) for (int u = 0; u < 28; u++) temp[v][u] = data[v][u];
			}
			else if (M == 14) {
				for (int v = 0; v < 28; v += 2) for (int u = 0; u < 28; u += 2)
					temp[v / 2][u / 2] = 0.25*(data[v][u] + data[v][u + 1] + data[v + 1][u] + data[v + 1][u + 1]);
			}
			else throw(M);
			v.push_back(temp);
			count++;
		}
	}
	free(p);
	return count;
}


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <libraries/stb_image_write.h>

bool writeImage(const char* filename, const double data[M][M]) {
	unsigned char p[M][M];
	for (int v = 0; v < M; v++) for (int u = 0; u < M; u++) {
		p[v][u] = (unsigned char)(data[v][u] * 255.99);
	}
	return stbi_write_png(filename, M, M, 1, p, M);
}

void writeAverage(const char* filename, const mnist_img *v, int N) {
	double p[M][M];
	for (int i = 0; i < M; i++) for (int j = 0; j < M; j++) {
		p[i][j] = 0;
		for (int u = 0; u < N; u++) p[i][j] += v[u][i][j];
		p[i][j] /= N;
	}
	writeImage(filename, p);
}
