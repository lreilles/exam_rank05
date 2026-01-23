#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct s_map {
	int rows;
	int cols;
	char empty;
	char obstacle;
	char full;
	char **lines;
} t_map;

static void free_map(t_map *m) {
	if (!m) return;
	if (m->lines) {
		for (int i = 0; i < m->rows; i++) {
			free(m->lines[i]);
		}
		free(m->lines);
	}
}

static int load_elements(FILE *fp, t_map *m) {
	int ret = fscanf(fp, "%d %c %c %c", &m->rows, &m->empty, &m->obstacle, &m->full);
	if (ret != 4)
		return 0;
	if (m->rows <= 0)
		return 0;
	if (m->empty == m->obstacle || m->empty == m->full || m->obstacle == m->full)
		return 0;
	if (m->empty < 32 || m->empty > 126)
		return 0;
	if (m->obstacle < 32 || m->obstacle > 126)
		return 0;
	if (m->full < 32 || m->full > 126)
		return 0;
	return 1;
}

static int read_map(FILE *fp, t_map *m) {
	char *line = NULL;
	size_t cap = 0;
	ssize_t len;

	m->lines = NULL;
	m->cols = 0;

	if (!load_elements(fp, m)) {
		free(line);
		return 0;
	}
	len = getline(&line, &cap, fp);
	if (len < 0) {
		free(line);
		return 0;
	}

	m->lines = (char **)calloc((size_t)m->rows, sizeof(char *));
	if (!m->lines) {
		free(line);
		return 0;
	}

	for (int r = 0; r < m->rows; r++) {
		len = getline(&line, &cap, fp);
		if (len < 0) {
			free(line);
			return 0;
		}
		// check si la ligne fini par un \n
		if (len == 0 || line[len - 1] != '\n') {
			free(line);
			return 0;
		}
		int row_len = (int)len - 1;
		// si la ligne est vide on retourne une erreur
		if (row_len <= 0) {
			free(line);
			return 0;
		}
		// check que toutes les lgines font lq meme taille
		if (r == 0) {
			m->cols = row_len;
		} else if (row_len != m->cols) {
			free(line);
			return 0;
		}
		m->lines[r] = (char *)malloc((size_t)m->cols + 1);
		if (!m->lines[r]) {
			free(line);
			return 0;
		}
		// verifie que la ligne a que les caracteres demander
		for (int c = 0; c < m->cols; c++) {
			char ch = line[c];
			if (ch != m->empty && ch != m->obstacle) {
				free(line);
				return 0;
			}
			m->lines[r][c] = ch;
		}
		m->lines[r][m->cols] = '\0';
	}

	free(line);
	return 1;
}

static int find_min(int a, int b, int c) {
	int min = a;
	if (b < min) 
		min = b;
	if (c < min) 
		min = c;
	return min;
}

static int **initialise_map(int raw, int col)
{
	int **res;

	res = calloc(raw, sizeof(int *));
	for (int i = 0; i < raw; i++)
		res[i] = calloc(col, sizeof(int));
	return (res);
}


static void solve_map(t_map *m) {
	int best_size = 0;
	int best_row = 0;
	int best_col = 0;
	int **tmp_map;

	tmp_map = initialise_map(m->rows, m->cols);
	for (int row = 0; row < m->rows; row++)
	{
		for (int col = 0; col < m->cols; col++)
		{
			if (m->lines[row][col] == m->obstacle)
				tmp_map[row][col] = 0;
			else if (row == 0 || col == 0)
				tmp_map[row][col] = 1;
			else
			{
				int min = find_min(tmp_map[row - 1][col], tmp_map[row - 1][col - 1], tmp_map[row][col - 1]);
				tmp_map[row][col] = min + 1;
			}
			if (tmp_map[row][col] > 0)
			{
				int top = row - tmp_map[row][col] + 1;
				int left = col - tmp_map[row][col] + 1;

				if (tmp_map[row][col] > best_size || (tmp_map[row][col] == best_size && (top < best_row || (top == best_row && left < best_col)))) {
					best_size = tmp_map[row][col];
					best_row = top;
					best_col = left;
				}
			}
		}
	}

	for (int r = best_row; r < best_row + best_size; r++) {
		for (int c = best_col; c < best_col + best_size; c++) {
			m->lines[r][c] = m->full;
		}
	}

	for (int row = 0; row < m->rows; row++)
		free(tmp_map[row]);
	free(tmp_map);
}

static void print_map(t_map *m) {
	for (int r = 0; r < m->rows; r++) {
		fputs(m->lines[r], stdout);
		fputs("\n", stdout);
	}
}

static int process_stream(FILE *fp) {
	t_map m;
	int ok = read_map(fp, &m);
	if (!ok) {
		free_map(&m);
		return 0;
	}
	solve_map(&m);
	print_map(&m);
	free_map(&m);
	return 1;
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		if (!process_stream(stdin)) {
			fputs("map error\n", stderr);
		}
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (!fp) {
			fputs("map error\n", stderr);
		} else {
			if (!process_stream(fp)) {
				fputs("map error\n", stderr);
			}
			fclose(fp);
		}
		if (i < argc - 1) {
			fputs("\n", stdout);
		}
	}
	return 0;
}
