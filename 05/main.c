#include <stdio.h>
#include <stdlib.h>

#define N 256
#define BUFF_SIZE 64

int is_fresh(long long id, long long id_ranges[][2], int n)
{
	for (int i = 0; i < n; ++i) {
		if (id >= id_ranges[i][0] && id <= id_ranges[i][1])
			return 1;
	}

	return 0;
}

void sort_ranges(long long id_ranges[][2], int n)
{
	int swapped;
	long long temp;

	for (int i = 0; i < n - 1; ++i) {
		swapped = 0;
		for (int j = 0; j < n - i - 1; ++j) {
			if (id_ranges[j][0] > id_ranges[j + 1][0]) {
				temp = id_ranges[j][0];
				id_ranges[j][0] = id_ranges[j + 1][0];
				id_ranges[j + 1][0] = temp;
				temp = id_ranges[j][1];
				id_ranges[j][1] = id_ranges[j + 1][1];
				id_ranges[j + 1][1] = temp;
				swapped = 1;
			}
		}

		if (!swapped)
			break;
	}
}

long long count_fresh(long long id_ranges[][2], int n)
{
	long long fresh = 0;
	long long left = -1;
	long long right = -1;

	sort_ranges(id_ranges, n);

	for (int i = 0; i < n; ++i) {
		if (left == -1)
			left = id_ranges[i][0];
		if (right == -1)
			right = id_ranges[i][1];

		if (i < n - 1 && right >= id_ranges[i + 1][0]) {
			right = (right > id_ranges[i + 1][1]) ? right : id_ranges[i + 1][1];
			continue;
		}

		fresh += right - left + 1;
		left = -1;
		right = -1;
	}

	return fresh;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.\n", file_path);
		exit(1);
	}

	long long id_ranges[N][2];
	long long curr_id;
	int ranges = 0;
	int fresh = 0;
	char buff[BUFF_SIZE];

	while (fscanf(fp, "%s", buff) != EOF) {
		if (sscanf(buff, "%lld-%lld", &id_ranges[ranges][0], &id_ranges[ranges][1]) == 2) {
			ranges += 1;
		} else {
			sscanf(buff, "%lld", &curr_id);
			if (is_fresh(curr_id, id_ranges, ranges))
				fresh += 1;
		}
	}
	fclose(fp);

	long long total_fresh = count_fresh(id_ranges, ranges);

	printf("[PART 1] Solution: %d\n", fresh);
	printf("[PART 2] Solution: %lld\n", total_fresh);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
