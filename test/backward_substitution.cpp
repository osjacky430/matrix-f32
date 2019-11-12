#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(BackwardSubstitution, backward_substitution_algorithm_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float bufferBackwardSub[]{
		CONST, CONST, CONST, CONST, CONST, -3.0f / 7.0f, 2.0f,	-2.0f, 0.0f,  14.0f,
		6.0f,  0.0f,  0.0f,	 2.0f,	CONST, CONST,		 CONST, CONST, CONST,
	};

	float backward_sub_non_homogeneous_buffer[]{CONST, CONST, CONST, CONST, CONST, -6.0f,
												10.0f, 4.0f,  CONST, CONST, CONST};

	float backward_sub_result_buffer[]{4.0f, -1.0f / 7.0f, 2.0f};

	MatrixF32Ptr lower_triangular = matrixf32CreateContainer({3, 3}, bufferBackwardSub + SOME_OFFSET, 3 * 3);
	MatrixF32Ptr backward_sub_col_vec =
		matrixf32CreateContainer({3, 1}, backward_sub_non_homogeneous_buffer + SOME_OFFSET, 3);
	MatrixF32Ptr backward_sub_result = matrixf32CreateContainer({3, 1}, backward_sub_result_buffer, 3);

	EXPECT_EQ(MatrixStatusOK,
			  matrixf32BackwardSubstitution(backward_sub_col_vec, lower_triangular, backward_sub_col_vec));
	EXPECT_TRUE(matrixf32TwoMatEqual(backward_sub_col_vec, backward_sub_result, 1e-4f));

	matrixf32Destroy(&lower_triangular);
	matrixf32Destroy(&backward_sub_col_vec);
	matrixf32Destroy(&backward_sub_result);
}

TEST(BackwardSubstitution, backward_substitution_out_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float bufferbackwardSub[]{
		CONST, CONST, CONST, CONST, CONST, -3.0f / 7.0f, 2.0f,	-2.0f, 0.0f,  14.0f,
		6.0f,  0.0f,  0.0f,	 2.0f,	CONST, CONST,		 CONST, CONST, CONST,
	};

	float backward_sub_non_homogeneous_buffer[]{CONST, CONST, CONST, CONST, CONST, -6.0f,
												10.0f, 4.0f,  CONST, CONST, CONST};

	float backward_sub_result_buffer[]{4.0f, -1.0f / 7.0f, 2.0f};

	MatrixF32Ptr upper_triangular = matrixf32CreateContainer({3, 3}, bufferbackwardSub + SOME_OFFSET, 3 * 3);
	MatrixF32Ptr backward_sub_to_solve = matrixf32Create({3, 1});
	MatrixF32Ptr backward_sub_non_homogeneous_col =
		matrixf32CreateContainer({3, 1}, backward_sub_non_homogeneous_buffer + SOME_OFFSET, 3);
	MatrixF32Ptr backward_sub_result = matrixf32CreateContainer({3, 1}, backward_sub_result_buffer, 3);

	EXPECT_EQ(MatrixStatusOK,
			  matrixf32BackwardSubstitution(backward_sub_to_solve, upper_triangular, backward_sub_non_homogeneous_col));
	EXPECT_TRUE(matrixf32TwoMatEqual(backward_sub_to_solve, backward_sub_result, 1e-4f));

	matrixf32Destroy(&upper_triangular);
	matrixf32Destroy(&backward_sub_to_solve);
	matrixf32Destroy(&backward_sub_non_homogeneous_col);
	matrixf32Destroy(&backward_sub_result);
}

TEST(BackwardSubstitution, singular_matrix_input_will_return_error) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;
	float test_case_buffer[]{
		CONST, CONST, CONST, CONST, CONST, -3.0f / 7.0f, 2.0f,	-2.0f, 0.0f,  14.0f,
		6.0f,  0.0f,  0.0f,	 2.0f,	CONST, CONST,		 CONST, CONST, CONST,
	};

	MatrixF32Ptr test_case = matrixf32CreateContainer({3, 3}, test_case_buffer + SOME_OFFSET, 9);
	MatrixF32Ptr column = matrixf32Create({3, 1});
	MatrixF32Ptr result = matrixf32Create({3, 1});
	matrixf32SetValueAt(test_case, {1, 1}, 0);

	EXPECT_EQ(MatrixStatusErrSingular, matrixf32BackwardSubstitution(result, test_case, column));

	matrixf32Destroy(&test_case);
	matrixf32Destroy(&column);
	matrixf32Destroy(&result);
}

TEST(BackwardSubstitution, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr square_matrix_3_by_3 = matrixf32Create({3, 3});
	MatrixF32Ptr square_matrix_4_by_4 = matrixf32Create({4, 4});
	MatrixF32Ptr column_vector_3_by_1 = matrixf32Create({4, 1});
	MatrixF32Ptr column_matrix_3_by_3 = matrixf32Create({3, 3});
	MatrixF32Ptr result = matrixf32Create({3, 1});

	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32BackwardSubstitution(result, square_matrix_3_by_3, column_vector_3_by_1));
	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32BackwardSubstitution(result, square_matrix_4_by_4, column_vector_3_by_1));
	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32BackwardSubstitution(result, square_matrix_3_by_3, column_matrix_3_by_3));

	matrixf32Destroy(&square_matrix_3_by_3);
	matrixf32Destroy(&square_matrix_4_by_4);
	matrixf32Destroy(&column_vector_3_by_1);
	matrixf32Destroy(&column_matrix_3_by_3);
	matrixf32Destroy(&result);
}

TEST(BackwardSubstitution, null_ptr_input_will_return_error) {
	MatrixF32Ptr upper_triangular = matrixf32Create({3, 3});
	MatrixF32Ptr backward_sub_to_solve = matrixf32Create({3, 1});
	MatrixF32Ptr backward_sub_non_homogeneous_col = matrixf32Create({3, 1});

	EXPECT_EQ(MatrixStatusErrNullPtr,
			  matrixf32BackwardSubstitution(0, upper_triangular, backward_sub_non_homogeneous_col));
	EXPECT_EQ(MatrixStatusErrNullPtr,
			  matrixf32BackwardSubstitution(backward_sub_to_solve, 0, backward_sub_non_homogeneous_col));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32BackwardSubstitution(backward_sub_to_solve, upper_triangular, 0));

	matrixf32Destroy(&upper_triangular);
	matrixf32Destroy(&backward_sub_to_solve);
	matrixf32Destroy(&backward_sub_non_homogeneous_col);
}
