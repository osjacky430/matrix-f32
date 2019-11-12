#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(ForwardSubstitution, forward_substitution_algorithm_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float bufferForwardSub[]{CONST, CONST, CONST, CONST,		CONST, 2.0f,  0.0f,	 0.0f,	6.0f, 14.0f,
							 0.0f,	-2.0f, 2.0f,  -3.0f / 7.0f, CONST, CONST, CONST, CONST, CONST};

	float forward_sub_non_homogeneous_buffer[]{CONST, CONST, CONST, CONST, CONST, 4.0f,
											   10.0f, -6.0f, CONST, CONST, CONST};

	float forward_sub_result_buffer[]{2.0f, -1.0f / 7.0f, 4.0f};

	MatrixF32Ptr lower_triangular = matrixf32CreateContainer({3, 3}, bufferForwardSub + SOME_OFFSET, 3 * 3);
	MatrixF32Ptr forward_sub_col_vec =
		matrixf32CreateContainer({3, 1}, forward_sub_non_homogeneous_buffer + SOME_OFFSET, 3);
	MatrixF32Ptr forward_sub_result = matrixf32CreateContainer({3, 1}, forward_sub_result_buffer, 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32ForwardSubstitution(forward_sub_col_vec, lower_triangular, forward_sub_col_vec));
	EXPECT_TRUE(matrixf32TwoMatEqual(forward_sub_col_vec, forward_sub_result, 1e-4f));

	matrixf32Destroy(&lower_triangular);
	matrixf32Destroy(&forward_sub_col_vec);
	matrixf32Destroy(&forward_sub_result);
}

TEST(ForwardSubstitution, forward_substitution_algorithm_out_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float bufferForwardSub[]{CONST, CONST, CONST, CONST,		CONST, 2.0f,  0.0f,	 0.0f,	6.0f, 14.0f,
							 0.0f,	-2.0f, 2.0f,  -3.0f / 7.0f, CONST, CONST, CONST, CONST, CONST};

	float forward_sub_non_homogeneous_buffer[]{CONST, CONST, CONST, CONST, CONST, 4.0f,
											   10.0f, -6.0f, CONST, CONST, CONST};

	float forward_sub_result_buffer[]{2.0f, -1.0f / 7.0f, 4.0f};

	MatrixF32Ptr lower_triangular = matrixf32CreateContainer({3, 3}, bufferForwardSub + SOME_OFFSET, 3 * 3);
	MatrixF32Ptr forward_sub_to_solve = matrixf32Create({3, 1});
	MatrixF32Ptr forward_sub_non_homogeneous_col =
		matrixf32CreateContainer({3, 1}, forward_sub_non_homogeneous_buffer + SOME_OFFSET, 3);
	MatrixF32Ptr forward_sub_result = matrixf32CreateContainer({3, 1}, forward_sub_result_buffer, 3);

	EXPECT_EQ(MatrixStatusOK,
			  matrixf32ForwardSubstitution(forward_sub_to_solve, lower_triangular, forward_sub_non_homogeneous_col));
	EXPECT_TRUE(matrixf32TwoMatEqual(forward_sub_to_solve, forward_sub_result, 1e-4f));

	matrixf32Destroy(&lower_triangular);
	matrixf32Destroy(&forward_sub_to_solve);
	matrixf32Destroy(&forward_sub_non_homogeneous_col);
	matrixf32Destroy(&forward_sub_result);
}

TEST(ForwardSubstitution, singular_matrix_input_will_return_error) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;
	float test_case_buffer[]{
		CONST, CONST, CONST, CONST,		   CONST, 2.0f,	 0.0f,	0.0f,  6.0f,  14.0f,
		0.0f,  -2.0f, 2.0f,	 -3.0f / 7.0f, CONST, CONST, CONST, CONST, CONST,
	};

	MatrixF32Ptr test_case = matrixf32CreateContainer({3, 3}, test_case_buffer + SOME_OFFSET, 9);
	MatrixF32Ptr column = matrixf32Create({3, 1});
	MatrixF32Ptr result = matrixf32Create({3, 1});
	matrixf32SetValueAt(test_case, {0, 0}, 0);

	EXPECT_EQ(MatrixStatusErrSingular, matrixf32ForwardSubstitution(result, test_case, column));

	matrixf32Destroy(&test_case);
	matrixf32Destroy(&column);
	matrixf32Destroy(&result);
}

TEST(ForwardSubstitution, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr lower_triangular_3_by_3 = matrixf32Create({3, 3});
	MatrixF32Ptr lower_triangular_4_by_4 = matrixf32Create({4, 4});
	MatrixF32Ptr column_vector_4_by_1 = matrixf32Create({4, 1});
	MatrixF32Ptr column_matrix_3_by_3 = matrixf32Create({3, 3});
	MatrixF32Ptr result = matrixf32Create({3, 1});

	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32ForwardSubstitution(result, lower_triangular_3_by_3, column_vector_4_by_1));
	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32ForwardSubstitution(result, lower_triangular_4_by_4, column_vector_4_by_1));
	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32ForwardSubstitution(result, lower_triangular_3_by_3, column_matrix_3_by_3));

	matrixf32Destroy(&lower_triangular_3_by_3);
	matrixf32Destroy(&lower_triangular_4_by_4);
	matrixf32Destroy(&column_vector_4_by_1);
	matrixf32Destroy(&column_matrix_3_by_3);
	matrixf32Destroy(&result);
}

TEST(ForwardSubstitution, null_ptr_input_will_return_error) {
	MatrixF32Ptr lower_triangular = matrixf32Create({3, 3});
	MatrixF32Ptr result = matrixf32Create({3, 1});
	MatrixF32Ptr column = matrixf32Create({3, 1});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32ForwardSubstitution(result, lower_triangular, 0));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32ForwardSubstitution(result, 0, column));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32ForwardSubstitution(0, lower_triangular, column));

	matrixf32Destroy(&lower_triangular);
	matrixf32Destroy(&result);
	matrixf32Destroy(&column);
}
