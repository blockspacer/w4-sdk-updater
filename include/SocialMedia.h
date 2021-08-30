#pragma once

#include <functional>
#include <vector>

#include "W4Common.h"
#include "W4Logger.h"
#include "W4JSON.h"

namespace w4::social {

enum class Type
{
    Stub = 0,
    Facebook = 1,
    Ok = 2,
    Vk = 3
};

NLOHMANN_JSON_SERIALIZE_ENUM(social::Type, {
    { social::Type::Stub, "Stub" },
    { social::Type::Facebook, "Facebook" },
    { social::Type::Ok, "Ok" },
    { social::Type::Vk, "Vk" }
})

enum class Platform
{
    Unknown = 0,
    iOS = 1,
    Android = 2,
    Web = 3,
    MobileWeb = 4
};

struct Player
{
    Player() = default;
    Player(const std::string& id,
        const std::string& name,
        const std::string& photo) :id(id), name(name), photo(photo) { }

    std::string id;
    std::string name;
    std::string photo;
};

class ISocialMedia
{
public:
    using BoolCallback = std::function<void(bool)>;
    using StrCallback = std::function<void(const std::string&)>;
    using PlayerCallback = std::function<void(const Player&)>;
    using PlayerArrayCallback = std::function<void(const std::vector<Player>&)>;

    virtual ~ISocialMedia() {}

    virtual void init(const BoolCallback&) const = 0;
    virtual void getPlayer(const PlayerCallback&) const = 0;
    virtual void getConnectedPlayers(const PlayerArrayCallback&) const = 0;
    [[deprecated]] virtual void signPlayerData(const std::string&, const StrCallback&) const = 0;
    virtual void shareScreenshot(const std::string& appID) = 0;
    virtual void makeShortcut(const BoolCallback& func) = 0;
    virtual Platform getPlatform() const = 0;
    virtual Type getType() const = 0;

    virtual void showRewardedVideo(const BoolCallback&) = 0;
    virtual void preloadRewardedVideo() = 0;
    virtual bool isRewardedVideoPreloaded() = 0;
};

class Stub : public ISocialMedia
{
public:
    void init(const BoolCallback&) const override;
    void getPlayer(const PlayerCallback&) const override;
    void getConnectedPlayers(const PlayerArrayCallback&) const override;
    void signPlayerData(const std::string&, const StrCallback&) const override;

    void shareScreenshot(const std::string& appID) override { }
    void makeShortcut(const BoolCallback& func) override { if (func) func(false); }
    Platform getPlatform() const override { return Platform::Unknown; }
    Type getType() const override { return Type::Stub; }

    void showRewardedVideo(const BoolCallback& cb) override { if (cb) cb(false); }
    void preloadRewardedVideo() override { }
    bool isRewardedVideoPreloaded() override { return false; }

private:
    mutable Player m_player;
};

class Facebook : public ISocialMedia
{
public:
    Facebook();
    void init(const BoolCallback&) const override;
    void getPlayer(const PlayerCallback&) const override;
    void getConnectedPlayers(const PlayerArrayCallback&) const override;
    void signPlayerData(const std::string&, const StrCallback&) const override;

    void shareScreenshot(const std::string& appID) override;
    void makeShortcut(const BoolCallback& func) override;
    Platform getPlatform() const override;
    Type getType() const override { return Type::Facebook; }

    void setPlacementID(const std::string&);

    void showRewardedVideo(const BoolCallback& cb) override;
    void preloadRewardedVideo() override;
    bool isRewardedVideoPreloaded() override;
private:
    Player m_player;
    std::string m_placementID;
};

class Ok : public ISocialMedia
{
public:
    void init(const BoolCallback&) const override;
    void getPlayer(const PlayerCallback&) const override;
    void getConnectedPlayers(const PlayerArrayCallback&) const override;
    void signPlayerData(const std::string&, const StrCallback&) const override;

    void shareScreenshot(const std::string& appID) override;
    void makeShortcut(const BoolCallback&) override;
    Platform getPlatform() const override { return Platform::Unknown; }
    Type getType() const override { return Type::Ok; }

    void showRewardedVideo(const BoolCallback& cb) override;
    void preloadRewardedVideo() override;
    bool isRewardedVideoPreloaded() override;
private:
    mutable Player m_player;
    bool m_isVideoPreloaded = false;
};

class Vk : public ISocialMedia
{
public:
    void init(const BoolCallback&) const override;
    void getPlayer(const PlayerCallback&) const override;
    void getConnectedPlayers(const PlayerArrayCallback&) const override;
    void signPlayerData(const std::string&, const StrCallback&) const override;

    void shareScreenshot(const std::string& appID) override;
    void makeShortcut(const BoolCallback&) override;
    Platform getPlatform() const override { return Platform::Unknown; }
    Type getType() const override { return Type::Vk; }

    void showRewardedVideo(const BoolCallback& cb) override;
    void preloadRewardedVideo() override;
    bool isRewardedVideoPreloaded() override;
private:
    mutable Player m_player;
    bool m_isVideoPreloaded = false;
};

static w4::sptr<ISocialMedia> getSocialMedia(Type social)
{
    switch (social)
    {
    case Type::Facebook: return w4::make::sptr<Facebook>();
    case Type::Ok: return w4::make::sptr<Ok>();
    case Type::Vk: return w4::make::sptr<Vk>();
    case Type::Stub: return w4::make::sptr<Stub>();
    default: W4_LOG_ERROR("getSocialMedia: social media %d isn't implemented, stub returned instead", social); return w4::make::sptr<Stub>();
    }
}

} // namespace
