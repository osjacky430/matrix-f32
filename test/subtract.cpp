#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Subtract, subtract_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float minuend_buffer[]{CONST, CONST, CONST, CONST, CONST, -1.5f, 8.7f,	6.1f,  -12.0f, 9.4f,
						   3.1f,  -7.5f, -2.6f, 2.8f,  3.5f,  4.1f,	 -4.9f, CONST, CONST,  CONST};

	float subtrahend_buffer[]{CONST, CONST, CONST, CONST, CONST, 2.0f,	3.0f, -5.8f, 6.8f,	4.9f,
							  -2.7f, -1.8f, -7.9f, 1.5f,  11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float subtract_result_buffer[]{-3.5f, 5.7f, 11.9f, -18.8f, 4.5f, 5.8f, -5.7f, 5.3f, 1.3f, -7.5f, 12.5f, -7.5f};

	MatrixF32Ptr result = matrixf32CreateContainer({4, 3}, subtract_result_buffer, 4 * 3);
	MatrixF32Ptr minuend = matrixf32CreateContainer({4, 3}, minuend_buffer + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr subtrahend = matrixf32CreateContainer({4, 3}, subtrahend_buffer + SOME_OFFSET, 4 * 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32Subtract(minuend, minuend, subtrahend));
	EXPECT_TRUE(matrixf32TwoMatEqual(result, minuend, 1e-6f));

	matrixf32Destroy(&result);
	matrixf32Destroy(&minuend);
	matrixf32Destroy(&subtrahend);
}

TEST(Subtract, subtract_out_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float minuend_buffer[]{CONST, CONST, CONST, CONST, CONST, -1.5f, 8.7f,	6.1f,  -12.0f, 9.4f,
						   3.1f,  -7.5f, -2.6f, 2.8f,  3.5f,  4.1f,	 -4.9f, CONST, CONST,  CONST};

	float subtrahend_buffer[]{CONST, CONST, CONST, CONST, CONST, 2.0f,	3.0f, -5.8f, 6.8f,	4.9f,
							  -2.7f, -1.8f, -7.9f, 1.5f,  11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float subtract_result_buffer[]{-3.5f, 5.7f, 11.9f, -18.8f, 4.5f, 5.8f, -5.7f, 5.3f, 1.3f, -7.5f, 12.5f, -7.5f};

	MatrixF32Ptr difference = matrixf32Create({4, 3});	// random size
	MatrixF32Ptr result = matrixf32CreateContainer({4, 3}, subtract_result_buffer, 4 * 3);
	MatrixF32Ptr minuend = matrixf32CreateContainer({4, 3}, minuend_buffer + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr subtrahend = matrixf32CreateContainer({4, 3}, subtrahend_buffer + SOME_OFFSET, 4 * 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32Subtract(difference, minuend, subtrahend));
	EXPECT_TRUE(matrixf32TwoMatEqual(result, difference, 1e-6f));

	matrixf32Destroy(&difference);
	matrixf32Destroy(&result);
	matrixf32Destroy(&minuend);
	matrixf32Destroy(&subtrahend);
}

TEST(Subtract, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr difference = matrixf32Create({4, 3});
	MatrixF32Ptr minuend = matrixf32Create({5, 3});
	MatrixF32Ptr subtrahend = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Subtract(difference, minuend, subtrahend));

	matrixf32Destroy(&difference);
	matrixf32Destroy(&minuend);
	matrixf32Destroy(&subtrahend);
}

TEST(Subtract, null_ptr_input_will_return_error) {
	MatrixF32Ptr difference = matrixf32Create({4, 3});
	MatrixF32Ptr minuend = matrixf32Create({4, 3});
	MatrixF32Ptr subtrahend = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Subtract(0, minuend, subtrahend));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Subtract(difference, 0, subtrahend));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Subtract(difference, minuend, 0));

	matrixf32Destroy(&difference);
	matrixf32Destroy(&minuend);
	matrixf32Destroy(&subtrahend);
}
