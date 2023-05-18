#pragma once
#include <string>
#include <filesystem>

// ���� ��θ� ����ϴ� Ŭ����
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

	std::string GetFileName(); // ���� �̸� ��ȯ �Լ�
	std::string GetStringPath() // ���� ��� ��ȯ �Լ�
	{
		return Path.string();
	}

	void SetCurrentPath(); // ���� ���� ��θ� Path�� �����ϴ�  �Լ�
	void MoveParent(); // ���� Path�� �θ� ��η� �̵���Ű�� �Լ�
	void MoveParentToExistsChild(const std::string& _ChildPath); // �θ��� ������ ������ ã���ִ� �Լ�
	void MoveChild(const std::string& _ChildPAth); // ���� Path�� �ڽ� ��η� �̵���Ű�� �Լ�
	std::string PlusFilePath(const std::string _ChildPath); // ���� ��ο� ���� �̸� �߰����ִ� �Լ�
	
	bool IsDirectory();

	static std::string GetParentString(const std::string& _ChildPath);

protected:
	std::filesystem::path Path;
private:
	
};

