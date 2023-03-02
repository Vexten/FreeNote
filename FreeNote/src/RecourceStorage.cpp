#include "../headers/RecourceStorage.h"

using namespace std::filesystem;
using namespace Freenote;

std::stringstream ss;
std::string f_name[2]{ "", "" };
char i = 0;

void parse_file(const directory_entry& dir_ent, std::map<std::string, Resource>* folder) {
	// WARNING: Below code assumes *.* filename format and WILL ignore everything
	// between dots in the middle of a filename
	ss << dir_ent.path().filename().string();
	std::getline(ss, f_name[i++], '.');
	//might break if there are no dots
	while (std::getline(ss, f_name[i], '.'));
	i = 0;
	//one if for all supported types
	{
		if (f_name[1] == "obj") {
			std::ifstream file(dir_ent.path());
			(*folder)[f_name[0]].shape_verts = new std::vector<SDL_Vertex>(LoadShape::load_vertex_data_from_obj(file, SDL_Color{ 255,255,255,255 }));
			(*folder)[f_name[0]].shape_indices = new std::vector<int>(LoadShape::load_ind_data_from_obj(file));
		}
	}
	ss.clear();
	f_name[0] = ""; f_name[1] = "";
}

void parse_dir(const directory_entry& dir, std::map<std::string, Resource>* folder) {
	for (auto const& dir_ent : directory_iterator(dir)) {
		if (dir_ent.is_regular_file()) {
			parse_file(dir_ent, folder);
		}
		if (dir_ent.is_directory()) {
			std::string dir = dir_ent.path().filename().string();
			(*folder)[dir].folder = new std::map<std::string, Resource>();
			parse_dir(dir_ent, (*folder)[dir].folder);
		}
	}
}

ResourceStorage::ResourceStorage(std::string application_path) {
	path res_path = path(application_path);
	res_path /= "resources";
	if (!exists(res_path)) throw RESOURCE_STORAGE_FOLDER_DOES_NOT_EXIST;
	parse_dir(directory_entry(res_path),&resource_tree);
}

Resource& ResourceStorage::operator[](std::string name)
{
	return resource_tree[name];
}
