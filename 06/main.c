#include <stdio.h>
#include <stdlib.h>

long long eval_by_rows(char *buff, int range_start, int range_end, char op)
{
	int size = range_end - range_start + 1;
	char *temp_buff = (char *) malloc(size + 1);
	long long result = (op == '*') ? 1 : 0;

	int pos = 0;
	int curr = 0;
	int flag = 0;
	int temp;

	for (int i = 0; buff[i] != '*' && buff[i] != '+'; ++i) {
		if (pos >= range_start && pos <= range_end) {
			temp_buff[curr++] = buff[i];
			flag = 1;
		} else if (flag) {
			temp_buff[curr] = '\0';
			sscanf(temp_buff, "%d", &temp);
			if (op == '*')
				result *= temp;
			else
				result += temp;
			curr = 0;
			flag = 0;
		}

		if (buff[i] == '\n')
			pos = 0;
		else
			pos++;
	}
	free(temp_buff);

	return result;
}

long long eval_by_cols(const char *buff, int rows, int line_size, int range_start, int range_end, char op)
{
	int size = rows - 1;
	char *temp_buff = (char *) malloc(size + 1);
	long long result = (op == '*') ? 1 : 0;

	int curr = 0;
	int temp;

	for (int i = range_start; i <= range_end; ++i) {
		for (int j = 0; j < rows - 1; ++j)
			temp_buff[curr++] = buff[i + line_size * j];

		temp_buff[curr] = '\0';
		sscanf(temp_buff, "%d", &temp);
		if (op == '*')
			result *= temp;
		else
			result += temp;
		curr = 0;
	}
	free(temp_buff);

	return result;
}

void col_range(const char *buff, int col, int *start, int *end, char *op)
{
	int curr_col = 0;
	int pos = 0;
	int i = 0;

	while (buff[i] != '\0') {
		if (buff[i] == '*' || buff[i] == '+') {
			*op = buff[i];
			curr_col++;
		} else if (buff[i] == '\n') {
			pos = 0;
			i++;
			continue;
		}

		i++;
		if (curr_col == col)
			break;
		pos++;
	}
	*start = pos;

	while (buff[i] != '\0') {
		if (buff[i] == '\n' || buff[i + 1] == '*' || buff[i + 1] == '+')
			break;
		pos++;
		i++;
	}
	*end = pos;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.\n", file_path);
		exit(1);
	}

	int cols = 0;
	int rows = 0;
	int line_size = 0;
	int count = 1;
	char ch;

	while ((ch = fgetc(fp)) != EOF) {
		if (count)
			line_size++;

		if (ch == '*' || ch == '+') {
			cols++;
		} else if (ch == '\n') {
			rows++;
			count = 0;
		}
	}

	printf("line_size: %d\n", line_size);
	printf("cols: %d\nrows: %d\n", cols, rows);
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);

	char *buff = (char *) malloc(size + 1);

	fseek(fp, 0, SEEK_SET);
	fread(buff, 1, size, fp);
	buff[size] = '\0';

	long long result1 = 0;
	long long result2 = 0;
	int end, start;
	char op;

	for (int i = 1; i <= cols; ++i) {
		col_range(buff, i, &start, &end, &op);
		result1 += eval_by_rows(buff, start, end, op);
		result2 += eval_by_cols(buff, rows, line_size, start, end, op);
	}

	fclose(fp);
	free(buff);

	printf("[PART 1] Solution: %lld\n", result1);
	printf("[PART 2] Solution: %lld\n", result2);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
