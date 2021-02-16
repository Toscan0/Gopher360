#include "VolumeManager.h"

void VolumeManager::GetStuff()
{
	HRESULT hr = NULL;

	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID *)&_device_enumerator);

	hr = _device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_default_device);
	_device_enumerator->Release();

	hr = _default_device->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&_endpoint_volume);
	_default_device->Release();
	
}

float VolumeManager::GetVolume() 
{
	float current_volume = 0;

	CoInitialize(NULL);
	GetStuff();
	
	_endpoint_volume->GetMasterVolumeLevelScalar(&current_volume);
	
	CoUninitialize();

	return current_volume;
}

void VolumeManager::SetVolume(float newVolume)
{
	CoInitialize(NULL);

	GetStuff();

	_endpoint_volume->SetMasterVolumeLevelScalar(newVolume, NULL);
	_endpoint_volume->Release();

	CoUninitialize();
}

void VolumeManager::DecreaseVolume(float toDecr)
{
	float current_volume = GetVolume();

	float newVolume = current_volume - toDecr;
	if (newVolume < 0.0f) {
		newVolume = 0;
	}

	SetVolume(newVolume);
	printf("volumme decreased to: %d%%\n", (int)(newVolume * 100));
}

void VolumeManager::IncrementVolume(float toIncr)
{
	float current_volume = GetVolume();

	float newVolume = current_volume + toIncr;
	if (newVolume > 1.0f) {
		newVolume = 1;
	}

	SetVolume(newVolume);
	printf("volumme increased to: %d%%\n", (int)(newVolume * 100));
}
