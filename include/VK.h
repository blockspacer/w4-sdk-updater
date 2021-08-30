#pragma once

#include <string>
#include <unordered_map>

#include "W4Common.h"

namespace w4::social::api {

class VK
{
public:
    enum class ErrorCode
    {
        RequestTimeout = -2,
        ServiceNotAvailable = -1,
        NoError = 0,
        UnknownError = 1,
        ApplicationOffline = 2,
        UnknownMethod = 3,
        UnknownSignature = 4,
        UserAuthorizeFailure = 5,
        TooMuchRequests = 6,
        NoAccessRight = 7,
        WrongRequest = 8,
        TooMuchSameRequests = 9,
        VKInternalError = 10,
        TestModeError = 11, //application must be offline for test mode
        CaptchaRequired = 14,
        AccessDenied = 15,
        HttpsRequired = 16,
        RequireUserValidation = 17,
        UserRemovedOrBlocked = 18,
        AllowedOnlyForStandaloneApps = 20,
        AllowedOnlyForStandaloneAndOpenAPIAps = 21,
        MethodDisabled = 23,
        RequireUserAccept = 24,
        GroupKeyExpired = 27,
        ApplicationKeyExpired = 28,
        MethodCallsLimitReached = 29,
        UserProfileIsPrivate = 30,
        NotImplemented = 33,
        UserBanned = 37,
        UnknownApplication = 38,
        UnknownUser = 39,
        UnknownGroup = 40,
        AdditionalSignupRequired = 41,
        RequiredParametersMissedOrWrong = 100,
        InvalidAppId = 101,
        InvalidUserId = 113,
        InvalidTimestamp = 150,
        AccessToAlbumDenied = 200,
        AccessToAudioDenied = 201,
        AccessToGroupDenied = 203,
        AlbumIsFull = 300,
        VotesActionDenied = 500,
        NoAccessRightForAdCabinet = 600,
        AdCabinetError = 603,
        RecaptchaRequired = 3300,
        PhoneValidationRequired = 3301,
        PasswordValidationRequired = 3302,
        OtpValidationRequired = 3303,
        AssertVotes = 3305,
        TokenExtensionRequired = 3609,
        UserIsDeactivated = 3610,
        ServiceIsDeactivatedForUser = 3611
    };
    struct UserInfo
    {
        int id;
        std::string firstName;
        std::string lastName;
        std::optional<std::string> photo50;
        std::optional<std::string> photo100;
        std::optional<std::string> photo200;
        std::optional<std::string> photo400;
        std::optional<std::string> photoMax;
    };

    struct UserInfoRequestParameters
    {
        bool photo50 = false;
        bool photo100 = false;
        bool photo200 = false;
        bool photo400 = false;
        bool photoMax = false;
    };

    template<typename ...Args>
    using OnSuccess = std::function<void(Args &&... args)>;
    using OnFailure = std::function<void(ErrorCode)>;

    enum class AdVideoResult
    {
        Completed = 0,
        Skipped = 1,
        Busy = 2,
        NoVideo = 3
    };

public:


    static void initialize(std::function<void(bool isSDKAvailable, bool isAdsAvailable)>);

    static bool isAvailable();
    static bool isAdsAvailable();

    static void enableTestMode(bool);
    static void setCorsProxy(const std::string& url);

    static const std::string& getPlatform();
    static const std::string& getAuthKey();
    static const int32_t getUserId();

    static void requestUserInfo(const UserInfoRequestParameters&, OnSuccess<UserInfo>, OnFailure = emptyFailureHandler);
    static void requestConnectedFriends(const UserInfoRequestParameters&, OnSuccess<std::vector<UserInfo>>, OnFailure = emptyFailureHandler);

    static void preloadRewardedVideo(OnSuccess<bool> = [](bool flag){}, OnFailure = emptyFailureHandler);
    static void showRewardedVideo(OnSuccess<bool /*isVideoWatched*/>, OnFailure = emptyFailureHandler);

    static void shareScreenshot(const std::string& description, const std::string& appID, OnSuccess<> onSuccess, OnFailure = emptyFailureHandler);

    static void addToHomeScreen(OnSuccess<>, OnFailure = emptyFailureHandler);
    static void addToFavorites(OnSuccess<>, OnFailure = emptyFailureHandler);

    static void setStorageValue(const std::string& key, const std::string& value, OnSuccess<>, OnFailure = emptyFailureHandler);
    static void getStorageValues(const std::vector<std::string>& keys, OnSuccess<std::unordered_map<std::string, std::string>>, OnFailure = emptyFailureHandler);
    static void getStorageKeys(int count, int offset, OnSuccess<std::vector<std::string>>, OnFailure = emptyFailureHandler);

    static void joinToGroup(int32_t groupId, OnSuccess<>, OnFailure = [](auto){});
    static void subscribeToGroup(int32_t groupId, const std::string& userKey, OnSuccess<>, OnFailure = emptyFailureHandler);

private:
    static void emptyFailureHandler(ErrorCode) {}
};


} // namespace
