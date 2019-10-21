#pragma once

char* UTIL_strcpy(char* destination, char* source)
{
	char* start = destination;

	while (*source != '\0')
	{
		*destination = *source;
		destination++;
		source++;
	}

	*destination = '\0';
	return start;
}