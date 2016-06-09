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

	if (smarty != NULL)
	{
		for (i = 0; i < smarty->size; i++)
		{
			free(smarty->array[i]);
		}

		free(smarty);
	}

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

	if (getSize(smarty) >= smarty->capacity)
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
	index = getSize(smarty);

	// Insert new string into smarty's internal array
	smarty->array[index] = stringToInsert;

	// increase smarty's size
	smarty->size = getSize(smarty) + 1;

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
			// tempArray holds pointers to contents of smarty's array
			tempArray[i] = smarty->array[i + index];
		}

		// copy the new string into [index]
		smarty->array[index] = stringToInsert;

		// Update smarty-> size to + 1
		smarty->size = smarty->size + 1;

		// copy pointers from tempArray into smarty's array
		for (i = 0; i < elementsToCopy; i++)
		{
			smarty->array[index + 1 + i] = tempArray[i];
		}

		// Free temp array
		free(tempArray);

		// return newly inserted string
		return smarty->array[index];
	}
}

int removeElement(SmartArray *smarty, int index)
{
	int elementsToMove;
	int i;
	char ** tempArray;
	char * tempString;
	int stringLength;
	int elementToFree;

	if (!smarty)
	{
		return 0;
	}

	// if index exceeds the array size, nothing is removed from the array
	if (index > (getSize(smarty) - 1))
	{
		return 0;
	}

	/*
	If the last element is being removed, 
	there is no need to shift the remaining elements in the array around.
	*/
	if (index == getSize(smarty) - 1)
	{
		free(smarty->array[index]);

		// update size
		smarty->size -= 1;
	}
	else
	{
		// Calculate how many elements to shift one space to the left
		elementsToMove = getSize(smarty) - index - 1;

		// allocate space for tempArray
		tempArray = malloc(elementsToMove * sizeof(char*));

		if (!tempArray)
		{
			return 0;
		}

		// copy elements to the right of index into tempArray.
		// Make a copy of the strings, so that we can free() the unneeded element at the end of the array
		for (i = 0; i < elementsToMove; i++)
		{
			// Get length of string
			stringLength = strlen(smarty->array[i + index + 1]);
			// Malloc space for string
			tempString = malloc(stringLength * sizeof(char) + 1);

			if (!tempString)
				return 0;

			// Make a copy of the string to move into tempArray
			strcpy(tempString, (smarty->array[i + index + 1]));

			// Copy string into tempArray
			tempArray[i] = tempString;
		}

		/* Copy elements from tempArray into smart array
		starting at index's place
		*/
		for (i = 0; i < elementsToMove; i++)
		{
			smarty->array[i + index] = tempArray[i];
		}

		// free tempArray. No longer needed.
		free(tempArray);

		// free last element that has now been shifted to the left by one element
		elementToFree = getSize(smarty) - 1;
		free(smarty->array[elementToFree]);

		// update size
		smarty->size -= 1;
	}

	// return 1 if successfully removed element
	return 1;
}

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

	if (!smarty || getSize(smarty) <= 0)
	{
		printf("(empty array)\n");
	}
	else
	{
		for (i = 0; i < getSize(smarty); i++)
		{
			printf("%s\n", smarty->array[i]);
		}
	}
}

double difficultyRating(void)
{
	return 3.5;
}

double hoursSpent(void)
{
	return 7.0;
}

mainASD()
{
	int size;
	char * firstString = "first string";
	char * secondString = "second string";
	char * thirdString = "third string";
	char * fourthString = "fourth string";
	char * insertString = "insertString";
	char * aString = "Hi I'm a string, but I don't exist in C baaaaa";
	char * getString;
	char * putString = "Hello I'm a new string";

	SmartArray * smart = createSmartArray(10);

	printf("Calling put() to insert strings...\n");
	put(smart, firstString);
	put(smart, secondString);
	put(smart, thirdString);
	put(smart, fourthString);
	printSmartArray(smart);

	printf("\n\n");

	removeElement(smart, 3);
	printf("Array contents after removing index 1:\n");
	printSmartArray(smart);


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

	if (smart == NULL)
	{
		printf("Smart array was succesfully destroyed\n");
	}
	else
	{
		printf("Smart array was not destroyed\n");
	}

	system("pause");
}