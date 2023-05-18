#pragma once
#include <string>
#include <filesystem>

// 파일 경로를 담담하는 클래스
class GameEnginePath
{
public:
	GameEnginePath();
	GameEnginePath(const std::string& _Path);
	~GameEnginePath();

	//GameEnginePath(const GameEnginePath& _Other) = delete;
	//GameEnginePath(GameEnginePath&& _Other) noexcept = delete;
	//GameEnginePath& operator=(const GameEnginePath& _Other) = delete;
	//GameEnginePath& operator=(GameEnginePath&& _Other) noexcept = delete;

	std::string GetFileName(); // 파일 이름 반환 함수
	std::string GetStringPath() // 파일 경로 반환 함수
	{
		return Path.string();
	}

	void SetCurrentPath(); // 현재 파일 경로를 Path에 저장하는  함수
	void MoveParent(); // 현재 Path에 부모 경로로 이동시키는 함수
	void MoveParentToExistsChild(const std::string& _ChildPath); // 부모경로 끝까지 파일을 찾아주는 함수
	void MoveChild(const std::string& _ChildPAth); // 현재 Path에 자식 경로로 이동시키는 함수
	std::string PlusFilePath(const std::string _ChildPath); // 현재 경로에 파일 이름 추가해주는 함수
	
	bool IsDirectory();

	static std::string GetParentString(const std::string& _ChildPath);

protected:
	std::filesystem::path Path;
private:
	
};

