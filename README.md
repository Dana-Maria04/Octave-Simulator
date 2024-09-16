# Octave-Simulator

// Copyright Caruntu Dana-Maria 311CAa 2023-2024
### My Octave Simulator

    This source code aims to simulate Octave operations on matrices based on characters input from the keyboard:
-> Loading a matrix into memory = L
-> Determining the dimensions of a matrix = D
-> Displaying a matrix = P
-> Resizing a matrix using Matlab/Octave-specific indexing = C
-> Multiplying two matrices = M
-> Sorting matrices = O
-> Transposing a matrix = T
-> Exponentiation in logarithmic time = R
-> Deleting a matrix from memory = F
-> Multiplying two matrices using Strassen's algorithm = S
-> Exiting the program and deallocating memory = Q

   Additionally, if any other letter different from the ones listed above is input, the program will display "Unrecognized command."

  The source code is based on defensive functions for matrix memory allocation, deallocation, and reallocation. Alongside these, the program includes other subroutines that implement the operations required by the input letter, which are executed when called in the main program.
   
   Furthermore, to execute the commands, there is a `char` type variable called "character," which the program checks and, based on the entered letter, executes the corresponding command. The integer variables "mat_crt", "m1", "m2", "matrix1", "matrix2", also input by the user, indicate to the program which matrix in the vector should be used to execute the commands.

   At the code level, there are two pointers, "idx_l" and "idx_c", which store the row index and column index for each matrix in the matrix vector "matrix", declared as a triple pointer.

  To avoid overflow issues when executing certain commands, I defined the variable "MODULO" = 10^4 + 7, and for this, I applied the following formula:

   result = ((result % MODULO) % MODULO + MODULO) % MODULO;

#### Loading a Matrix into Memory (L)

   This command involves a matrix vector where I store each matrix input from the keyboard. I allocate memory for it and read it. Additionally, the indices of the newly added matrix will be stored in the row and column index vectors. These vectors will have their memory reallocated using the "realloc_matrix" subroutine as new matrices are loaded, and their count will increment.

#### Determining the Dimensions of a Matrix (D)

   For the D command, the code uses the "idx_l" and "idx_c" vectors, which store the number of rows and columns for each matrix loaded (from the L command) and displays them only if the desired matrix number meets the conditions imposed by the matrix vector, i.e., it is greater than 0 and less than the total number of matrices. Otherwise, "No matrix with the given index" will be displayed.

#### Displaying a Matrix (P)

   After entering P and the desired matrix index, through the "print" function, if the index meets the limits imposed by the matrix vector, the corresponding matrix will be displayed. If not, "No matrix with the given index" will be displayed.

#### Resizing a Matrix Using Matlab/Octave-Specific Indexing (C)

   After entering the command C along with the number of rows and columns, and the row and column indices for resizing, the desired operation is executed through the "redimens" subroutine. In this subroutine, two new pointers, "lin" and "col," store the indices for resizing, and memory is allocated for them. The subroutine uses a matrix "red" to which it allocates memory and stores the resized matrix, eliminating the old matrix. To perform the in-place process, the "red" matrix will replace the old matrix, and the row and column index vectors will be updated with the desired row and column count.

#### Multiplying Two Matrices (M)

   With the M command and the two indices, the program understands which matrices to access from the vector for multiplication. It relies on the "multiply_matrix" function, which first checks the rules before starting the multiplication process: the matrices must be in the vector, and the number of columns of the first matrix must not differ from the number of rows of the second matrix. If the rules are met, the two matrices will be multiplied and stored in the "prod" matrix, which will be added to the end of the matrix vector. Its indices will be added to the end of the row and column index vectors, increasing the matrix count "size_matrix".

#### Sorting Matrices (O)

   To sort matrices, I created the "sum" function, which calculates the sum of elements in a matrix to obtain the sorting criterion. Thus, in the "sort" subroutine, if the sum of elements in one matrix is greater than the sum of the next matrix, they are swapped to create ascending order, and the corresponding row and column indices are also swapped through bubble sort. After entering the "O" character, the "sort" function is called to perform the command.

#### Transposing a Matrix (T)

   Similar to displaying a matrix, this command is represented by the letter T, followed by the matrix index for which the transpose operation is to be performed. The "trans" function checks if the desired matrix is in the vector and then transposes it.
   In the transposition process, I used an auxiliary matrix, "transpose", in which I swapped the rows and columns of the original matrix. Finally, I freed the old matrix and swapped the row and column indices stored in the two vectors to update the new dimensions.

#### Exponentiation in Logarithmic Time (R)

   The R command, along with the matrix index we want to exponentiate and the power to which we want to raise it, executes the command. First, it checks the conditions: the matrix must be in the vector, the power must be positive, and the matrix must be able to be multiplied. If these conditions are met, the "power" subroutine is called to raise the matrix to the desired power in logarithmic time, with a complexity of O(log2 n).

   The exponentiation is based on the "multiply" function, which multiplies two matrices, called within the "power" subroutine, depending on the parity of the power.

   Additionally, at the start, I created the identity matrix, `In`, to preserve the desired matrix during multiplication.
   The formula used is:
-> A ^ n = A ^ (n / 2) * A ^ (n / 2), for n = even
-> A ^ n = A * A ^ ((n-1) / 2) * A ^ ((n-1) / 2), for n = odd
   The subroutine returns the result, with which I update the matrix at the desired index, the exponentiation being done in-place.

#### Deleting a Matrix from Memory (F)

   For the index provided in the F command, if it exists in the matrix vector, the program frees the memory allocated for that matrix as well as the memory allocated for the indices stored in the "idx_l" and "idx_c" vectors, and decrements the matrix count. For efficient memory management during deletion, I reallocate memory only for half the size.

#### Multiplying Two Matrices Using Strassen's Algorithm (S)

   Similar to the M command, this command receives the matrix indices to be multiplied using Strassen's algorithm. It calls the "strassen" function, which relies on three additional subroutines: "sum_matrix", which sums two chosen matrices, "dif_matrix", which computes the difference between two matrices, and "divide", which splits the matrix into multiple matrix blocks (submatrices).
   
   In the "strassen" function, memory is allocated for the matrix blocks, created from the two matrices being multiplied, and will be freed at the end of the multiplication process. Auxiliary matrices are also used to store intermediate results, such as "aux1", "aux2", and matrices M1->M7.
   
   Additionally, memory is allocated for matrices C11->C22, which are the final matrix blocks that, once merged, will provide the final result.

   The basic formula for this algorithm is:

 M1 = (A11 + A22) * (B11 + B22)
 M2 = (A21 + A22) * B11
 M3 = A11 * (B11 - B22)
 M4 = A22 * (B21 - B11)
 M5 = A22 * (B21 - B11)
 M6 = (A21 - A22) * (B11 + B12)
 M7 = (A12 - A22) * (B21 + B22)

 where,

 A11, A12, A21, A22 are the blocks of matrix A
 B11, B12, B21, B22 are the blocks of matrix B

 the final result being:

 C = [M1+M4-M5+M7, M3+M5]
     [M2+M4, M1-M2+M3+M6]

   This division into blocks follows the divide and conquer algorithm, where the matrix is divided into four blocks, and each block is further divided until it can no longer be split.

#### Exiting the Program and Deallocating Memory (Q)

   At the Q command level, the row vector, column vector, and matrix vector will be freed, and "size_matrix" (the number of matrices in the vector) becomes 0 through the "end" subroutine call.
