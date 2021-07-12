#include "stdafx.h"
#include "View.h"

#define MAX_VIEW_COUNT 4

static int viewCount;
static const View* activeViews[MAX_VIEW_COUNT];

const View* PeekView(void)
{
	return viewCount > 0 ? activeViews[viewCount - 1] : NULL;
}

void PushView(const View* view)
{
	// CRASH: View count is at maximum
	if (viewCount == MAX_VIEW_COUNT) {
		TraceLog(LOG_ERROR, TextFormat("VIEW: Maximum amount of views are already open (%d/%d)", viewCount, MAX_VIEW_COUNT));
		return;
	}

	// CRASH: View is already open
	if (IsViewOpen(view)) {
		TraceLog(LOG_ERROR, "VIEW: Attempted to open an already open view");
		return;
	}

	activeViews[viewCount] = view;
	viewCount++;

	if (activeViews[viewCount - 1]->Open)
		activeViews[viewCount - 1]->Open();
}

void PopView(void)
{
	// CRASH: There are no views to close
	if (viewCount == 0) {
		TraceLog(LOG_ERROR, "VIEW: There are no views to close");
		return;
	}

	if (activeViews[viewCount - 1]->Close)
		activeViews[viewCount - 1]->Close();

	activeViews[viewCount - 1] = NULL;
	viewCount--;
}

void PopAllViews(void)
{
	int i;

	for (i = viewCount - 1; i >= 0; i--) {
		if (activeViews[i]->Close)
			activeViews[i]->Close();

		activeViews[i] = NULL;
	}

	viewCount = 0;
}

void SetView(const View* view)
{
	if (viewCount > 0)
		PopAllViews();

	PushView(view);
}

bool IsViewOpen(const View* view)
{
	int i;

	for (i = 0; i < viewCount; i++) {
		// Assume views are constant pointers
		if (view == activeViews[i])
			return true;
	}

	return false;
}

int CountViews(void)
{
	return viewCount;
}

int MaxViews(void)
{
	return MAX_VIEW_COUNT;
}

bool ShouldProcessGameActors(void)
{
	int i;

	if (viewCount == 0)
		return true;

	for (i = 0; i < viewCount; i++) {
		if (activeViews[i]->disableGameActorProcess)
			return false;
	}

	return true;
}

bool ShouldRenderGameWorld(void)
{
	int i;

	if (viewCount == 0)
		return true;

	for (i = 0; i < viewCount; i++) {
		if (activeViews[i]->disableGameWorldRender)
			return false;
	}

	return true;
}

void ControlView(void)
{
	// CRASH: There must be at least 1 view
	if (viewCount == 0) {
		TraceLog(LOG_ERROR, "VIEW: No view is set");
		return;
	}

	if (activeViews[viewCount - 1]->Control)
		activeViews[viewCount - 1]->Control();
}

void RenderViews(void)
{
	int i;

	for (i = 0; i < viewCount; i++) {
		if (activeViews[i]->Render)
			activeViews[i]->Render();
	}
}