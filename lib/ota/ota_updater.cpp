#include "ota_updater.h"

OTAUpdater::OTAUpdater() {}

void OTAUpdater::update(
    char *url,
    std::function<void()> onStart,
    std::function<void(uint8_t)> onProgress,
    std::function<void()> onFinished,
    std::function<void(const char*)> onError
) {
    if (_isUpdating) return;

    ESPhttpUpdate.onStart([this, onStart]() {
        if (this->_onFirmwareUpdateStarted) this->_onFirmwareUpdateStarted();
        this->_isUpdating = true;
        onStart();
    });

    ESPhttpUpdate.onEnd([this, onFinished]() {
        if (this->_onFirmwareUpdateFinished) this->_onFirmwareUpdateFinished(true);
        this->_isUpdating = false;
        onFinished();
    });

    ESPhttpUpdate.onProgress([this, onProgress](int current, int total) {
        static Timer tmr(1000);
        if (tmr.period()) {
            auto percentage = static_cast<uint8_t>((static_cast<double>(current) / total) * 100 + 0.5);
            if (this->_onFirmwareUpdateProgress) this->_onFirmwareUpdateProgress(percentage);
            onProgress(percentage);
        }
    });

    ESPhttpUpdate.onError([this, onError](int errorCode) {
        if (this->_onFirmwareUpdateFinished) this->_onFirmwareUpdateFinished(false);
        this->_isUpdating = false;
        onError(ESPhttpUpdate.getLastErrorString().c_str());
    });

    BearSSL::WiFiClientSecure UpdateClient;
	UpdateClient.setInsecure();
    ESPhttpUpdate.update(UpdateClient, url);
}