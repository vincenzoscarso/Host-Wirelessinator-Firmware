#ifndef WIFISERVERHANDLER_H
#define WIFISERVERHANDLER_H

#define HOST_NAME "Host-Wirelessinator"
#define DELAY_BETWEEN_WIFI_CONNECTION_TRIALS (5000)

namespace WifiHandler {
void wifiSetup();
void sendMagicPacket(std::vector<byte> receiver_mac_address);
}
#endif /* WIFISERVERHANDLER_H */