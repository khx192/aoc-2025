#include <stdio.h>
#include <stdlib.h>

#define N 256

int len_str(const char *str)
{
	int len = 0;
	while (str[len] != '\0')
		len++;

	return len;
}

int accessible_roll(char grid[N][N], int row, int col, int total_rows, int total_cols)
{
	int rolls_around = 0;
	for (int i = row - 1; i <= row + 1; ++i) {
		for (int j = col - 1; j <= col + 1; ++j) {
			if (i == row && j == col)
				continue;
			
			if (i >= 0  && i < total_rows && j >= 0 && j < total_cols && grid[i][j] == '@')
				rolls_around += 1;
		}
	}

	return rolls_around < 4;
}

int count_rolls(char grid[N][N], int rows, int cols)
{
	int total = 0;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (grid[i][j] == '@' && accessible_roll(grid, i, j, rows, cols))
				total += 1;
		}
	}

	return total;
}

int removed_rolls(char grid[N][N], int rows, int cols)
{
	char new_grid[N][N];
	int removed = 0;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (grid[i][j] == '@' && accessible_roll(grid, i, j, rows, cols)) {
				removed += 1;
				new_grid[i][j] = '.';
			} else {
				new_grid[i][j] = grid[i][j];
			}
		}
	}

	if (removed > 0)
		removed += removed_rolls(new_grid, rows, cols);

	return removed;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.", file_path);
		exit(1);
	}

	int rows = 0;
	char grid[N][N];

	while (fscanf(fp, "%s", grid[rows]) != EOF)
		rows++;
	fclose(fp);

	int cols = len_str(grid[0]);
	int total = count_rolls(grid, rows, cols);
	int removed = removed_rolls(grid, rows, cols);

	printf("[PART 1] Solution: %d\n", total);
	printf("[PART 2] Solution: %d\n", removed);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
