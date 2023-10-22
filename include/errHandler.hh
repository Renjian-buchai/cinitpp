#if !defined(ERR_HANDLER_HH_)
#define ERR_HANDLER_HH_

#include <cstdint>
#include <system_error>

void reset(uint8_t errNumber, std::error_code& err);

#endif  // ERR_HANDLER_HH_
