#include "ota_updater.h"

OTAUpdater::OTAUpdater(LedService &strip) : _strip(&strip)
{
}

void OTAUpdater::update(
    char *url,
    std::function<void()> onStart,
    std::function<void(uint8_t)> onProgress,
    std::function<void()> onFinished,
    std::function<void(const char*)> onError
)
{
    ESPhttpUpdate.onStart(onStart);
    ESPhttpUpdate.onEnd(onFinished);
    ESPhttpUpdate.onProgress([onProgress](int current, int total){
        static Timer tmr(1000);
        if (tmr.period()) {
            auto percentage = static_cast<uint8_t>((static_cast<double>(current) / total) * 100 + 0.5);
            onProgress(percentage);
        }
    });
    ESPhttpUpdate.onError([onError](int errorCode) {
        onError(ESPhttpUpdate.getLastErrorString().c_str());
    });

    _strip->clear();
    _strip->setBrightness(50);
    _strip->fill(CRGB::Red);
    _strip->update();

    BearSSL::WiFiClientSecure UpdateClient;
	UpdateClient.setInsecure();
    ESPhttpUpdate.update(UpdateClient, url);
}