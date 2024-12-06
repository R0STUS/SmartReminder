#include <iostream>
#include <fstream>
#include <nlohmann/json>

using Json = nlohmann::json;

struct RemindDay {
    unsigned long time;
    bool isReminded;
    std::string name;
    int typeOfDay;
};

struct Config {
	float defVolume;
};

Json getJson(const std::string& path) {
	Json output;
	std::ifstream file(path);
	try {
		output = Json::parse(file);
	} catch (const Json::parse_exception& e) {
		std::cout << "ERROR! " << e.what() << std::endl;
	}
	return output;
}

Config jtconfig(Json& jconfig) {
	Config output;
	output.defVolume = jconfig["defVolume"];
	return output;
}

int main() {
    std::cout << "Initialising core..." << std::endl;
	Json jconfig = getJson("res/config.json");
	Config config = jtconfig(jconfig);
}