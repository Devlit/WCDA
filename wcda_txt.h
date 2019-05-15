#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using std::vector;
using std::pair;
using std::string;
using std::fstream;
using std::cout;
using std::endl;
using std::cin;
using std::stringstream;
using val_range = pair<double, double>;

struct PMT_Info {
	string id;
	string nHIT;
	string time_first;
	string time_from_corsika;
	vector<pair<string, string>> hit_info;
};

struct SelectedItemInfo {
	// line1
	string id_prim;
	string E_prim;
	string theta_prim;
	string phi_prim;
	// line2
	string x_prim;
	string y_prim;
	// line3
	string z_first_inter;
	string num;
	string number_of_weighted_particlesber_of_particles;
	string first_time;
	// line4
	string n_EM;
	string EM_energy;
	string n_MU;
	string MU_energy;
	string N_Hadron;
	string Hadron_energy;
	// line5
	string nPMT;

	vector<PMT_Info> pmt_info;
};

class Wcda_txt
{
public:
	static Wcda_txt* getInstance() {
		if (m_instance == nullptr)
		{
			m_instance = new Wcda_txt();
		}
		return m_instance;
	}
	std::string version()
	{
		return m_version;
	}

	bool transformAllFiles(const std::string& input_dir, const std::string& ouput_dir);
	void transformBatchedFiles(const vector<string>& input_files, const vector<string>& output_files);
	bool transformSingleFile(const std::string& input_file, const std::string& ouput_file);
	
private:
	Wcda_txt();
	Wcda_txt(const Wcda_txt&) {}
	Wcda_txt& operator=(const Wcda_txt&) {}
	static Wcda_txt* m_instance;
	const std::string m_version = "1.8_8inch_Gamma_1.e12_1.e13";
	const int header_lines_count = 117;

	bool scanFile(const string& file_name, vector<SelectedItemInfo>& output);
	bool extractData(const vector<SelectedItemInfo>& data_info, const string & output_file);

	void update_range(val_range& val, const string& new_val_str);
	void update_range(const SelectedItemInfo& info);
	void init_range(val_range& val);
	void save_range();


	int n_threads = 100;

	val_range id_prim;
	val_range E_prim;
	val_range theta_prim;
	val_range phi_prim;
	// line2
	val_range x_prim;
	val_range y_prim;
	// line3
	val_range z_first_inter;
	val_range num;
	val_range number_of_weighted_particlesber_of_particles;
	val_range first_time;
	// line4
	val_range n_EM;
	val_range EM_energy;
	val_range n_MU;
	val_range MU_energy;
	val_range N_Hadron;
	val_range Hadron_energy;

	
};