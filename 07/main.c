#include <stdio.h>
#include <stdlib.h>

typedef unsigned long u64;

int process_manifold(char *buff, u64 *manifold, int size)
{
	int splits = 0;

	for (int i = 0; i < size; ++i) {
		if (buff[i] == 'S') {
			manifold[i] = 1;
			break;
		} else if (buff[i] == '^' && manifold[i] > 0) {
			if (i > 0) {
				manifold[i - 1] += manifold[i];
			}
			if (i < size - 1) {
				manifold[i + 1] += manifold[i];
			}
			manifold[i] = 0;
			splits++;
		}
	}

	return splits;
}

u64 count_timelines(u64 *manifold, int size)
{
	u64 timelines = 0;

	for (int i = 0; i < size; ++i)
		timelines += manifold[i];

	return timelines;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.\n", file_path);
		exit(1);
	}

	int size = 0;
	char ch;

	while ((ch = fgetc(fp)) != '\n')
		size++;
	fseek(fp, 0, SEEK_SET);

	u64 *manifold = (u64 *) malloc(size * sizeof(u64));
	char *buff = (char *) malloc(size + 1);
	int splits = 0;

	while (fscanf(fp, "%s", buff) != EOF)
		splits += process_manifold(buff, manifold, size);

	u64 timelines = count_timelines(manifold, size);

	fclose(fp);
	free(manifold);
	free(buff);

	printf("[PART 1] Solution: %d\n", splits);
	printf("[PART 2] Solution: %lu\n", timelines);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
