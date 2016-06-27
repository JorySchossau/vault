#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cstddef>
#include <fstream>

// example usage (fields definition block)
//#define FIELDS \
//FIELD_INT(born) \
//FIELD_INT(age) \
//FIELD_STATIC_INT(time) \
//FIELD_STATIC_STRING(experiment)
//
//#include "vault/vault.h" // make sure this is after the definition block

namespace VAULT {
	// struct declaration
	#include "data_undef.h"
	#define FIELD_INT(name) int name;
	#define FIELD_FLOAT(name) float name;
	#define FIELD_STRING(name) std::string name;
	#define FIELD_STATIC_INT(name) static int name;
	#define FIELD_STATIC_FLOAT(name) static float name;
	#define FIELD_STATIC_STRING(name) static std::string name;
	struct Record {
		FIELDS
	};
	#include "data_undef.h"
	// struct static member declarations
	#include "data_def.h"
	#undef FIELD_STATIC_INT
	#undef FIELD_STATIC_FLOAT
	#undef FIELD_STATIC_STRING
	#define FIELD_STATIC_INT(name) int Record::name;
	#define FIELD_STATIC_FLOAT(name) float Record::name;
	#define FIELD_STATIC_STRING(name) std::string Record::name;
		FIELDS
	#include "data_undef.h"
	
	namespace _priv {
	// int names
	#include "data_def.h"
	#undef FIELD_INT
	#define FIELD_INT(name) #name,
	std::vector<std::string> names_int = {
		FIELDS
	};
	#include "data_undef.h"

	// float names
	#include "data_def.h"
	#undef FIELD_FLOAT
	#define FIELD_FLOAT(name) #name,
	std::vector<std::string> names_float = {
		FIELDS
	};
	#include "data_undef.h"

	// string names
	#include "data_def.h"
	#undef FIELD_STRING
	#define FIELD_STRING(name) #name,
	std::vector<std::string> names_string = {
		FIELDS
	};
	#include "data_undef.h"

	// static int names
	#include "data_def.h"
	#undef FIELD_STATIC_INT
	#define FIELD_STATIC_INT(name) #name,
	std::vector<std::string> names_static_int = {
		FIELDS
	};
	#include "data_undef.h"

	// static float names
	#include "data_def.h"
	#undef FIELD_STATIC_FLOAT
	#define FIELD_STATIC_FLOAT(name) #name,
	std::vector<std::string> names_static_float = {
		FIELDS
	};
	#include "data_undef.h"

	// static string names
	#include "data_def.h"
	#undef FIELD_STATIC_STRING
	#define FIELD_STATIC_STRING(name) #name,
	std::vector<std::string> names_static_string = {
		FIELDS
	};
	#include "data_undef.h"
	
	// int offsets
	#include "data_def.h"
	#undef FIELD_INT
	#define FIELD_INT(name) offsetof(Record, name),
	std::vector<size_t> offsets_int = {
		FIELDS
	};
	#include "data_undef.h"

	// float offsets
	#include "data_def.h"
	#undef FIELD_FLOAT
	#define FIELD_FLOAT(name) offsetof(Record, name),
	std::vector<size_t> offsets_float = {
		FIELDS
	};
	#include "data_undef.h"

	// string offsets
	#include "data_def.h"
	#undef FIELD_STRING
	#define FIELD_STRING(name) offsetof(Record, name),
	std::vector<size_t> offsets_string = {
		FIELDS
	};
	#include "data_undef.h"

	// addresses of static ints
	#include "data_def.h"
	#undef FIELD_STATIC_INT
	#define FIELD_STATIC_INT(name) (std::uintptr_t)&Record::name,
	std::vector<std::uintptr_t> addresses_static_int = {
		FIELDS
	};
	#include "data_undef.h"

	// addresses of static floats
	#include "data_def.h"
	#undef FIELD_STATIC_FLOAT
	#define FIELD_STATIC_FLOAT(name) (std::uintptr_t)&Record::name,
	std::vector<std::uintptr_t> addresses_static_float = {
		FIELDS
	};
	#include "data_undef.h"

	// addresses of static strings
	#include "data_def.h"
	#undef FIELD_STATIC_STRING
	#define FIELD_STATIC_STRING(name) (std::uintptr_t)&Record::name,
	std::vector<std::uintptr_t> addresses_static_string = {
		FIELDS
	};
	#include "data_undef.h"

	std::list<Record> _records;
	}

	Record* newRecord() {
		VAULT::_priv::_records.emplace_back();
		return &*std::prev(VAULT::_priv::_records.end());
	}

	void save(std::string filename, bool overwrite=false) {
		std::ios::openmode mode = (overwrite) ? std::ios::out : std::ios::out|std::ios::app;
		/// check if file exists
		std::ifstream filechecker(filename.c_str());
		bool fileExists = filechecker.is_open();
		if (fileExists) filechecker.close();
		/// begin writing file
		std::ofstream out(filename.c_str(), mode);
		/// only write header if file doesn't exist or overwrite is true
		if ((fileExists == false) || ((overwrite == true) && (fileExists == true))) {
			for (auto& name : _priv::names_static_int) out << name << " ";
			for (auto& name : _priv::names_static_float) out << name << " ";
			for (auto& name : _priv::names_static_string) out << name << " ";
			for (auto& name : _priv::names_int) out << name << " ";
			for (auto& name : _priv::names_float) out << name << " ";
			for (auto& name : _priv::names_string) out << name << " ";
			out << std::endl;
		}
		for (auto& record : _priv::_records) {
			for (auto& address : _priv::addresses_static_int) out << *reinterpret_cast<int*>(address) << " ";
			for (auto& address : _priv::addresses_static_float) out << *reinterpret_cast<float*>(address) << " ";
			for (auto& address : _priv::addresses_static_string) out << *reinterpret_cast<std::string*>(address) << " ";
			for (auto& offset : _priv::offsets_int) out << *reinterpret_cast<int*>((char*)&record+offset) << " ";
			for (auto& offset : _priv::offsets_float) out << *reinterpret_cast<float*>((char*)&record+offset) << " ";
			for (auto& offset : _priv::offsets_string) out << *reinterpret_cast<std::string*>((char*)&record+offset) << " ";
			out << std::endl;
		}
	}
}
