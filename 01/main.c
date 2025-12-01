#include <stdio.h>
#include <stdlib.h>

#define START 50
#define MAX_ROTATIONS 100
#define BUFF_SIZE 256

int rotate(const char *instruction, int *curr)
{
	int rotations;
	sscanf(instruction + 1, "%d", &rotations);

	int total_clicks = 0;

	if (instruction[0] == 'R') {
		total_clicks += (*curr + rotations) / MAX_ROTATIONS;
		*curr = (*curr + rotations) % MAX_ROTATIONS;
	} else if (instruction[0] == 'L') {
		*curr = (MAX_ROTATIONS - *curr) % MAX_ROTATIONS;
		total_clicks += (*curr + rotations) / MAX_ROTATIONS;
		*curr = (*curr + rotations) % MAX_ROTATIONS;
		*curr = (MAX_ROTATIONS - *curr) % MAX_ROTATIONS;
	}

	return total_clicks;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.", file_path);
		exit(1);
	}

	char buff[BUFF_SIZE];
	int curr = START;
	int clicks = 0;
	int total_clicks = 0;

	while (fscanf(fp, "%s", buff) != EOF) {
		total_clicks += rotate(buff, &curr);
		if (curr % MAX_ROTATIONS == 0)
			clicks += 1;
	}
	fclose(fp);

	printf("[PART 1] Solution: %d\n", clicks);
	printf("[PART 2] Solution: %d\n", total_clicks);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
