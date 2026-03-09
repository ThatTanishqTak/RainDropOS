#include <iostream>
#include "../include/IpcMessages.h"

int main()
{
    // Test 1 — serialize and deserialize a game_start message
    IpcMessage msg;
    msg.type  = MessageType::GameStart;
    msg.appid = "440";

    std::string raw = msg.Serialize();
    std::cout << "Serialized: " << raw << "\n";

    IpcMessage msg2 = IpcMessage::Deserialize(raw);
    std::cout << "Type match:  " << (msg2.type == MessageType::GameStart ? "PASS" : "FAIL") << "\n";
    std::cout << "Appid match: " << (msg2.appid == "440" ? "PASS" : "FAIL") << "\n";

    // Test 2 — serialize and deserialize telemetry
    TelemetryData data;
    data.cpu_percent  = 45.5f;
    data.temp_celsius = 72.0f;
    data.ram_used_mb  = 4096.0f;

    std::string rawData = data.Serialize();
    std::cout << "Serialized: " << rawData << "\n";

    TelemetryData data2 = TelemetryData::Deserialize(rawData);
    std::cout << "CPU match:  " << (data2.cpu_percent  == 45.5f  ? "PASS" : "FAIL") << "\n";
    std::cout << "Temp match: " << (data2.temp_celsius == 72.0f  ? "PASS" : "FAIL") << "\n";
    std::cout << "RAM match:  " << (data2.ram_used_mb  == 4096.0f ? "PASS" : "FAIL") << "\n";

    return 0;
}