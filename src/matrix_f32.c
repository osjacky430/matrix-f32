/**
 * @Date:   2019-11-12T14:46:23+08:00
 * @Last modified time: 2019-11-12T22:36:43+08:00
 */

#include "util/runtime_error.h"

#include "include/matrix_f32.h"

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

struct MatrixF32 {
	size_t m_row;
	size_t m_col;

	float *m_val;
	AddrOwnerShip m_addrOwnerShip;
};

// private function
static inline bool m_matrixf32HaveNullPtr(int t_argc, ...) {
	va_list list;
	va_start(list, t_argc);

	for (int i = 0; i < t_argc; ++i) {
		MatrixF32Ptr input = va_arg(list, MatrixF32Ptr);

		if (input == 0 || input->m_val == 0) {
			return true;
		}
	}

	va_end(list);

	return false;
}

static inline bool m_matrixf32InputsDimMatch(int t_argc, ...) {
	va_list list;
	va_start(list, t_argc);

	MatrixF32Ptr to_compare = va_arg(list, MatrixF32Ptr);
	for (int i = 1; i < t_argc; ++i) {
		MatrixF32Ptr be_compared = va_arg(list, MatrixF32Ptr);

		if (to_compare->m_row != be_compared->m_row || to_compare->m_col != be_compared->m_col) return false;
	}

	va_end(list);

	return true;
}

static inline bool m_matrixf32MultDimMatch(const MatrixF32Ptr t_mat_a, const MatrixF32Ptr t_mat_b,
										   const MatrixF32Ptr t_mat_c) {
	return (t_mat_b->m_col == t_mat_c->m_row) && (t_mat_a->m_row == t_mat_b->m_row) &&
		   (t_mat_a->m_col == t_mat_c->m_col);
}

static inline int m_matrixf32IsSquareMatrix(int t_argc, ...) {
	va_list list;
	va_start(list, t_argc);

	for (int i = 1; i <= t_argc; ++i) {
		MatrixF32Ptr to_check = va_arg(list, MatrixF32Ptr);

		if (to_check->m_row != to_check->m_col) {
			return i;
		}
	}

	return 0;
}

static inline int m_matrixf32IsColumnVector(int t_argc, ...) {
	va_list list;
	va_start(list, t_argc);

	for (int i = 1; i <= t_argc; ++i) {
		MatrixF32Ptr to_check = va_arg(list, MatrixF32Ptr);

		if (to_check->m_col != 1) return i;
	}

	return 0;
}

static inline bool m_matrixf32InitWithInvalidDimension(MatrixDimType t_dim) {
	return (t_dim.m_row == 0 || t_dim.m_col == 0);
}

static inline size_t m_matrixf32TotalSize(const MatrixF32Ptr t_target) { return t_target->m_row * t_target->m_col; }

MatrixF32Ptr matrixf32Create(MatrixDimType t_dim) {
	MatrixF32Ptr mat_ptr = (MatrixF32Ptr)malloc(sizeof(struct MatrixF32));

	if (mat_ptr != 0) {
		if (m_matrixf32InitWithInvalidDimension(t_dim)) {
			THROW("Matrix Error: Invalid dimension", MatrixStatusErrDimMismatch);
		}

		mat_ptr->m_row = t_dim.m_row;
		mat_ptr->m_col = t_dim.m_col;

		mat_ptr->m_addrOwnerShip = OwnerShipSelf;
		size_t buff_size = mat_ptr->m_row * mat_ptr->m_col * sizeof(float);
		void *block = malloc(buff_size);
		mat_ptr->m_val = (float *)block;

		if (block != 0) {
			memset(mat_ptr->m_val, 0, buff_size);
		}
	}

	return mat_ptr;
}

MatrixF32Ptr matrixf32CreateContainer(MatrixDimType t_dim, float *t_val, int t_buffer_len) {
	MatrixF32Ptr mat_ptr = (MatrixF32Ptr)malloc(sizeof(struct MatrixF32));

	if (mat_ptr != 0) {
		if ((int)(t_dim.m_row * t_dim.m_col) > t_buffer_len) {
			THROW("Matrix Error: Out of bound", MatrixStatusErrOutOfBound);
		}

		if (m_matrixf32InitWithInvalidDimension(t_dim)) {
			THROW("Matrix Error: Invalid dimension", MatrixStatusErrDimMismatch);
		}

		mat_ptr->m_row = t_dim.m_row;
		mat_ptr->m_col = t_dim.m_col;

		mat_ptr->m_addrOwnerShip = OwnerShipOuter;
		mat_ptr->m_val = t_val;
	}

	return mat_ptr;
}

MatrixStatus matrixf32Destroy(MatrixF32Ptr *t_dest) {
	if (t_dest == NULL) {
		return MatrixStatusErrNullPtr;
	}

	// repeatedly destroy will not effect the result
	// @TODO this may cause issue
	if ((*t_dest) != NULL && (*t_dest)->m_addrOwnerShip == OwnerShipSelf) {
		free((*t_dest)->m_val);
	}

	free(*t_dest);
	*t_dest = 0;

	return MatrixStatusOK;
}

size_t matrixf32GetColNumber(MatrixF32Ptr t_dest) {
	if (t_dest == 0) {
		THROW("Matrix Error: Null pointer", MatrixStatusErrNullPtr);
	}

	return t_dest->m_col;
}

size_t matrixf32GetRowNumber(MatrixF32Ptr t_dest) {
	if (t_dest == 0) {
		THROW("Matrix Error: Null pointer", MatrixStatusErrNullPtr);
	}

	return t_dest->m_row;
}

AddrOwnerShip matrixf32GetOwnership(MatrixF32Ptr t_dest) {
	if (t_dest == 0) {
		THROW("Matrix Error: Null pointer", MatrixStatusErrNullPtr);
	}

	return t_dest->m_addrOwnerShip;
}

float matrixf32GetValueAt(MatrixF32Ptr t_dest, MatrixDimType t_dim) {
	if (m_matrixf32HaveNullPtr(1, t_dest)) {
		THROW("Matrix Error: Null pointer", MatrixStatusErrNullPtr);
	}

	bool bound_error = t_dim.m_row >= t_dest->m_row || t_dim.m_col >= t_dest->m_col;

	if (bound_error) {
		THROW("Matrix Error: Out of bound", MatrixStatusErrOutOfBound);
	}

	return t_dest->m_val[t_dest->m_col * t_dim.m_row + t_dim.m_col];
}

float *matrixf32GetBuffer(MatrixF32Ptr t_dest) {
	if (t_dest == 0) {
		THROW("Matrix Error: Null pointer", MatrixStatusErrNullPtr);
	}

	return t_dest->m_val;
}

MatrixStatus matrixf32SetValueAt(MatrixF32Ptr t_dest, MatrixDimType t_dim, float t_val) {
	if (m_matrixf32HaveNullPtr(1, t_dest)) return MatrixStatusErrNullPtr;

	if (t_dim.m_row < t_dest->m_row && t_dim.m_col < t_dest->m_col) {
		t_dest->m_val[t_dest->m_col * t_dim.m_row + t_dim.m_col] = t_val;

		return MatrixStatusOK;
	} else {
		return MatrixStatusErrDimMismatch;
	}
}

MatrixStatus matrixf32SetAllEntriesTo(MatrixF32Ptr t_dest, float t_val) {
	if (m_matrixf32HaveNullPtr(1, t_dest)) return MatrixStatusErrNullPtr;

	for (size_t i = 0; i < t_dest->m_row * t_dest->m_col; ++i) {
		t_dest->m_val[i] = t_val;
	}

	return MatrixStatusOK;
}

bool matrixf32TwoMatEqual(const MatrixF32Ptr t_mat_a, const MatrixF32Ptr t_mat_b, float t_tolerance) {
	if (m_matrixf32HaveNullPtr(2, t_mat_a, t_mat_b)) return false;
	if (t_mat_a == t_mat_b) return true;
	if (!m_matrixf32InputsDimMatch(2, t_mat_a, t_mat_b)) return false;

	for (size_t i = 0; i < t_mat_a->m_row * t_mat_a->m_col; i++) {
		if (fabsf(t_mat_a->m_val[i] - t_mat_b->m_val[i]) > t_tolerance) {
			return false;
		}
	}

	return true;
}

MatrixStatus matrixf32Add(const MatrixF32Ptr t_dest, const MatrixF32Ptr t_mat_b, const MatrixF32Ptr t_mat_c) {
	if (m_matrixf32HaveNullPtr(3, t_dest, t_mat_b, t_mat_c)) return MatrixStatusErrNullPtr;
	if (!m_matrixf32InputsDimMatch(3, t_dest, t_mat_b, t_mat_c)) return MatrixStatusErrDimMismatch;

	for (size_t i = 0; i < m_matrixf32TotalSize(t_dest); ++i) {
		t_dest->m_val[i] = t_mat_c->m_val[i] + t_mat_b->m_val[i];
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32Scale(const MatrixF32Ptr t_dest, float t_val, MatrixF32Ptr t_mat_b) {
	if (m_matrixf32HaveNullPtr(2, t_dest, t_mat_b)) return MatrixStatusErrNullPtr;
	if (!m_matrixf32InputsDimMatch(2, t_dest, t_mat_b)) return MatrixStatusErrDimMismatch;

	for (size_t i = 0; i < m_matrixf32TotalSize(t_dest); ++i) {
		t_dest->m_val[i] = t_val * t_mat_b->m_val[i];
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32Subtract(const MatrixF32Ptr t_dest, const MatrixF32Ptr t_mat_b, const MatrixF32Ptr t_mat_c) {
	if (m_matrixf32HaveNullPtr(3, t_dest, t_mat_b, t_mat_c)) return MatrixStatusErrNullPtr;
	if (!m_matrixf32InputsDimMatch(3, t_dest, t_mat_b, t_mat_c)) return MatrixStatusErrDimMismatch;

	for (size_t i = 0; i < m_matrixf32TotalSize(t_dest); ++i) {
		t_dest->m_val[i] = t_mat_b->m_val[i] - t_mat_c->m_val[i];
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32Multiplication(const MatrixF32Ptr t_dest, const MatrixF32Ptr t_mat_b,
									 const MatrixF32Ptr t_mat_c) {
	if (m_matrixf32HaveNullPtr(3, t_dest, t_mat_b, t_mat_c)) return MatrixStatusErrNullPtr;
	if (!m_matrixf32MultDimMatch(t_dest, t_mat_b, t_mat_c)) return MatrixStatusErrDimMismatch;

	for (size_t i = 0; i < t_mat_b->m_row; ++i) {
		for (size_t k = 0; k < t_mat_c->m_col; ++k) {
			float temp = 0;
			for (size_t j = 0; j < t_mat_b->m_col; ++j) {
				temp += t_mat_b->m_val[j + i * t_mat_b->m_col] * t_mat_c->m_val[k + j * t_mat_c->m_col];
			}

			t_dest->m_val[t_mat_c->m_col * i + k] = temp;
		}
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32Copy(const MatrixF32Ptr t_dest, const MatrixF32Ptr t_source) {
	if (m_matrixf32HaveNullPtr(2, t_dest, t_source)) return MatrixStatusErrNullPtr;
	if (!m_matrixf32InputsDimMatch(2, t_dest, t_source)) return MatrixStatusErrDimMismatch;

	for (size_t i = 0; i < m_matrixf32TotalSize(t_source); ++i) {
		t_dest->m_val[i] = t_source->m_val[i];
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32Swap(const MatrixF32Ptr t_to_swap, size_t t_from, size_t t_to, MatrixSwapOpt t_opt) {
	if (m_matrixf32HaveNullPtr(1, t_to_swap)) return MatrixStatusErrNullPtr;

	int begin, end, diff;  // matrix position index
	int i;				   // loop index

	float temp;

	if (t_opt == SwapOptRow) {
		if (t_to_swap->m_row > t_to && t_to_swap->m_row > t_from) {
			if (t_to == t_from) return MatrixStatusOK;

			begin = t_to_swap->m_col * t_to;
			end = t_to_swap->m_col * (t_to + 1);
			diff = t_from - t_to;

			for (i = begin; i < end; ++i) {
				temp = t_to_swap->m_val[i];
				t_to_swap->m_val[i] = t_to_swap->m_val[i + t_to_swap->m_col * diff];
				t_to_swap->m_val[i + t_to_swap->m_col * diff] = temp;
			}

			return MatrixStatusOK;
		}

		return MatrixStatusErrDimMismatch;
	} else if (t_opt == SwapOptCol) {
		if (t_to_swap->m_col > t_to && t_to_swap->m_col > t_from) {
			if (t_to == t_from) return MatrixStatusOK;

			begin = t_to;
			end = t_to + t_to_swap->m_col * t_to_swap->m_row;
			diff = t_from - t_to;

			for (i = begin; i < end; i += t_to_swap->m_col) {
				temp = t_to_swap->m_val[i];
				t_to_swap->m_val[i] = t_to_swap->m_val[i + diff];
				t_to_swap->m_val[i + diff] = temp;
			}

			return MatrixStatusOK;
		}

		return MatrixStatusErrDimMismatch;
	}

	return MatrixStatusErrUnknownOpt;
}

MatrixStatus matrixf32InPlaceLU(const MatrixF32Ptr t_dest_and_source, const MatrixF32Ptr t_permutation) {
	if (m_matrixf32HaveNullPtr(2, t_dest_and_source, t_permutation)) return MatrixStatusErrNullPtr;
	if (m_matrixf32IsSquareMatrix(1, t_dest_and_source) != 0) return MatrixStatusErrDimMismatch;
	if (t_dest_and_source->m_row != t_permutation->m_row) return MatrixStatusErrDimMismatch;

	size_t i, j, k, biggest_index = 0;

	float biggest = 0.0f, temp = 0.0f;

	// initialize t_permutation matrix, t_permutation should be a column vector
	// but it is fine if it is not a column vector
	for (i = 0; i < t_permutation->m_row; ++i) {
		// first column
		t_permutation->m_val[i * t_permutation->m_col] = (float)i;
	}

	// partial pivot (largest value in each column)
	for (i = 0; i < t_dest_and_source->m_col; ++i) {
		biggest = 0;

		for (k = i; k < t_dest_and_source->m_row; ++k) {
			temp = fabsf(t_dest_and_source->m_val[i + k * t_dest_and_source->m_row]);

			if (temp > biggest) {
				biggest_index = k;
				biggest = temp;
			}
		}

		// the matrix has a column of zeros	-> singular
		if (biggest == 0.0f) {
			return MatrixStatusErrSingular;
		}

		// swap m_row
		matrixf32Swap(t_dest_and_source, biggest_index, i, SwapOptRow);
		matrixf32Swap(t_permutation, biggest_index, i, SwapOptRow);

		for (j = i + 1; j < t_dest_and_source->m_row; ++j) {
			t_dest_and_source->m_val[j * t_dest_and_source->m_col + i] /=
				(t_dest_and_source->m_val[i * t_dest_and_source->m_col + i] + 1e-6f);

			for (k = i + 1; k < t_dest_and_source->m_col; ++k) {
				t_dest_and_source->m_val[j * t_dest_and_source->m_col + k] -=
					t_dest_and_source->m_val[j * t_dest_and_source->m_col + i] *
					t_dest_and_source->m_val[i * t_dest_and_source->m_col + k];
			}
		}
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32OutPlaceLU(const MatrixF32Ptr t_dest, const MatrixF32Ptr t_source,
								 const MatrixF32Ptr t_permutation) {
	if (m_matrixf32HaveNullPtr(3, t_dest, t_source, t_permutation)) return MatrixStatusErrNullPtr;

	MatrixStatus retval = matrixf32Copy(t_dest, t_source);
	if (retval != MatrixStatusOK) return retval;

	return matrixf32InPlaceLU(t_dest, t_permutation);
}

MatrixStatus matrixf32ForwardSubstitution(const MatrixF32Ptr t_result, const MatrixF32Ptr t_lower_triangular,
										  const MatrixF32Ptr t_column) {
	if (m_matrixf32HaveNullPtr(3, t_lower_triangular, t_column, t_result)) return MatrixStatusErrNullPtr;

	bool dim_not_matched = t_lower_triangular->m_row != t_column->m_row ||
						   t_lower_triangular->m_row != t_result->m_row || t_result->m_col != 1 ||
						   t_column->m_col != 1 || (m_matrixf32IsSquareMatrix(1, t_lower_triangular) != 0);
	if (dim_not_matched) return MatrixStatusErrDimMismatch;

	size_t i = 0, j = 0;
	float temp, main_diagonal_term;

	for (i = 0; i < t_lower_triangular->m_row; ++i) {
		temp = t_column->m_val[i];

		for (j = 0; j < i; ++j) {
			temp -= t_lower_triangular->m_val[i * t_lower_triangular->m_col + j] * t_result->m_val[j];
		}

		main_diagonal_term = t_lower_triangular->m_val[i * t_lower_triangular->m_col + i];

		if (fabsf(main_diagonal_term) <= 1e-6f) {
			return MatrixStatusErrSingular;
		}

		t_result->m_val[i] = temp / main_diagonal_term;
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32BackwardSubstitution(const MatrixF32Ptr t_result, const MatrixF32Ptr t_upper_triangular,
										   const MatrixF32Ptr t_column) {
	if (m_matrixf32HaveNullPtr(3, t_upper_triangular, t_column, t_result)) return MatrixStatusErrNullPtr;
	bool dim_not_matched = t_upper_triangular->m_row != t_column->m_row ||
						   t_upper_triangular->m_row != t_result->m_row || t_result->m_col != 1 ||
						   t_column->m_col != 1 || (m_matrixf32IsSquareMatrix(1, t_upper_triangular) != 0);
	if (dim_not_matched) return MatrixStatusErrDimMismatch;

	int i = 0;
	size_t j = 0;
	float temp, main_diagonal_term;

	for (i = t_upper_triangular->m_col - 1; i >= 0; --i) {
		temp = t_column->m_val[i];

		for (j = i + 1; j < t_upper_triangular->m_row; ++j) {
			temp -= t_upper_triangular->m_val[i * t_upper_triangular->m_col + j] * t_result->m_val[j];
		}

		main_diagonal_term = t_upper_triangular->m_val[i * t_upper_triangular->m_col + i];

		if (fabsf(main_diagonal_term) <= 1e-6f) {
			return MatrixStatusErrSingular;
		}

		t_result->m_val[i] = temp / main_diagonal_term;
	}

	return MatrixStatusOK;
}

MatrixStatus matrixf32Inverse(const MatrixF32Ptr t_dest, const MatrixF32Ptr t_source) {
	MatrixStatus ret_val;

	if (m_matrixf32HaveNullPtr(2, t_dest, t_source)) return MatrixStatusErrNullPtr;
	if ((m_matrixf32IsSquareMatrix(2, t_dest, t_source) != 0) || !m_matrixf32InputsDimMatch(2, t_dest, t_source))
		return MatrixStatusErrDimMismatch;

	float temp = 0.0f;
	size_t i = 0, j = 0, k = 0;
	MatrixF32Ptr permutation = matrixf32Create((MatrixDimType){t_source->m_row, 1});
	MatrixF32Ptr result = matrixf32Create((MatrixDimType){t_source->m_row, t_source->m_col});

	ret_val = matrixf32OutPlaceLU(result, t_source, permutation);

	if (ret_val != MatrixStatusOK) {
		matrixf32Destroy(&permutation);
		matrixf32Destroy(&result);

		return ret_val;
	}

	MatrixF32Ptr column = matrixf32Create((MatrixDimType){t_source->m_row, 1});

	if (column == 0) {
		matrixf32Destroy(&column);

		return MatrixStatusErrNullPtr;
	}

	for (i = 0; i < t_source->m_row; ++i) {
		for (j = 0; j < t_source->m_row; ++j) {
			column->m_val[j] = (float)(permutation->m_val[j] == i);
		}

		// forward substitution without dividing the diagonal term
		for (j = 0; j < result->m_row; ++j) {
			temp = column->m_val[j];

			for (k = 0; k < j; ++k) {
				temp -= result->m_val[j * result->m_col + k] * column->m_val[k];
			}

			column->m_val[j] = temp;
		}

		ret_val = matrixf32BackwardSubstitution(column, result, column);

		if (ret_val != MatrixStatusOK) {
			matrixf32Destroy(&permutation);
			matrixf32Destroy(&result);
			matrixf32Destroy(&column);

			return ret_val;
		}

		for (k = 0; k < t_source->m_col; ++k) {
			t_dest->m_val[k * t_source->m_col + i] = column->m_val[k];
		}
	}

	matrixf32Destroy(&permutation);
	matrixf32Destroy(&column);
	matrixf32Destroy(&result);

	return MatrixStatusOK;
}
