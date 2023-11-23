#include "Auth.hpp"
#include "HTTP/HTTPSRequest.hpp"
#include "Core/Logger.hpp"
#define websiteAddress "http://192.168.56.1:8000"
//#define websiteAddress "https://foxycheats.lol"

namespace base {
	void auth::init() {
		#if defined(DISABLE_SECURITY)
		if (!g_auth.m_isLoggedIn)
			g_auth.m_isLoggedIn = true;
		#else
		security::eraseHeaders(g_module);
		if (!m_isLoggedIn) {
			g_auth.m_isLoggedIn = true;
			return;
			if (auto path = std::filesystem::path(std::getenv(xorstr_("appdata"))).append(xorstr_("Foxy\\Cache")).append(xorstr_("login.file")); std::filesystem::exists(path)) {
				std::ifstream file(path);
				std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
				file.close();
				if (!content.empty()) {
					strcpy(m_username, content.substr(0, content.find(xorstr_("__"))).c_str());
					strcpy(m_password, content.substr(content.find(xorstr_("__")) + 2).c_str());
					m_isLoggedIn = authenticate();
				}
				else {
					LOG(Info, "File is empty!");
					//exit(0);
				}
				if (!m_isLoggedIn) {
					LOG(Info, "Auth failed");
					//exit(0);
				}
			}
			else {
				LOG(Fatal, "You're missing the cache folder, please reinject...");
				//exit(0);
			}
		}
		#endif
	}
	bool auth::updateData(std::string data, std::string value) {
		curlWrapper curl{};
		std::vector<std::string> headers{
			xorstr_("Accept: text/html"),
			xorstr_("Content-Type: application/x-www-form-urlencoded")
		};
		std::string res{ curl.post(xorstr_(websiteAddress"/auth/update.php"), std::format("username={}&password={}&data_type={}&value={}", m_username, m_password, data, value), headers) };
		return base64Decode(res) == xorstr_("success");
	}
	bool auth::authenticate() {
		return true;
		std::string res{};
		curlWrapper curl{};
		std::vector<std::string> headers{
			xorstr_("Accept: text/html"),
			xorstr_("Content-Type: application/x-www-form-urlencoded")
		};
		res = curl.post(xorstr_(websiteAddress"/auth/login.php"), std::format("username={}&password={}&hwid={}", m_username, m_password, security::hwid()), headers);
		if (base64Decode(res).find(xorstr_("75236ca9")) != -1
			&& base64Decode(res).find(xorstr_("7a9cf311f5e70ec255d678c06e1c548909cd8c64a0dbae48a3fc1f1affb72f15e6a1e45e7993f6d8cb2338e85ade7e72b4e8d0bce02b20b16128cd3d4fad236d")) != -1
			&& base64Decode(res).find(xorstr_("16f2f8d32f1ef725ff90d73a0e78251c2229f475b49030b0b04283b3e66754b352a6708d14608ae454940a6312b2c51ec79e879e9df7e66edb3ffe71e5ec20e7")) != -1
			&& base64Decode(res).find(xorstr_("01")) != -1) {
			return true;
		}
		else {
			if (base64Decode(res).find(xorstr_("2960fdf33df090f17d81c8c14676fbb2")) != -1) {
				LOG(Fatal, "Your HWID does not match the one linked to your account");
			}
			else if (base64Decode(res).find(xorstr_("40b3a04940ed771362dc41af1422b24d")) != -1) {
				LOG(Fatal, "You provided the invalid username {}", m_username);
			}
			else if (base64Decode(res).find(xorstr_("de394f7c821cdd70cf9699126d376914")) != -1) {
				LOG(Fatal, "You provided an invalid password");
			}
			else if (base64Decode(res).find(xorstr_("1b16e68aadaee15280bf55e8adfa821d")) != -1) {
				updateData(xorstr_("hwid"), security::hwid());
				return authenticate();
			}
			else {
				LOG(Fatal, "Unknown auth error! Response: {}", res);
			}
			return false;
		}
	}
	bool auth::ensureOnlineConnection() {
		return authenticate();
	}
}