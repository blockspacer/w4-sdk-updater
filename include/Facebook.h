#pragma once

#include <cstdio>
#include <string>
#include <queue>

#include "W4JSON.h"

namespace w4::social::api {

class Facebook
{
public:
    static std::string getID();
    static std::string getName();
    static std::string getPhoto();

    static void getConnectedPlayers(const std::function<void(const nlohmann::json&)>& func); //TODO this method captured only one handler. Dont call it inside handler again.
    static void makeSignedPlayerData(const std::string& json, const std::function<void(const std::string&)>& func);

    static void setPlayerData(const std::string& json);
    static void getPlayerData(const std::string& jsonKeys, const std::function<void(const std::string&)>& func); //TODO this method captured only one handler. Dont call it inside handler again.
    static void setSessionData(const std::string& json);

    static uint getLastErrorCode();
    static bool isRewardedVideoPreloaded();
    static void preloadRewardedVideo(const std::string&);
    static void showRewardedVideo(const std::string& id, const std::function<void(bool)>& func); //TODO this method captured only one handler. Dont call it inside handler again.
    static void logEvent(const std::string& name, const std::string& jsonParams, int value = 0);

    static void shareScreenshot();
    static void makeShortcut(const std::function<void(bool)>& func); //TODO this method captured only one handler. Dont call it inside handler again.

    static void subscribeBot(const std::function<void(bool)>& func); //TODO this method captured only one handler. Dont call it inside handler again.

    /* Possible platfrom values
    *  Unknown = 0,
    *  iOS     = 1,
    *  Android = 2,
    *  Web     = 3,
    *  MobileWeb = 4
    */
    static uint getPlatform();
private:
    static void _processNextSign();
    static void _makeShortcut(bool);
    static void _subscribeBot(bool);
    static void _rewardedVideoShown(bool);
    static void _getConnectedPlayers(const char*);
    static void _getPlayerData(const char*);
    static void _getSignedPlayerData(void* ctx, const char*);

    static std::function<void(const nlohmann::json&)> connectPlayersFunc;
    static std::function<void(const std::string&)>    playerDataFunc;
    static std::function<void(bool)>                  rewardedVideoFunc;
    static std::function<void(bool)>                  makeShortcutFunc;
    static std::function<void(bool)>                  subscribeBotFunc;

    static std::unordered_map<void*, w4::sptr<std::function<void(const std::string&)>>> signedPlayerDataFunc;
    static std::queue<std::pair<std::string, w4::sptr<std::function<void(const std::string&)>>>> m_signedCallQueue;
};


} // namespace
