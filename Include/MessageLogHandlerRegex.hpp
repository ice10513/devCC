#pragma once
#include <string>

namespace Shb
{
const std::string g_respMsgNameRegex = R"(message recv (\w+_\w+Resp)\(0x[\dA-F]{1,4}\))";
const std::string g_resultStatusRegex = R"(\tmessageResult.status = (\d+))";
const std::string g_resultCauseRegex = R"(\tmessageResult.cause = (\d+))";
const std::string g_resultSpecificCauseRegex = R"(\tmessageResult.specificCause = (\d+))";
const std::string g_ueContextIdRegex = R"(\tuecContextId = (\d+))";
const std::string g_crntiRegex = R"(\tcrnti = (\d+))";
const std::string g_asnMsgIdRegex = R"(\tasnMsgId = (\w+))";
const std::string g_unorderedCommonRegex = R"(\t[\w\.]+ = (\w+))";
const std::string g_msgSentRegex = R"(message sent (\w+_\w+)\(0x[\dA-F]{1,4}\))";
const std::string g_msgRecvRegex = R"(message recv (\w+_\w+)\(0x[\dA-F]{1,4}\))"; // C++ not support (?<!Resp)
} // namespace Shb