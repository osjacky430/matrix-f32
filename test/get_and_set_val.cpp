#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(GetVal, null_ptr_input_will_throw_assertion_failure) {
	// @TODO match string
	ASSERT_DEATH(matrixf32GetValueAt(0, {2, 1}), "");
}

TEST(GetVal, invalid_dimension_input_will_throw_assertion_failure) {
	MatrixF32Ptr mat = matrixf32Create({4, 3});
	// @TODO match string
	ASSERT_DEATH(matrixf32GetValueAt(mat, {5, 9}), "");

	matrixf32Destroy(&mat);
}

TEST(SetVal, invalid_dimension_input_will_return_error) {
	MatrixF32Ptr mat = matrixf32Create({4, 3});

	EXPECT_EQ(MatrixStatusErrDimMismatch, matrixf32SetValueAt(mat, {5, 9}, 2.0f));

	matrixf32Destroy(&mat);
}

TEST(SetVal, null_ptr_input_will_return_error) {
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32SetValueAt(0, {4, 2}, 0.0f));
}

TEST(SetAllEntriesTo, null_ptr_input_will_return_error) {
	EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32SetAllEntriesTo(0, 0.0f));
}

TEST(SetAllEntriesTo, set_all_entries_does_not_write_exceed_the_array) {
	static constexpr float CONST = 100.0f;
	static constexpr float OTHER_CONST = 20.0f;
	static constexpr size_t SOME_OFFSET = 5;
	float buffer[]{
		CONST, CONST, CONST, CONST, CONST, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CONST, CONST, CONST, CONST, CONST,
	};

	MatrixF32Ptr mat_a = matrixf32CreateContainer({4, 3}, buffer + SOME_OFFSET, 4 * 3);

	matrixf32SetAllEntriesTo(mat_a, OTHER_CONST);

	for (size_t i = 0; i < matrixf32GetRowNumber(mat_a); ++i) {
		for (size_t j = 0; j < matrixf32GetColNumber(mat_a); ++j) {
			EXPECT_NEAR(OTHER_CONST, matrixf32GetValueAt(mat_a, {i, j}), 1e-10);
		}
	}

	size_t j = SOME_OFFSET + matrixf32GetColNumber(mat_a) * matrixf32GetRowNumber(mat_a);
	for (size_t i = SOME_OFFSET; i < j; ++i) {
		EXPECT_NEAR(OTHER_CONST, buffer[i], 1e-10);
	}

	for (size_t i = 0; i < SOME_OFFSET; ++i) {
		EXPECT_NEAR(CONST, buffer[i], 1e-10);
	}

	for (; j < sizeof(buffer) / sizeof(float); j++) {
		EXPECT_NEAR(CONST, buffer[j], 1e-10);
	}

	matrixf32Destroy(&mat_a);
}
