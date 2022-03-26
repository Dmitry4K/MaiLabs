#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cuda.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
//#include<chrono>


//#define PRE_FILE_READING

#define BLOCKS dim3(16, 16)
#define THREADS dim3(16, 16)

#define CUDA_ERROR_HANDLER(err) do { if (err != cudaSuccess) { std::cout << cudaGetErrorString(err) << std::endl; exit(1);} } while (0)
#define CEH(err) CUDA_ERROR_HANDLER(err)

/*
namespace chrono_timer {
	template<class T = std::chrono::nanoseconds>
	class timer {
		std::chrono::steady_clock::time_point start_point = std::chrono::high_resolution_clock::now();
		std::chrono::steady_clock::time_point end_point = start_point;
		bool is_stoped = true;
	public:
		using time_type = T;
		void start() {
			start_point = std::chrono::high_resolution_clock::now();
			is_stoped = false;
		}
		void stop() {
			end_point = std::chrono::high_resolution_clock::now();
			is_stoped = true;
		}
		long long time() {
			return is_stoped ? std::chrono::duration_cast<time_type>(end_point - start_point).count() : std::chrono::duration_cast<time_type>(std::chrono::high_resolution_clock::now() - start_point).count();
		}
		long long measure(void f(void)) {
			this->start();
			f();
			return this->time();
		}
	};
	long long nanoseconds_time(void f(void)) {
		timer<> timer;
		timer.start();
		f();
		return timer.time();
	}

	template<class T>
	long long measure_time(void f(void)) {
		timer<T> timer;
		timer.start();
		f();
		return timer.time();
	}

	using timer_nns = timer <>;
	using timer_mls = timer < std::chrono::milliseconds >;
	using timer_mcs = timer < std::chrono::microseconds >;
	using timer_sec = timer < std::chrono::seconds >;
	using timer_min = timer < std::chrono::minutes >;
};*/

const float PI = 3.141592653589f;

const float SPEED = 0.02f;
const float OFFSET = 0.3f;

namespace COLORS {
	uchar4 RED = uchar4{ 255, 0, 0 };
	uchar4 BLUE = uchar4{ 0, 0, 255 };
	uchar4 GREEN = uchar4{ 0, 255, 0 };
	uchar4 PINK = uchar4{ 225, 0 ,255 };
	uchar4 YELLOW = uchar4{ 255, 255, 0 };
	uchar4 CYAN = uchar4{ 0, 255, 255 };
	uchar4 ORANGE = uchar4{ 240, 116,39 };
	uchar4 PURPLE = uchar4{ 148, 87, 235 };
}

__device__ __host__ float3 operator-(const float3& a, const float3& b) { return make_float3(a.x - b.x, a.y - b.y, a.z - b.z); }
__device__ __host__ float3 operator+(const float3& a, const float3& b) { return make_float3(a.x + b.x, a.y + b.y, a.z + b.z); }
__device__ __host__ float3 operator*(const float3& a, const float b) { return make_float3(a.x * b, a.y * b, a.z * b); }
__device__ __host__ float3 operator*(const float3& a, const float3& b) { return make_float3(a.x * b.x, a.y * b.y, a.z * b.z); }
std::ostream& operator<<(std::ostream& out, const float3& a) { return (out << a.x << ' ' << a.y << ' ' << a.z); }
std::istream& operator>>(std::istream& in, float3& a) { return (in >> a.x >> a.y >> a.z); }


__device__ __host__ float dot(float3 a, float3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
__device__ __host__ float3 prod(float3 a, float3 b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
__device__ __host__ float3 norm(float3 v) { float l = sqrtf(dot(v, v)); return { v.x / l, v.y / l, v.z / l }; }
__device__ __host__ float len(float3 a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }
__device__ __host__ float3 mult(float3 a, float3 b, float3 c, float3 v) {
	return { a.x * v.x + b.x * v.y + c.x * v.z,
				a.y * v.x + b.y * v.y + c.y * v.z,
				a.z * v.x + b.z * v.y + c.z * v.z };
}

using uchar = unsigned char;

struct key_config {
	key_config(int argc, char* argv[]) {

		if (argc == 1) {
			is_gpu = true; return;
		}

		if (strcmp(argv[1], "--gpu") == 0) is_gpu = true;

		else if (strcmp(argv[1], "--cpu") == 0) {
			is_cpu = true;
			is_gpu = false;
		}
		else if (strcmp(argv[1], "--default") == 0) {
			is_default = true;
			is_gpu = false;
		}
		else {
			std::cout << "Bad key: " << argv[1] << std::endl;
		}
		if (((int)is_gpu + (int)is_cpu + (int)is_default) != 1) {
			std::cout << "Choose only one type of execution: on CPU (--cpu) or on GPU (--gpu) or DEFAULT (--default)" << std::endl;
			exit(0);
		}
	}
	bool is_gpu = false, is_cpu = false, is_default = false;
};
struct camera_settings {
	int width;
	int height;
	int scaled_width;
	int scaled_height;
	long long total_pixel_count;
	long long total_scaled_pixel_count;
	float angle;
	void init(int sqrt_rays_per_pixel) {
		scaled_height = height * sqrt_rays_per_pixel;
		scaled_width = width * sqrt_rays_per_pixel;
		total_pixel_count = (long long)height * width;
		total_scaled_pixel_count = (long long)scaled_height * scaled_width;
	}
} CAMERA_SETTINGS;
std::istream& operator>>(std::istream& in, camera_settings& cs) {
	return (in >> cs.width >> cs.height >> cs.angle);
}
struct cylindrical_coordinates {
private:
	float r, A_r, A_z, omega_r, omega_z, omega_phi, p_r, p_z, z, phi;
public:
	float rByT(float t) {
		return r + A_r * sinf(omega_r * t + p_r);
	}
	float zByT(float t) {
		return z + A_z * sinf(omega_z * t + p_z);
	}
	float phiByT(float t) {
		return phi + omega_phi * t;
	}
	float3 vecByT(float t) {
		float r = rByT(t);
		float phi = phiByT(t);
		float z = zByT(t);
		return { r * cosf(phi), r * sinf(phi), z };
	}
	friend std::istream& operator>>(std::istream& in, cylindrical_coordinates& cc);
};
std::istream& operator>>(std::istream& in, cylindrical_coordinates& cc) {
	return (in >> cc.r >> cc.z >> cc.phi >> cc.A_r >> cc.A_z >> cc.omega_r >> cc.omega_z >> cc.omega_phi >> cc.p_r >> cc.p_z);
}
struct camera_movement_settings {
	cylindrical_coordinates positionMovement;
	cylindrical_coordinates viewMovement;
	friend std::istream& operator>>(std::istream& in, camera_movement_settings& cms);
} CAMERA_MOVEMENT_SETTINGS;
std::istream& operator>>(std::istream& in, camera_movement_settings& cms) {
	return (in >> cms.positionMovement >> cms.viewMovement);
}
struct floor_settings {
	float3 point1, point2, point3, point4, color;
	std::string texturePath;
	float reflection;
} FLOOR;
std::istream& operator>>(std::istream& in, floor_settings& fs) {
	return (in >> fs.point1 >> fs.point2 >> fs.point3 >> fs.point4 >> fs.texturePath >> fs.color >> fs.reflection);
}
struct light {
	float3 position, color;
	float intensity = 1.5;
};
std::istream& operator>>(std::istream& in, light& l) {
	return (in >> l.position >> l.color);
}
struct base_figure {
	float3 position, color;
	float radius, reflection, transparency; // отражение прозрачность
	int countOfLights;
	bool isLight = false;
	static base_figure fromLight(const light& l) {
		auto light_cube = base_figure();
		light_cube.position = l.position;
		light_cube.color = l.color;
		light_cube.radius = 0.01f;
		light_cube.isLight = true;
		return light_cube;
	}
};
std::istream& operator>>(std::istream& in, base_figure& f) {
	return (in >> f.position >> f.color >> f.radius >> f.reflection >> f.transparency >> f.countOfLights);
}


int FRAMES_COUNT;
struct trig {
	float3 a;
	float3 b;
	float3 c;
	uchar4 color;
	bool isLight = false;
	float reflection;
	trig(float3 a_, float3 b_, float3 c_, uchar4 color_, bool isLight_, float reflection_) {
		a = a_;
		b = b_;
		c = c_;
		color = color_;
		isLight = isLight_;
		reflection = reflection_;
	}
};

std::vector<trig> TRIGS;
std::string OUTPUT_RENDERED_IMAGE_PATH;
base_figure FIGURE_1;
base_figure FIGURE_2;
base_figure FIGURE_3;
int LIGHTS_COUNT;
std::vector<light> LIGHTS;
int RECURSIVE_DEPTH;
int SQRT_OF_LIGHTS_COUNT_PER_PIXEL;

struct loading_cash {
	float prev_percent = 0.0;
	void init() {
		prev_percent = 0.0;
	}
	void print_label_if_percent_significantly_increased(float current_percent, int frame_num, long long pixel_num) {
		if (std::abs(current_percent - prev_percent) > 5.0 || prev_percent == 0.0) {
			prev_percent = current_percent;
			std::cout << "\rRendering " << frame_num + 1 << " of " << FRAMES_COUNT << " each containing ";
			std::cout << CAMERA_SETTINGS.total_scaled_pixel_count << " pixels.. [";
			int full_tens = (int)current_percent / 10;
			int last_tens = 10 - full_tens;
			int remainder = (int)current_percent % 10;
			for (int i = 0; i < full_tens; ++i) {
				std::cout << "*";
			}
			if (remainder != 0) {
				--last_tens;
				std::cout << "+";
			}
			for (int i = 0; i < last_tens; ++i) {
				std::cout << ".";
			}
			std::cout << "]" << std::setw(3) << (int)current_percent << "%";
		}
	}
} LOADING_CASH;

void input() {
#ifndef PRE_FILE_READING
	std::istream& in = std::cin;
#else
	std::ifstream in = std::ifstream("C:\\dev\\CUDA\\KP\\KP\\1\\test1.txt");
	if (!in.is_open()) {
		std::cout << "BAD FILE";
		exit(0);
	}
#endif
	in >> FRAMES_COUNT >> OUTPUT_RENDERED_IMAGE_PATH >> CAMERA_SETTINGS;
	in >> CAMERA_MOVEMENT_SETTINGS;
	in >> FIGURE_1 >> FIGURE_2 >> FIGURE_3;
	in >> FLOOR >> LIGHTS_COUNT;
	for (int i = 0; i < LIGHTS_COUNT; ++i) {
		light l;
		in >> l;
		LIGHTS.push_back(l);
	}
	in >> RECURSIVE_DEPTH >> SQRT_OF_LIGHTS_COUNT_PER_PIXEL;
}
void init() {
	CAMERA_SETTINGS.init(SQRT_OF_LIGHTS_COUNT_PER_PIXEL);
}
void default_calc() {
	std::cout << "100" << std::endl;
	std::cout << "./img_ % d.data 1280 720 40" << std::endl;
	std::cout << "9.0 3.0 0.0 0.0 2.0 0.0 2.0 1.2 1.0 0.0" << std::endl;
	std::cout << "0.0 0.0 0.0 0.0 0.5 3.0 5.0 0.1 3.0 0.2" << std::endl;
	std::cout << "-1.0 0.15 -0.5 0.5 0.5 0.4 0.1 0.8 0.2 5" << std::endl;
	std::cout << "-1.2 -1.5 -0.0 1.0 0.0 0.0 0.4 0.9 0.1 5" << std::endl;
	std::cout << "0.1 0.0 0.0 0.0 0.7 0.7 0.7 0.7 0.3 2" << std::endl;
	std::cout << "-5.0 -5.0 -1.0 -5.0 5.0 -1.0 5.0 5.0 -1.0 5.0 -5.0 -1.0 ~/floor.data 1.0 1.0 0.0 0.5" << std::endl;
	std::cout << "4" << std::endl;
	std::cout << "1.9 1.9 0.3 1.0 1.0 1.0" << std::endl;
	std::cout << "-0.5 -1.0 0.7	1.0 0.6 1.0" << std::endl;
	std::cout << "1.5 -0.8 0.4 1.0 1.0 0.0" << std::endl;
	std::cout << "-1.9 1.9 0.0 0.7 1.0 1.0" << std::endl;
	std::cout << "1 1" << std::endl;
}

__host__ std::pair<int, float> find_crossing(float3 pos, float3 dir, int igTrigIndex = -1, bool ignoreLights = false) {
	int k_min = -1;
	float ts_min;
	for (int k = 0; k < TRIGS.size(); k++) {
		if (k == igTrigIndex) {
			continue;
		}
		trig& curTrig = TRIGS[k];
		if (curTrig.isLight && ignoreLights) {
			continue;
		}
		float3 e1 = curTrig.b - curTrig.a;
		float3 e2 = curTrig.c - curTrig.a;
		float3 p = prod(dir, e2);
		float div = dot(p, e1);
		if (fabs(div) < 1e-10)
			continue;
		float3 t = pos - curTrig.a;
		float u = dot(p, t) / div;
		if (u < 0.0 || u > 1.0)
			continue;
		float3 q = prod(t, e1);
		float v = dot(q, dir) / div;
		if (v < 0.0 || v + u > 1.0)
			continue;
		float ts = dot(q, e2) / div;
		if (ts < 0.0)
			continue;
		if (k_min == -1 || ts < ts_min) {
			k_min = k;
			ts_min = ts;
		}
	}
	return { k_min, ts_min };
}


struct int_float_pair {
	int first;
	float second;
};
__device__ int_float_pair find_crossing_gpu(float3 pos, float3 dir, trig* trigs, long long trigsSize , int igTrigIndex = -1, bool ignoreLights = false) {
	int k_min = -1;
	float ts_min;
	for (int k = 0; k < trigsSize; k++) {
		if (k == igTrigIndex) {
			continue;
		}
		trig curTrig = trigs[k];
		if (curTrig.isLight && ignoreLights) {
			continue;
		}
		float3 e1 = curTrig.b - curTrig.a;
		float3 e2 = curTrig.c - curTrig.a;
		float3 p = prod(dir, e2);
		float div = dot(p, e1);
		if (fabs(div) < 1e-10)
			continue;
		float3 t = pos - curTrig.a;
		float u = dot(p, t) / div;
		if (u < 0.0 || u > 1.0)
			continue;
		float3 q = prod(t, e1);
		float v = dot(q, dir) / div;
		if (v < 0.0 || v + u > 1.0)
			continue;
		float ts = dot(q, e2) / div;
		if (ts < 0.0)
			continue;
		if (k_min == -1 || ts < ts_min) {
			k_min = k;
			ts_min = ts;
		}
	}
	return { k_min, ts_min };
}

__host__ uchar4 ray_cpu(float3 pos, float3 dir) {
	auto res = find_crossing(pos, dir);
	int k_min = res.first;
	float ts_min = res.second;

	if (k_min == -1) return { 0, 0, 0, 0 };

	trig& minTrig = TRIGS[k_min];
	
	if (!minTrig.isLight) {
		float3 normal = norm(prod(minTrig.a - minTrig.b, minTrig.a - minTrig.c));
		float3 point = pos + (dir * ts_min);
		float3 result_color{ 0.0f, 0.0f, 0.0f };
		for (light& l : LIGHTS) {
			float3 light_dir = norm(l.position - point);
			auto cross_ans = find_crossing(point, light_dir, k_min, true);
			if (cross_ans.first == -1) {
				float3 ambient = l.color * 0.2f;
				float diffusK = std::max(dot(normal, light_dir), 0.0f);
				float3 diffuse = l.color * minTrig.reflection * diffusK;
				float3 halfwayDir = norm(light_dir + (dir * -1));
				float specK = std::pow(std::max(dot(normal, halfwayDir), 0.0f), 64.0f);
				float3 specular = l.color * 0.7f * specK;
				float3 color;
				color.x = (float)minTrig.color.x / 255.0f;
				color.y = (float)minTrig.color.y / 255.0f;
				color.z = (float)minTrig.color.z / 255.0f;
				float3 result = (ambient + diffuse + specular) * color;
				float distToLight = len(l.position - point);
			if (distToLight > 0.7) {
				float distK = distToLight + 0.3f;
				result = result * (1.0f / (sqrt(distK * distK * distK)));
			}
				result_color = result_color + result * l.intensity;
			}
		}
		if (result_color.x > 1) result_color.x = 1;
		if (result_color.y > 1) result_color.y = 1;
		if (result_color.z > 1) result_color.z = 1;
		if (result_color.x < 0) result_color.x = 0;
		if (result_color.y < 0) result_color.x = 0;
		if (result_color.z < 0) result_color.x = 0;

		uchar x = uchar((float)(result_color.x) * 255.0f);
		uchar y = uchar((float)(result_color.y) * 255.0f);
		uchar z = uchar((float)(result_color.z) * 255.0f);
		return make_uchar4(x, y, z, minTrig.color.w);
	}
	else {
		return make_uchar4(minTrig.color.x, minTrig.color.y, minTrig.color.z, minTrig.color.w);
	}
}

__device__ float device_pow(float b, unsigned long long e) {
	float v = 1.0;
	while (e != 0) {
		if ((e & 1) != 0) {
			v *= b;
		}
		b *= b;
		e >>= 1;
	}
	return v;
}

__device__ uchar4 ray_gpu(float3 pos, float3 dir, trig* trigs, long long trigsSize, light* lights, long long lightsSize) {
	auto res = find_crossing_gpu(pos, dir, trigs, trigsSize);
	int k_min = res.first;
	float ts_min = res.second;

	if (k_min == -1) return { 0, 0, 0, 0 };

	trig minTrig = trigs[k_min];

	if (!minTrig.isLight) {
		float3 normal = norm(prod(minTrig.a - minTrig.b, minTrig.a - minTrig.c));
		float3 point = pos + (dir * ts_min);
		float3 result_color{ 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < lightsSize; ++i) {
			light l = lights[i];
			float3 light_dir = norm(l.position - point);
			auto cross_ans = find_crossing_gpu(point, light_dir, trigs, trigsSize, k_min, true);
			if (cross_ans.first == -1) {
				float3 ambient = l.color * 0.2;
				float diffusK = max(dot(normal, light_dir), 0.0f);
				float3 diffuse = l.color * minTrig.reflection * diffusK;
				float3 halfwayDir = norm(light_dir + (dir * -1));
				float specK = device_pow(max(dot(normal, halfwayDir), 0.0f), 64);
				float3 specular = l.color * 0.7f * specK;
				float3 color;
				color.x = (float)minTrig.color.x / 255.0f;
				color.y = (float)minTrig.color.y / 255.0f;
				color.z = (float)minTrig.color.z / 255.0f;
				float3 result = (ambient + diffuse + specular) * color;
				float distToLight = len(l.position - point);
				if (distToLight > 0.7) {
					float distK = distToLight + 0.3;
					result = result * (1.0f / (sqrt(distK * distK * distK)));
				}
				result_color = result_color + result * l.intensity;
			}
		}
		if (result_color.x > 1) result_color.x = 1;
		if (result_color.y > 1) result_color.y = 1;
		if (result_color.z > 1) result_color.z = 1;
		if (result_color.x < 0) result_color.x = 0;
		if (result_color.y < 0) result_color.x = 0;
		if (result_color.z < 0) result_color.x = 0;

		unsigned char x = (float)(result_color.x) * 255.0f;
		unsigned char y = (float)(result_color.y) * 255.0f;
		unsigned char z = (float)(result_color.z) * 255.0f;
		return make_uchar4(x, y, z, minTrig.color.w);
	}
	else {
		return make_uchar4(minTrig.color.x, minTrig.color.y, minTrig.color.z, minTrig.color.w);
	}
}

inline void rendering_label_cpu(int frame_num, long long pixel_num) {
	LOADING_CASH.print_label_if_percent_significantly_increased(
		(float)pixel_num * 100 / (float)CAMERA_SETTINGS.total_scaled_pixel_count, frame_num, pixel_num
	);
}

void render_cpu(float3 pc, float3 pv, int w, int h, float angle, uchar4* data, int frame_num) {
	int i, j;
	float dw = 2.0f / (w - 1.0f);
	float dh = 2.0f / (h - 1.0f);
	float z = 1.0f / tanf(angle * PI / 360.0f);
	float3 bz = norm(pv - pc);
	float3 bx = norm(prod(bz, { 0.0, 0.0, 1.0 }));
	float3 by = norm(prod(bx, bz));
	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++) {
			int pixel_num = i * h + j;
			rendering_label_cpu(frame_num, pixel_num);
			float3 v = { -1.0f + dw * (float)i, (-1.0f + dh * (float)j) * (float)h / (float)w, (float)z };
			float3 dir = mult(bx, by, bz, v);
			data[(h - 1 - j) * w + i] = ray_cpu(pc, norm(dir));
		}
}

__global__ void render_gpu(
	float3 pc, float3 pv, int w, int h, float angle, uchar4* data,
	trig* trigs, long long trigsSize,
	light* lights, long long lightsSize
) {
	const float PI = 3.141592653589;
	int i, j;
	float dw = 2.0f / (w - 1.0f);
	float dh = 2.0f / (h - 1.0f);
	float z = 1.0f / tanf(angle * PI / 360.0f);
	float3 bz = norm(pv - pc);
	float3 bx = norm(prod(bz, { 0.0, 0.0, 1.0 }));
	float3 by = norm(prod(bx, bz));
	int idX = blockDim.x * blockIdx.x + threadIdx.x;
	int offsetX = blockDim.x * gridDim.x;
	int idY = blockDim.y * blockIdx.y + threadIdx.y;
	int offsetY = blockDim.y * gridDim.y;
	for (i = idX; i < w; i+= offsetX)
		for (j = idY; j < h; j+=offsetY) {
			float3 v = { -1.0f + dw * (float)i, (-1.0f + dh * (float)j) * (float)h / (float)w, (float)z };
			float3 dir = mult(bx, by, bz, v);
			data[(h - 1 - j) * w + i] = ray_gpu(pc, norm(dir), trigs, trigsSize, lights, lightsSize);
		}
}

void cpu_ssaa(uchar4* picture, const uchar4* image, int w, int h, int sqrt_per_pixel) {
	if (sqrt_per_pixel <= 1) {
		memcpy(picture, image, sizeof(uchar4) * w * h);
		return;
	}
	int big_w = w * sqrt_per_pixel;

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			int start_y = i * sqrt_per_pixel;
			int start_x = j * sqrt_per_pixel;

			longlong3 mean = { 0, 0, 0 };

			// Compute single pixel of picture as average value of window.
			for (int n = start_y; n < start_y + sqrt_per_pixel; ++n) {
				for (int m = start_x; m < start_x + sqrt_per_pixel; ++m) {
					mean.x += image[n * big_w + m].x;
					mean.y += image[n * big_w + m].y;
					mean.z += image[n * big_w + m].z;
				}
			}
			mean.x /= ((long long)sqrt_per_pixel * sqrt_per_pixel);
			mean.y /= ((long long)sqrt_per_pixel * sqrt_per_pixel);
			mean.z /= ((long long)sqrt_per_pixel * sqrt_per_pixel);
			if (mean.x > 255)
				mean.x = 255;
			if (mean.y > 255)
				mean.y = 255;
			if (mean.z > 255)
				mean.z = 255;
			// Write value to picture
			picture[i * w + j] = make_uchar4((uchar)mean.x, (uchar)mean.y, (uchar)mean.z, 255);
		}
	}
}
__global__ void gpu_ssaa(uchar4* picture, const uchar4* image, int w, int h, int sqrt_per_pixel) {
	int big_w = w * sqrt_per_pixel;
	int idX = blockDim.x * blockIdx.x + threadIdx.x;
	int offsetX = blockDim.x * gridDim.x;
	int idY = blockDim.y * blockIdx.y + threadIdx.y;
	int offsetY = blockDim.y * gridDim.y;
	for (int i = idX; i < h; i+=offsetX) {
		for (int j = idY; j < w; j+=offsetY) {
			int start_y = i * sqrt_per_pixel;
			int start_x = j * sqrt_per_pixel;

			longlong3 mean = { 0, 0, 0 };

			// Compute single pixel of picture as average value of window.
			for (int n = start_y; n < start_y + sqrt_per_pixel; ++n) {
				for (int m = start_x; m < start_x + sqrt_per_pixel; ++m) {
					mean.x += image[n * big_w + m].x;
					mean.y += image[n * big_w + m].y;
					mean.z += image[n * big_w + m].z;
				}
			}
			mean.x /= ((long long)sqrt_per_pixel * sqrt_per_pixel);
			mean.y /= ((long long)sqrt_per_pixel * sqrt_per_pixel);
			mean.z /= ((long long)sqrt_per_pixel * sqrt_per_pixel);
			if (mean.x > 255)
				mean.x = 255;
			if (mean.y > 255)
				mean.y = 255;
			if (mean.z > 255)
				mean.z = 255;
			// Write value to picture
			picture[i * w + j] = make_uchar4((uchar)mean.x, (uchar)mean.y, (uchar)mean.z, 255);
		}
	}
}
void cpu_calc() {
	//chrono_timer::timer_sec timer;
	//timer.start();
	std::cout << "CPU CALCULATION\n";
	char buff[256];
	uchar4* data = (uchar4*)malloc(sizeof(uchar4) * CAMERA_SETTINGS.total_scaled_pixel_count);
	uchar4* data_out = (uchar4*)malloc(sizeof(uchar4) * CAMERA_SETTINGS.total_pixel_count);
	for (int frame_number = 0; frame_number < FRAMES_COUNT; frame_number++) {
		float t = SPEED * frame_number + OFFSET;
		float3 pc = CAMERA_MOVEMENT_SETTINGS.positionMovement.vecByT(t);
		float3 pv = CAMERA_MOVEMENT_SETTINGS.viewMovement.vecByT(t);
		render_cpu(
			pc, pv,
			CAMERA_SETTINGS.scaled_width, CAMERA_SETTINGS.scaled_height,
			CAMERA_SETTINGS.angle, data, frame_number
		);

		//ssaa
		cpu_ssaa(data_out, data, CAMERA_SETTINGS.width, CAMERA_SETTINGS.height, SQRT_OF_LIGHTS_COUNT_PER_PIXEL);

		sprintf(buff, OUTPUT_RENDERED_IMAGE_PATH.c_str(), frame_number);
		std::cout << "\r                                                                               ";
		std::cout << "\r" << frame_number + 1<< ": " << buff << "\n";

		FILE* out = fopen(buff, "wb");
		fwrite(&CAMERA_SETTINGS.width, sizeof(int), 1, out);
		fwrite(&CAMERA_SETTINGS.height, sizeof(int), 1, out);
		fwrite(data_out, sizeof(uchar4), (size_t)CAMERA_SETTINGS.width * CAMERA_SETTINGS.height, out);
		fclose(out);
	}
	free(data);
	//std::cout << "\nTIME SPENT : " << timer.time() << "min\n";
}
void gpu_calc() {
	std::cout << "GPU CALCULATION\n";
	//chrono_timer::timer_sec timer;
	//timer.start();
	char buff[256];
	uchar4* data = new uchar4[CAMERA_SETTINGS.total_scaled_pixel_count];
	uchar4* data_out = new uchar4[CAMERA_SETTINGS.total_pixel_count];
	uchar4* data_dev = nullptr, * data_out_dev = nullptr;
	trig* trigs_dev;
	light* lights_dev;

	cudaMalloc(&data_dev, sizeof(uchar4) * CAMERA_SETTINGS.total_scaled_pixel_count);

	for (int frame_number = 0; frame_number < FRAMES_COUNT; frame_number++) {
		float t = SPEED * frame_number + OFFSET;
		float3 pc = CAMERA_MOVEMENT_SETTINGS.positionMovement.vecByT(t);
		float3 pv = CAMERA_MOVEMENT_SETTINGS.viewMovement.vecByT(t);
		
		cudaMalloc(&trigs_dev, sizeof(trig) * TRIGS.size());
		cudaMalloc(&lights_dev, sizeof(light) * LIGHTS.size());
		cudaMemcpy(trigs_dev, TRIGS.data(), sizeof(trig) * TRIGS.size(), cudaMemcpyHostToDevice);
		cudaMemcpy(lights_dev, LIGHTS.data(), sizeof(light) * LIGHTS.size(), cudaMemcpyHostToDevice);
		render_gpu <<< BLOCKS, THREADS >>> (
			pc, pv,
			CAMERA_SETTINGS.scaled_width, CAMERA_SETTINGS.scaled_height,
			CAMERA_SETTINGS.angle, data_dev,
			trigs_dev, TRIGS.size(),
			lights_dev, LIGHTS.size()
		);
		auto err = cudaGetLastError();
		CEH(err);
		cudaFree(lights_dev);
		cudaFree(trigs_dev);
		//ssaa
		if (SQRT_OF_LIGHTS_COUNT_PER_PIXEL > 1) {
			cudaMalloc(&data_out_dev, sizeof(uchar4) * CAMERA_SETTINGS.total_pixel_count);
			gpu_ssaa << < BLOCKS, THREADS >> > (data_out_dev, data_dev, CAMERA_SETTINGS.width, CAMERA_SETTINGS.height, SQRT_OF_LIGHTS_COUNT_PER_PIXEL);
			cudaMemcpy(data_out, data_out_dev, sizeof(uchar4) * CAMERA_SETTINGS.total_pixel_count, cudaMemcpyDeviceToHost);
			cudaFree(data_out_dev);
		}
		else {
			cudaMemcpy(data_out, data_dev, sizeof(uchar4) * CAMERA_SETTINGS.total_pixel_count, cudaMemcpyDeviceToHost);
		}
		sprintf(buff, OUTPUT_RENDERED_IMAGE_PATH.c_str(), frame_number);
		std::cout << "\r                                                                               ";
		std::cout << "\r" << "["  << frame_number + 1<< '/' << FRAMES_COUNT <<  "]: " << buff;

		FILE* out = fopen(buff, "wb");
		fwrite(&CAMERA_SETTINGS.width, sizeof(int), 1, out);
		fwrite(&CAMERA_SETTINGS.height, sizeof(int), 1, out);
		fwrite(data_out, sizeof(uchar4), (size_t)CAMERA_SETTINGS.width * CAMERA_SETTINGS.height, out);
		fclose(out);
	}

	cudaFree(data_dev);
	delete[] data_out;
	delete[] data;
	//std::cout << "\nTIME SPENT : " << timer.time() << "sec\n";
}


__host__ void add_cube(base_figure figure) {
	std::vector<float3> ver_mas;
	float R = figure.radius;
	ver_mas.push_back({ R / (2 * cosf(30)), R / (2 * cosf(30)), -R / (2 * cosf(30)) });
	ver_mas.push_back({ -R / (2 * cosf(30)), R / (2 * cosf(30)), -R / (2 * cosf(30)) });
	ver_mas.push_back({ -R / (2 * cosf(30)), -R / (2 * cosf(30)), -R / (2 * cosf(30)) });
	ver_mas.push_back({ R / (2 * cosf(30)), -R / (2 * cosf(30)), -R / (2 * cosf(30)) });

	ver_mas.push_back({ R / (2 * cosf(30)), R / (2 * cosf(30)), R / (2 * cosf(30)) });
	ver_mas.push_back({ -R / (2 * cosf(30)), R / (2 * cosf(30)), R / (2 * cosf(30)) });
	ver_mas.push_back({ -R / (2 * cosf(30)), -R / (2 * cosf(30)),  R / (2 * cosf(30)) });
	ver_mas.push_back({ R / (2 * cosf(30)), -R / (2 * cosf(30)),  R / (2 * cosf(30)) });


	for (auto& el : ver_mas) {
		el.x += figure.position.x;
		el.y += figure.position.y;
		el.z += figure.position.z;
	}

	uchar4 color = { 
		uchar(figure.color.x * 255.0f),
		uchar(figure.color.y * 255.0f),
		uchar(figure.color.z * 255.0f),
		255
	};

	bool isLight = figure.isLight;
	TRIGS.push_back(trig{ ver_mas[1], ver_mas[4], ver_mas[0], color, isLight, figure.reflection });//-
	TRIGS.push_back(trig{ ver_mas[5], ver_mas[4], ver_mas[1], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[1], ver_mas[2], ver_mas[5], color, isLight, figure.reflection });//+
	TRIGS.push_back(trig{ ver_mas[2], ver_mas[6], ver_mas[5], color, isLight, figure.reflection });//+

	TRIGS.push_back(trig{ ver_mas[3], ver_mas[6], ver_mas[2], color, isLight, figure.reflection });//-
	TRIGS.push_back(trig{ ver_mas[7], ver_mas[6], ver_mas[3], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[7], ver_mas[3], ver_mas[0], color, isLight, figure.reflection });//+
	TRIGS.push_back(trig{ ver_mas[4], ver_mas[7], ver_mas[0], color, isLight, figure.reflection });//+

	TRIGS.push_back(trig{ ver_mas[4], ver_mas[5], ver_mas[6], color, isLight, figure.reflection });//+
	TRIGS.push_back(trig{ ver_mas[6], ver_mas[7], ver_mas[4], color, isLight, figure.reflection });//+

	TRIGS.push_back(trig{ ver_mas[0], ver_mas[2], ver_mas[1], color, isLight, figure.reflection });//+
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[3], ver_mas[2], color, isLight, figure.reflection });//+
}
__host__ void add_icosahedron(base_figure figure) // определить массив вершин
{
	// начальные значения
	float R = figure.radius;
	float a = 4 * R / sqrtf(10 + 2 * sqrtf(5)); // сторона икосаэдра
	float alpha = acosf((1 - a * a / 2 / R / R)); // первый угол поворота по тэта
	float k = PI / 180.0f;
	// точки икосаэдра
	std::vector<float3> ver_mas;
	ver_mas.push_back({ 0,0, R });
	ver_mas.push_back(float3{ R * sinf(alpha) * sinf(0), R * sinf(alpha) * cosf(0), R * cosf(alpha) });
	ver_mas.push_back({ R * sinf(alpha) * sinf(72 * k), R * sinf(alpha) * cosf(72 * k), R * cosf(alpha) });
	ver_mas.push_back({ R * sinf(alpha) * sinf(2 * 72 * k), R * sinf(alpha) * cosf(2 * 72 * k), R * cosf(alpha) });
	ver_mas.push_back({ R * sinf(alpha) * sinf(3 * 72 * k), R * sinf(alpha) * cosf(3 * 72 * k), R * cosf(alpha) });
	ver_mas.push_back({ R * sinf(alpha) * sinf(4 * 72 * k), R * sinf(alpha) * cosf(4 * 72 * k), R * cosf(alpha) });
	ver_mas.push_back({ R * sinf(PI - alpha) * sinf(-36 * k), R * sinf(PI - alpha) * cosf(-36 * k), R * cosf(PI - alpha) });
	ver_mas.push_back({ R * sinf(PI - alpha) * sinf(36 * k), R * sinf(PI - alpha) * cosf(36 * k),  R * cosf(PI - alpha) });
	ver_mas.push_back({ R * sinf(PI - alpha) * sinf((36 + 72) * k), R * sinf(PI - alpha) * cosf((36 + 72) * k), R * cosf(PI - alpha) });
	ver_mas.push_back({ R * sinf(PI - alpha) * sinf((36 + 2 * 72) * k), R * sinf(PI - alpha) * cosf((36 + 2 * 72) * k), R * cosf(PI - alpha) });
	ver_mas.push_back({ R * sinf(PI - alpha) * sinf((36 + 3 * 72) * k), R * sinf(PI - alpha) * cosf((36 + 3 * 72) * k), R * cosf(PI - alpha) });
	ver_mas.push_back({ 0, 0, -R });

	for (auto& el : ver_mas) {
		el.x += figure.position.x;
		el.y += figure.position.y;
		el.z += figure.position.z;
	}

	bool isLight = figure.isLight;
	uchar4 color = { 
		uchar(figure.color.x * 255.0f),
		uchar(figure.color.y * 255.0f), 
		uchar(figure.color.z * 255.0f),
		255
	};
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[2], ver_mas[1], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[3], ver_mas[2], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[4], ver_mas[3], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[5], ver_mas[4], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[1], ver_mas[5], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[6], ver_mas[1], ver_mas[7], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[7], ver_mas[1], ver_mas[2], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[7], ver_mas[2], ver_mas[8], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[8], ver_mas[2], ver_mas[3], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[8], ver_mas[3], ver_mas[9], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[9], ver_mas[3], ver_mas[4], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[9], ver_mas[4], ver_mas[10], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[10], ver_mas[4], ver_mas[5], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[10], ver_mas[5], ver_mas[6], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[6], ver_mas[5], ver_mas[1], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[7], ver_mas[11], ver_mas[6], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[8], ver_mas[11], ver_mas[7], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[9], ver_mas[11], ver_mas[8], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[10], ver_mas[11], ver_mas[9], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[6], ver_mas[11], ver_mas[10], color, isLight, figure.reflection });
}
__host__ void add_octahedron(base_figure figure) {
	std::vector<float3> ver_mas;
	float R = figure.radius;
	ver_mas.push_back({ 0, 0, R });
	ver_mas.push_back({ 0, 0, -R });
	ver_mas.push_back({ R * sinf(45), R * cosf(45), 0 });
	ver_mas.push_back({ -R * sinf(45), R * cosf(45), 0 });
	ver_mas.push_back({ -R * sinf(45), -R * cosf(45), 0 });
	ver_mas.push_back({ R * sinf(45), -R * cosf(45), 0 });

	for (auto& el : ver_mas) {
		el.x += figure.position.x;
		el.y += figure.position.y;
		el.z += figure.position.z;
	}

	uchar4 color = { 
		uchar(figure.color.x * 255.0f),
		uchar(figure.color.y * 255.0f),
		uchar(figure.color.z * 255.0f),
		255
	};

	bool isLight = figure.isLight;
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[3], ver_mas[2], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[4], ver_mas[3], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[5], ver_mas[4], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[0], ver_mas[2], ver_mas[5], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[1], ver_mas[2], ver_mas[3], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[1], ver_mas[3], ver_mas[4], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[1], ver_mas[4], ver_mas[5], color, isLight, figure.reflection });
	TRIGS.push_back(trig{ ver_mas[1], ver_mas[5], ver_mas[2], color, isLight, figure.reflection });
}
__host__ void add_floor() {
	uchar4 color = { 
		uchar(FLOOR.color.x * 255.0f),
		uchar(FLOOR.color.y * 255.0f),
		uchar(FLOOR.color.z * 255.0f),
		255 
	};
	TRIGS.push_back(trig{ FLOOR.point1, FLOOR.point3, FLOOR.point2, color, false, FLOOR.reflection });
	TRIGS.push_back(trig{ FLOOR.point1, FLOOR.point4, FLOOR.point3, color, false, FLOOR.reflection });
}

void build_space() {
	add_cube(FIGURE_1);
	add_octahedron(FIGURE_2);
	add_icosahedron(FIGURE_3);
	for (int i = 0; i < LIGHTS_COUNT; ++i) {
		add_cube(base_figure::fromLight(LIGHTS[i]));
	}
	add_floor();
}

int main(int argc, char* argv[]) {
	std::ios::sync_with_stdio(false);
	key_config key_cfg(argc, argv);

	input();
	init();
	build_space();

	if (key_cfg.is_default) {
		//возвращаем наиболее красивый результат
		default_calc();
	}
	else if (key_cfg.is_cpu) {
		//считаем на cpu
		cpu_calc();
	}
	else {
		//считаем на gpu
		gpu_calc();
	}
	//system("python big_conv.py \\out");
	//system("C:\\dev\\CUDA\\KP\\KP\\1\\ffmpeg-master-latest-win64-gpl-shared\\bin\\ffmpeg.exe -y -i C:\\dev\\CUDA\\KP\\KP\\1\\out\\img_%d.jpg -loop 0 -filter:v setpts=2*PTS C:\\dev\\CUDA\\KP\\KP\\1\\out\\res.gif");
	exit(0);
}
