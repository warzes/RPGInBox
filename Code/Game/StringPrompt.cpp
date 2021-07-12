#include "stdafx.h"
#if ASCII_DEFINE
#include "View.h"

static void OnOpenView(void);
static void OnCloseView(void);
static void OnControlView(void);
static void OnRenderView(void);

static const View STRING_PROMPT_VIEW = {
	.Open = OnOpenView,
	.Close = OnCloseView,
	.Control = OnControlView,
	.Render = OnRenderView,
	.disableGameActorProcess = true,
	.disableGameWorldRender = false
};

static bool promptIsOpen;
static char** promptOutString;
static int promptMaxStringLength;
static const char* promptTitleText;
static const char* promptMessageText;
static const char* promptAcceptText;

void OpenStringPrompt(char** outString, int maxStringLength, const char* titleText, const char* messageText, const char* acceptText)
{
	// TODO
}

static void OnOpenView(void)
{
	//
}

static void OnCloseView(void)
{
	//
}

static void OnControlView(void)
{
	//
}

static void OnRenderView(void)
{
	//
}
#endif