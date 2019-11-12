#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(LUDecomposition, lu_decomposition_works_with_permutation_mat_as_matrix) {
	static constexpr size_t SOME_OFFSET = 5;
	static constexpr float CONST = 100.0f;
	float test_case[]{CONST, CONST, CONST, CONST, CONST, 1.0f, 2.6f, -8.1f, 9.2f,  -1.5f, 5.7f,	 7.7f, -9.9f,
					  12.0f, -5.1f, 4.2f,  -4.6f, 6.1f,	 2.8f, 2.9f, 8.4f,	CONST, CONST, CONST, CONST};

	float lu_result_buffer[]{12.0f,	  -5.1f,	  4.2f,			-4.6f,		0.508333f,	5.3925f,
							 0.765f,  10.7383f,	  0.083333f,	0.5609643f, -8.879138f, 3.559511662f,
							 -0.125f, 0.9388039f, -0.84544414f, -17.546821f};

	float perm_result_buffer[]{2.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

	MatrixDimType square_mat_dim = {4, 4};

	MatrixF32Ptr source = matrixf32CreateContainer(square_mat_dim, test_case + SOME_OFFSET, 16);
	MatrixF32Ptr dest = matrixf32Create(square_mat_dim);
	MatrixF32Ptr permutation = matrixf32Create({4, 3});
	MatrixF32Ptr lu_result = matrixf32CreateContainer({4, 4}, lu_result_buffer, 4 * 4);
	MatrixF32Ptr perm_result = matrixf32CreateContainer({4, 3}, perm_result_buffer, 12);

	EXPECT_EQ(MatrixStatusOK, matrixf32OutPlaceLU(dest, source, permutation));
	EXPECT_TRUE(matrixf32TwoMatEqual(dest, lu_result, 1e-4f));
	EXPECT_TRUE(matrixf32TwoMatEqual(permutation, perm_result, 1e-4f));

	matrixf32Destroy(&source);
	matrixf32Destroy(&dest);
	matrixf32Destroy(&permutation);
	matrixf32Destroy(&lu_result);
	matrixf32Destroy(&perm_result);
}

TEST(LUDecomposition, lu_decomposition_works_with_permutation_mat_as_column_vector) {
	static constexpr size_t SOME_OFFSET = 5;
	static constexpr float CONST = 100.0f;
	float test_case[]{CONST, CONST, CONST, CONST, CONST, 1.0f, 2.6f, -8.1f, 9.2f,  -1.5f, 5.7f,	 7.7f, -9.9f,
					  12.0f, -5.1f, 4.2f,  -4.6f, 6.1f,	 2.8f, 2.9f, 8.4f,	CONST, CONST, CONST, CONST};

	float lu_result_buffer[]{12.0f,	  -5.1f,	  4.2f,			-4.6f,		0.508333f,	5.3925f,
							 0.765f,  10.7383f,	  0.083333f,	0.5609643f, -8.879138f, 3.559511662f,
							 -0.125f, 0.9388039f, -0.84544414f, -17.546821f};

	float perm_result_buffer[]{2.0f, 3.0f, 0.0f, 1.0f};

	MatrixDimType square_mat_dim = {4, 4};

	MatrixF32Ptr source = matrixf32CreateContainer(square_mat_dim, test_case + SOME_OFFSET, 16);
	MatrixF32Ptr dest = matrixf32Create(square_mat_dim);
	MatrixF32Ptr permutation = matrixf32Create({4, 1});
	MatrixF32Ptr lu_result = matrixf32CreateContainer({4, 4}, lu_result_buffer, 4 * 4);
	MatrixF32Ptr perm_result = matrixf32CreateContainer({4, 1}, perm_result_buffer, 4);

	EXPECT_EQ(MatrixStatusOK, matrixf32OutPlaceLU(dest, source, permutation));
	EXPECT_TRUE(matrixf32TwoMatEqual(lu_result, dest, 1e-4f));
	EXPECT_TRUE(matrixf32TwoMatEqual(perm_result, permutation, 1e-4f));

	matrixf32Destroy(&source);
	matrixf32Destroy(&dest);
	matrixf32Destroy(&permutation);
	matrixf32Destroy(&lu_result);
	matrixf32Destroy(&perm_result);
}

TEST(LUDecomposition, null_ptr_input_will_return_error) {
	MatrixF32Ptr source = matrixf32Create({4, 4});
	MatrixF32Ptr permutation = matrixf32Create({4, 1});
	MatrixF32Ptr dest = matrixf32Create({4, 4});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32OutPlaceLU(0, source, permutation));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32OutPlaceLU(dest, 0, permutation));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32OutPlaceLU(dest, source, 0));

	matrixf32Destroy(&source);
	matrixf32Destroy(&permutation);
	matrixf32Destroy(&dest);
}

TEST(LUDecomposition, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr source = matrixf32Create({4, 4});
	MatrixF32Ptr wrong_dim_square_src_mat = matrixf32Create({3, 3});
	MatrixF32Ptr non_square_src_mat = matrixf32Create({3, 5});
	MatrixF32Ptr wrong_permutation_mat = matrixf32Create({3, 1});
	MatrixF32Ptr right_permutation_mat = matrixf32Create({4, 1});
	MatrixF32Ptr dest = matrixf32Create({4, 4});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32OutPlaceLU(dest, source, wrong_permutation_mat));
	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32OutPlaceLU(dest, non_square_src_mat, right_permutation_mat));
	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32OutPlaceLU(dest, wrong_dim_square_src_mat, right_permutation_mat));

	matrixf32Destroy(&source);
	matrixf32Destroy(&wrong_dim_square_src_mat);
	matrixf32Destroy(&non_square_src_mat);
	matrixf32Destroy(&wrong_permutation_mat);
	matrixf32Destroy(&right_permutation_mat);
	matrixf32Destroy(&dest);
}
