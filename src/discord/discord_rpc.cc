#include "discord_rpc.hh"

#include <functional>

#include <spdlog/spdlog.h>

namespace flrpc
{
    discord_rpc::discord_rpc(std::string_view clientid)
        : m_start_time(time(0))
    {
        std::memset(&m_handlers, 0, sizeof(DiscordEventHandlers));

        m_handlers.debug = [](char isOut, const char* opcodeName, const char* message, std::uint32_t msg_len) -> void {
            SPDLOG_LOGGER_DEBUG(spdlog::default_logger(), "[RPC] {}", message);
        };

        std::memset(&m_presence, 0, sizeof(DiscordRichPresence));

        m_presence.startTimestamp = m_start_time;
        m_presence.smallImageText = "FL-RPC";
        m_presence.largeImageKey = "fl_studio_logo";

        Discord_Initialize(clientid.data(), &m_handlers, 1, NULL);
    }

    discord_rpc::~discord_rpc()
    {
        Discord_Shutdown();
    }

    auto discord_rpc::update_presence(std::string_view state) -> void
    {
        m_presence.state = state.data();

        Discord_UpdatePresence(&m_presence);
    }

    auto discord_rpc::clear_presence() -> void
    {
        Discord_ClearPresence();
        m_presence.startTimestamp = time(0);
    }
}