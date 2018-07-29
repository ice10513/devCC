#pragma once

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
#include <experimental/optional>
using std::experimental::optional;
#endif

#define SHB_LOG std::cout << __FILE__ << ":" << __LINE__ << " " << __func__ << ":"

#define SHB_ERR std::cerr << __FILE__ << ":" << __LINE__ << " " << __func__ << ":"

#define SHB_LOG_LINE_REGEX_PREFIX R"(\d{4}-UECexe <\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d{6}Z> )"