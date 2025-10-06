#include "AntiCrasher.hpp"

void AntiCrasher::onEnable() {
    Listen(this, PacketEvent, &AntiCrasher::onPacketReceive)
    Module::onEnable();
}

void AntiCrasher::onDisable() {
    Deafen(this, PacketEvent, &AntiCrasher::onPacketReceive)
    Module::onDisable();
}

void AntiCrasher::defaultConfig() {
    Module::defaultConfig("core");
}

void AntiCrasher::settingsRender(float settingsOffset) {
    initSettingsPage();

    FlarialGUI::UnsetScrollView();

    resetPadding();
}

void AntiCrasher::onPacketReceive(PacketEvent& event) {
    if (!this->isEnabled()) return;
    MinecraftPacketIds id = event.getPacket()->getId();
    if (id == MinecraftPacketIds::CommandRequest) {
        auto* pkt = reinterpret_cast<CommandRequestPacket*>(event.getPacket());
        now = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now()
        ).time_since_epoch().count();
            auto plname = pkt->command;
            if (std::find(bannedPlayers.begin(), bannedPlayers.end(), plname) != bannedPlayers.end()) {
                event.setCancelled(true);
                return;
            }
            if (lastPacketTime[plname] && now - lastPacketTime[plname] < 3)
            {
                event.setCancelled(true);
                bannedPlayers.push_back(plname);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                SendAnticrashMesssage(plname);
            }
            lastPacketTime[plname] = now;
    }
}
void AntiCrasher::SendAnticrashMesssage(std::string name)
{
            std::shared_ptr<Packet> packet = SDK::createPacket(0x4D);
            auto* crp3 = reinterpret_cast<CommandRequestPacket*>(packet.get());
            crp3->command = "/tellraw @a {\"rawtext\":[{\"text\":\"§4§lSomeone attempt to use crasher!\"}]}";
            crp3->origin.type = CommandOriginType::Player;
            crp3->InternalSource = true;
            SDK::clientInstance->getPacketSender()->sendToServer(crp3);
}
