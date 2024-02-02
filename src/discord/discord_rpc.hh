#ifndef __DISCORD__DISCORD__HH__
#define __DISCORD__DISCORD__HH__

#include <cstdint>
#include <cstring>
#include <ctime>

#include <string_view>

#include <discord_rpc.h>

namespace flrpc
{
    class discord_rpc
    {
    public:
        discord_rpc(std::string_view clientid);
        ~discord_rpc();

    public:
        auto update_presence(std::string_view state) -> void;
        auto clear_presence() -> void;

    private:
        DiscordEventHandlers m_handlers;
        DiscordRichPresence m_presence;

        std::int64_t m_start_time;
    };
}

#endif // __DISCORD__DISCORD__HH__