#pragma once

#include "../Module.hpp"
#include "../../../../SDK/Client/Network/Packet/TextPacket.hpp"
#include "../../../../SDK/Client/Network/Packet/CommandRequestPacket.hpp"
#include "Events/Network/PacketEvent.hpp"
#include "../../../../Assets/Assets.hpp"

class AntiCrasher : public Module {

public:
	AntiCrasher() : Module("Anti Crasher", "Prevent Horion Crasher",
		IDR_LIKE_PNG, "", false, {"lmfao"}) {
	};

	void onEnable() override;

	void onDisable() override;

	void defaultConfig() override;

	void settingsRender(float settingsOffset) override;

	void onPacketReceive(PacketEvent& event);

	void SendAnticrashMesssage(std::string name);

	std::unordered_map<std::string, std::int64_t> lastPacketTime;

	uint64_t now;
};