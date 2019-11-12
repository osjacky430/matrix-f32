#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Swap, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr to_swap = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Swap(to_swap, 5, 1, SwapOptRow));
	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Swap(to_swap, 5, 1, SwapOptCol));

	matrixf32Destroy(&to_swap);
}

TEST(Swap, col_swap_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float test_case[]{CONST, CONST, CONST, CONST, CONST, 2.0f,	3.0f, -5.8f, 6.8f,	4.9f,
					  -2.7f, -1.8f, -7.9f, 1.5f,  11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float swap_col_result_buffer[]{
		3.0f, 2.0f, -5.8f, 4.9f, 6.8f, -2.7f, -7.9f, -1.8f, 1.5f, -8.4f, 11.0f, 2.6f,
	};

	MatrixF32Ptr to_swap = matrixf32CreateContainer({4, 3}, test_case + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr swap_result = matrixf32CreateContainer({4, 3}, swap_col_result_buffer, 12);

	EXPECT_EQ(MatrixStatusOK, matrixf32Swap(to_swap, 1, 0, SwapOptCol));
	EXPECT_TRUE(matrixf32TwoMatEqual(to_swap, swap_result, 1e-4f));

	matrixf32Destroy(&to_swap);
	matrixf32Destroy(&swap_result);
}

TEST(Swap, row_swap_in_place) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t SOME_OFFSET = 5;

	float test_case[]{CONST, CONST, CONST, CONST, CONST, 2.0f,	3.0f, -5.8f, 6.8f,	4.9f,
					  -2.7f, -1.8f, -7.9f, 1.5f,  11.0f, -8.4f, 2.6f, CONST, CONST, CONST};

	float swap_row_result_buffer[]{2.0f, 3.0f, -5.8f, -1.8f, -7.9f, 1.5f, 6.8f, 4.9f, -2.7f, 11.0f, -8.4f, 2.6f};

	MatrixF32Ptr to_swap = matrixf32CreateContainer({4, 3}, test_case + SOME_OFFSET, 4 * 3);
	MatrixF32Ptr swap_result = matrixf32CreateContainer({4, 3}, swap_row_result_buffer, 12);

	EXPECT_EQ(MatrixStatusOK, matrixf32Swap(to_swap, 2, 1, SwapOptRow));
	EXPECT_TRUE(matrixf32TwoMatEqual(to_swap, swap_result, 1e-4f));

	matrixf32Destroy(&to_swap);
	matrixf32Destroy(&swap_result);
}

TEST(Swap, null_ptr_input_will_return_error) {
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Swap(0, 2, 1, SwapOptRow));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Swap(0, 2, 1, SwapOptCol));
}

TEST(Swap, unknown_swapping_option_will_return_error) {
	MatrixF32Ptr to_swap = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrUnknownOpt, matrixf32Swap(to_swap, 2, 1, SwapOptNull));

	matrixf32Destroy(&to_swap);
}
