/**
 * @Date:   2019-11-12T14:46:23+08:00
 * @Last modified time: 2019-11-12T22:30:38+08:00
 */
#ifndef MATRIX_F32_H_
#define MATRIX_F32_H_

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	SwapOptNull = 0,
	SwapOptRow,
	SwapOptCol,
} MatrixSwapOpt;

typedef enum { OwnerShipNull = 0, OwnerShipSelf = 1, OwnerShipOuter = 2 } AddrOwnerShip;

typedef struct MatrixDim {
	size_t m_row;
	size_t m_col;
} MatrixDimType;

typedef enum {
	MatrixStatusOK = 0,
	MatrixStatusErrDimMismatch = 0b00001,
	MatrixStatusErrOutOfBound = 0b00010,
	MatrixStatusErrNullPtr = 0b00100,
	MatrixStatusErrSingular = 0b01000,
	MatrixStatusErrUnknownOpt = 0b10000
} MatrixStatus;

#ifdef __cplusplus
extern "C" {
#endif

/**
 *	Defines the pointer to the ADT
 */
typedef struct MatrixF32 *MatrixF32Ptr;

/**
 *	Defines the interface for the ADT
 */
MatrixF32Ptr matrixf32Create(MatrixDimType t_dim);
MatrixF32Ptr matrixf32CreateContainer(MatrixDimType t_dim, float *t_val, int t_buffer_len);
MatrixStatus matrixf32Destroy(MatrixF32Ptr *t_dest);

/**
 *	Getter functions
 */
size_t matrixf32GetColNumber(MatrixF32Ptr t_dest);
size_t matrixf32GetRowNumber(MatrixF32Ptr t_dest);
AddrOwnerShip matrixf32GetOwnership(MatrixF32Ptr t_dest);
float matrixf32GetValueAt(MatrixF32Ptr t_dest, MatrixDimType t_dim);
float *matrixf32GetBuffer(MatrixF32Ptr t_dest);	 // @TODO: remove this

/**
 *	Setter functions
 */
MatrixStatus matrixf32SetValueAt(MatrixF32Ptr t_dest, MatrixDimType t_dim, float t_val);
MatrixStatus matrixf32SetAllEntriesTo(MatrixF32Ptr t_dest, float t_val);

/**
 * @brief This function checks if two matrix difference are within tolerance
 *
 * @param  t_mat_a     A matrix to compare
 * @param  t_mat_b     B matrix to compare
 * @param  t_tolerance The maximum difference between two matrix
 * @return           true if two matrix difference are within tolerance, false otherwise
 */
bool matrixf32TwoMatEqual(const MatrixF32Ptr t_mat_a, const MatrixF32Ptr t_mat_b, float t_tolerance);

/**
 * @brief  This function adds two matrix and store to the other matrix
 *
 * @param  t_result The buffer to store the result of addition
 * @param  t_mat_b  Summand B
 * @param  t_mat_c  Summand C
 * @return        	MatrixStatus
 */
MatrixStatus matrixf32Add(const MatrixF32Ptr t_result, const MatrixF32Ptr t_mat_b, const MatrixF32Ptr t_mat_c);

/**
 * @brief  This function scales the matrix and store to the other matrix
 *
 * @param  t_result The buffer to store the result of scaling
 * @param  t_val    coefficient of scaling
 * @param  t_mat_b  Matrix to be scaled
 * @return          MatrixStatus
 */
MatrixStatus matrixf32Scale(const MatrixF32Ptr t_result, float t_val, MatrixF32Ptr t_mat_b);

/**
 * @brief  This function subtracts two matrix and store to the other matrix
 * @param  t_result The buffer to store the result of subtraction
 * @param  t_mat_b  Minuend B
 * @param  t_mat_c  Subtrahend C
 * @return          MatrixStatus
 */
MatrixStatus matrixf32Subtract(const MatrixF32Ptr t_result, const MatrixF32Ptr t_mat_b, const MatrixF32Ptr t_mat_c);

/**
 * @brief	 This function multiplicates two matrix and store to the other matrix
 * @param  t_result The buffer to store the result of multiplication
 * @param  t_mat_b  Multiplicand B
 * @param  t_mat_c  Multiplier C
 * @return          MatrixStatus
 */
MatrixStatus matrixf32Multiplication(const MatrixF32Ptr t_result, const MatrixF32Ptr t_mat_b,
									 const MatrixF32Ptr t_mat_c);

/**
 * @brief  This function copies one matrix to the other matrix value-wise
 * @param  t_result The buffer to store the result of copy
 * @param  t_source The source to be copied
 * @return          MatrixStatus
 */
MatrixStatus matrixf32Copy(const MatrixF32Ptr t_result, const MatrixF32Ptr t_source);

/**
 * @brief  This function swaps column or roll of the matrix
 * @param  t_to_swap Matrix to be swapped
 * @param  t_from    From which col/row
 * @param  t_to      To which col/row
 * @param  t_opt     Possible options are SwapOptRow and SwapOptCol.
 * @return           MatrixStatus
 */
MatrixStatus matrixf32Swap(const MatrixF32Ptr t_to_swap, size_t t_from, size_t t_to, MatrixSwapOpt t_opt);

/**
 * @brief  This function does LU decomposition in place, the resulting matrix will take the following form
 * 				 (take 3 by 3 as example @todo add example)
 *
 * @param  t_result      The buffer for the result and the source of the decomposed matrix
 * @param  t_permutation Permutation matrix indicate the row swapping sequence
 * @return               MatrixStatus
 */
MatrixStatus matrixf32InPlaceLU(const MatrixF32Ptr t_result, const MatrixF32Ptr t_permutation);

/**
 * @brief This function does out place LU decomposition
 *
 * @param  t_result      The buffer to store the result of LU decomposition
 * @param  t_source      The source of LU decomposition
 * @param  t_permutation The permutation matrix indicate the row swapping sequence
 * @return               MatrixStatus
 */
MatrixStatus matrixf32OutPlaceLU(const MatrixF32Ptr t_result, const MatrixF32Ptr t_source,
								 const MatrixF32Ptr t_permutation);

/**
 * @brief  This function does forward substitution, i.e. solve Lx = c
 *
 * @param  t_result           The buffer to store the result of forward substitution
 * @param  t_lower_triangular The lower triangular matrix for forward substitution
 * @param  t_column           solution vector
 * @return                    MatrixStatus
 */
MatrixStatus matrixf32ForwardSubstitution(const MatrixF32Ptr t_result, const MatrixF32Ptr t_lower_triangular,
										  const MatrixF32Ptr t_column);

/**
 * @brief  This function does backward substitution, i.e. solve Ux = c
 * @param  t_result           The buffer to store the resutl of backward substitution
 * @param  t_upper_triangular The upper triangular matrix for backward substitution
 * @param  t_column           solution vector
 * @return                    MatrixStatus
 */
MatrixStatus matrixf32BackwardSubstitution(const MatrixF32Ptr t_result, const MatrixF32Ptr t_upper_triangular,
										   const MatrixF32Ptr t_column);

/**
 * @brief  This function does matrix inverse
 *
 * @param  t_result The buffer to store the result of the matrix inverse
 * @param  t_source The source matrix to perform inverse operation
 * @return          MatrixStatus
 */
MatrixStatus matrixf32Inverse(const MatrixF32Ptr t_result, const MatrixF32Ptr t_source);

#ifdef __cplusplus
}
#endif

#endif	// MATRIX_F32_H_
