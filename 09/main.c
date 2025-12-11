#include <stdio.h>
#include <stdlib.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
	long x;
	long y;
} Point;

long max_area(Point *points, int points_size)
{
	long max_area = 0;
	long curr_area;

	for (int i = 0; i < points_size - 1; ++i) {
		for (int j = i + 1; j < points_size; ++j) {
			curr_area = (ABS(points[i].x - points[j].x) + 1) * (ABS(points[i].y - points[j].y) + 1);
			max_area = curr_area > max_area ? curr_area : max_area;
		}
	}

	return max_area;
}

int point_inside(Point *points, int points_size, Point target)
{
	int cross = 0;
	Point p1, p2;

	for (int i = 0; i < points_size; ++i) {
		p1 = points[i];
		p2 = points[(i + 1) % points_size];

		if (target.x <= MAX(p1.x, p2.x) && target.x >= MIN(p1.x, p2.x) && target.y <= MAX(p1.y, p2.y) && target.y >= MIN(p1.y, p2.y))
			return 1;

		if (target.y < p1.y != target.y < p2.y && target.x < MAX(p1.x, p2.x))
			cross++;

		p1 = p2;
	}

	return cross % 2 == 1;
}

int segment_inside(Point *points, int points_size, Point seg_p1, Point seg_p2)
{
	if (!point_inside(points, points_size, seg_p1) || !point_inside(points, points_size, seg_p2))
		return 0;

	Point p1, p2;

	for (int i = 0; i < points_size; ++i) {
		p1 = points[i];
		p2 = points[(i + 1) % points_size];

		if (seg_p1.x == seg_p2.x && p1.x == p2.x || seg_p1.y == seg_p2.y && p1.y == p2.y)
			continue;

		if (seg_p1.x == seg_p2.x) {
			if (seg_p1.x > MIN(p1.x, p2.x) && seg_p1.x < MAX(p1.x, p2.x) && p1.y > MIN(seg_p1.y, seg_p2.y) && p1.y < MAX(seg_p1.y, seg_p2.y))
				return 0;
		} else if (seg_p1.y == seg_p2.y) {
			if (seg_p1.y > MIN(p1.y, p2.y) && seg_p1.y < MAX(p1.y, p2.y) && p1.x > MIN(seg_p1.x, seg_p2.x) && p1.x < MAX(seg_p1.x, seg_p2.x))
				return 0;
		}
	}

	return 1;
}

long max_area_in_polygon(Point *points, int points_size)
{
	long max_area = 0;
	long curr_area;
	int max_x, min_x, max_y, min_y;

	for (int i = 0; i < points_size - 1; ++i) {
		for (int j = i + 1; j < points_size; ++j) {
			max_x = MAX(points[i].x, points[j].x);
			min_x = MIN(points[i].x, points[j].x);
			max_y = MAX(points[i].y, points[j].y);
			min_y = MIN(points[i].y, points[j].y);

			if (!segment_inside(points, points_size, (Point) {min_x, min_y}, (Point) {min_x, max_y}))
				continue;
			if (!segment_inside(points, points_size, (Point) {min_x, max_y}, (Point) {max_x, max_y}))
				continue;
			if (!segment_inside(points, points_size, (Point) {max_x, max_y}, (Point) {max_x, min_y}))
				continue;
			if (!segment_inside(points, points_size, (Point) {max_x, min_y}, (Point) {min_x, min_y}))
				continue;

			curr_area = (ABS(points[i].x - points[j].x) + 1) * (ABS(points[i].y - points[j].y) + 1);
			max_area = curr_area > max_area ? curr_area : max_area;
		}
	}

	return max_area;
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
		if (ch =='\n')
			points_size++;
	}
	fseek(fp, 0, SEEK_SET);

	Point *points = (Point *) malloc(points_size * sizeof(Point));
	int curr_point = 0;

	while (fscanf(fp, "%ld,%ld", &points[curr_point].x, &points[curr_point].y) != EOF)
		curr_point++;

	long result1 = max_area(points, points_size);
	long result2 = max_area_in_polygon(points, points_size);

	free(points);
	fclose(fp);

	printf("[PART 1] Solution: %ld\n", result1);
	printf("[PART 2] Solution: %ld\n", result2);
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; ++i)
		solve_file(argv[i]);

	return 0;
}
