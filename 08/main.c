#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TARGET_CONNECTIONS 1000

typedef struct {
	int x;
	int y;
	int z;
	int circuit;
} Point;

typedef struct {
	int point1;
	int point2;
	float distance;
} Pair;

int parse_points(Point *points, int points_size, Pair *pairs)
{
	int curr_pair = 0;
	float curr_distance;

	for (int i = 0; i < points_size - 1; ++i) {
		for (int j = i + 1; j < points_size; ++j) {
			pairs[curr_pair].point1 = i;
			pairs[curr_pair].point2 = j;

			curr_distance = sqrt(pow(points[j].x - points[i].x, 2) + pow(points[j].y - points[i].y, 2) + pow(points[j].z - points[i].z, 2));
			pairs[curr_pair].distance = curr_distance;

			curr_pair++;
		}
	}
}

int compare_pair(const void *pair1, const void *pair2)
{
	if (((Pair *) pair1)->distance > ((Pair *) pair2)->distance)
		return 1;
	else if (((Pair *) pair1)->distance < ((Pair *) pair2)->distance)
		return -1;
	return 0;
}

int compare_int(const void *a, const void *b)
{
	return *((int *) b) - *((int *) a);
}

void merge_circuits(Point *points, int points_size, int left, int right)
{
	int new_circuit = points[left].circuit;
	int circuit_to_merge = points[right].circuit;

	for (int i = 0; i < points_size; ++i) {
		if (points[i].circuit == circuit_to_merge)
			points[i].circuit = new_circuit;
	}
}

int all_connected(Point *points, int points_size)
{
	int prev_circuit = points[0].circuit;

	for (int i = 1; i < points_size; ++i) {
		if (points[i].circuit != prev_circuit)
			return 0;

		prev_circuit = points[i].circuit;
	}

	return 1;
}

long connect_points(Point *points, int points_size, Pair *pairs, int pairs_size, int target_connections, int *max_circuits_prod)
{
	int curr = 0;
	int last_left = -1;
	int last_right = -1;
	long last_points_prod;

	while (curr < pairs_size && !all_connected(points, points_size)) {
		if (curr == target_connections) {
			int *circuits = (int *) malloc(points_size * sizeof(int));

			for (int i = 0; i < points_size; ++i)
				circuits[i] = 0;

			for (int i = 0; i < points_size; ++i)
				circuits[points[i].circuit] += 1;

			qsort(circuits, points_size, sizeof(int), compare_int);
			*max_circuits_prod = circuits[0] * circuits[1] * circuits[2];
			free(circuits);
		}

		if (points[pairs[curr].point1].circuit != points[pairs[curr].point2].circuit) {
			last_left = pairs[curr].point1;
			last_right = pairs[curr].point2;
			merge_circuits(points, points_size, last_left, last_right);
		}
		curr++;
	}

	if (last_left == -1 && last_right == -1)
		return 0;

	last_points_prod = (long) points[last_left].x * points[last_right].x;

	return last_points_prod;
}

void solve_file(const char *file_path)
{
	FILE *fp = fopen(file_path, "r");
	if (!fp) {
		fprintf(stderr, "[ERROR] File `%s` doesn't exists.\n", file_path);
		exit(1);
	}

	int points_size = 0;
	char ch;

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n')
			points_size++;
	}
	fseek(fp, 0, SEEK_SET);

	Point *points = (Point *) malloc(points_size * sizeof(Point));

	int curr = 0;
	while (fscanf(fp, "%d,%d,%d", &points[curr].x, &points[curr].y, &points[curr].z) != EOF) {
		points[curr].circuit = curr;
		curr++;
	}

	int pairs_size = points_size * (points_size - 1) / 2;
	Pair *pairs = (Pair *) malloc(pairs_size * sizeof(Pair));

	parse_points(points, points_size, pairs);
	qsort(pairs, pairs_size, sizeof(Pair), compare_pair);

	int max_circuits_prod;
	long last_points_prod = connect_points(points, points_size, pairs, pairs_size, TARGET_CONNECTIONS, &max_circuits_prod);

	free(points);
	free(pairs);
	fclose(fp);

	printf("[PART 1] Solution: %d\n", max_circuits_prod);
	printf("[PART 2] Solution: %ld\n", last_points_prod);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
