#ifndef DRIVERSCONTAINER_COMMON_H
#define DRIVERSCONTAINER_COMMON_H

#include "Config.h"
#include "../../Shared/packet.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("console");

/* Global variable to store latest received ack from client */
uint64_t g_TcpRecvLastMillis = 0;

#endif //DRIVERSCONTAINER_COMMON_H
