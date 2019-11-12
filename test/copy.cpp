#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Copy, copied_matrix_should_be_equal_to_the_original_one) {
	float buffer[6] = {20, 20, 20, 20, 20, 20};
	MatrixF32Ptr mat = matrixf32CreateContainer({2, 3}, buffer, 6);
	MatrixF32Ptr mat_copy = matrixf32Create({2, 3});

	EXPECT_EQ(MatrixStatusOK, matrixf32Copy(mat_copy, mat));
	EXPECT_TRUE(matrixf32TwoMatEqual(mat_copy, mat, 1e-6f));

	matrixf32Destroy(&mat);
	matrixf32Destroy(&mat_copy);
}

TEST(Copy, null_ptr_input_will_return_error) {
	float buffer[6] = {20, 20, 20, 20, 20, 20};
	MatrixF32Ptr mat = matrixf32CreateContainer({2, 3}, buffer, sizeof(buffer) / sizeof(float));
	MatrixF32Ptr mat_copy = matrixf32Create({2, 3});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Copy(0, mat));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Copy(mat_copy, 0));

	matrixf32Destroy(&mat);
	matrixf32Destroy(&mat_copy);
}

TEST(Copy, wrong_input_matrix_dimension_will_return_error) {
	float buffer[6] = {20, 20, 20, 20, 20, 20};
	MatrixF32Ptr mat = matrixf32CreateContainer({2, 3}, buffer, 6);
	MatrixF32Ptr mat_copy = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Copy(mat_copy, mat));

	matrixf32Destroy(&mat);
	matrixf32Destroy(&mat_copy);
}
