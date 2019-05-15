#include "wcda_txt.h"
#include <locale>
#include <limits>
#include <thread>
#include "utils.h"

Wcda_txt::Wcda_txt()
{
	init_range(id_prim);
	init_range(E_prim);
	init_range(theta_prim);
	init_range(phi_prim);

	init_range(x_prim);
	init_range(y_prim);

	init_range(z_first_inter);
	init_range(num);
	init_range(number_of_weighted_particlesber_of_particles);
	init_range(first_time);

	init_range(n_EM);
	init_range(EM_energy);
	init_range(n_MU);
	init_range(MU_energy);
	init_range(N_Hadron);
	init_range(Hadron_energy);
}

Wcda_txt* Wcda_txt::m_instance = nullptr;

bool Wcda_txt::transformAllFiles(const std::string & input_dir, const std::string & ouput_dir)
{
	auto&& files = listDir(input_dir);

	for (auto& file:files)
	{
		auto&& input_path = path_join(input_dir, file);
		auto&& ouput_path = path_join(ouput_dir, file);
		transformSingleFile(input_path, ouput_path);
	}
	
	save_range();
	return true;
}

bool Wcda_txt::transformSingleFile(const std::string & input_file, const std::string & ouput_file)
{
	vector<SelectedItemInfo> data_info;
	if (!scanFile(input_file, data_info)) {
		cout << "error in <bool scanFile(const string& file_name, vector<SelectedItemInfo>& output)>" << endl;
		return false;
	}

	if (!this->extractData(data_info, ouput_file)) {
		cout << "error in <bool extractData(const vector<SelectedItemInfo>& data_info, const string & output_file)>" << endl;

		return false;
	}

	cout << input_file << " extracted!" << endl;
	return true;
}



void Wcda_txt::transformBatchedFiles(const vector<string>& input_paths, const vector<string>& output_paths)
{
	for (int i = 0; i < input_paths.size(); i++)
		transformSingleFile(input_paths[i], output_paths[i]);
}

bool Wcda_txt::scanFile(const string & file_name, vector<SelectedItemInfo>& output)
{
	fstream g4_file(file_name);
	if (!g4_file.is_open())
	{
		cout << "open file failed!" << endl;
		return false;
	}

	char buffer[256] = {};


	int idx = 0;
	while (idx++ < header_lines_count)
		g4_file.getline(buffer, 256);

	while (!g4_file.eof())
	{
		// 1
		g4_file.getline(buffer, 256);
		vector<string> line1 = split(buffer);
		if (line1.size() != 4)
			break;
		////cout << buffer << endl;

		g4_file.getline(buffer, 256);
		vector<string> line2 = split(buffer);
		////cout << buffer << endl;

		g4_file.getline(buffer, 256);
		vector<string> line3 = split(buffer);
		////cout << buffer << endl;

		g4_file.getline(buffer, 256);
		vector<string> line4 = split(buffer);
		////cout << buffer << endl;

		SelectedItemInfo selected_info;
		// line1
		selected_info.id_prim = line1.at(0);
		selected_info.E_prim = line1.at(1);
		selected_info.theta_prim = line1.at(2);
		selected_info.phi_prim = line1.at(3);
		// line2
		selected_info.x_prim = line2.at(0);
		selected_info.y_prim = line2.at(1);
		// line3
		selected_info.z_first_inter = line3.at(0);
		selected_info.num = line3.at(1);
		selected_info.number_of_weighted_particlesber_of_particles = line3.at(2);
		selected_info.first_time = line3.at(3);
		// line4
		selected_info.n_EM = line4.at(0);
		selected_info.EM_energy = line4.at(1);
		selected_info.n_MU = line4.at(2);
		selected_info.MU_energy = line4.at(3);
		selected_info.N_Hadron = line4.at(4);
		selected_info.Hadron_energy = line4.at(5);


		// 2
		g4_file.getline(buffer, 256);
		vector<string> line5_npmt = split(buffer);
		//cout << buffer << endl;


		if (line5_npmt.size() == 0)
			break;

		if (line5_npmt.at(0) != "0")
		{
			selected_info.nPMT = line5_npmt.at(0);
			int nPmt = 0;
			str2any(selected_info.nPMT,nPmt);

			for (int i = 0; i < nPmt; i++)
			{
				g4_file.getline(buffer, 256);
				//cout << buffer << endl;
				vector<string> pmt_info_line = split(buffer);
				PMT_Info pmt_info;
				pmt_info.id = pmt_info_line.at(0);
				pmt_info.nHIT = pmt_info_line.at(1);
				pmt_info.time_first = pmt_info_line.at(2);
				pmt_info.time_from_corsika = pmt_info_line.at(3);
				int nHit = 0;
				str2any(pmt_info.nHIT, nHit);


				for (int j = 0; j < nHit; j++)
				{
					g4_file.getline(buffer, 256);
					//cout << buffer << endl;
					vector<string> items = split(buffer);
					pair<string, string> time_pair;
					str2any(items.at(0), time_pair.first);
					str2any(items.at(1), time_pair.second);
					pmt_info.hit_info.push_back(time_pair);
				}// for
				selected_info.pmt_info.push_back(pmt_info);
			}// for

			output.push_back(selected_info);
		} //if
		else {
			selected_info.nPMT = "0";
			output.push_back(selected_info);
		}

		// nPrints
		g4_file.getline(buffer, 256);
		//cout << buffer << endl;
		vector<string> items2 = split(buffer);
		int print_lines = 0;
		str2any(items2.at(0), print_lines);
		if (print_lines == 0)
			continue;
		else
		{
			while (print_lines--) {
				g4_file.getline(buffer, 256);
				//cout << buffer << endl;
			}
		}

	}// while
	g4_file.close();
	return true;
}

bool Wcda_txt::extractData(const vector<SelectedItemInfo>& data_info, const string & output_file)
{
	fstream file(output_file, std::ios::out | std::ios::app);

	if (file.fail())
		return false;

	for (auto& info : data_info) {
		update_range(info);

		if (info.nPMT == "0")
			continue;

		file << info.id_prim << " " << info.E_prim << " " << info.theta_prim << " " << info.phi_prim << " ";

		// line2
		file << info.x_prim << " " << info.y_prim << " ";

		// line3
		file << info.z_first_inter << " " << info.num << " " << info.number_of_weighted_particlesber_of_particles << " " << info.first_time << " ";

		// line4
		file << info.n_EM << " " << info.n_MU << " " << info.MU_energy << " " << info.N_Hadron << " " << info.Hadron_energy << endl;

		// line5
		//file << info.nPMT << endl;

		for (auto& pmt : info.pmt_info) {
			file << pmt.id << " " << pmt.nHIT << endl;
			for (auto& hit : pmt.hit_info) {
				file << hit.first<<endl;
			}
		}
	}
	file.close();
	return true;
}

void Wcda_txt::update_range(val_range & val, const string& new_val_str)
{
	double new_val = 0.0;
	str2any(new_val_str, new_val);

	if (new_val > val.second)
		val.second = new_val;
	if (new_val < val.first)
		val.first = new_val;
}

void Wcda_txt::update_range(const SelectedItemInfo & info)
{
	update_range(id_prim,info.id_prim);
	update_range(E_prim,info.E_prim);
	update_range(theta_prim,info.theta_prim);
	update_range(phi_prim,info.phi_prim);
	update_range(x_prim,info.x_prim);
	update_range(y_prim,info.y_prim);
	update_range(z_first_inter,info.z_first_inter);
	update_range(num,info.num);
	update_range(number_of_weighted_particlesber_of_particles,info.number_of_weighted_particlesber_of_particles);
	update_range(first_time,info.first_time);
	update_range(n_EM,info.n_EM);
	update_range(n_MU,info.n_MU);
	update_range(MU_energy,info.MU_energy);
	update_range(N_Hadron,info.N_Hadron);
	update_range(Hadron_energy,info.Hadron_energy);
}

void Wcda_txt::init_range(val_range & val)
{
	val.first = std::numeric_limits<double>::max();
	val.second = std::numeric_limits<double>::min();
}

void Wcda_txt::save_range()
{
	fstream file("corsika_vals_range.txt", std::ios::out);
	file << "id_prim:" << id_prim.first << "," << id_prim.second << endl;
	file << "E_prim:" << E_prim.first << "," << E_prim.second << endl;
	file << "theta_prim:" << theta_prim.first << "," << theta_prim.second << endl;
	file << "phi_prim:" << phi_prim.first << "," << phi_prim.second << endl;
	file << "x_prim:" << x_prim.first << "," << x_prim.second << endl;
	file << "y_prim:" << y_prim.first << "," << y_prim.second << endl;
	file << "z_first_inter:" << z_first_inter.first << "," << z_first_inter.second << endl;
	file << "num:" << num.first << "," << num.second << endl;
	file << "number_of_weighted_particlesber_of_particles:" << number_of_weighted_particlesber_of_particles.first << "," << number_of_weighted_particlesber_of_particles.second << endl;
	file << "first_time:" << first_time.first << "," << first_time.second << endl;
	file << "n_EM:" << n_EM.first << "," << n_EM.second << endl;
	file << "EM_energy:" << EM_energy.first << "," << EM_energy.second << endl;
	file << "n_MU:" << n_MU.first << "," << n_MU.second << endl;
	file << "MU_energy:" << MU_energy.first << "," << MU_energy.second << endl;
	file << "N_Hadron:" << N_Hadron.first << "," << N_Hadron.second << endl;
	file << "Hadron_energy:" << Hadron_energy.first << "," << Hadron_energy.second << endl;
	file.close();
}
