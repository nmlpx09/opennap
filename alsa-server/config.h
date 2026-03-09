#pragma once

#include <cstdint>
#include <string>

//net
static const std::string IP = "0.0.0.0";
static const std::uint16_t PORT = 1234;
static const std::size_t DATASIZE = 3840;

//alsa
static const std::string DEVICE = "sysdefault:CARD=Q20";
static const std::string FORMAT = "s32le";
static const std::int8_t CHANNELS = 2;
static const std::uint32_t RATE = 48000;

//queue
static const std::size_t MAXQUEUESIZE = 20;
