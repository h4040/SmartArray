#include <stdio.h>
#include "SmartArray.h"

// Boundary check to make sure your difficultyRating() and hoursSpent()
// functions are implemented correctly.

int main(void)
{
	if (difficultyRating() < 1.0 || difficultyRating() > 5.0)
		printf("difficultyRating() test: **fail**\n");
	else
		printf("difficultyRating() test: PASS\n");

	if (hoursSpent() < 0.0)
		printf("hoursSpent() test: **fail**\n");
	else
		printf("hoursSpent() test: PASS\n");

	system("pause");
	return 0;
}
