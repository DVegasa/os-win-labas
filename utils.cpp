#include <iomanip>
#include <iostream>
#include <string>

#include "windows.h"

using namespace std;

string stringedSize(long long int bytes) {
    double result = bytes;
    string suffix = "";

    if (bytes < 1024) {
        suffix = "B";

    } else if (bytes < 1024 * 1024) {
        result = bytes / 1024.0;
        suffix = "KB";

    } else if (bytes < 1024 * 1024 * 1024) {
        result = bytes / (1024.0 * 1024);
        suffix = "MB";

    } else {
        result = bytes / (1024.0 * 1024 * 1024);
        suffix = "GB";
    }

    stringstream ss;
    ss << fixed << setprecision(1);
    ss << result << " " << suffix;
    return ss.str();
}