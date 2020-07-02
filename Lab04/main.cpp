#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "PNM_ex.h"
#include "Functs.h"
#include <stdlib.h>

using namespace std;


int main(int argc, char *argv[]) {

	if (argc != 11) {
		cerr << "Wrong number of arguments!";
		return 1;
	}

	//обработка аргументов

	vector<FILE *>fin;
	int from = -1;
	int to = -1;
	int count = 0;
	int num = 0;
	map <string, int> space_numbers = { {"RGB", 0}, {"HSL", 1}, {"HSV", 2}, {"YCbCr.601", 3},
										{"YCbCr.709", 4}, {"YCoCg", 5}, {"CMY", 6} };
	//RGB - 0, HSL - 1, HSV - 2, YCbCr.601 - 3, YCbCr.709 - 4, YCoCg - 5, CMY - 6
	string name;
	for (int i = 1; i < argc;) {
		switch (argv[i][1]) {
		case 'f':
			i++;
			from = space_numbers.at(argv[i]);
			i++;
			break;
		case 't':
			i++;
			to = space_numbers.at(argv[i]);
			i++;
			break;
		case 'i':
			i++;
			count = atoi(argv[i]);
			i++;
			name = argv[i];
			char buf[1];
			for (int st = i; i < st + count; i++) {
				itoa(i - st + 1, buf, 10);
				string new_name = name;
				if (count == 3) {
					new_name.insert(new_name.find_last_of('.'), "_");
					new_name.insert(new_name.find_last_of('.'), buf);
				}
				fin.push_back(fopen(new_name.c_str(), "rb")); 
				if (!fin[i - st]) {
					cerr << "Input file error!\n";
					cerr << argv[i] << '\n';
					return 1;
				}
			}
			break;
		case 'o':
			i++;
			num = atoi(argv[i]);
			i++;
			i += num;
			break;
		default:
			cerr << "Wrong operation!";
			return 1;
			
		}
	}

	//инициализация PNM-файлов

	vector<PNM_File> files(count);
	for (int i = 0; i < count; i++) {
		try {
			files[i] = PNM_File(fin[i]);
		}
		catch (exception err) {
			fclose((fin[i]));
			cerr << err.what();
			return 1;
		}
		fclose(fin[i]);
	}

	//переход в другое цветовое пространство
	
	convert(files, from, to);

	//считка и открытие выходных файлов

	vector <FILE *> fout;
	for (int i = 1; i < argc;) {
		if (argv[i][1] == 'o' && argv[i][0] == '-') {
			i++;
			count = atoi(argv[i]);
			i++;
			string name = argv[i];
			char buf[1];
			for (int st = i; i < st + count; i++) {
				itoa(i - st + 1, buf, 10);
				string new_name = name;
				if (count == 3) {
					new_name.insert(new_name.find_last_of('.'), "_");
					new_name.insert(new_name.find_last_of('.'), buf);
				}
				fout.push_back(fopen(new_name.c_str(), "wb"));
				if (!fout[i - st]) {
					cerr << "Output file error!\n";
					cerr << argv[i] << '\n';
					return 1;
				}
			}
			
		} else {
			i++;
		}
	}

	vector <PNM_File> res;
	//запись и закрытие выходных файлов
	if (count != files.size()) {
		if (count == 3) {
			res.push_back(PNM_File(files[0], 0));
			res.push_back(PNM_File(files[0], 1));
			res.push_back(PNM_File(files[0], 2));
		} else {
			res.push_back(PNM_File(files));
		}
	} else {
		res = files;
	}
	for (int i = 0; i < count; i++) {
		res[i].write(fout[i]);
	}
	return 0;
}