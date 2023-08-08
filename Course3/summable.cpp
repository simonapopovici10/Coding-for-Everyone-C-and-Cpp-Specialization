//   Convert this program to C++

//  change to C++ io

//  change to one line comments

//  change defines of constants to const

//  change array to vector<>

//  inline any short function



#include <iostream>
#include <vector>

using namespace std;

const int N = 40;

template <typename T>
T const& sum(T& accum, const T size, const vector<int>& data) {
	accum = 0;

	for(int i = 0; i < size; ++i)
		accum += data[i];

	return accum;
}

int main(int argc, char** argv) {
	int accum = 0;

	vector<int> data(N);

	for(int i = 0; i < N; ++i)
		data[i] = i;

	sum(accum, N, data);

	cout << "sum is " << accum << endl;

	return 0;
}
