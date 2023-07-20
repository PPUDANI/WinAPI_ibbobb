#pragma once
#include <string>
class SoundLoadManager
{
public:
	// Constructor Destructor
	SoundLoadManager();
	~SoundLoadManager();

	// Delete Function
	SoundLoadManager(const SoundLoadManager& _Other) = delete;
	SoundLoadManager(SoundLoadManager&& _Other) noexcept = delete;
	SoundLoadManager& operator=(const SoundLoadManager& _Other) = delete;
	SoundLoadManager& operator=(SoundLoadManager&& _Other) noexcept = delete;
	static void LoadSound(const std::string& _TypeName, const std::string _FileName);
protected:

private:

};