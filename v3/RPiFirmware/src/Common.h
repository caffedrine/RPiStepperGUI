#ifndef DRIVERSCONTAINER_COMMON_H
#define DRIVERSCONTAINER_COMMON_H

#include "Config.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

/* Logger instance - multi-threaded */
auto console = spdlog::stdout_color_mt("Console");

#endif //DRIVERSCONTAINER_COMMON_H
