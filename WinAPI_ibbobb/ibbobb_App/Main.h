#pragma once
class Main
{
public:
	Main();
	~Main();

	Main(const Main& _Other) = delete;
	Main(Main&& _Other) noexcept = delete;
	Main& operator=(const Main& _Other) = delete;
	Main& operator=(Main&& _Other) noexcept = delete;
protected:

private:

};

