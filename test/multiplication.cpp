#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Multiplication, multiplication_out_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float factor_A_buffer[]{CONST, CONST, CONST, CONST, CONST, 2.0f,  3.0f, -5.8f, 6.8f,  4.9f,
							-2.7f, -1.8f, -7.9f, 1.5f,	11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float factor_B_buffer[]{CONST, CONST, CONST, CONST, CONST, -8.1f, 4.2f, 3.3f,  5.7f,  1.7f,
							5.2f,  10.0f, 21.4f, 7.2f,	3.5f,  8.6f,  9.9f, CONST, CONST, CONST};

	float mult_result_buffer[]{-52.86f, 3.7f,	 -13.28f, 18.18f,	-66.19f, 44.59f, 48.22f,  116.89f,
							   11.95f,	-43.39f, -72.04f, -164.47f, -84.66f, 11.62f, -25.34f, -91.32f};

	MatrixF32Ptr factor_A = matrixf32CreateContainer({4, 3}, factor_A_buffer + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr factor_B = matrixf32CreateContainer({3, 4}, factor_B_buffer + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr product = matrixf32Create({4, 4});
	MatrixF32Ptr result = matrixf32CreateContainer({4, 4}, mult_result_buffer, 4 * 4);

	EXPECT_EQ(MatrixStatusOK, matrixf32Multiplication(product, factor_A, factor_B));
	EXPECT_TRUE(matrixf32TwoMatEqual(product, result, 1e-4f));

	matrixf32Destroy(&factor_A);
	matrixf32Destroy(&factor_B);
	matrixf32Destroy(&product);
	matrixf32Destroy(&result);
}

TEST(Multiplication, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr mult_result_5_by_4 = matrixf32Create({5, 4});
	MatrixF32Ptr mult_result_4_by_4 = matrixf32Create({4, 4});
	MatrixF32Ptr factor_A_4_by_3 = matrixf32Create({4, 3});
	MatrixF32Ptr factor_A_5_by_4 = matrixf32Create({5, 4});
	MatrixF32Ptr factor_B_3_by_4 = matrixf32Create({3, 4});

	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32Multiplication(mult_result_4_by_4, factor_A_5_by_4, factor_B_3_by_4));
	EXPECT_EQ(MatrixStatusErrDimMismatch,
			  matrixf32Multiplication(mult_result_5_by_4, factor_A_4_by_3, factor_B_3_by_4));

	matrixf32Destroy(&mult_result_5_by_4);
	matrixf32Destroy(&mult_result_4_by_4);
	matrixf32Destroy(&factor_A_4_by_3);
	matrixf32Destroy(&factor_A_5_by_4);
	matrixf32Destroy(&factor_B_3_by_4);
}

TEST(Multiplication, null_ptr_input_will_return_error) {
	MatrixF32Ptr mult_result = matrixf32Create({4, 4});
	MatrixF32Ptr factor_A = matrixf32Create({4, 3});
	MatrixF32Ptr factor_B = matrixf32Create({3, 4});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Multiplication(0, factor_A, factor_B));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Multiplication(mult_result, 0, factor_B));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Multiplication(mult_result, factor_A, 0));

	matrixf32Destroy(&mult_result);
	matrixf32Destroy(&factor_A);
	matrixf32Destroy(&factor_B);
}
