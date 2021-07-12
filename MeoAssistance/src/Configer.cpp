#include "Configer.h"

#include <fstream>
#include <sstream>
#include <Windows.h>

#include "json.h"

using namespace asst;

json::object Configer::dataObj;
json::object Configer::handleObj;
json::object Configer::optionsObj;

std::string Configer::m_curDir;

bool Configer::reload()
{
	std::string filename = getResDir() + "config.json";
	std::ifstream ifs(filename, std::ios::in);
	if (!ifs.is_open()) {
		return false;
	}
	std::stringstream iss;
	iss << ifs.rdbuf();
	ifs.close();
	std::string content(iss.str());

	auto ret = json::parser::parse(content);
	if (!ret) {
		return false;
	}

	auto root = std::move(ret).value();
	dataObj = root["data"].as_object();
	handleObj = root["handle"].as_object();
	optionsObj = root["options"].as_object();


	return true;
}

std::string Configer::getCurDir()
{
	if (m_curDir.empty()) {
		char exepath_buff[_MAX_PATH] = { 0 };
		::GetModuleFileNameA(NULL, exepath_buff, _MAX_PATH);
		std::string exepath(exepath_buff);
		m_curDir = exepath.substr(0, exepath.find_last_of('\\') + 1);
	}
	return m_curDir;
}

std::string Configer::getResDir()
{
	return getCurDir() + "resource\\";
}