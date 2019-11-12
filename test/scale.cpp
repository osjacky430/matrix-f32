#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Scale, scale_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float test_case[]{CONST, CONST, CONST, CONST, CONST, -1.5f, 8.7f,  6.1f,  -12.0f, 9.4f,
					  3.1f,	 -7.5f, -2.6f, 2.8f,  3.5f,	 4.1f,	-4.9f, CONST, CONST,  CONST};

	float scale_result_buffer[]{-3.0f, 17.4f, 12.2f, -24.0f, 18.8f, 6.2f, -15.0f, -5.2f, 5.6f, 7.0f, 8.2f, -9.8f};

	MatrixF32Ptr scale_result = matrixf32CreateContainer({4, 3}, scale_result_buffer, 4 * 3);
	MatrixF32Ptr source = matrixf32CreateContainer({4, 3}, test_case + SOME_OFFSET, 4 * 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32Scale(source, 2.0f, source));
	EXPECT_TRUE(matrixf32TwoMatEqual(source, scale_result, 1e-5f));

	matrixf32Destroy(&scale_result);
	matrixf32Destroy(&source);
}

TEST(Scale, scale_out_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float test_case[]{CONST, CONST, CONST, CONST, CONST, -1.5f, 8.7f,  6.1f,  -12.0f, 9.4f,
					  3.1f,	 -7.5f, -2.6f, 2.8f,  3.5f,	 4.1f,	-4.9f, CONST, CONST,  CONST};

	float scale_result_buffer[]{-3.0f, 17.4f, 12.2f, -24.0f, 18.8f, 6.2f, -15.0f, -5.2f, 5.6f, 7.0f, 8.2f, -9.8f};

	MatrixF32Ptr scale_result = matrixf32CreateContainer({4, 3}, scale_result_buffer, 4 * 3);
	MatrixF32Ptr result = matrixf32Create({4, 3});
	MatrixF32Ptr source = matrixf32CreateContainer({4, 3}, test_case + SOME_OFFSET, 4 * 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32Scale(result, 2.0f, source));
	EXPECT_TRUE(matrixf32TwoMatEqual(scale_result, result, 1e-5f));

	matrixf32Destroy(&scale_result);
	matrixf32Destroy(&result);
	matrixf32Destroy(&source);
}

TEST(Scale, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr result = matrixf32Create({4, 3});
	MatrixF32Ptr source = matrixf32Create({5, 3});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Scale(result, 2.0, source));

	matrixf32Destroy(&result);
	matrixf32Destroy(&source);
}

TEST(Scale, null_ptr_input_will_return_error) {
	MatrixF32Ptr result = matrixf32Create({4, 3});
	MatrixF32Ptr source = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Scale(0, 2.0f, source));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Scale(result, 2.0f, 0));

	matrixf32Destroy(&result);
	matrixf32Destroy(&source);
}
