#pragma once

#if ASCII_DEFINE

typedef struct Handle 
{
	unsigned int version;
	int index;
} Handle;

#define NULL_HANDLE CLITERAL(Handle){ .version = 0, .index = 0U }

inline static bool IsHandleNull(Handle handle)
{
	return handle.version == 0 && handle.index < 0;
}

inline static bool AreHandlesEqual(Handle h0, Handle h1)
{
	return h0.version == h1.version && h0.index == h1.index;
}

#endif