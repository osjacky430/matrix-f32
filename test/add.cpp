#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Add, add_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float addend_A_buffer[]{CONST, CONST, CONST, CONST, CONST, 2.0f,  3.0f, -5.8f, 6.8f,  4.9f,
							-2.7f, -1.8f, -7.9f, 1.5f,	11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float addend_B_buffer[]{CONST, CONST, CONST, CONST, CONST, -1.5f, 8.7f,	 6.1f,	-12.0f, 9.4f,
							3.1f,  -7.5f, -2.6f, 2.8f,	3.5f,  4.1f,  -4.9f, CONST, CONST,	CONST};

	float add_result_buffer[]{0.5f, 11.7f, 0.3f, -5.2f, 14.3f, 0.4f, -9.3f, -10.5f, 4.3f, 14.5f, -4.3f, -2.3f};

	MatrixF32Ptr result = matrixf32CreateContainer({4, 3}, add_result_buffer, 4 * 3);  // random size
	MatrixF32Ptr addend_A = matrixf32CreateContainer({4, 3}, addend_A_buffer + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr addend_B = matrixf32CreateContainer({4, 3}, addend_B_buffer + SOME_OFFSET, 4 * 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32Add(addend_A, addend_A, addend_B));
	EXPECT_TRUE(matrixf32TwoMatEqual(result, addend_A, 1e-6f));

	matrixf32Destroy(&result);
	matrixf32Destroy(&addend_A);
	matrixf32Destroy(&addend_B);
}

TEST(Add, add_out_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float addend_A_buffer[]{CONST, CONST, CONST, CONST, CONST, 2.0f,  3.0f, -5.8f, 6.8f,  4.9f,
							-2.7f, -1.8f, -7.9f, 1.5f,	11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float addend_B_buffer[]{CONST, CONST, CONST, CONST, CONST, -1.5f, 8.7f,	 6.1f,	-12.0f, 9.4f,
							3.1f,  -7.5f, -2.6f, 2.8f,	3.5f,  4.1f,  -4.9f, CONST, CONST,	CONST};

	float add_result_buffer[]{0.5f, 11.7f, 0.3f, -5.2f, 14.3f, 0.4f, -9.3f, -10.5f, 4.3f, 14.5f, -4.3f, -2.3f};

	MatrixF32Ptr sum = matrixf32Create({4, 3});
	MatrixF32Ptr result = matrixf32CreateContainer({4, 3}, add_result_buffer, 4 * 3);  // random size
	MatrixF32Ptr addend_A = matrixf32CreateContainer({4, 3}, addend_A_buffer + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr addend_B = matrixf32CreateContainer({4, 3}, addend_B_buffer + SOME_OFFSET, 4 * 3);

	EXPECT_EQ(MatrixStatusOK, matrixf32Add(sum, addend_A, addend_B));
	EXPECT_TRUE(matrixf32TwoMatEqual(result, sum, 1e-6f));

	matrixf32Destroy(&sum);
	matrixf32Destroy(&result);
	matrixf32Destroy(&addend_A);
	matrixf32Destroy(&addend_B);
}

TEST(Add, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr result = matrixf32Create({4, 3});	// random size
	MatrixF32Ptr addend_A = matrixf32Create({5, 3});
	MatrixF32Ptr addend_B = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Add(result, addend_A, addend_B));

	matrixf32Destroy(&result);
	matrixf32Destroy(&addend_A);
	matrixf32Destroy(&addend_B);
}

TEST(Add, null_ptr_input_will_return_error) {
	MatrixF32Ptr result = matrixf32Create({4, 3});	// random size
	MatrixF32Ptr addend_A = matrixf32Create({4, 3});
	MatrixF32Ptr addend_B = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Add(0, addend_A, addend_B));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Add(result, 0, addend_B));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Add(result, addend_A, 0));

	matrixf32Destroy(&result);
	matrixf32Destroy(&addend_A);
	matrixf32Destroy(&addend_B);
}
