#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Inverse, inverse_algorithm) {
	static constexpr size_t SOME_OFFSET = 5;
	static constexpr float CONST = 100.0f;

	float test_case[]{CONST, CONST, CONST, CONST, CONST, 1.0f, 2.6f, -8.1f, 9.2f,  -1.5f, 5.7f,	 7.7f, -9.9f,
					  12.0f, -5.1f, 4.2f,  -4.6f, 6.1f,	 2.8f, 2.9f, 8.4f,	CONST, CONST, CONST, CONST};

	float inverse_result_buffer[]{0.07644136f,	0.035759674f,	0.08023302f,   0.00236097324f,
								  0.11466476f,	0.11672867f,	-0.000827724f, 0.011534579f,
								  -0.13193906f, -0.0228465795f, -0.040387194f, 0.09546156f,
								  -0.04818218f, -0.056990381f,	-0.044045348f, 0.080531277f};

	MatrixF32Ptr dest = matrixf32Create({4, 4});
	MatrixF32Ptr source = matrixf32CreateContainer({4, 4}, test_case + SOME_OFFSET, 4 * 4);
	MatrixF32Ptr inverse_result = matrixf32CreateContainer({4, 4}, inverse_result_buffer, 4 * 4);

	EXPECT_EQ(MatrixStatusOK, matrixf32Inverse(dest, source));
	EXPECT_TRUE(matrixf32TwoMatEqual(dest, inverse_result, 1e-4f));

	matrixf32Destroy(&dest);
	matrixf32Destroy(&source);
	matrixf32Destroy(&inverse_result);
}

TEST(Inverse, singular_matrix_input_will_return_error) {
	static constexpr size_t SOME_OFFSET = 5;
	static constexpr float CONST = 100.0f;

	float test_case[]{CONST, CONST, CONST, CONST, CONST, 1.0f, 2.6f, -8.1f, 9.2f,  -1.5f, 5.7f,	 7.7f, -9.9f,
					  12.0f, -5.1f, 4.2f,  -4.6f, 6.1f,	 2.8f, 2.9f, 8.4f,	CONST, CONST, CONST, CONST};

	MatrixF32Ptr dest = matrixf32Create({4, 4});
	MatrixF32Ptr source = matrixf32CreateContainer({4, 4}, test_case + SOME_OFFSET, 4 * 4);

	for (size_t i = 0; i < matrixf32GetColNumber(source); ++i) {
		matrixf32SetValueAt(source, {1, i}, 0);
	}

	EXPECT_EQ(MatrixStatusErrSingular, matrixf32Inverse(dest, source));

	matrixf32Destroy(&dest);
	matrixf32Destroy(&source);
}

TEST(Inverse, wrong_input_matrix_dimension_will_return_error) {
	MatrixF32Ptr square_mat_4_by_4 = matrixf32Create({4, 4});
	MatrixF32Ptr square_mat_3_by_3 = matrixf32Create({3, 3});
	MatrixF32Ptr non_square_mat = matrixf32Create({3, 4});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Inverse(non_square_mat, square_mat_4_by_4));
	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32Inverse(square_mat_3_by_3, square_mat_4_by_4));

	matrixf32Destroy(&square_mat_4_by_4);
	matrixf32Destroy(&square_mat_3_by_3);
	matrixf32Destroy(&non_square_mat);
}

TEST(Inverse, null_ptr_input_will_return_error) {
	MatrixF32Ptr source = matrixf32Create({4, 4});
	MatrixF32Ptr dest = matrixf32Create({4, 4});

	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Inverse(dest, 0));
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Inverse(0, source));

	matrixf32Destroy(&source);
	matrixf32Destroy(&dest);
}
