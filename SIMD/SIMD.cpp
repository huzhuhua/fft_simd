// SIMD.cpp: 定义控制台应用程序的入口点。
// Target: Be familiar with the SIMD command.

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <intrin.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
using namespace std;
#define PI 3.141592653589793238462643383279502884
int standard_dft( vector<double>dft_in, vector<complex<double>> &dft_out );
int fft(vector<double> &fft_in, vector<complex<double>> &fft_out);
int main()
{
	/*----------------------------------------------------------------
	*	SIMD test.
	* -Task 1. Implement standard DFT algorithm with C++.
	* Task 2. Rewrite the DFT with Cooley–Tukey FFT algorithm.
	* Task 3. Ues SIMD to accelerate the FFT alogrithm.
	* Task 4. Make a time consuming contrast among these algorithms.
	----------------------------------------------------------------*/
	//const double  PI = acos(-1.0);

	// read the testing data.
	ifstream fi("fft_1024.txt");
	vector<double> data;
	string temp;
	while (fi.good())
	{
		getline(fi,temp);
		data.push_back( stod(temp));
	}
	vector<complex<double>> result(1024);
	//	standard_dft(data, result); // Task 1.
	cout << "start\n";
	fft(data, result);	// Task 2.
	cout << "end \n";
	// output the result
	ofstream fo("fft_out_ct.txt", ios::ate);
	for (auto per_data : result) {
		fo << per_data.real() <<" + "<< per_data.imag()<<"j"<< "\n";
	}

	fi.close();
	fo.close();
	

    return 0;
}

int standard_dft(vector<double> dft_in, vector<complex<double>> &dft_out)
{
	// Standard fft algorithm without any optimization.
	// Ref:https://en.wikipedia.org/wiki/DFT

	int N = dft_in.size();
	for (size_t k = 0; k != N; ++k) {
		//complex<double> x_n(0, 0);
		double re = 0;												// the real part
		double im = 0;												// the imag part
		for (size_t n = 0; n != N; ++n) {
			re += dft_in[n] * cos(-double(k * 2 * PI * n / N));		// Euler's formula： exp( i*x ) = cos_x + i*sin_x
			im += dft_in[n] * sin(-double(k * 2 * PI * n / N));
			// x_n += fft_in[n] * exp( (double)k *-1i * 2.0*PI* (double)( n )/double( N) );	
			// Be careful for the integer division  in C++
		}

		dft_out.push_back(complex<double>(re, im));
	}
	return 0;
}

int fft(vector<double> &fft_in, vector<complex<double>>& fft_out)
{
	int N = fft_in.size();		
	if (N > 2) {

		// 分成两部分
		vector<double> odd;		// 奇数部分
		vector<double> even;	// 偶数部分
		if (N % 2 == 0) {
			for (size_t n = 0; n != N / 2; ++n) {
				even.push_back(fft_in.at(n * 2));
				odd.push_back(fft_in.at(n * 2 + 1));
			}
		}
		else
		{
			for (size_t n = 0; n != N / 2; ++n) {
				even.push_back(fft_in.at(n * 2));
				odd.push_back(fft_in.at(n * 2 + 1));
			}
			even.push_back(fft_in.at(2 * (N / 2)));
		}
		// 递归处理
		//fft(even, fft_out);
		//fft(odd, fft_out);

		// 合并
		complex<double> odd_out;
		complex<double> even_out;
		for (size_t n = 0; n != N / 2; ++n) {
			odd_out = fft_out[n + N / 2];
			even_out = fft_out[n];
			//complex<double> w = exp(1i*double(-2 * PI *n) / (double)(N));
			complex<double> w = exp( complex<double>(0, double(-2 * PI *n) / (double)(N)) );
			fft_out[n] = even_out + w * odd_out;	// even part
			fft_out[n + N / 2] = even_out - w * odd_out;	// odd part

		}
	}
	else
		return 0;
	return 0;
}
