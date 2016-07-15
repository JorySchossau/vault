#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>

// example usage (fields definition block)
//#define FIELDS \
//FIELD(bool, born) \
//FIELD(int, age) \
//FIELD_STATIC(float, time) \
//FIELD_STATIC(std::string, experiment)
//
//#include "vault/vault.h" // make sure this is after the definition block

namespace VAULT {
	// struct declaration
	struct Record {
#define STRINGIFY( ... ) STRINGIFYL2( __VA_ARGS__ )
#define STRINGIFYL2( ... ) #__VA_ARGS__
#define FIELD( type, name ) type name;
#define FIELD_STATIC( type, name ) static type name;
		FIELDS
#include "data_undef.h"
#define FIELD( type, name ) << rec.name << " "
#define FIELD_STATIC( type, name ) << name << " "
		friend std::ostream&  operator<<(std::ostream& os, const Record& rec) {
			std::stringstream ss;
			ss.str("");
			ss FIELDS;
			/// remove trailing space and write string to output
			ss.seekp(-1,ss.cur);
			ss << std::endl;
			os << ss.rdbuf();
			return os;
		};
#include "data_undef.h"
	};
	// struct static member declarations
#include "data_def.h"
#undef FIELD_STATIC
#define FIELD_STATIC( type, name ) type Record::name;
		FIELDS
#include "data_undef.h"
	
	namespace _priv {
		std::list<Record> _records;
	}

	Record* newRecord() {
		VAULT::_priv::_records.emplace_back();
		return &*std::prev(VAULT::_priv::_records.end());
	}

	Record& back() {
		return VAULT::_priv::_records.back();
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
#define FIELD( type, name ) name
#define FIELD_STATIC( type, name ) name
		if ((fileExists == false) || ((overwrite == true) && (fileExists == true))) {
			out << STRINGIFY(FIELDS) << std::endl;
		}
#include "data_undef.h"
		/// write data rows
		for (auto& record : _priv::_records) {
			out << record;
		}
	}
}
