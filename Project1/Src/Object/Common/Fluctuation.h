#pragma once
#include <memory>

class Controller;

class Fluctuation
{
public:
	Fluctuation();
	~Fluctuation();
	void Init(void);
	void Update(void);
	void Release(void);

private:
	//“ü—Íî•ñ
	std::unique_ptr<Controller> controller_;
};

