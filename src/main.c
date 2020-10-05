#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RESULT_STRING_LENGTH 32
#define error(...) (fprintf(stderr, __VA_ARGS__))
#define array_length(x) (sizeof(x) / sizeof(x[0]))

double calculate_rectangle_rule(double left_border, double right_border) {
	return (right_border - left_border) * sin((left_border + right_border) / 2);
}

double calculate_simpsons_rule(double left_border, double right_border) {
	return (right_border - left_border) / 6 * (sin(left_border) + 4 * sin((left_border + right_border) / 2) + sin(right_border));
}

char* calculate_integral(double left_border, double right_border, int fragments_count) {
	double result_rectangle_rule = 0;
	double result_simpsons_rule = 0;
	double step = (right_border - left_border) / fragments_count;
	double temp_left_border = left_border;
	double temp_right_border = left_border + step;
	for (int i = 0; i < fragments_count; i++) {
		result_rectangle_rule += calculate_rectangle_rule(temp_left_border, temp_right_border);
		result_simpsons_rule += calculate_simpsons_rule(temp_left_border, temp_right_border);
		temp_left_border = temp_right_border;
		temp_right_border += step;
	}
	char* result_string = (char*)malloc(MAX_RESULT_STRING_LENGTH * sizeof(char));
	if (!sprintf(result_string, "%d %1.5lf %1.5lf", fragments_count, result_rectangle_rule, result_simpsons_rule)) {
		error("Cannot write results to string in experiment\n");
		return NULL;
	}
	return result_string;
}

int read_interval(double* left_border, double* right_border) {
	if (printf("Enter interval's left border: ") < 0) {
		error("Cannot write to stdout to stdout");
		return -1;
	}
	if (scanf("%lf", left_border) != 1) {
		error("Cannot read interval's left border");
		return -1;
	}
	if (*left_border < 0) {
		error("Left border of the interval must be greater than or equal to 0");
		return -1;
	}
	if (printf("Enter interval's right border: ") < 0) {
		error("Cannot write to stdout");
		return -1;
	}
	if ((scanf("%lf", right_border)) != 1) {
		error("Cannot read interval's right border");
		return -1;
	}
	if (*right_border > M_PI) {
		error("Right border of the interval must be less than or equal to pi");
		return -1;
	}
	if (*left_border >= *right_border) {
		error("Right border of the interval must be greater than left");
		return -1;
	}
	return 0;
}

void free_array_of_strings(char** array_of_strings, int count_of_strings_in_array) {
	for (unsigned int i = 0; i < count_of_strings_in_array; i++) {
		free(array_of_strings[i]);
	}
	free(array_of_strings);
}

char** calculate_integrals(double left_border, double right_border, int* fragment_count_sizes, int count) {
	char** results = (char**)malloc(sizeof(char*) * count);
	if (!results) {
		return NULL;
	}
	for (int i = 0; i < count; i++) {
		results[i] = (char*)malloc(MAX_RESULT_STRING_LENGTH * sizeof(char));
		if (!results[i]) {
			free_array_of_strings(results, i);
			error("Cannot allocate memory for result string in %d experiment\n", i);
			return NULL;
		}
		results[i] = calculate_integral(left_border, right_border, fragment_count_sizes[i]);
	}
	return results;
}

int main() {
	double left_border, right_border;
	if (read_interval(&left_border, &right_border)) {
		return 1;
	}
	int fragments_count_sizes[] = { 5, 10, 20, 100, 500, 1000 };
	int count = array_length(fragments_count_sizes);
	char** results = calculate_integrals(left_border, right_border, fragments_count_sizes, count);
	if (!results) {
		return 1;
	}
	for (unsigned int i = 0; i < count; ++i) {
		if (printf("%s\n", results[i]) < 0) {
			error("Cannot wtite %d result to stdout", i);
			break;
		}
	}
	free_array_of_strings(results, count);
	return 0;
}
