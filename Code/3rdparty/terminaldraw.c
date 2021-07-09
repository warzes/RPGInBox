#include "terminaldraw.h"

static int ClampInt(int i, int min, int max);
static void SwapIntScalar(int *i0, int *i1);

inline void DrawTerminalBox(int x, int y, int width, int height, TerminalTile fill, TerminalTile outline)
{
	DrawTerminalBoxFill(x, y, width, height, fill);
	DrawTerminalBoxOutline(x, y, width, height, outline);
}

void DrawTerminalBoxFill(int x, int y, int width, int height, TerminalTile fill)
{
	int ix;
	int iy;
	int terminalWidth;
	int terminalHeight;
	int top;
	int right;
	int bottom;
	int left;

	terminalWidth = GetTerminalWidth();
	terminalHeight = GetTerminalHeight();
	top = ClampInt(y, 0, terminalHeight);
	right = ClampInt(x + width, 0, terminalWidth);
	bottom = ClampInt(y + height, 0, terminalHeight);
	left = ClampInt(x, 0, terminalWidth);

	if (left > right)
		SwapIntScalar(&left, &right);

	if (top > bottom)
		SwapIntScalar(&top, &bottom);

	for (iy = top; iy < bottom; iy++) {
		for (ix = left; ix < right; ix++)
			SetTerminalTile(ix, iy, fill);
	}
}

void DrawTerminalBoxOutline(int x, int y, int width, int height, TerminalTile outline)
{
	int terminalWidth;
	int terminalHeight;
	int top;
	int right;
	int bottom;
	int left;
	int clampedTop;
	int clampedRight;
	int clampedBottom;

	terminalWidth = GetTerminalWidth();
	terminalHeight = GetTerminalHeight();
	top = y;
	right = x + width;
	bottom = y + height;
	left = x;
	clampedTop = (top < 0) ? 0 : top;
	clampedRight = (right > terminalWidth) ? terminalWidth : right;
	clampedBottom = (bottom > terminalHeight) ? terminalHeight : bottom;

	// Draw top and bottom borders
	if (top >= 0 && top < terminalHeight && left < clampedRight) {
		int ix;

		for (ix = left; ix < clampedRight; ix++)
			SetTerminalTile(ix, top, outline);
		
		for (ix = left; ix < clampedRight; ix++)
			SetTerminalTile(ix, bottom - 1, outline);
	}

	// Draw middle-left border
	if (left >= 0 && left < terminalWidth && clampedTop < clampedBottom) {
		int iy;

		for (iy = top; iy < bottom; iy++)
			SetTerminalTile(left, iy, outline);
		
		for (iy = top; iy < bottom; iy++)
			SetTerminalTile(right - 1, iy, outline);
	}
}

inline static int ClampInt(int i, int min, int max)
{
	return (i < min) ? min : (i > max) ? max : i;
}

inline static void SwapIntScalar(int *i0, int *i1)
{
	int store;

	store = *i0;
	*i0 = *i1;
	*i1 = store;
}
