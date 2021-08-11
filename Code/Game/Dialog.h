#pragma once

class Dialog
{
	typedef void (Dialog::* UpdateFunc)();

	static const int TextLines = 8;
	static const int BoxLines = 11;

	const char* lines[TextLines];
	const char* itemName;

	UpdateFunc curUpdate;
	int frameTimer;
	bool isClosed;
	int visibleBoxLines;

public:
	Dialog();

	void Reinit(const char* text, const char* itemName);
	bool IsClosed();

	void Update();
	void Draw();

private:
	void UpdateOpen();
	void UpdateIdle();
	void UpdateClose();

	void SplitText(const char* text);
};