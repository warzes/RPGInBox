#include "terminal.h"

#include <stdlib.h> // for NULL

static int terminalWidth;
static int terminalHeight;
static TerminalTile *terminalBuffer;
static int terminalFontScale;
static Texture2D terminalFont;

void InitTerminal(int width, int height)
{
	// CRASH: Can only initialize once
	if (terminalWidth != 0 && terminalHeight != 0) {
		TraceLog(LOG_ERROR, "TERMINAL: Already initialized");
		return;
	}

	SetTerminalSize(width, height);
	TraceLog(LOG_INFO, "TERMINAL: Initialized successfully");
}

void CloseTerminal(void)
{
	if (terminalBuffer) {
		MemFree(terminalBuffer);
		terminalBuffer = NULL;
	}

	terminalWidth = 0;
	terminalHeight = 0;
}

void LoadTerminalFont(const char *fileName, int scale)
{
	Image image;
	Color *colors;

	image = LoadImage(fileName);
	ImageFormat(&image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	// Set scale
	if (scale > 1)
		ImageResizeNN(&image, image.width * scale, image.height * scale);

	terminalFontScale = scale;
	
	// Set transparency based on first pixel
	colors = LoadImageColors(image);
	ImageColorReplace(&image, colors[0], ColorAlpha(colors[0], 0));

	// Load final texture
	UnloadTexture(terminalFont);
	terminalFont = LoadTextureFromImage(image);

	// Allocation clean up
	UnloadImageColors(colors);
	UnloadImage(image);
}

void ClearTerminal(void)
{
	int i;
	int length;

	length = terminalWidth * terminalHeight;

	for (i = 0; i < length; i++) {
		terminalBuffer[i].background = BLACK;
		terminalBuffer[i].foreground = GRAY;
		terminalBuffer[i].symbol = ' ';
	}
}

inline int GetTerminalXYtoI(int posX, int posY)
{
	return (terminalWidth * posY) + posX;
}

inline int GetTerminalWidth(void)
{
	return terminalWidth;
}

inline int GetTerminalHeight(void)
{
	return terminalHeight;
}

inline int GetTerminalFontWidth(void)
{
	return terminalFont.width / 16;
}

inline int GetTerminalFontHeight(void)
{
	return terminalFont.height / 16;
}

inline int GetTerminalScreenWidth(void)
{
	return GetTerminalFontWidth() * terminalWidth;
}

inline int GetTerminalScreenHeight(void)
{
	return GetTerminalFontHeight() * terminalHeight;
}

inline int GetTerminalFontScale(void)
{
	return terminalFontScale;
}

inline TerminalTile GetTerminalTile(int posX, int posY)
{
	return terminalBuffer[GetTerminalXYtoI(posX, posY)];
}

inline TerminalTile *GetTerminalTileBuffer(void)
{
	return terminalBuffer;
}

inline bool IsWithinTerminal(int posX, int posY)
{
	return posX >= 0
		&& posX < terminalWidth
		&& posY >= 0
		&& posY < terminalHeight;
}

inline bool IsWithinTerminalV(Vector2 position)
{
	return (int)position.x >= 0
		&& (int)position.x < terminalWidth
		&& (int)position.y >= 0
		&& (int)position.y <= terminalHeight;
}

void SetTerminalFontScale(int scale)
{
	if (terminalFontScale == scale) {
		TraceLog(LOG_WARNING, TextFormat("TERMINAL: Terminal font scale is already %d", scale));
		return;
	} else {
		Image image;

		if (scale < 1)
			scale = 1;

		image = LoadImageFromTexture(terminalFont);
		UnloadTexture(terminalFont);

		if (terminalFontScale != 1)
			ImageResizeNN(&image, image.width / terminalFontScale, image.height / terminalFontScale);

		ImageResizeNN(&image, image.width * scale, image.height * scale);
		UnloadTexture(terminalFont);
		terminalFont = LoadTextureFromImage(image);
		terminalFontScale = scale;
		UnloadImage(image);
	}
}

void SetTerminalSize(int width, int height)
{
	// EXIT: Already the same size
	if (terminalWidth == width && terminalHeight == height) {
		TraceLog(LOG_WARNING, TextFormat("TERMINAL: Failed to resize terminal to %dx%d because terminal is already %dx%d", width, height, terminalWidth, terminalHeight));
		return;
	}

	if (terminalBuffer)
		MemFree(terminalBuffer);

	terminalBuffer = (TerminalTile*)MemAlloc(width * height * sizeof(*terminalBuffer));

	// CRASH: Allocation failure
	if (!terminalBuffer) {
		terminalWidth = 0;
		terminalHeight = 0;
		TraceLog(LOG_ERROR, TextFormat("TERMINAL: Allocation failed when setting terminal size to %dx%d", width, height));
		return;
	}

	terminalWidth = width;
	terminalHeight = height;
	ClearTerminal();
}

inline void SetTerminalTile(int posX, int posY, TerminalTile tile)
{
	terminalBuffer[GetTerminalXYtoI(posX, posY)] = tile;
}

inline void SetTerminalTileV(Vector2 position, TerminalTile tile)
{
	terminalBuffer[GetTerminalXYtoI((int)position.x, (int)position.y)] = tile;
}

void DrawTerminal(void)
{
	int x;
	int y;
	int xRenderOffset;
	int yRenderOffset;
	int tileWidth;
	int tileHeight;

	// Store sizes
	tileWidth = GetTerminalFontWidth();
	tileHeight = GetTerminalFontHeight();

	// Store render offsets for screen centering
	xRenderOffset = GetScreenWidth() - GetTerminalScreenWidth();
	yRenderOffset = GetScreenHeight() - GetTerminalScreenHeight();

	if (xRenderOffset < 0)
		xRenderOffset = 0;
	else if (xRenderOffset > 1)
		xRenderOffset = xRenderOffset / 2;

	if (yRenderOffset < 0)
		yRenderOffset = 0;
	else if (yRenderOffset > 1)
		yRenderOffset = yRenderOffset / 2;

	// Render tiles
	for (y = 0; y < terminalHeight; y++) {
		for (x = 0; x < terminalWidth; x++) {
			Vector2 position;
			TerminalTile *tile;

			position.x = (tileWidth * x) + xRenderOffset;
			position.y = (tileHeight * y) + yRenderOffset;
			tile = &terminalBuffer[GetTerminalXYtoI(x, y)];

			// Draw background part
			DrawRectangle(position.x, position.y, tileWidth, tileHeight, tile->background);

			// Draw symbol part
			if (tile->symbol != ' ' && tile->symbol != '\n') {
				Rectangle cutout;

				cutout.x = (tile->symbol % 16) * tileWidth;
				cutout.y = (tile->symbol / 16) * tileHeight;
				cutout.width = tileWidth;
				cutout.height = tileHeight;
				DrawTextureRec(terminalFont, cutout, position, tile->foreground);
			}
		}
	}
}
