#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"

char *get_wifi_disconnection_string(wifi_err_reason_t wifi_err_reason)
{
    switch (wifi_err_reason)
    {
    case WIFI_REASON_UNSPECIFIED:
        return "WIFI_REASON_UNSPECIFIED";
    case WIFI_REASON_AUTH_EXPIRE:
        return "WIFI_REASON_AUTH_EXPIRE";
    case WIFI_REASON_AUTH_LEAVE:
        return "WIFI_REASON_AUTH_LEAVE";
    case WIFI_REASON_ASSOC_EXPIRE:
        return "WIFI_REASON_ASSOC_EXPIRE";
    case WIFI_REASON_ASSOC_TOOMANY:
        return "WIFI_REASON_ASSOC_TOOMANY";
    case WIFI_REASON_NOT_AUTHED:
        return "WIFI_REASON_NOT_AUTHED";
    case WIFI_REASON_NOT_ASSOCED:
        return "WIFI_REASON_NOT_ASSOCED";
    case WIFI_REASON_ASSOC_LEAVE:
        return "WIFI_REASON_ASSOC_LEAVE";
    case WIFI_REASON_ASSOC_NOT_AUTHED:
        return "WIFI_REASON_ASSOC_NOT_AUTHED";
    case WIFI_REASON_DISASSOC_PWRCAP_BAD:
        return "WIFI_REASON_DISASSOC_PWRCAP_BAD";
    case WIFI_REASON_DISASSOC_SUPCHAN_BAD:
        return "WIFI_REASON_DISASSOC_SUPCHAN_BAD";
    case WIFI_REASON_BSS_TRANSITION_DISASSOC:
        return "WIFI_REASON_BSS_TRANSITION_DISASSOC";
    case WIFI_REASON_IE_INVALID:
        return "WIFI_REASON_IE_INVALID";
    case WIFI_REASON_MIC_FAILURE:
        return "WIFI_REASON_MIC_FAILURE";
    case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
        return "WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT";
    case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT:
        return "WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT";
    case WIFI_REASON_IE_IN_4WAY_DIFFERS:
        return "WIFI_REASON_IE_IN_4WAY_DIFFERS";
    case WIFI_REASON_GROUP_CIPHER_INVALID:
        return "WIFI_REASON_GROUP_CIPHER_INVALID";
    case WIFI_REASON_PAIRWISE_CIPHER_INVALID:
        return "WIFI_REASON_PAIRWISE_CIPHER_INVALID";
    case WIFI_REASON_AKMP_INVALID:
        return "WIFI_REASON_AKMP_INVALID";
    case WIFI_REASON_UNSUPP_RSN_IE_VERSION:
        return "WIFI_REASON_UNSUPP_RSN_IE_VERSION";
    case WIFI_REASON_INVALID_RSN_IE_CAP:
        return "WIFI_REASON_INVALID_RSN_IE_CAP";
    case WIFI_REASON_802_1X_AUTH_FAILED:
        return "WIFI_REASON_802_1X_AUTH_FAILED";
    case WIFI_REASON_CIPHER_SUITE_REJECTED:
        return "WIFI_REASON_CIPHER_SUITE_REJECTED";
    case WIFI_REASON_TDLS_PEER_UNREACHABLE:
        return "WIFI_REASON_TDLS_PEER_UNREACHABLE";
    case WIFI_REASON_TDLS_UNSPECIFIED:
        return "WIFI_REASON_TDLS_UNSPECIFIED";
    case WIFI_REASON_SSP_REQUESTED_DISASSOC:
        return "WIFI_REASON_SSP_REQUESTED_DISASSOC";
    case WIFI_REASON_NO_SSP_ROAMING_AGREEMENT:
        return "WIFI_REASON_NO_SSP_ROAMING_AGREEMENT";
    case WIFI_REASON_BAD_CIPHER_OR_AKM:
        return "WIFI_REASON_BAD_CIPHER_OR_AKM";
    case WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION:
        return "WIFI_REASON_NOT_AUTHORIZED_THIS_LOCATION";
    case WIFI_REASON_SERVICE_CHANGE_PERCLUDES_TS:
        return "WIFI_REASON_SERVICE_CHANGE_PERCLUDES_TS";
    case WIFI_REASON_UNSPECIFIED_QOS:
        return "WIFI_REASON_UNSPECIFIED_QOS";
    case WIFI_REASON_NOT_ENOUGH_BANDWIDTH:
        return "WIFI_REASON_NOT_ENOUGH_BANDWIDTH";
    case WIFI_REASON_MISSING_ACKS:
        return "WIFI_REASON_MISSING_ACKS";
    case WIFI_REASON_EXCEEDED_TXOP:
        return "WIFI_REASON_EXCEEDED_TXOP";
    case WIFI_REASON_STA_LEAVING:
        return "WIFI_REASON_STA_LEAVING";
    case WIFI_REASON_END_BA:
        return "WIFI_REASON_END_BA";
    case WIFI_REASON_UNKNOWN_BA:
        return "WIFI_REASON_UNKNOWN_BA";
    case WIFI_REASON_TIMEOUT:
        return "WIFI_REASON_TIMEOUT";
    case WIFI_REASON_PEER_INITIATED:
        return "WIFI_REASON_PEER_INITIATED";
    case WIFI_REASON_AP_INITIATED:
        return "WIFI_REASON_AP_INITIATED";
    case WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT:
        return "WIFI_REASON_INVALID_FT_ACTION_FRAME_COUNT";
    case WIFI_REASON_INVALID_PMKID:
        return "WIFI_REASON_INVALID_PMKID";
    case WIFI_REASON_INVALID_MDE:
        return "WIFI_REASON_INVALID_MDE";
    case WIFI_REASON_INVALID_FTE:
        return "WIFI_REASON_INVALID_FTE";
    case WIFI_REASON_TRANSMISSION_LINK_ESTABLISH_FAILED:
        return "WIFI_REASON_TRANSMISSION_LINK_ESTABLISH_FAILED";
    case WIFI_REASON_ALTERATIVE_CHANNEL_OCCUPIED:
        return "WIFI_REASON_ALTERATIVE_CHANNEL_OCCUPIED";
    case WIFI_REASON_BEACON_TIMEOUT:
        return "WIFI_REASON_BEACON_TIMEOUT";
    case WIFI_REASON_NO_AP_FOUND:
        return "WIFI_REASON_NO_AP_FOUND";
    case WIFI_REASON_AUTH_FAIL:
        return "WIFI_REASON_AUTH_FAIL";
    case WIFI_REASON_ASSOC_FAIL:
        return "WIFI_REASON_ASSOC_FAIL";
    case WIFI_REASON_HANDSHAKE_TIMEOUT:
        return "WIFI_REASON_HANDSHAKE_TIMEOUT";
    case WIFI_REASON_CONNECTION_FAIL:
        return "WIFI_REASON_CONNECTION_FAIL";
    case WIFI_REASON_AP_TSF_RESET:
        return "WIFI_REASON_AP_TSF_RESET";
    case WIFI_REASON_ROAMING:
        return "WIFI_REASON_ROAMING";
    // case WIFI_REASON_ASSOC_COMEBACK_TIME_TOO_LONG:
    //     return "WIFI_REASON_ASSOC_COMEBACK_TIME_TOO_LONG";
    // case WIFI_REASON_SA_QUERY_TIMEOUT:
    //     return "WIFI_REASON_SA_QUERY_TIMEOUT";
    default:
        break;
    }
    return "UNKNOWN";
}