#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 256

int count_digits(long long num)
{
	int digits = 1;
	while ((num = num / 10))
		digits++;

	return digits;
}

int invalid_id(long long id)
{
	int id_len = count_digits(id);
	long long div = 1;
	long long prev, curr;

	for (int i = 1; i < id_len / 2 + 1; ++i) {
		div *= 10;
		if (id_len % i != 0)
			continue;

		curr = id;
		do {
			prev = curr % div;
			curr /= div;
		} while (curr % div == prev);

		if (curr == 0)
			return i;
	}

	return 0;
}

int halves_eq(long long id)
{
	int id_len = count_digits(id);
	long long div = 1;

	for (int i = 0; i < id_len / 2; ++i)
		div *= 10;

	return id % div == id / div;
}

long long sum_inv(const char *id_range, long long *halves)
{
	long long start, end;
	sscanf(id_range, "%lld-%lld", &start, &end);
	
	int seq_len;
	long long inv = 0;
	for (long long i = start; i <= end; i += 1) {
		seq_len = invalid_id(i);
		if (seq_len) {
			inv += i;
			if (halves_eq(i))
				*halves += i;
		}
	}
	return inv;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.", file_path);
		exit(1);
	}

	char buff[BUFF_SIZE];
	int curr = 0;
	long long halves = 0;
	long long inv = 0;
	char ch;

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == ',' || ch == '\n') {
			buff[curr] = '\0';
			inv += sum_inv(buff, &halves);
			curr = 0;
			continue;
		}
		buff[curr++] = ch;
	}
	fclose(fp);

	printf("[PART 1] Solution: %lld\n", halves);
	printf("[PART 2] Solution: %lld\n", inv);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
