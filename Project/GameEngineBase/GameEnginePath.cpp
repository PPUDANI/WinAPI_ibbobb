#include "GameEnginePath.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEnginePath::GameEnginePath()
{

}

GameEnginePath::GameEnginePath(const std::string& _Path)
	: Path(_Path)
{

}

GameEnginePath::~GameEnginePath()
{
}

std::string GameEnginePath::GetFileName()
{
	return Path.filename().string();
}

void GameEnginePath::GetCurrentPath()
{
	Path = std::filesystem::current_path();
}

void GameEnginePath::MoveChild(const std::string& _ChildPath)
{
	std::filesystem::path CheckPath = Path;
	CheckPath.append(_ChildPath);

	if (false == std::filesystem::exists(CheckPath))
	{
		MsgBoxAssert("�������� �ʴ� ��η� �̵��Ϸ��� �߽��ϴ�." + CheckPath.string());
	}

	Path = CheckPath;
}

void GameEnginePath::MoveParent()
{
	Path = Path.parent_path();
}

void GameEnginePath::MoveParentToExistsChild(const std::string& _ChildPath)
{
	while (true) 
	{
		std::filesystem::path CheckPath = Path;
		CheckPath.append(_ChildPath);

		if (false == std::filesystem::exists(CheckPath))
		{
			MoveParent();
		}
		else
		{
			break;
		}

		if (Path.root_path() == Path)
		{
			MsgBoxAssert("��Ʈ ��� ������ �˻� ���" + _ChildPath + "������ ã�� �� �����ϴ�.");
			return;
		}
	}
}

