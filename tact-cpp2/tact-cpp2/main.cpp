#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <iomanip>
#include <windows.h>
#include "library.h"

using namespace std;

int main() {

	cout << "Hello World!" << endl;

	int res = -1;
	std::ifstream file;
	std::string mappingjson = "";
	std::string event = "heartbeat";
    std::string appid = "";
    std::string sdkKey = "";
    std::string macAddress = "";

    float intensity = 1.0f;
    float duration = 1.0f;
    float angleX = 0.0f;
    float offsetY = 0.0f;
    int interval = 1000;
    int maxCount = 10;

	res = registryAndInit(sdkKey.c_str(), appid.c_str(), mappingjson.c_str()); //dev mode

	if (res <= 0) {
		cout << "Connection Failed" << endl;
		return -1;
	}
	auto connected = wsIsConnected();
	cout << "Connected: " << std::to_string(connected) << endl;

    std::string str;
    int requestId = -1;
    while (std::getline(std::cin, str)) {
        if (str == "o") {
            registryAndInit(sdkKey.c_str(), appid.c_str(), nullptr);
        }
        if (str == "quit") {
            wsClose();
        }
        if (str == "re") {
            cout << reInitMessage(sdkKey.c_str(), appid.c_str(), nullptr) << endl;
        }
        if (str == "z") {
            requestId = play(event.c_str());
        }
        if (str == "qwer") {
            requestId = playParam(event.c_str(), rand(), intensity, duration, angleX, offsetY);
        }
        if (str == "f") {
            stop(requestId);
        }
        if (str == "a") {
            stopAll();
        }
        if (str == "i") {
            cout << isPlayingByRequestId(requestId) << endl;
        }
        if (str == "l") {
            cout << isPlaying() << endl;
        }
        if (str == "c") {
            cout << isbHapticsConnected(0) << endl;
        }
        
        if (str == "loop") {
            requestId = playLoop(event.c_str(), 0, intensity, duration, angleX, offsetY, interval, maxCount);
        }
        if (str == "v") {
            int rst = -1;

            // no workspace
            bHapticsGetHapticMessage(sdkKey.c_str(), "no_workspace", -1, rst);
            cout << "Result: " << rst << endl;
            bHapticsGetHapticMessage("invalid_key", appid.c_str(), -1, rst);
            cout << "Result: " << rst << endl;

            // Not deployed
            bHapticsGetHapticMessage(sdkKey.c_str(), appid.c_str(), -1, rst);
            cout << "Result: " << rst << endl;

            bHapticsGetHapticMessage(sdkKey.c_str(), appid.c_str(), -1, rst);
            cout << "Result: " << rst << endl;
        }
        if (str == "b") {
            int rst = -1;
            bHapticsGetHapticMappings(sdkKey.c_str(), appid.c_str(), -1, rst);
            cout << "Result: " << rst << endl;
        }
        if (str == "w") {
            cout << isPlayingByEventId(event.c_str()) << endl;
        }
        if (str == "q") {
            cout << stopByEventId(event.c_str()) << endl;
        }
        if (str == "m") {
            cout << getDeviceInfoJson() << endl;
        }
        if (str == "n") {
            cout << getHapticMappingsJson() << endl;
        }
        if (str == "u") {
            cout << ping(macAddress.c_str()) << endl;
        }
        if (str == "j") {
            cout << pingAll() << endl;
        }
        if (str == "k") {
            int len = 6;
            int* motorValues = new int[len];

            for (int i = 0; i < len; i++) {
                motorValues[i] = 40;
            }

            cout << playDot(0, 8, 2000, motorValues, len) << endl;
            //Sleep(1);
            stopAll();
        }
        if (str == "event") {
            cout << getEventTime("scan4") << endl;
        }
        if (str == "g") {
            int len = 8;
            int* motorValues = new int[len];
            int* playTime = new int[len];
            int* shape = new int[len];
            int repeatCount = 100;

            for (int i = 0; i < len; i++) {
                motorValues[i] = 100;
            }
            for (int i = 0; i < len; i++) {
                playTime[i] = 8;
            }
            for (int i = 0; i < len; i++) {
                shape[i] = 2;
            }

            for (int kk = 0; kk < 5; kk++) {
                cout << playWaveform(0, 8, motorValues, playTime, shape, repeatCount, len) << endl;
                Sleep(40);
            }

            Sleep(500);

            for (int kk = 0; kk < 5; kk++) {
                cout << playWaveform(0, 9, motorValues, playTime, shape, repeatCount, len) << endl;
                Sleep(40);
            }

        }
        if (str == "pp") {
            int len = 7;
            float x[] = { 0.738,0.723,0.709,0.696,0.682,0.667,0.653,0.639,0.624,0.611,0.597,0.584,0.57,0.557,0.542,0.528,0.515,0.501,0.487,0.474,0.46,0.447,0.432,0.419,0.406,0.393,0.378,0.365,0.352,0.338,0.324,0.311,0.297 };
            float y[] = { 0.68,0.715,0.749,0.782,0.816,0.852,0.885,0.921,0.956,0.952,0.918,0.885,0.848,0.816,0.779,0.743,0.71,0.673,0.639,0.606,0.571,0.537,0.5,0.467,0.434,0.4,0.363,0.329,0.296,0.261,0.226,0.192,0.157 };

            len = (sizeof(x) / sizeof(*x));

            int* intensity = new int[len];

            for (int i = 0; i < len; i++) {
                intensity[i] = 100;
            }

            cout << "pp: " << len << endl;
            cout << playPath(0, 0, 1000, x, y, intensity, len) << endl;
        }
    }

	return 0;
}