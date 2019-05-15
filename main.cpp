#include "wcda_txt.h"
#include <iostream>
#include <time.h>
using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "please input like this:" << endl;
		cout << " C:\\input_dir C:\\ouput_dir" << endl;
		return -1;
	}
	
	clock_t start, finish;
	double totaltime;
	start = clock();
	std::string input_dir = argv[1];
	std::string output_dir = argv[2];
	cout << input_dir << endl;
	cout << output_dir << endl;
	auto&& wcda = Wcda_txt::getInstance();
	wcda->transformAllFiles(input_dir, output_dir);

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "time=" << totaltime << "s." << endl;
	return 0;
}