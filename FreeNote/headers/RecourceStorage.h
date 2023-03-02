#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <map>
#include "Shape.h"
#include "Errors.h"

namespace Freenote {
	
	struct Resource {

		// add more here if needed 
		#define type(...) __VA_ARGS__
		#define RESOURCES \
		X(type(std::vector<SDL_Vertex>*), shape_verts) \
		X(type(std::vector<int>*), shape_indices) \
		X(type(SDL_Texture*), tex) \
		X(type(std::map<std::string, Resource>*), folder)

		#define X(t, n) t n = nullptr;
		RESOURCES
		#undef X

		Resource& operator[](std::string name) {
			return (*folder)[name];
		}

		~Resource() {
			#define X(t,n) if (n != nullptr) delete n;
			RESOURCES
			#undef X
		}

		#undef RESOURCES
		#undef type
	};

	class ResourceStorage {

	private:
		std::map<std::string, Resource> resource_tree;

	public:
		ResourceStorage(std::string application_path);
		Resource& operator[](std::string name);
	};
}