#include <mmdeviceapi.h> // volume
#include <endpointvolume.h> // volume
#include <stdio.h> // for printf

#pragma once
class VolumeManager
{
private:
	IMMDeviceEnumerator *_device_enumerator = NULL;
	IMMDevice *_default_device = NULL;
	IAudioEndpointVolume *_endpoint_volume = NULL;

	void GetStuff();
	float GetVolume();
	void SetVolume(float newVolume);

public:
	void DecreaseVolume(float toDecr);
	void IncrementVolume(float toDecr);
};

