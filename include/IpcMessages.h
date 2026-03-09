#pragma once

#include <string>
#include <nlohmann/json.hpp>

// Message types the shell and daemon exchange
enum class MessageType
{
    GameStart,
    GameStop,
    GetTelemetry,
    Unknown
};

// A message sent from the shell to the daemon
struct IpcMessage
{
    MessageType type  = MessageType::Unknown;
    std::string appid;

    std::string Serialize() const
    {
        nlohmann::json j;

        switch (type)
        {
            case MessageType::GameStart:
                j["type"]  = "game_start";
                j["appid"] = appid;
                break;
            case MessageType::GameStop:
                j["type"]  = "game_stop";
                j["appid"] = appid;
                break;
            case MessageType::GetTelemetry:
                j["type"] = "get_telemetry";
                break;
            default:
                j["type"] = "unknown";
                break;
        }

        return j.dump();
    }

    static IpcMessage Deserialize(const std::string& raw)
    {
        IpcMessage msg;

        try
        {
            nlohmann::json j        = nlohmann::json::parse(raw);
            std::string t = j.value("type", "unknown");

            if      (t == "game_start")    msg.type = MessageType::GameStart;
            else if (t == "game_stop")     msg.type = MessageType::GameStop;
            else if (t == "get_telemetry") msg.type = MessageType::GetTelemetry;
            else                           msg.type = MessageType::Unknown;

            if (j.contains("appid"))
                msg.appid = j["appid"].get<std::string>();
        }
        catch (...) {}

        return msg;
    }
};

// Telemetry data sent from the daemon back to the shell
struct TelemetryData
{
    float cpu_percent  = 0.0f;
    float temp_celsius = 0.0f;
    float ram_used_mb  = 0.0f;

    std::string Serialize() const
    {
        nlohmann::json j;
        j["type"]         = "telemetry";
        j["cpu_percent"]  = cpu_percent;
        j["temp_celsius"] = temp_celsius;
        j["ram_used_mb"]  = ram_used_mb;
        return j.dump();
    }

    static TelemetryData Deserialize(const std::string& raw)
    {
        TelemetryData data;

        try
        {
            nlohmann::json j            = nlohmann::json::parse(raw);
            data.cpu_percent  = j.value("cpu_percent",  0.0f);
            data.temp_celsius = j.value("temp_celsius", 0.0f);
            data.ram_used_mb  = j.value("ram_used_mb",  0.0f);
        }
        catch (...) {}

        return data;
    }
};