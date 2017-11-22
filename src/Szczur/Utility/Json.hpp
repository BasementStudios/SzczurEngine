#pragma once

#if __has_include(<Json/json.hpp>)
    #include <Json/json.hpp>
    using Json = nlohmann::json;
#elif __has_include(<json.hpp>)
    #include <json.hpp>
    using Json = nlohmann::json;
#else
    #error No <json.hpp> header is avaible
#endif
