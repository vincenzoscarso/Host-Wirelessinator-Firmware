#ifndef WIFISERVERHANDLER_H
#define WIFISERVERHANDLER_H

#define HOST_NAME "Host-Wirelessinator"
#define DELAY_BETWEEN_WIFI_CONNECTION_TRIALS (5000)
#define DELAY_BETWEEN_WIFI_CONNECTION_CHECK (10000)

namespace WifiHandler {
void wifiSetup();
void wifiLoop();
void sendMagicPacket(std::vector<byte> receiver_mac_address);
}
#endif /* WIFISERVERHANDLER_H */