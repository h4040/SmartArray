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

		// Allocate space for internal array using DEFAULT_INIT_LEN number of elements
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
			return 0;
		}
	}
	else // If smarty malloc failed, return 0
	{
		return 0;
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

			printf("-> Expanded SmartArray to size %i\n", length);

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

			// return smarty pointer
			return smarty;
		}
	}
}

char *put(SmartArray *smarty, char *str)
{
	int stringLength;
	char * stringToInsert;
	int index;
	int newCapacity;

	if (smarty && str) // if smarty and str is not null.
	{
		if (smarty->size >= smarty->capacity)
		{
			// expand capacity before inserting string, if size == capacity
			newCapacity = smarty->capacity * 2 + 1;
			smarty = expandSmartArray(smarty, newCapacity);
		}

		// Get length of string
		stringLength = strlen(str);

		// Allocate space for new string
		stringToInsert = malloc(stringLength * sizeof(char));

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
		return stringToInsert;
	}
	else
	{
		// if smarty or str is null, return null
		return NULL;
	}


}

char *get(SmartArray *smarty, int index)
{
	if (index < 0 || index > smarty->size || !smarty)
	{
		return NULL;
	}

	return smarty->array[index];
}

char *set(SmartArray *smarty, int index, char *str);

char *insertElement(SmartArray *smarty, int index, char *str);

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
	char firstName[] = { 'T', 'o', 'n', 'n', 'y','\0' };
	char lastName[] = { 'B', 'o', 'e', 's','e','n','b','a','e','k', '\0' };
	char * aString = "Hi I'm a string, but I don't exist in C baaaaa";
	char * getString;

	SmartArray * smart = createSmartArray(50);

	printf("Calling put() to insert string...\n");
	put(smart, firstName);
	put(smart, lastName);
	put(smart, aString);

	expandSmartArray(smart, 70);

	printf("Contents of SmartArray:\n");
	printSmartArray(smart);

	printf("Trimming SmartArray:\n");
	trimSmartArray(smart);

	size = getSize(smart);
	printf("SmartArray's capacity after trimming: %i\n", size);
	
	getString = get(smart, 1);
	printf("String at index 1: %s\n", getString);

	system("pause");
}