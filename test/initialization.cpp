#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(Create, invalid_dimension_will_throw_assertion_failure) {
	MatrixDimType dim = {0, 2};
	MatrixF32Ptr mat;

	// @TODO match string
	ASSERT_DEATH(mat = matrixf32Create(dim), "");
}

TEST(Create, member_variables_are_initialized_and_can_be_accessed_from_getter_correctly) {
	MatrixDimType dim = {4, 3};
	MatrixF32Ptr mat = matrixf32Create(dim);

	EXPECT_EQ(OwnerShipSelf, matrixf32GetOwnership(mat));
	EXPECT_EQ(dim.m_col, matrixf32GetColNumber(mat));
	EXPECT_EQ(dim.m_row, matrixf32GetRowNumber(mat));

	for (size_t i = 0; i < dim.m_row; ++i) {
		for (size_t j = 0; j < dim.m_col; ++j) {
			EXPECT_NEAR(0.0f, matrixf32GetValueAt(mat, {i, j}), 1e-10);	 // @TODO array comp
		}
	}

	matrixf32Destroy(&mat);
}

TEST(CreateContainer, invalid_dimension_will_throw_assertion_failure) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t BUFFER_SIZE = 6;

	float buffer[]{CONST, CONST, CONST, CONST, CONST, CONST};

	MatrixDimType dim = {0, 2};
	MatrixF32Ptr mat;

	// @TODO match string
	ASSERT_DEATH(mat = matrixf32CreateContainer(dim, buffer, BUFFER_SIZE), "");
}

TEST(CreateContainer, member_variables_are_initialized_and_can_be_accessed_from_getter_correctly) {
	static constexpr float CONST = 100.0f;

	float buffer[]{CONST, CONST, CONST, CONST, CONST, CONST};

	MatrixDimType dim = {3, 2};
	MatrixF32Ptr mat = matrixf32CreateContainer(dim, buffer, sizeof(buffer) / sizeof(float));

	ASSERT_TRUE(matrixf32GetBuffer(mat) == buffer);
	EXPECT_EQ(OwnerShipOuter, matrixf32GetOwnership(mat));
	EXPECT_EQ(dim.m_col, matrixf32GetColNumber(mat));
	EXPECT_EQ(dim.m_row, matrixf32GetRowNumber(mat));

	for (size_t i = 0; i < matrixf32GetColNumber(mat); i++) {
		for (size_t j = 0; j < matrixf32GetRowNumber(mat); j++) {
			EXPECT_NEAR(CONST, matrixf32GetValueAt(mat, {j, i}), 1e-10);
		}
	}
}

TEST(CreateContainer, out_of_boundary_will_throw_assertion_failuer) {
	static constexpr float CONST = 100.0f;
	static constexpr size_t BUFFER_SIZE = 4;

	float buffer[]{CONST, CONST, CONST, CONST, CONST, CONST};

	MatrixDimType dim = {3, 2};
	MatrixF32Ptr mat;

	// @TODO match string
	ASSERT_DEATH(mat = matrixf32CreateContainer(dim, buffer, BUFFER_SIZE), "");

	// assertion fail, no need to destroy it, as the system may already halt at some point
}
