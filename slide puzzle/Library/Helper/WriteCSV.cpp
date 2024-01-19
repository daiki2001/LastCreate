#include "WriteCSV.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;
void WriteCSV(const std::string& ward, const size_t& mapSize, const std::string& FilePath) {
	ofstream ofs(FilePath);
	ofs << ward << endl;
}
