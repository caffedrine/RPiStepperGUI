#ifndef DRIVERSCONTAINER_COMMON_H
#define DRIVERSCONTAINER_COMMON_H


#include "Config.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("console");

/* Global variable to store latest received ack from client */
uint64_t g_TcpRecvLastMillis = 0;

//#include "communication/ServerTCP.h"
//extern ServerTCP g_TcpServer;

#endif //DRIVERSCONTAINER_COMMON_H
