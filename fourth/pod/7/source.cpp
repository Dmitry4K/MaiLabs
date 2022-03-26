#include <stdio.h>
#include <mpi.h>
#include <fstream>
#include<iostream>
#include <iomanip>
#include<sstream>
#include<string>
#include<cmath>
#include <omp.h>


#define _i(i, j, k) (k+1)*(block_x + 2)*(block_y + 2) + (j + 1)*(block_x + 2) + (i + 1)
#define _i_glob(i, j, k) k*proc_grid_x*proc_grid_y + j*proc_grid_x + i
const int tag_up = 1;
const int tag_down = 2;
const int tag_left = 3;
const int tag_right = 4;
const int tag_front = 5;
const int tag_back = 6;

struct ijk {
	int i;
	int j;
	int k;
};

void cerr_p(int id, std::string str) {
	//std::cerr << std::to_string(id) + " " + str + '\n';
	return;
}

ijk id_to_ijk_glob(int id, int block_x_glob, int block_y_glob, int block_z_glob) {
	ijk res;
	res.k = id / (block_x_glob * block_y_glob);
	int tmp = id % (block_x_glob * block_y_glob);
	res.j = tmp / block_x_glob;
	res.i = tmp % block_x_glob;
	return res;
}

ijk id_to_ijk_block(int id, int block_x, int block_y, int block_z) {
	ijk res;
	res.k = id / (block_x * block_y);
	int tmp = id % (block_x * block_y);
	res.j = tmp / block_x;
	res.i = tmp % block_x;
	return res;
}

int id_up(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	tmp.k += 1;
	int id_send = _i_glob(tmp.i, tmp.j, tmp.k);
	return id_send;
}

int id_down(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	tmp.k -= 1;
	int id_send = _i_glob(tmp.i, tmp.j, tmp.k);
	return id_send;
}

int id_left(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	tmp.i -= 1;
	int id_send = _i_glob(tmp.i, tmp.j, tmp.k);
	return id_send;
}

int id_right(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	tmp.i += 1;
	int id_send = _i_glob(tmp.i, tmp.j, tmp.k);
	return id_send;
}

int id_front(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	tmp.j -= 1;
	int id_send = _i_glob(tmp.i, tmp.j, tmp.k);
	return id_send;
}

int id_back(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	tmp.j += 1;
	int id_send = _i_glob(tmp.i, tmp.j, tmp.k);
	return id_send;
}

bool is_smth_up(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	if (tmp.k == proc_grid_z - 1) {
		return false;
	}
	else {
		return true;
	}
}

bool is_smth_down(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	if (tmp.k == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool is_smth_left(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	if (tmp.i == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool is_smth_right(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	if (tmp.i == proc_grid_x - 1) {
		return false;
	}
	else {
		return true;
	}
}

bool is_smth_front(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	if (tmp.j == 0) {
		return false;
	}
	else {
		return true;
	}
}

bool is_smth_back(int id, int proc_grid_x, int proc_grid_y, int proc_grid_z) {
	ijk tmp = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
	if (tmp.j == proc_grid_y - 1) {
		return false;
	}
	else {
		return true;
	}
}

double max_diff(double* block, double* new_block, int block_x, int block_y, int block_z) {
	double res = 0.0;

	for (int k = 0; k < block_z; k++) {
		for (int j = 0; j < block_y; j++) {
			for (int i = 0; i < block_x; i++) {
				res = std::max(res, std::abs(block[_i(i, j, k)] - new_block[_i(i, j, k)]));
			}
		}
	}
	return res;
}

void send_to_up_recieve_from_down(
	double* block, int id, double u_down,
	int block_x, int block_y, int block_z,
	int proc_grid_x, int proc_grid_y, int proc_grid_z
) {
	MPI_Request req;
	double* buf = new double[block_x * block_y];
	int count = 0;
	MPI_Status status;

	if (is_smth_up(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		for (int i = 0; i < block_x; i++) {
			for (int j = 0; j < block_y; j++) {
				buf[count] = block[_i(i, j, block_z - 1)];
				count++;
			}
		}


		MPI_Isend(buf, block_x * block_y, MPI_DOUBLE, id_up(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_down, MPI_COMM_WORLD, &req);//����� ������� ���� ��� ������
		MPI_Wait(&req, &status);
	}
	count = 0;
	if (is_smth_down(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		MPI_Irecv(buf, block_x * block_y, MPI_DOUBLE, id_down(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_down, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
		for (int i = 0; i < block_x; i++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(i, j, -1)] = buf[count];
				count++;
			}
		}
	}
	else {
		for (int i = 0; i < block_x; i++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(i, j, -1)] = u_down;
			}
		}
	}

	delete[] buf;
	return;
}

void send_to_down_recieve_from_up(
	double* block, int id, double u_up,
	int block_x, int block_y, int block_z,
	int proc_grid_x, int proc_grid_y, int proc_grid_z
) {
	MPI_Request req;
	double* buf = new double[block_x * block_y];
	int count = 0;
	MPI_Status status;
	if (is_smth_down(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		for (int i = 0; i < block_x; i++) {
			for (int j = 0; j < block_y; j++) {
				buf[count] = block[_i(i, j, 0)];
				count++;
			}
		}



		MPI_Isend(buf, block_x * block_y, MPI_DOUBLE, id_down(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_up, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
	}
	count = 0;
	if (is_smth_up(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		MPI_Irecv(buf, block_x * block_y, MPI_DOUBLE, id_up(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_up, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
		for (int i = 0; i < block_x; i++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(i, j, block_z)] = buf[count];
				count++;
			}
		}
	}
	else {
		for (int i = 0; i < block_x; i++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(i, j, block_z)] = u_up;
			}
		}
	}

	delete[] buf;
	return;
}


void send_to_left_recieve_from_right(
	double* block, int id, double u_right,
	int block_x, int block_y, int block_z,
	int proc_grid_x, int proc_grid_y, int proc_grid_z
) {
	MPI_Request req;
	MPI_Status status;
	double* buf = new double[block_z * block_y];
	int count = 0;
	if (is_smth_left(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		for (int k = 0; k < block_z; k++) {
			for (int j = 0; j < block_y; j++) {
				buf[count] = block[_i(0, j, k)];
				count++;
			}
		}


		MPI_Isend(buf, block_z * block_y, MPI_DOUBLE, id_left(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_right, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
	}
	count = 0;
	if (is_smth_right(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		MPI_Irecv(buf, block_z * block_y, MPI_DOUBLE, id_right(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_right, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);

		for (int k = 0; k < block_z; k++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(block_x, j, k)] = buf[count];
				count++;
			}
		}
	}
	else {
		for (int k = 0; k < block_z; k++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(block_x, j, k)] = u_right;
			}
		}
	}

	delete[] buf;
	return;
}


void send_to_right_recieve_from_left(
	double* block, int id, double u_left,
	int block_x, int block_y, int block_z,
	int proc_grid_x, int proc_grid_y, int proc_grid_z
) {
	MPI_Request req;
	MPI_Status status;
	double* buf = new double[block_z * block_y];
	int count = 0;
	if (is_smth_right(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		for (int k = 0; k < block_z; k++) {
			for (int j = 0; j < block_y; j++) {
				buf[count] = block[_i(block_x - 1, j, k)];
				count++;
			}
		}


		MPI_Isend(buf, block_z * block_y, MPI_DOUBLE, id_right(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_left, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
	}
	count = 0;
	if (is_smth_left(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		MPI_Irecv(buf, block_z * block_y, MPI_DOUBLE, id_left(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_left, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);

		for (int k = 0; k < block_z; k++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(-1, j, k)] = buf[count];
				count++;
			}
		}
	}
	else {
		for (int k = 0; k < block_z; k++) {
			for (int j = 0; j < block_y; j++) {
				block[_i(-1, j, k)] = u_left;
			}
		}
	}

	delete[] buf;
	return;
}



void send_to_front_recieve_from_back(
	double* block, int id, double u_back,
	int block_x, int block_y, int block_z,
	int proc_grid_x, int proc_grid_y, int proc_grid_z
) {
	MPI_Request req;
	MPI_Status status;
	double* buf = new double[block_x * block_z];
	int count = 0;
	if (is_smth_front(id, proc_grid_x, proc_grid_y, proc_grid_z)) {

		for (int k = 0; k < block_z; k++) {
			for (int i = 0; i < block_x; i++) {
				buf[count] = block[_i(i, 0, k)];
				count++;
			}
		}
		MPI_Isend(buf, block_z * block_x, MPI_DOUBLE, id_front(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_back, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
	}
	count = 0;
	if (is_smth_back(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		MPI_Irecv(buf, block_z * block_x, MPI_DOUBLE, id_back(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_back, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
		for (int k = 0; k < block_z; k++) {
			for (int i = 0; i < block_x; i++) {
				block[_i(i, block_y, k)] = buf[count];
				count++;
			}
		}
	}
	else {
		for (int k = 0; k < block_z; k++) {
			for (int i = 0; i < block_x; i++) {
				block[_i(i, block_y, k)] = u_back;
			}
		}
	}

	delete[] buf;
	return;
}


void send_to_back_recieve_from_front(
	double* block, int id, double u_front,
	int block_x, int block_y, int block_z,
	int proc_grid_x, int proc_grid_y, int proc_grid_z
) {
	MPI_Request req;
	MPI_Status status;
	double* buf = new double[block_x * block_z];
	int count = 0;
	if (is_smth_back(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		for (int k = 0; k < block_z; k++) {
			for (int i = 0; i < block_x; i++) {
				buf[count] = block[_i(i, block_y - 1, k)];
				count++;
			}
		}

		MPI_Status status;

		MPI_Isend(buf, block_z * block_x, MPI_DOUBLE, id_back(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_front, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
	}
	count = 0;
	if (is_smth_front(id, proc_grid_x, proc_grid_y, proc_grid_z)) {
		MPI_Irecv(buf, block_z * block_x, MPI_DOUBLE, id_front(id, proc_grid_x, proc_grid_y, proc_grid_z), tag_front, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
		for (int k = 0; k < block_z; k++) {
			for (int i = 0; i < block_x; i++) {
				block[_i(i, -1, k)] = buf[count];
				count++;
			}
		}
	}
	else {
		for (int k = 0; k < block_z; k++) {
			for (int i = 0; i < block_x; i++) {
				block[_i(i, -1, k)] = u_front;
			}
		}
	}

	delete[] buf;
	return;
}

using namespace std;


int main(int argc, char* argv[]) {

	int id, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Barrier(MPI_COMM_WORLD);
	int proc_grid_x, proc_grid_y, proc_grid_z;
	int block_x, block_y, block_z;
	string output;
	double eps;
	double* block, * new_block;
	double l_x, l_y, l_z;
	double u_down, u_up, u_left, u_right, u_front, u_back, u_0;


	if (id == 0) {
		cin >> proc_grid_x >> proc_grid_y >> proc_grid_z >> block_x >> block_y >> block_z
			>> output
			>> eps
			>> l_x >> l_y >> l_z
			>> u_down >> u_up >> u_left >> u_right >> u_front >> u_back >> u_0;
	}
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Bcast(&proc_grid_x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&proc_grid_y, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&proc_grid_z, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&block_x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&block_y, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&block_z, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&eps, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&l_x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&l_y, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&l_z, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_down, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_up, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_left, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_right, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_front, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_back, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&u_0, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);


	char tmp_char = ' ';
	int str_len = output.length();
	MPI_Bcast(&str_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (id != 0) {
		output.resize(str_len);
	}
	for (int i = 0; i < str_len; ++i) {
		if (id == 0) {
			tmp_char = output[i];
		}
		MPI_Bcast(&tmp_char, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
		output[i] = tmp_char;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double h_x, h_y, h_z;
	h_x = l_x / (double)(proc_grid_x * block_x);
	h_y = l_y / (double)(proc_grid_y * block_y);
	h_z = l_z / (double)(proc_grid_z * block_z);


	if (id == 0) std::cerr
		<< proc_grid_x << ' ' << proc_grid_y << ' ' << proc_grid_z << '\n'
		<< block_x << ' ' << block_y << ' ' << block_z << '\n'
		<< output << ' ' << eps << '\n'
		<< l_x << ' ' << l_y << ' ' << l_z << '\n'
		<< u_down << ' ' << u_up << ' ' << u_left << ' ' << u_right << ' ' << u_front << ' ' << u_back << ' ' << u_0 << '\n';

	block = new double[(block_x + 2) * (block_y + 2) * (block_z + 2)];
	new_block = new double[(block_x + 2) * (block_y + 2) * (block_z + 2)];
	for (int i = -1; i <= block_x; i++) {
		for (int j = -1; j <= block_y; j++) {
			for (int k = -1; k <= block_z; k++) {
				block[_i(i, j, k)] = u_0;
			}
		}
	}

	double diff = eps;
	double global_diff = eps;
	int num_threads = omp_get_num_threads();
	if (id == 0) std::cerr << num_threads;
	double* diffs = new double[num_threads];
	for (int i = 0; i < num_threads; ++i) {
		diffs[i] = 0.0;
	}

	if (id == 0) std::cerr << "start calc\n";
	while (global_diff >= eps) {
		MPI_Barrier(MPI_COMM_WORLD);
		send_to_up_recieve_from_down(block, id, u_down, block_x, block_y, block_z, proc_grid_x, proc_grid_y, proc_grid_z);
		send_to_down_recieve_from_up(block, id, u_up, block_x, block_y, block_z, proc_grid_x, proc_grid_y, proc_grid_z);
		send_to_left_recieve_from_right(block, id, u_right, block_x, block_y, block_z, proc_grid_x, proc_grid_y, proc_grid_z);
		send_to_right_recieve_from_left(block, id, u_left, block_x, block_y, block_z, proc_grid_x, proc_grid_y, proc_grid_z);
		send_to_back_recieve_from_front(block, id, u_front, block_x, block_y, block_z, proc_grid_x, proc_grid_y, proc_grid_z);
		send_to_front_recieve_from_back(block, id, u_back, block_x, block_y, block_z, proc_grid_x, proc_grid_y, proc_grid_z);
		MPI_Barrier(MPI_COMM_WORLD);
		#pragma omp parallel
		{
			int thread_num = omp_get_thread_num();
			for (int number = thread_num; number < block_x * block_y * block_z; number += num_threads) {
				ijk ijk_in_block = id_to_ijk_block(number, block_x, block_y, block_z);
				int i = ijk_in_block.i, j = ijk_in_block.j, k = ijk_in_block.k;
				new_block[_i(i, j, k)] =
					(((block[_i(i + 1, j, k)] + block[_i(i - 1, j, k)]) / (h_x * h_x))
						+ ((block[_i(i, j + 1, k)] + block[_i(i, j - 1, k)]) / (h_y * h_y))
						+ ((block[_i(i, j, k + 1)] + block[_i(i, j, k - 1)]) / (h_z * h_z)))
					/ (2.0 * ((1.0 / (h_x * h_x)) + (1.0 / (h_y * h_y)) + (1.0 / (h_z * h_z))));
				diffs[number % num_threads] = std::max(diffs[number % num_threads], std::abs(new_block[_i(i, j, k)] - block[_i(i, j, k)]));
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		diff = diffs[0];
		diffs[0] = 0.0;
		for (int i = 1; i < num_threads; ++i) {
			diff = std::max(diffs[i], diff);
			diffs[i] = 0.0;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		double* tmp = block;
		block = new_block;
		new_block = tmp;
		MPI_Allreduce(&diff, &global_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
	}


	MPI_Barrier(MPI_COMM_WORLD);

	if (id == 0) cerr << "start printing\n";
	MPI_Datatype filetype;
	MPI_File fp;
	MPI_File_delete(output.c_str(), MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, output.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
	
	for (int k = 0; k < block_z; ++k) {
		const int buffer_size = 20;	
		std::stringstream ss;
		ss << scientific << setprecision(7);
		for (int j = 0; j < block_y; ++j) {
			for (int i = 0; i < block_x; i++) {
				int prev = ss.str().length();
				ss << block[_i(i, j, k)];
				int after = ss.str().length();
				for (int n = 0; n < buffer_size - (after - prev) - 1; ++n) {
					ss << ' ';
				}
				if ((i == block_x - 1) &&
					id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z).i == proc_grid_x - 1
					) {
					ss << '\n';
				}
				else {
					ss << ' ';
				}
			}
		}
		int stride = proc_grid_x * block_x * buffer_size * sizeof(char);
		MPI_Type_create_hvector(block_y, buffer_size * block_x, stride, MPI_CHAR, &filetype);
		MPI_Type_commit(&filetype);
		ijk ijk_glob = id_to_ijk_glob(id, proc_grid_x, proc_grid_y, proc_grid_z);
		int offset =
			ijk_glob.k * proc_grid_x * proc_grid_y * block_x * block_y * block_z +
			ijk_glob.j * proc_grid_x * block_x * block_y +
			ijk_glob.i * block_x
			+ k * proc_grid_x * proc_grid_y * block_x * block_y;
		offset *= buffer_size * sizeof(char);
		//cerr_p(id, std::to_string(ijk_glob.i) + " " + std::to_string(ijk_glob.j) + " " + std::to_string(ijk_glob.k));
		//cerr_p(id, std::to_string(offset) + " " + std::to_string(stride));
		MPI_File_set_view(fp, offset, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
		MPI_File_write_all(fp, ss.str().c_str(), ss.str().length(), MPI_CHAR, MPI_STATUS_IGNORE);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_File_close(&fp);

	if (id == 0) cerr << "end printing\n";
	delete[] block;
	delete[] new_block;

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	return 0;
}
