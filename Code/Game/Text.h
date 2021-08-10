#pragma once

namespace Text
{
	enum
	{
		FontA,
		FontB,
	};

	bool Init();
	void Uninit();

	void DrawString(const char* str, int x, int y);
	void DrawStringRight(const char* str, int x, int y);
	void DrawStringCenter(const char* str, int x, int y, int width);
	void DrawChar(char c, int count, int x, int y);
	void DrawChar(char c, int x, int y, Color tint);

	void DrawString(const char* str, int fontId, int x, int y, Color tint);
	void DrawStringRight(const char* str, int fontId, Color tint, int x, int y);

	void DrawDialogString(const char* str, int x, int y, const char* itemName);
	void DrawDialogString(const char* str, int x, int y, const char* itemName, Color tint);

	void DrawBox(int x, int y, int width, int height);
	void DrawBoxPart(int x, int y, int width, int height, int visHeight);

	void DrawCursor(int x, int y);
}
