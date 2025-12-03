#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 128

int len_str(const char *str)
{
	int len = 0;
	while (str[len] != '\0')
		len++;

	return len;
}

int max_in_str(const char *str, int start, int end)
{
	int max_index = start;
	for (int i = start + 1; i <= end; ++i) {
		if (str[i] > str[max_index])
			max_index = i;
	}

	return max_index;
}

long long max_joltage(const char *bank, int joltage_len)
{
	long long joltage = 0;
	int bank_len = len_str(bank);
	int offset = joltage_len - 1;
	int left = -1;

	long long mask = 1;
	for (int i = 1; i < joltage_len; ++i)
		mask *= 10;

	while (mask > 0) {
		left = max_in_str(bank, left + 1, bank_len - offset - 1);
		joltage += (bank[left] - '0') * mask;
		offset -= 1;
		mask /= 10;
	}

	return joltage;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.", file_path);
		exit(1);
	}

	char buff[BUFF_SIZE];
	long long total_joltage1 = 0;
	long long total_joltage2 = 0;

	while (fscanf(fp, "%s", buff) != EOF) {
		total_joltage1 += max_joltage(buff, 2);
		total_joltage2 += max_joltage(buff, 12);
	}
	fclose(fp);

	printf("[PART 1] Solution: %lld\n", total_joltage1);
	printf("[PART 2] Solution: %lld\n", total_joltage2);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
