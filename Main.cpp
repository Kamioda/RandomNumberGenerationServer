#if !defined NOMINMAX
#define NOMINMAX
#endif
#include "Random.hpp"
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <iostream>
#include <filesystem>
#include <fstream>

inline nlohmann::json ReadJson(const std::filesystem::path& FilePath) {
	if (!std::filesystem::exists(FilePath)) throw std::runtime_error(FilePath.string() + " : file is not found.");
	std::ifstream ifs(FilePath);
	if (ifs.fail()) throw std::runtime_error(FilePath.string() + " : File load error.");
	const std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();
	return nlohmann::json::parse(json);
}

int main() {
	try {
		const nlohmann::json json = ReadJson("./config.json");
		httplib::Server server{};
		server.Get("/", 
			[](const httplib::Request& Req, httplib::Response& Res) {
				try {
					const auto maxIt = Req.params.find("max");
					const auto minIt = Req.params.find("min");
					const long long Max = maxIt == Req.params.end() ? std::numeric_limits<long long>::max() : std::stoll(maxIt->second);
					const long long Min = minIt == Req.params.end() ? std::numeric_limits<long long>::min() : std::stoll(minIt->second);
					if (Max < Min) {
						Res.status = 400;
						Res.set_content("'min' is larger than 'max'", "text/plain");
						return;
					}
					std::uniform_int_distribution<long long> rand(Min, Max);
					Res.status = 200;
					Res.set_content(std::to_string(rand(Random::GetEngine())), "text/plain");
				}
				catch (const std::exception& er) {
					Res.status = 500;
					std::cout << er.what() << std::endl;
				}
			}
		);
		std::cout << "If you want to stop this server, please press [Ctrl + C]." << std::endl;
		server.listen(json["host"].get<std::string>().c_str(), json["port"].get<int>());
	}
	catch (const std::exception& er) {
		std::cout << er.what() << std::endl;
	}
	return 0;
}
