#include "smartArray.h"
#include <stdio.h>
#include <stdlib.h>

SmartArray * createSmartArray(int length)
{
	int lengthToUse;

	// Allocate space for the struct itself
	SmartArray * smarty = malloc(sizeof(SmartArray));

	if (smarty) // if smarty malloc succeeded
	{
		if (length < DEFAULT_INIT_LEN)
		{
			lengthToUse = DEFAULT_INIT_LEN;
		}
		else
		{
			lengthToUse = length;
		}

		// Allocate space for internal array 
		smarty->array = malloc(lengthToUse * sizeof(char*));

		if (smarty)
		{
			smarty->capacity = lengthToUse;
			smarty->size = 0;
			printf("-> Created new SmartArray of size %i\n", smarty->capacity);
			return smarty;
		}
		else
		{
			return NULL;
		}
	}
	else // If smarty malloc failed, return NULL
	{
		return NULL;
	}
}

SmartArray *destroySmartArray(SmartArray *smarty)
{
	int i;

	for (i = 0; i < smarty->size; i++)
	{
		free(smarty->array[i]);
	}

	free(smarty);

	return NULL;
}

SmartArray *expandSmartArray(SmartArray *smarty, int length)
{
	char ** newArray;
	int i;

	if (length <= smarty->capacity || !smarty)
	{
		// If specified length is <= current length or smarty is null, just return from the function.
		return;
	}
	else
	{
		// Allocate new array
		newArray = malloc(length * sizeof(char*));

		if (!smarty->array)
		{
			// Return null if malloc failed for new array.
			return NULL;
		}
		else
		{
			// copy contents of old array into new array
			for (i = 0; i < smarty->size; i++)
			{
				newArray[i] = smarty->array[i];
			}

			// Free old array
			free(smarty->array);

			// Set smarty array to point to newly created array
			smarty->array = newArray;

			// Update capacity
			smarty->capacity = length;

			printf("-> Expanded SmartArray to size %i\n", smarty->capacity);

			// return smarty pointer
			return smarty;
		}
	}
}

SmartArray *trimSmartArray(SmartArray *smarty)
{
	char ** trimmedArray;
	int i;

	if (!smarty)
	{
		// If smarty is null, return null
		return NULL;
	}

	if (smarty->capacity > smarty->size)
	{
		trimmedArray = malloc(smarty->size * sizeof(char*));

		if (!trimmedArray)
		{
			// Return null is malloc for trimmedArray failed
			return NULL;
		}
		else
		{
			// copy contents of old array into new array
			for (i = 0; i < smarty->size; i++)
			{
				trimmedArray[i] = smarty->array[i];
			}

			// Free old array
			free(smarty->array);

			// Set smarty array to point to newly created array
			smarty->array = trimmedArray;

			// Update capacity of smarty's array
			smarty->capacity = smarty->size;

			printf("-> Trimmed SmartArray to size %i\n", smarty->capacity);
		}
	}

	// return smarty pointer
	return smarty;
}

char *put(SmartArray *smarty, char *str)
{
	int stringLength;
	char * stringToInsert;
	int index;
	int newCapacity;

	if (smarty == NULL || str == NULL) // if smarty and str is not null.
	{
		// if smarty or str is null, return null
		return NULL;
	}

	if (smarty->size >= smarty->capacity)
	{
		// expand capacity before inserting string, if size == capacity
		newCapacity = smarty->capacity * 2 + 1;
		smarty = expandSmartArray(smarty, newCapacity);
	}

	// Get length of string
	stringLength = strlen(str);

	// Allocate space for new string. Add +1 because of \0 string terminator
	stringToInsert = malloc(stringLength * sizeof(char) + 1);

	if (!stringToInsert) // If malloc failed, return null.
	{
		return NULL;
	}

	// Copy string into new array
	strcpy(stringToInsert, str);

	// Get the first empty cell
	index = smarty->size;

	// Insert new string into smarty's internal array
	smarty->array[index] = stringToInsert;

	// increase smarty's size
	smarty->size = smarty->size + 1;

	// return pointer to the newly inserted string
	return smarty->array[index];
}


char *get(SmartArray *smarty, int index)
{
	if (index < 0 || index > smarty->size || !smarty)
	{
		return NULL;
	}

	return smarty->array[index];
}

char *set(SmartArray *smarty, int index, char *str)
{
	char * newString;
	int stringLength;

	if (index < 0 || index > smarty->size || !smarty || !str)
	{
		return NULL;
	}

	stringLength = strlen(str);
	// Add +1 to add space for \0 terminator
	newString = malloc(stringLength * sizeof(char) + 1);

	if (!newString)
	{
		// return null if malloc failed for new array.
		return NULL;
	}

	strcpy(newString, str);

	// free old string
	free(smarty->array[index]);

	// Insert new string into smarty's array
	smarty->array[index] = newString;

	// free newString
	free(newString);

	// Return pointer to newly inserted string
	return smarty->array[index];
}

char *insertElement(SmartArray *smarty, int index, char *str)
{
	char * stringToInsert;
	char * tempString;
	char ** tempArray;
	int newCapacity;
	int stringLength;
	int elementsToCopy;
	int i;

	if (!smarty || !str)
		return NULL;

	if (smarty->size >= smarty->capacity)
	{
		// expand capacity before inserting string, if size == capacity
		newCapacity = smarty->capacity * 2 + 1;
		smarty = expandSmartArray(smarty, newCapacity);
	}

	// Get length of string
	stringLength = strlen(str);

	// Allocate space for new string. Add +1 because of \0 string terminator
	stringToInsert = malloc(stringLength * sizeof(char) + 1);

	if (!stringToInsert) // If malloc failed, return null.
		return NULL;

	// Copy string into new array
	strcpy(stringToInsert, str);

	// If index is greater than current size of array, insert into first empty element
	if (index > smarty->size)
	{
		//smarty->array[smarty->size] = stringToInsert;
		put(smarty, stringToInsert);
	}
	else
	{
		// copy elements that are past the insertion point (index)
		elementsToCopy = smarty->size - index;

		// malloc tempArray.
		tempArray = malloc(elementsToCopy * sizeof(char*));

		if (!tempArray)
			return NULL;

		// Copy from smart array to tempArray
		for (i = 0; i < smarty->size; i++)
		{
			// Get length of string in smart array
			stringLength = strlen(smarty->array[i + index]);
			// Malloc space for tempString
			tempString = malloc(stringLength * sizeof(char) + 1);
			// Copy string from smartArray to tempString
			strcpy(tempString, smarty->array[i + index]);
			// Assign tempString pointer to tempArray
			tempArray[i] = tempString;
		}

		for (i = 0; i < elementsToCopy; i++)
		{
			printf("Content of tempArray: %s\n", tempArray[i]);
		}

		// Free old elements
		for (i = index; i < smarty->size; i++)
		{
			free(smarty->array[i]);
		}

		// copy the new string into [index]
		smarty->array[index] = stringToInsert;

		// Update smarty-> size to + 1
		smarty->size = smarty->size + 1;

		// copy contents of tempArray into smarty's array
		for (i = 0; i < elementsToCopy; i++)
		{
			// Get length of string in tempArray
			stringLength = strlen(tempArray[i]);
			// Malloc space for tempString
			tempString = malloc(stringLength * sizeof(char) + 1);
			// Copy string from tempString to smartArray
			strcpy(tempString, tempArray[i]);
			// Assign tempString to smartArray
			smarty->array[index + 1 + i] = tempString;
		}

		// Free temp array
		for (i = 0; i < elementsToCopy; i++)
		{
			free(tempArray[i]);
		}

		free(tempArray);

		// return newly inserted string
		return smarty->array[index];
	}
}

int removeElement(SmartArray *smarty, int index);

int getSize(SmartArray *smarty)
{
	if (smarty)
	{
		return smarty->size;
	}
	else
	{
		return -1;
	}
}

void printSmartArray(SmartArray *smarty)
{
	int i;

	if (!smarty || smarty->size <= 0)
	{
		printf("(empty array)\n");
	}
	else
	{
		for (i = 0; i < smarty->size; i++)
		{
			printf("%s\n", smarty->array[i]);
		}
	}
}

double difficultyRating(void);

double hoursSpent(void);

main()
{
	int size;
	char * firstString = "first string";
	char * secondString = "second string";
	char * insertString = "insertString";
	char * aString = "Hi I'm a string, but I don't exist in C baaaaa";
	char * getString;
	char * putString = "Hello I'm a new string";

	SmartArray * smart = createSmartArray(50);

	printf("Calling put() to insert string...\n");
	put(smart, firstString);
	put(smart, secondString);

	insertElement(smart, 0, insertString);
	printf("Inserted new string at index 0. Contents of array:\n");
	printSmartArray(smart);



	expandSmartArray(smart, 70);

	printf("Contents of SmartArray:\n");
	printSmartArray(smart);

	printf("Trimming SmartArray:\n");
	trimSmartArray(smart);

	size = getSize(smart);
	printf("SmartArray's capacity after trimming: %i\n", size);

	getString = get(smart, 1);
	printf("String at index 1: %s\n", getString);

	set(smart, 1, putString);
	printf("Put new string at index 1. Contents of array:\n");
	printSmartArray(smart);



	printf("Destroying smart array...\n");
	smart = destroySmartArray(smart);

	if (smart != NULL)
	{
		printf("Smart array was not destroyed\n");
	}
	else
	{
		printf("Smart array was succesfully destroyed\n");
	}

	system("pause");
}