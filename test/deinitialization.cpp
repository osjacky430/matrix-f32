#include "../include/matrix_f32.h"
#include "gtest/gtest.h"

TEST(DeInitialization, null_ptr_input_will_return_error) { EXPECT_EQ(MatrixStatusErrNullPtr, matrixf32Destroy(0)); }

TEST(DeInitialization, repeatedly_delete_do_not_return_error_thus_do_not_crash) {
	MatrixDimType dim = {dim.m_row = 2, dim.m_col = 1};
	MatrixF32Ptr mat = matrixf32Create(dim);

	EXPECT_EQ(MatrixStatusOK, matrixf32Destroy(&mat));
	matrixf32Destroy(&mat);
}

TEST(DeInitialization, the_pointer_deinitiailized_will_be_nulled) {
	MatrixDimType dim = {dim.m_row = 2, dim.m_col = 1};
	MatrixF32Ptr mat = matrixf32Create(dim);

	EXPECT_EQ(MatrixStatusOK, matrixf32Destroy(&mat));
	EXPECT_EQ(0, mat);
}

TEST(DeInitialization, the_buffer_with_external_ownership_will_not_be_freed_thus_do_not_crash) {
	float buffer[6] = {1000, 1000, 1000, 1000, 1000, 1000};
	MatrixDimType dim = {dim.m_row = 2, dim.m_col = 1};
	MatrixF32Ptr mat = matrixf32CreateContainer(dim, buffer, dim.m_row * dim.m_col);

	matrixf32Destroy(&mat);
}
