#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Equal, tolerance_too_small_may_return_false_due_to_precision) {
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

	matrixf32Multiplication(product, factor_A, factor_B);

	EXPECT_FALSE(matrixf32TwoMatEqual(product, result, 1e-5f));
	EXPECT_TRUE(matrixf32TwoMatEqual(product, result, 1e-4f));

	matrixf32Destroy(&factor_A);
	matrixf32Destroy(&factor_B);
	matrixf32Destroy(&product);
	matrixf32Destroy(&result);
}

TEST(Equal, self_equality_will_return_true) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;
	float buffer[]{CONST,		  CONST,		 CONST,			 CONST,			CONST,		 0.07644136f,
				   0.035759674f,  0.08023302f,	 0.00236097324f, 0.11466476f,	0.11672867f, -0.000827724f,
				   0.011534579f,  -0.13193906f,	 -0.0228465795f, -0.040387194f, 0.09546156f, -0.04818218f,
				   -0.056990381f, -0.044045348f, 0.080531277f,	 CONST,			CONST,		 CONST};

	MatrixF32Ptr mat_A = matrixf32CreateContainer({4, 3}, buffer + SOME_OFFSET, 4 * 3);

	EXPECT_TRUE(matrixf32TwoMatEqual(mat_A, mat_A, 1e-6f));

	matrixf32Destroy(&mat_A);
}

TEST(Equal, different_input_matrix_dimension_will_return_false) {
	MatrixF32Ptr mat_A = matrixf32Create({5, 3});
	MatrixF32Ptr mat_B = matrixf32Create({4, 3});
	MatrixF32Ptr mat_C = matrixf32Create({4, 4});

	EXPECT_FALSE(matrixf32TwoMatEqual(mat_A, mat_B, 1e-6f));
	EXPECT_FALSE(matrixf32TwoMatEqual(mat_B, mat_C, 1e-6f));

	matrixf32Destroy(&mat_A);
	matrixf32Destroy(&mat_B);
	matrixf32Destroy(&mat_C);
}

TEST(Equal, null_ptr_input_will_return_false) {
	MatrixF32Ptr mat_A = matrixf32Create({4, 3});
	MatrixF32Ptr mat_B = matrixf32Create({4, 3});

	EXPECT_FALSE(matrixf32TwoMatEqual(0, 0, 1e-6f));
	EXPECT_FALSE(matrixf32TwoMatEqual(0, mat_B, 1e-6f));
	EXPECT_FALSE(matrixf32TwoMatEqual(mat_A, 0, 1e-6f));

	matrixf32Destroy(&mat_A);
	matrixf32Destroy(&mat_B);
}
